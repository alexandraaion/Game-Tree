#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

int main(int argc, char* argv[])
{
    FILE *in = fopen(argv[2], "r");
    FILE *out = fopen(argv[3], "w");

    if (strcmp(argv[1], "-c1") == 0) {
        int n, m;
        char next;
        int i, j;
        int tabs = 0;
        Node *root = (Node *)malloc(sizeof(Node));
        
        fscanf(in, "%d %d %c ", &n, &m, &next);  
        initNode(&root, n, m);                      //creez radacina
        for (i = 0; i < n; i++)
            for (j = 0; j < m; j++)
                fscanf(in, "%c ", &(root->matrix[i][j]));
        
        nextLevel(&root, n, m, next);               //creez arborele

        printTree(root, tabs, n, m, out);          //printez arborele

        deleteTree(&root, n, m);                    //eliberez memoria
        
    } else if (strcmp(argv[1], "-c2") == 0) {
        int h = 0, i, j, nr_line = 0, nivel = 1;
        int tabs = 0;
        char *buf = NULL;
        char *token = NULL;
        size_t len = 0;
        int *idx;
        char ***arg;

        fscanf(in, "%d ", &h);
        idx = (int *)calloc(h, sizeof(int));
        arg = (char ***)malloc(h * sizeof(char **)); 

        for (i = 0; i < h; i++) {
            arg[i] = (char **)malloc(100000 * sizeof(char*));
            for (j = 0; j < 100000; j++)
                arg[i][j] = (char *)malloc(10 * sizeof(char));
        }

        for (i = 0; i < h; i++) {
            getline(&buf, &len, in);
            token = strtok(buf, " ");
            int idx = 0;
            while (token != NULL) {
                strcpy(arg[i][idx], token);
                token = strtok(NULL, " ");
                idx++;
            }
        }


        Node2 *root = (Node2 *)calloc(1, sizeof(Node2));

        createNode(&root, arg[0][0], nivel, h);     //creez radacina        
        nr_line++;                                  //din a cata linie citim

        createChildren(&root, arg, nr_line, nivel, idx, h);     //creez arborele

        minmax(&root);                  //calculez valorile din arbore

        printTree2(&root, tabs, out);   //printez arborele
    
        deleteTree2(&root);             //eliberez memoria 

        for (i = 0; i < h; i++) {
            for (j = 0; j < 100000; j++) 
                free(arg[i][j]);
            free(arg[i]);
        }
        
        free(arg);
        free(idx);
        free(buf);
        
    }


    fclose(in);
    fclose(out);
    return 0;
}
