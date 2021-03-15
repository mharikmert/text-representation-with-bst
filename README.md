# Text Representation with Binary Search Trees
## How it works?
It reads __number__, __word__, __frequency__ structured input files in files folder. <br>
Firstly words are compared and be sorted in alphabetical order in a binary search tree. <br>
Then frequencies are compared and words exchanes in the binary search tree according to their frequencies.<br>
After the binary tree is created, total access time of the BST is also calculated. <br>
Idea behind creating two BTS is to see the change of the total access time between two different BTS

## Sample Inputs 
| Number| Word          | Frequency |
| ----- |:-------------:| ----------:|
| 1     | people     |6|
|2| country|10|
| 3     | city |15|
| 4     | news    |2|
| 5     | population   |1|
| 6     | society    |28|
| 7     | university    |35|
| 8     | sports    |62|
|9|basketball|28
## Outputs 
#### First Binary Search Tree
![Screenshot from 2021-03-15 19-42-22](https://user-images.githubusercontent.com/42295478/111189086-c92b2980-85c6-11eb-8b32-7722fa42d945.png)

#### Second Binary Search Tree 
![Screenshot from 2021-03-15 19-42-34](https://user-images.githubusercontent.com/42295478/111189123-d2b49180-85c6-11eb-8f65-319ef4654fc8.png)


## Compilation and Run

```
-> gcc -o main main.c -lm
-> ./main
```
