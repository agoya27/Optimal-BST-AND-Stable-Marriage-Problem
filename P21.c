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
#include <stdbool.h>

//struct is used in storing the matched pair values of men and women
typedef struct pair {
  int men;
  int women;
  int check;
}pair;

void create_matrix_four(char content[50][50],int men[4][4],int women[4][4]);
void create_matrix_three(char content[50][50],int men[3][3],int women[3][3]);
void stable_matching_three(int n,int men[3][3],int women[3][3]);
void stable_matching_four(int n,int men[4][4],int women[4][4]);

//Driver function of the program
int main()
{
    char f_name[30];
    printf("Please enter the name of the File: ");
    scanf("%s",f_name);

    FILE* fptr = fopen(f_name,"r");
    if(fptr == NULL)
    {
        printf("File could not be opened\n");
        return 0;
    }

    char lines[50];
    char content[50][50];
    int line_count = 0;
    while(fgets(lines,sizeof(lines),fptr) != NULL)
    {
        strcpy(content[line_count],lines);
        line_count++;
    }
    fclose(fptr);
    int n = atoi(content[0]);
    int men[n][n];
    int women[n][n];

    if(n == 3)
    {
        create_matrix_three(content,men,women);
        stable_matching_three(n,men,women);
    }
    else if(n == 4)
    {
        create_matrix_four(content,men,women);
        stable_matching_four(n,men,women);
    }
    else
    {
        printf("Not a Valid Input\n");
        return 0;
    }

    return 0;
}

//Finds the stable match when n = 3 and prints the result
void stable_matching_three(int n,int men[3][3],int women[3][3])
{
    int with_Par[n];
    bool free_males[n];

    //all pairs are initialized to -1
    for(int i=0;i<n;i++)
    {
        with_Par[i] = -1;
        free_males[i] = true;
    }
    int free = n;
    while(free > 0)
    {
        int m;
        for(m=0;m<n;m++)
        {
            if(free_males[m])
            {
                break;
            }
        }
        for(int i=0;(i<n && free_males[m]);i++)  //checking if males are free before assigning them
        {
            int w = men[m][i];
            if(with_Par[w] == -1)
            {
                with_Par[w] = m;
                free_males[m] = false;
                free--;
            }
            else
            {
                int me = with_Par[w];    //Pairing males if they are yet not taken
                for(int q = 0;q<n;q++)
                {
                    if(women[w][i] == me)
                    {
                        break;
                    }
                    if(women[w][i] == m)  //Finding if current male has a better match with a previousaly matched women
                    {
                        with_Par[w] = m;
                        free_males[m] = false;
                        free_males[me] = true;
                        free--;
                    }
                }
            }
        }
    }

    pair my_pairs[3];   //Creting a pairs array to store the results
    for(int i=0;i<n;i++)
    {
        my_pairs[i].check = 0;
    }
    int w1 = -1;
    int w2 = -1;
    int w3 = -1;
    for(int i=0;i<n;i++)
    {
        if((with_Par[i]+1) < n)
        {
            my_pairs[i].men = i+1;
            my_pairs[i].women = with_Par[i]+1;
            my_pairs[i].check = 1;
            if((with_Par[i]+1) == 0)
            {
                w1 = with_Par[i]+1;
            }
            if((with_Par[i]+1) == 1)
            {
                w2 = with_Par[i]+1;
            }
            if((with_Par[i]+1) == 2)
            {
                w3 = with_Par[i]+1;
            }
        }
    }

    for(int i=0;i<n;i++)  //Checking if all the pairs are filled and none of the pairs are duplicate or have multiple values
    {
        if(my_pairs[i].check == 0)
        {
            if(w1 == -1)
            {
                my_pairs[i].men = i+1;
                my_pairs[i].women = 0;
            }
            if(w2 == -1)
            {
                my_pairs[i].men = i+1;
                my_pairs[i].women = 1;
            }
            if(w3 == -1)
            {
                my_pairs[i].men = i+1;
                my_pairs[i].women = 2;
            }
        }
    }

    printf("The output matrix is: \n");
    for(int i=0;i<n;i++)  //printing the final result to the screen
    {
        for(int j=0;j<n;j++)
        {
            if(j == my_pairs[i].women)
            {
                printf("1  ");
            }
            else
            {
                printf("0  ");
            }
        }
        printf("\n");
    }
}

//creating a pair of couple when n = 4 and prints the results
void stable_matching_four(int n,int men[4][4],int women[4][4])
{
    int with_Par[n];
    bool free_males[n];

    for(int i=0;i<n;i++)  //Initialize all the pairs with -1 value
    {
        with_Par[i] = -1;
        free_males[i] = true;
    }
    int free = n;
    while(free > 0)
    {
        int m;
        for(m=0;m<n;m++)
        {
            if(free_males[m])
            {
                break;
            }
        } 
        for(int i=0;i<n && free_males[m];i++) //chekc if males are free before assigning them partners 
        {
            int w = men[m][i];
            if(with_Par[w] == -1)
            {
                with_Par[w] = m;
                free_males[m] = false;
                free--;
            }
            else           //if free check the most compatible female pair
            {
                int me = with_Par[w];
                for(int q = 0;q<n;q++)
                {
                    if(women[w][i] == me)
                    {
                        break;
                    }
                    if(women[w][i] == m)
                    {
                        with_Par[w] = m;
                        free_males[m] = false;
                        free_males[me] = true;
                        free--;
                    }
                }
            }
        }
    }

    pair my_pairs[4];
    for(int i=0;i<n;i++)
    {
        my_pairs[i].check = 0;
    }
    int w1 = -1;
    int w2 = -1;
    int w3 = -1;
    int w4 = -1;

    for(int i=0;i<n;i++)  //check if there are no duplicate values
    {
        if((with_Par[i]+1) < n)
        {
            my_pairs[i].men = i+1;
            my_pairs[i].women = with_Par[i]+1;
            my_pairs[i].check = 1;
            if((with_Par[i]+1) == 0)
            {
                w1 = with_Par[i]+1;
            }
            if((with_Par[i]+1) == 1)
            {
                w2 = with_Par[i]+1;
            }
            if((with_Par[i]+1) == 2)
            {
                w3 = with_Par[i]+1;
            }
            if((with_Par[i]+1) == 3)
            {
                w4 = with_Par[i]+1;
            }
        }
    }

    for(int i=0;i<n;i++)
    {
        if(my_pairs[i].check == 0)
        {
            if(w1 == -1)
            {
                my_pairs[i].men = i+1;
                my_pairs[i].women = 0;
            }
            if(w2 == -1)
            {
                my_pairs[i].men = i+1;
                my_pairs[i].women = 1;
            }
            if(w3 == -1)
            {
                my_pairs[i].men = i+1;
                my_pairs[i].women = 2;
            }
            if(w4 == -1)
            {
                my_pairs[i].men = i+1;
                my_pairs[i].women = 3;
            }
        }
    }

    printf("The output matrix is: \n");
    for(int i=0;i<n;i++)  //print the final results to the screen
    {
        for(int j=0;j<n;j++)
        {
            if(j == my_pairs[i].women)
            {
                printf("1  ");
            }
            else
            {
                printf("0  ");
            }
        }
        printf("\n");
    }
}

//create a 3x3 matrix for both men and women priorities
void create_matrix_three(char content[50][50],int men[3][3],int women[3][3])
{
    char* delim = " ";
    for(int i=1;i<4;i++)  //create a 3x3 pattern for male pairs
    {
        char* word_1= strtok(content[i],delim);
        char* word_2 = strtok(NULL,delim);
        char* word_3 = strtok(NULL,delim);
        if(word_1 != NULL)
        {
            int point = atoi(word_1);   //convert a string to interger
            men[i-1][0] = point;
        }
        if(word_2 != NULL)
        {
            int point = atoi(word_2);
            men[i-1][1] = point;
        }
        if(word_3 != NULL)
        {
            int point = atoi(word_3);
            men[i-1][2] = point;
        }
    }

    for(int i=5;i<8;i++)  //create a 3x3 matrix for women pairs
    {
        char* word_1= strtok(content[i],delim);  //breaking the string to store resutls as integer values
        char* word_2 = strtok(NULL,delim);
        char* word_3 = strtok(NULL,delim);
        if(word_1 != NULL)
        {
            int point = atoi(word_1);
            women[i-5][0] = point;
        }
        if(word_2 != NULL)
        {
            int point = atoi(word_2);
            women[i-5][1] = point;
        }
        if(word_3 != NULL)
        {
            int point = atoi(word_3);
            women[i-5][2] = point;
        }
    }
}

//create a 4x4 matrix for both male and females 
void create_matrix_four(char content[50][50],int men[4][4],int women[4][4])
{
    char* delim = " ";
    for(int i=1;i<5;i++)  //create a 4x4 matric for males
    {
        char* word_1= strtok(content[i],delim);
        char* word_2 = strtok(NULL,delim);
        char* word_3 = strtok(NULL,delim);
        char* word_4 = strtok(NULL,delim);
        if(word_1 != NULL)
        {
            int point = atoi(word_1);
            men[i-1][0] = point;
        }
        if(word_2 != NULL)
        {
            int point = atoi(word_2);
            men[i-1][1] = point;
        }
        if(word_3 != NULL)
        {
            int point = atoi(word_3);
            men[i-1][2] = point;
        }
        if(word_4 != NULL)
        {
            int point = atoi(word_4);
            men[i-1][3] = point;
        }
    }

    for(int i=6;i<10;i++)   //create a 4x4 matric for females
    {
        char* word_1= strtok(content[i],delim);
        char* word_2 = strtok(NULL,delim);
        char* word_3 = strtok(NULL,delim);
        char* word_4 = strtok(NULL,delim);
        if(word_1 != NULL)
        {
            int point = atoi(word_1);
            women[i-6][0] = point;
        }
        if(word_2 != NULL)
        {
            int point = atoi(word_2);
            women[i-6][1] = point;
        }
        if(word_3 != NULL)
        {
            int point = atoi(word_3);
            women[i-6][2] = point;
        }
        if(word_4 != NULL)
        {
            int point = atoi(word_4);
            women[i-6][3] = point;
        }
    }
}