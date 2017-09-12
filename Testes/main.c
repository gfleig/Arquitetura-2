#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//registradores globais
int r[10];
int a, b, c, d, operation;

int main(void)
{
    //getline
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    //strtok
    char *pch;
    
    //strtol
    char *pEnd;

    fp = fopen("teste.txt", "r");
    if (fp == NULL)
    {
        exit(EXIT_FAILURE);
    }

    while ((read = getline(&line, &len, fp)) != -1)
    {
        //printf("Retrieved line of length %zu : ", read);
        //printf("%s\n", line);
        pch = strtok(line, " ");                //necessário para começar a dividir tokens
        while(pch != NULL)
        {
            //printf("%s ", pch);            
            if(strcmp(pch, "LOAD") == 0)
            {
                operation = 0;

                pch = strtok(NULL, " ");
                printf("Registrador: %s\n", pch);
                int rId = pch[1] - 48;

                printf("%d\n", rId);

                pch = strtok(NULL, " ");
                long int value = strtol(pch, &pEnd, 10);
                r[rId] = value;

                printf("Valor do registrador r%d: %ld", rId, value);
                
            }
            pch = strtok(NULL, " ");
        }

    }

    free(line);
    exit(EXIT_SUCCESS);
}