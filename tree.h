#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

//structura pentru task-ul 1
typedef struct Node {
    char **matrix;   
    struct Node **children; 
    int nrOfChildren; 
} Node;

//structura pentru task-ul 2
typedef struct Node2 {
    int val;
    int niv;   
    struct Node2 **children; 
    int nrOfChildren; 
} Node2;

//functii task1
void initChildren(Node **node, int *idx, int n, int m, char next);
void initNode(Node **node, int n, int m);
void nextLevel(Node **node, int n, int m, char next);
int checkWin(char **matrix, int n, int m);
void printTree(Node *root, int tabs, int n, int m, FILE* out);
void deleteTree(Node **node, int n, int m);

//functii task2
void createNode(Node2 **node, char *token, int nivel, int h);
void createChildren(Node2 **node, char*** arg, int nr_line, int nivel, int *idx, 
                    int h);
void minmax(Node2 **root);
void printTree2(Node2 **node, int tabs, FILE* out);
void deleteTree2(Node2 **node);