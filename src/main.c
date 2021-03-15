//including c libraries to use

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> // compile ----> gcc -o project project.c -lm <-----
#include <malloc.h>
#include <limits.h>
#include <stdbool.h>
#define COUNT 15

//define struct Word
struct Word{
    int number, frequency, depth;
    char word [25];
    struct Word *left, *right, *next;
};

//define a queue
struct queue{
    int front, rear, size;
    struct Word* *arr;
};

struct Word * firstTree = NULL; // representing first binary search tree(alphabetically sorted)
struct Word * secondTree = NULL;// representing second binary tree (acc to minimazing total access time)
struct Word * head = NULL; // and a linked list ref to store the nodes with highest frequency

//methods to use
//
struct Word * newNode(int number,char * word, int frequency);
struct Word * insert(struct Word * node, int number, char * word, int frequency);
struct Word * insertToList(struct Word * node, int number, char * word, int frequency, int depth);
void insertFrequencyOrder(struct Word * node);
int depth(struct Word * node, char * word, int levet);
int getDepth(struct Word * node, char * word);
struct Word * search(struct Word * node, char * word);
void print2D(struct Word * node, int space);
void printTree(struct Word * node);
void printData(struct Word * node);
void printFreqs(struct Word * node);
void swap(struct Word ** a, struct Word ** b);
void sort(struct Word * head);
int length(struct Word * iter);
int secAccessTime(struct Word * root);
void convertList2BT(struct Word * node);

/*
 * Main method
 */
int main(int argc, char* argv[]){

	//define a char * for file name
    char * fileName = "../files/input.txt";
    //create a file pointer to read the file
    FILE* file = fopen(fileName, "r");
    //line buffer
    char line[256];
    //a char pointer token to split the line with','
    char * token;
    const char values [3] = ",";
    //calculating access time while reading the input file
    int firstAccessTime = 0, secondAccessTime = 0;

    //read the file with fgets() method
    while (fgets(line, sizeof(line), file)) {
	    char * elements[3];
	    int counter = 0;
	    token = strtok(line, values);

	    //elements[0] -> number, elements[1]-> word, elements[2] -> frequency
	    while(token != NULL){
		    elements[counter++] = token;
	    	    token = strtok(NULL , values);
	    }

	    // insert the values line by line to first binary search tree
	    firstTree = insert(firstTree, atoi(elements[0]), elements[1], atoi(elements[2]));

	    /* calculate the current node's depth and access time */
	    struct Word * current = search(firstTree, elements[1]);
	    current -> depth = getDepth(firstTree, current-> word);
	    //calculate the firstTree's access time
	    firstAccessTime += current-> frequency * current -> depth;

	    //inserting values in the same time to the linked list with next pointer
	    head = insertToList(head, atoi(elements[0]), elements[1], atoi(elements[2]),current->depth);
	    //sort the linked list acc to frequencies
	    sort(head);
	    //give the depths and calculate the access time;
	    secondAccessTime = secAccessTime(head);
    }

    fclose(file);
    /*
     * print trees in two dimension and print the access time
     */

    printf("\n\n----------------------------------------------------------FIRST BINARY SEARCH TREE WITH ALPHABETICAL ORDER (LEFT ROTATED)-----------------------------------------------------------\n\n");

    printTree(firstTree);

    printf("\n\n-----------------------------------------------------------  First BST Total Accsess Time : %d -------------------------------------------------------------------------\n\n", firstAccessTime);

    printf("\n\n----------------------------------------------------------SECOND BINARY TREE WITH FREQUENCIAL ORDER (LEFT ROTATED)-----------------------------------------------------------\n\n");

    convertList2BT(head);

    printTree(secondTree);


    printf("\n\n-----------------------------------------------------------  Second BT Total Accsess Time : %d -------------------------------------------------------------------------\n\n", secondAccessTime);


}

//create a new binary search tree node with values
struct Word* newNode(int number,char * word, int frequency){

	//allocating memory space for node
	struct Word* temp =(struct Word * )malloc(sizeof(struct Word));
    //assignin values in parameter
    temp->number = number;
    strcpy(temp-> word , word);
    temp-> frequency = frequency;
    temp->left = temp->right = NULL;
    //return the node
    return temp;
}

//insert method for binary search tree with nodes values
struct Word* insert(struct Word * node,int number, char * word, int frequency) {

	//if node is empty, create one with values
    if (node == NULL)
       return newNode(number, word, frequency);

    //if word is smaller than the word as alphabetically put it to the left child
    if (strcmp(node->word, word) > 0)
        node->left = insert(node->left, number, word, frequency);

    // if word is greather than or the same word, put it to the right child
    else //if (strcmp(node->word, word) < 0 || strcmp(node -> word, word))
        node->right = insert(node->right, number, word, frequency);
    //lastly return the node
    return node;
}

//insert method for linked list to hold highest frequency nodes in sorted
struct Word * insertToList(struct Word * root, int number, char * word, int frequency, int depth) {

    if (root == NULL)  {

	    struct Word *new_node = (struct Word * ) malloc(sizeof(struct Word));
	    new_node->frequency = frequency;
	    new_node -> number = number;
	    strcpy(new_node-> word, word);
	    new_node -> depth = depth;
	    new_node->next = NULL;
	    return new_node;
    }

    struct Word * head = root;

    while(head -> next != NULL){
	    head = head -> next;
    }

    head -> next = (struct Word *) malloc(sizeof(struct Word));
    head -> next -> frequency = frequency;
    head -> next -> number = number;
    strcpy(head -> next -> word, word);
    head -> next -> depth = depth;
    head -> next -> next = NULL;

    return root;
}

//create node method to use convering list to binary tree
struct Word* createNode(struct Word *root){
     //Create a new node
     struct Word *newNode = (struct Word*)malloc(sizeof(struct Word));
     //Assign values to newNode, set left and right child to NULL
     newNode -> number = root-> number;
     strcpy(newNode->word, root -> word);
     newNode -> frequency = root -> frequency;
     newNode -> depth = root -> depth;
     newNode->left = NULL;
     newNode->right = NULL;

     return newNode;
 }
//createQueue() will create a queue
struct queue* createQueue(){
    	//memory space for new queue
	struct queue* newQueue = (struct queue*) malloc(sizeof( struct queue ));

	//init values for queue
    newQueue->front = -1;
    newQueue->rear = 0;
    newQueue->size = 0;

    //queue arr to hold values
    newQueue->arr = (struct Word**) malloc(100 * sizeof( struct Word* ));

    return newQueue;
}

//Adds a node to queue
void enqueue(struct queue* queue, struct Word *temp){
    queue->arr[queue->rear++] = temp;
    queue->size++;
}

//Deletes a node from queue
struct Word *dequeue(struct queue* queue){
    queue->size--;
    return queue->arr[++queue->front];
}

//inserting values according to frequency order from linked list to second binary tree
void insertFrequencyOrder(struct Word * node){

	struct Word *newNode = createNode(node);

     //Check whether tree is empty
     if(secondTree == NULL){
         secondTree = newNode;
         return;
     }
     else {
         //Queue will be used to keep track of nodes of tree level-wise
         struct queue* queue = createQueue();
         //Add root to the queue
         enqueue(queue, secondTree);

         while(true) {
             struct Word *node = dequeue(queue);
             //If node has both left and right child, add both the child to queue
             if(node->left != NULL && node->right != NULL) {
                 enqueue(queue, node->left);
                 enqueue(queue, node->right);
             }
             else {
                 //If node has no left child, make newNode as left child
                 if(node->left == NULL) {
                     node->left = newNode;
                     enqueue(queue, node->left);
                 }
                 //If node has left child but no right child, make newNode as right child
                 else {
                     node->right = newNode;
                     enqueue(queue, node->right);
                 }
                 break;
             }
         }
     }


}
//print method to print the three in two dimensional
void print2D(struct Word *root, int space) {
	if (root == NULL)
		return;

	space += COUNT;

	//tree traversal and print the values to the screen properly
	print2D(root->right, space);

	printf("\n");
	for (int i = COUNT; i < space; i++)
		printf(" ");
	printf("%s , f: %d, depth :%d \n",root-> word ,root-> frequency,  root->depth);

	print2D(root->left, space);
}

//calling another method to avoid space parameter
void printTree(struct Word * root){
	print2D(root, 0);
}

//printData method for check the nodes values
void printData(struct Word* node){
	printf("word : %s, frequency: %d depth : %d\n ", node-> word, node -> frequency, node -> depth);

}
//recursive depth method to define depths of all the nodes for first binary search tree
int depth(struct Word * node, char * word, int level){
	if(node == NULL) return 0;

	//if word is mathced returned the level;
	if(strcmp(node->word, word) == 0) return level;

	//tree traversal
	int downLevel = depth(node->left, word, level + 1);

	if(downLevel != 0) return downLevel;

	return depth(node -> right,word, level + 1);
}

int getDepth(struct Word * node,char * word){
	return depth(node,word,1);
}
//recursive search method to find the spesific node with its word
struct Word * search(struct Word * root, char * word){

	if(root == NULL) return NULL;

	struct Word * current = root;

	while(strcmp(current->word, word) != 0){

		//go to the left tree
		if(strcmp(current->word, word) > 0) current = current -> left;

		//go to the right three
		else current = current -> right;
	}
	return current;
}


//print method for linked list
void printFreqs(struct Word * node){
	while(node){
		printData(node);
		node = node -> next;
	}
}
//swap method to change two nodes values
void swap(struct Word ** a, struct Word ** b){
  struct Word * temp = *a;
  *a = *b;
  *b = temp;
}
//sort method to sort linked list acc to their frequencies
void sort(struct Word * head){
	struct Word * current = head, *index = NULL;
	int temp;
	if(head == NULL) return;
	else {
		while(current != NULL){
			index = current -> next;
			while(index != NULL){
				if(current -> frequency < index -> frequency){
					swap(&current, &index);
				}
				index = index -> next;
			}
			current = current -> next;
		}
	}
}

//return the length of the linked list
int length(struct Word * iter){
	int length = 0;
	while(iter != NULL){
		iter = iter -> next;
		length++;
	}
	return length;
}
//defining depth's of the nodes for the second binary tree and returning total access time
int secAccessTime(struct Word * root){
  int time = 0, depth_level = 0;

  struct Word * iter = root;
	while(iter -> next != NULL){
		for(int i = 0; i < pow(2,depth_level); i++){
			  iter -> depth = depth_level+1;
			  time += iter -> depth * iter-> frequency;
			  if(iter -> next == NULL) break;
			  iter = iter-> next;

		 }
	  depth_level++;
 	}
  return time;
}

//convert linked list to binary tree --> secondTree
void convertList2BT(struct Word * node){

    struct Word * head = node;
    //add the values and insert to second three until end of the linked list
    while(head != NULL){
	    insertFrequencyOrder(head);
	    head = head -> next;
    }
}
