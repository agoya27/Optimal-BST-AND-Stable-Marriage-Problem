/*
Name: Anurag Goyal
ID  : 1096870
Date: March 27, 2023
Assignment Number: 4
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

//Struct created to store the freq count of all the words in the file
typedef struct freq_count {
  char* w;
  int b;
  double c;
}freq_count;

//Struct created to create a tree node
typedef struct node {
  struct freq_count node_key;
  struct node *left;
  struct node *right;
}node;

void sort_word_list(char** words,int word_count);
int create_freq_table(struct freq_count * freq_table,char** words,int word_count);
struct node* create_tree_node(struct freq_count key);
struct node* create_tree(struct freq_count * freq_table,int i,int j);
void search(node* root,char* search_word);

//Driver function of the code to control the program
int main()
{
    char f_name[] = "data_A4_Q1.txt";
    char content[2045][50];
    FILE *f_point = fopen(f_name,"r");

    if(!f_point) //Exit the program if file could not be opened
    {
        printf("File could not be opened.\nExit the program\n");
        return 0;
    }
    int line_count = 0;
    while(fscanf(f_point,"%s",content[line_count]) != EOF)
    {
        line_count++;
    }
    fclose(f_point);
    char* delim = " ";
    char *words[2070];
    int word_count = 0;

    //Data from file is read as a single string and broken into array of strings
    //Every element of this array represent a word in the file
    for(int i=0;i<line_count;i++)
    {
        char* word = strtok(content[i],delim);
        while(word != NULL && word_count < 2070)
        {
            words[word_count] = word;
            word_count++;
            word = strtok(NULL,delim);
        }
    }

    //String array is sorted in alphabetical order usinf the sort function
    sort_word_list(words,word_count);
    struct freq_count freq_table[word_count]; //Frequency table is created using the string array
    int unique_word_count = create_freq_table(freq_table,words,word_count);

    int max_freq = freq_table[0].b; //Find the word with the maximum frequency in the file
    int max_index = 0;
    for(int i=1;i<unique_word_count;i++)
    {
        if(freq_table[i].b > max_freq)
        {
            max_freq = freq_table[i].b;
            max_index = i;
        }
    }
    //The word with maximum frequency is the root of the tree
    node* root = create_tree_node(freq_table[max_index]);
    root->left = create_tree(freq_table,0,max_index-1);
    root->right = create_tree(freq_table,max_index+1,unique_word_count-1);

    char search_word[40];
    printf("A binary search tree has been successfully created using the Greedy technique\n");
    printf("Please enter the word (key) to search in the tree: ");  
    scanf("%s",search_word);  //Asking for the key to search after creating the Optimal BST
    search(root,search_word); //Search function is called to complete the search and print the result to the screen
    return 0;
}

//Search function is used to find the word inside the BST
void search(node* root,char* search_word)
{
    if(root == NULL)  //If we reach the leaf node, then the word was not found in the tree
    {
        printf("Word not found\n");
        return;
    }
    if(strcmp(root->node_key.w,search_word) == 0) //Print and Exit the search if word is found in the tree
    {
        printf("Compared with %s (%lf), Found\n",root->node_key.w,root->node_key.c);
        return;
    }
    else if(strcmp(root->node_key.w,search_word) > 0) //If word does not match the current node, check if we have to go to left or right using the frequency count
    {
        printf("Compared with %s (%lf), go left subtree\n",root->node_key.w,root->node_key.c);
        root = root->left;
        search(root,search_word);
    }
    else
    {
        printf("Compared with %s (%lf), go right subtree\n",root->node_key.w,root->node_key.c);
        root = root->right;
        search(root,search_word);
    }
}

//Function is used to create a BST using the Greedy technique
//funciton returns the root of the new subtree created
struct node* create_tree(struct freq_count * freq_table,int start,int stop)
{
    if(start > stop)
    {
        return NULL;
    }
    if(start == stop)
    {
        return create_tree_node(freq_table[start]); //Creates a tree with single node and return it 
    }

    int max_freq = freq_table[start].b; //Finds the maximum frequency count for the subtree
    int max_index = start;

    for(int k=start+1;k<=stop;k++)
    {
        if(freq_table[k].b > max_freq) 
        {
            max_freq = freq_table[k].b;
            max_index = k;
        }
    }

    //Key with maximum frequency is the root for subtree
    node* root = create_tree_node(freq_table[max_index]);
    root->left = create_tree(freq_table,start,max_index-1);
    root->right = create_tree(freq_table,max_index+1,stop);
    return root;
}

//Function to create a node of the tree and initialize the values
//function returns the new node created
struct node* create_tree_node(struct freq_count key)
{
    struct node* new_node = (struct node*)malloc(sizeof(node));
    new_node -> node_key = key;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

//Function to create the frequency table from the string array passed as a argument
//Function returns the number of unique words in the array
int create_freq_table(struct freq_count * freq_table,char** words,int word_count)
{
    int unique_word_count = 0;
    freq_table[0].w = words[0];
    freq_table[0].b = 1;
    unique_word_count++;
    int exist = 0;
    int denum = 2045;

    for(int i=1;i<word_count;i++)
    {
        for(int j=0;j<unique_word_count;j++)
        {
            //if condiiton to check if the word is already part of the frequency table
            //if yes, then no need to create a new row in the table
            //Count of the existing row can be incremented
            if(strcmp(freq_table[j].w,words[i])==0)
            {
                freq_table[j].b++;
                exist = 1;
                break;
            }
        }
        //if word does not exist in the row, create a new row and initialize the count value to 1
        if(exist == 0)
        {
            freq_table[unique_word_count].w = words[i];
            freq_table[unique_word_count].b = 1;
            unique_word_count++;
        }
        exist = 0;
    }

    //Probabilited of all the words are calculated and stored in the table
    for(int i=0;i<unique_word_count;i++)
    {
        freq_table[i].c = (double) freq_table[i].b/denum;
    }
    return unique_word_count;
}

//Function is used to sort of array of strings in alphabetical order
void sort_word_list(char** words,int word_count)
{
    char temp[50];
    for(int i=0;i<word_count;i++)
    {
        for(int j=0;j<word_count-1-i;j++)
        {
            if((strcmp(words[j],words[j+1])) > 0)
            {
                strcpy(temp,words[j]);
                strcpy(words[j],words[j+1]);
                strcpy(words[j+1],temp);
            }
        }
    }
}