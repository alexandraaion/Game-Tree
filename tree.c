#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "tree.h"

//functii task1
//functia aloca memorie pentru un nod
void initNode(Node** node, int n, int m)
{
    int i;
    (*node)->matrix = (char **)malloc(n * sizeof(char *)); 
    for (i = 0; i < n; i++) 
        (*node)->matrix[i] = (char *)malloc(m * sizeof(char));
    (*node)->children = (Node**)malloc(m*sizeof(Node*));
    (*node)->nrOfChildren = 0;
}

//functia verifica daca o matrice este castigatoare
int checkWin(char **matrix, int n, int m) {
    int col, row;
    //verificare pe linii
    for(row = 0; row < n; row++)
        for(col = 0; col < m - 3; col++)
            if(matrix[row][col] != '-' && matrix[row][col] == matrix[row][col+1]
                && matrix[row][col] == matrix[row][col + 2] 
                && matrix[row][col]== matrix[row][col + 3])
                return 1;

    //verificare pe coloane
    for(col = 0; col < m; col++)
        for(row = 0; row < n-3; row++)
            if(matrix[row][col] != '-' && matrix[row][col] == matrix[row+1][col]
                && matrix[row][col] == matrix[row+2][col] 
                && matrix[row][col]== matrix[row+3][col])
                return 1;

    //verificare pe diagonala "\"
    for(row = 0; row < n - 3; row++)
        for(col = 0; col < m - 3; col++)
            if(matrix[row][col] != '-' 
                && matrix[row][col] == matrix[row+1][col+1]
                && matrix[row][col] == matrix[row+2][col+2] 
                && matrix[row][col] == matrix[row+3][col+3])
                return 1;

    //verificare pe diagonala "/"
    for(row = 0; row < n - 3; row++)
        for(col = 3; col < m; col++)
            if(matrix[row][col] != '-' 
                && matrix[row][col] == matrix[row+1][col-1]
                && matrix[row][col] == matrix[row+2][col-2] 
                && matrix[row][col] == matrix[row+3][col-3])
                return 1;

    return 0;

}

//functia creeaza un nou nivel in arbore
void nextLevel(Node **node, int n, int m, char next)
{
    int idx = 0, i;
    int check = checkWin((*node)->matrix, n, m);

    if (check == 1)             //verificam daca matricea este castigatoare
        return;
    initChildren(node, &idx, n, m, next);   //initializez copii nodului

    //schimb caracterul ce trebuie adaugat
    if (next == 'R') {
            next = 'B';
    } else if (next == 'B') {
            next = 'R';
    }
    //pentru fiecare copil al nodului, creez un nou nivel in arbore
    for (i = 0; i < (*node)->nrOfChildren; i++) {
        Node* child = (*node)->children[i];

        nextLevel(&child, n, m, next);
    }
}

//functia initializeaza, recursiv, copii unui nod
void initChildren(Node **node, int *idx, int n, int m, char next)
{
    char aux[n][m];
    int i, j, poz;
    int c = (*node)->nrOfChildren;

    //salvez matricea parintelui intr-o variabila auxiliara
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            aux[i][j] = (*node)->matrix[i][j];

    //gasim prima pozitie libera, unde putem adauga, reprezentata cu "-" 
    for (poz = *idx; poz < m; poz++) {
        if (aux[0][poz] == '-'){
            i = 0;
            while ((aux[i][poz] == '-') && (i < n))     //caracterul cade in jos
                i++;
            
            aux[i-1][poz] = next;
            (*node)->nrOfChildren++;
            
            //aloca un nou copil
            (*node)->children[c] = (Node*)malloc(sizeof(Node));
            Node* child = (*node)->children[c];

            //initializez un nou copil
            initNode(&child, n, m);
            for (i = 0; i < n; i++)
                for (j = 0; j < m; j++)
                    child->matrix[i][j] = aux[i][j];

            break;
        }
    }

    //actualizam pozitia de la care sa incepem cautarea de adaugare
    *idx = poz + 1; 
    
    //daca mai putem cauta, o facem :)
    if (*idx < m) {
        initChildren(node, idx, n, m, next);
    }

}

//functia printeaza recursiv arborele
void printTree(Node *root, int tabs, int n, int m, FILE *out)
{
    int i,j,k;

    //printam matricea parintelui
    for (i = 0; i < n; i++) {
        //respectam identarea
        for (k = 0; k < tabs; k++)
                fprintf(out, "\t");
        for (j = 0; j < m; j++){
            if (j == m-1){
                fprintf(out, "%c", root->matrix[i][j]);
            } else {
                fprintf(out, "%c ", root->matrix[i][j]);
            }
        }
        fprintf(out, "\n");
    }
    fprintf(out, "\n");

    //actualizam identarea
    tabs++;

    //pentru fiecare copil, apelam functia
    for (i = 0; i < root->nrOfChildren; i++) {
        Node* child = root->children[i];

        printTree(child, tabs, n, m, out);
    }
}

//functia elibereaza memoria arborelui
void deleteTree(Node **node, int n, int m)  
{ 
    if (*node == NULL) return; 
    int i;

    for (i = 0; i < (*node)->nrOfChildren; i++) {
        deleteTree(&(*node)->children[i], n, m);
    }

    for (i = 0; i < n; i++) 
        free((*node)->matrix[i]);
    free((*node)->matrix);
    free((*node)->children);
 
    free(*node); 
}

//functii task2
//functia creeaza un nod in functie de token-ul citit de la tastatura
void createNode(Node2 **node, char* token, int nivel, int h)
{
    int nr;

    //verificam daca este valoarea unei frunze sau numarul de copii
    if (token[0] == '(') {
        sscanf(token,"(%d)",&nr);
        //realizam initializarile
        (*node)->nrOfChildren = nr;
        (*node)->children = (Node2 **)malloc(nr*sizeof(Node*));
        (*node)->niv = nivel;

        //verificam daca se afla pe un nivel de minim sau de maxim
        if (nivel % 2 == 1) {
            (*node)->val = INT_MIN;
        } else {
            (*node)->val = INT_MAX;
        }
    } else if (token[0] == '[') {
        sscanf(token,"[%d]",&nr);
        //realizam initializarile
        (*node)->val = nr;
        (*node)->niv = nivel;
        (*node)->nrOfChildren = 0;      //marcam nodul ca fiind frunza
    }
}

//functia creeaza recursiv arborele
void createChildren(Node2 **node, char*** arg, int nr_line, int nivel, int *idx, 
                    int h)
{
    //nivelul nodului in arbore
    nivel++;
    if (nr_line < h){
        int i;

        for (i = 0; i < (*node)->nrOfChildren; i++) {
            (*node)->children[i] = (Node2*)malloc(sizeof(Node2));
            Node2 *child = (*node)->children[i];

            //creez nodul
            createNode(&child, arg[nr_line][idx[nr_line]], nivel, h);
            idx[nr_line]++;
            nr_line++;

            createChildren(&child, arg, nr_line, nivel, idx, h);
            nr_line--;
        }
    }
}


//functia calculeaza recursiv valorile din nodurile interne ale arborelui
void minmax(Node2 **node){
    int j;

    for (j = 0; j < (*node)->nrOfChildren; j++) {
        Node2 *child = (*node)->children[j];

        //daca nu suntem pe penultimul nivel, mergem si mai jos 
        if (child->nrOfChildren != 0) {
            minmax(&child);                                   
        }

        /*calculez valoarea minima sau maxima dintre copii, in functie de 
        nivelul din arbore pe care se afla nodul
        */
        if ((*node)->niv % 2 == 1) {
                if ((*node)->val < child->val){
                    (*node)->val = child->val;
                }
            } else {
                if ((*node)->val > child->val){
                    (*node)->val = child->val;
                }
            }         
    }
}

//functia printeaza rebursiv arborele
void printTree2(Node2 **node, int tabs, FILE* out) {
    int i;

    //respecatm identarea
    for(i = 0; i < tabs; i++)
        fprintf(out, "\t");
    fprintf(out, "%d\n", (*node)->val);

    //actualizam identarea
    tabs++;
    for (i = 0; i < (*node)->nrOfChildren; i++) {
        Node2 *child = (*node)->children[i];

        printTree2(&child, tabs, out);
    }
}

//functia elibereaza recursiv memoria arborelui
void deleteTree2(Node2 **node)  
{ 
    if (*node == NULL) return; 
    int i;

    for (i = 0; i < (*node)->nrOfChildren; i++)
        deleteTree2(&(*node)->children[i]);

    if ((*node)->nrOfChildren != 0)
        free((*node)->children);
 
    free(*node); 
}