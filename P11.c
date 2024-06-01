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
  double cost;
}node;

int create_freq_table(struct freq_count * freq_table,char** words,int word_count);
void sort_word_list(char** words,int word_count);
void search(node* root,char* search_word);
node* create_tree(int unique_word_count,int start,int stop,int root_table[][unique_word_count],struct freq_count * freq_table,int cost_table[][unique_word_count],int weight_table[][unique_word_count]);
void compute_table(struct freq_count * freq_table,int unique_word_count,int root_table[][unique_word_count],int cost_table[][unique_word_count],int weight_table[][unique_word_count]);

//driver function of the code
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
    char *words[2070];  //Array of Strings to store the words read from file
    int word_count = 0;

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

    sort_word_list(words,word_count);   //Sort the array of words in alphabetical order
    struct freq_count freq_table[word_count];  
    int unique_word_count = create_freq_table(freq_table,words,word_count); //Create the frequency table of the words
    
    int root_table[unique_word_count][unique_word_count];
    int cost_table[unique_word_count][unique_word_count];
    int weight_table[unique_word_count][unique_word_count];

    compute_table(freq_table,unique_word_count,root_table,cost_table,weight_table); //Compute the root table and cost table 
    //Create the tree using the root table created
    struct node* root = create_tree(unique_word_count,0,unique_word_count,root_table,freq_table,cost_table,weight_table);
    char search_word[40];
    //Asking the user to enter the search key once the tree has been created
    printf("A binary search tree has been successfully created using the Dynamic Programming technique\n");
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
        printf("Compared with %s (%lf), Found\n",root->node_key.w,root->cost);
        return;
    }
    else if(strcmp(root->node_key.w,search_word) > 0) //If word does not match the current node, check if we have to go to left or right using the frequency count
    {
        printf("Compared with %s (%lf), go left subtree\n",root->node_key.w,root->cost);
        root = root->left;
        search(root,search_word);
    }
    else
    {
        printf("Compared with %s (%lf), go right subtree\n",root->node_key.w,root->cost);
        root = root->right;
        search(root,search_word);
    }
}

//Tree is created using the root table values
//returns the root node of the tree
node* create_tree(int unique_word_count,int start,int stop,int root_table[][unique_word_count],struct freq_count * freq_table,int cost_table[][unique_word_count],int weight_table[][unique_word_count])
{
    node* new_node;

    if(start == stop)
    {
        new_node = NULL;
    }
    else
    {
        new_node = (node*)malloc(sizeof(node));
        new_node -> node_key = freq_table[root_table[start][stop]];  //Root table is used to find the next node of the tree
        new_node -> cost = (double)cost_table[start][stop]/(double)weight_table[start][stop];
        //Dynammic programming approach is used to create the left and right node of the tree respectively
        new_node -> left = create_tree(unique_word_count,start,root_table[start][stop]-1,root_table,freq_table,cost_table,weight_table);
        new_node -> right = create_tree(unique_word_count,root_table[start][stop],stop,root_table,freq_table,cost_table,weight_table);
    }
    return new_node;
}

//Computes the Root table and cost table 
void compute_table(struct freq_count * freq_table,int unique_word_count,int root_table[][unique_word_count],int cost_table[][unique_word_count],int weight_table[][unique_word_count])
{
    //Weight table is a helper table used to store the weight search of all the possible nodes in the tree
    for(int i=0;i<=unique_word_count;i++) //Start with initializifn the weight table
    {
        weight_table[i][i] = 0;
        for(int j=i+1;j<=unique_word_count;j++)
        {
            weight_table[i][j] = weight_table[i][j-1] + freq_table[j].b;
        }
    }

    for(int i=0;i<=unique_word_count;i++)
    {
        cost_table[i][i] = weight_table[i][i];
    }
    //Root table and cost table are initialed along the diagonal of the table
    for(int i=0;i<=(unique_word_count-1);i++)
    {
        int j = i + 1;
        cost_table[i][j] = cost_table[i][i] + cost_table[j][j] + weight_table[i][j];
        root_table[i][j] = j;
    }

    for(int l=2;l<=unique_word_count;l++)
    {
        for(int i=0;i<=unique_word_count-l;i++)
        {
            int j = i + l;
            int pos = root_table[i][j-1];
            int min_k = cost_table[i][pos-1] + cost_table[pos][j];
            for(int k=pos+1;k<=root_table[i+1][j];k++)  //Find the root table value which give he minimum cost
            {
                int cost = cost_table[i][k-1] + cost_table[k][j];
                if(cost < min_k)  //If cost is less than current minimum, replace with new cost
                {
                    pos = k;
                    min_k = cost;
                }
            }
            cost_table[i][j] = weight_table[i][j] + min_k;
            root_table[i][j] = pos;  //Vlue of root table is updates for root that gives lowest cost
        }
    }
}

//Function creates the frequency table for all thw words read for the file
//Returns the number of unique words found in the file
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