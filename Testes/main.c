/*
*   O primeiro argumento sempre é o destino
*
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//registradores globais
int r[10];
int a, b, c, d, operation;

//flags
int flag_z,     //zero
    flag_g,     //greater
    flag_l,     //lesser
    flag_ge,    //greater-equals
    flag_le,    //lesser-equals
    flag_e,     //equals    
    flag_d;     //different

//resto
int resto;    

//struct para instruções
struct command{
    char instruction_part[3][30];           //cada instrução tem 3 partes(strings de tamanho 10)
};

struct memory
{
    int status;
    int content;
};

//array de instruções
struct command instruction_list[100];
struct memory memory_list[100];

int main(void)
{
    //getline
    FILE *fp, *fpMemory;
    char *line = NULL, *lineMemory = NULL;
    size_t len = 0, lenMemory = 0;
    ssize_t read, readMemory;

    //strtok
    char *pch, *pchMemory;
    
    //strtol
    char *pEnd, *pEndMemory;

    //instruction pointer
    int inst_pointer = 1;    

    fp = fopen("telefones.txt", "r");
    fpMemory = fopen("memory.txt", "r");
    if ((fp == NULL) || (fpMemory == NULL))
    {
        exit(EXIT_FAILURE);
    }

    int nInstructions = 0;                      //funciona no loop de leitura das instruções e serve como número total de insts
    int nData = 0;
    //loop para leitura do arquivo de instruções
    while ((read = getline(&line, &len, fp)) != -1)
    {        

        pch = strtok(line, " ");                //necessário para começar a dividir tokens
        //if(strlen(&line) < 3) continue;
        int i;                                  // 0 a 2
        for(i = 0; pch != NULL; i++)            //copia as instruções do arquivo pro array de instruções
        {             
            strcpy(instruction_list[nInstructions].instruction_part[i], pch);
            //printf("%s ", instruction_list[i].instruction_part[i]);
            pch = strtok(NULL, " ");
        }
        nInstructions++;
    }

    //loop para leitura do arquivo de dados
    while ((readMemory = getline(&lineMemory, &lenMemory, fpMemory)) != -1)
    {        

        pchMemory = strtok(lineMemory, " ");                //necessário para começar a dividir tokens
        
        if(*pchMemory == '\n')
        {
            nData++;
            continue;
        }

        memory_list[nData].content = (int)strtol(pchMemory, &pEndMemory, 10);
        memory_list[nData].status = 1;
        //printf("\nMemória posição %d: %d \n", nData, memory[nData]);
        nData++;
    }


    for(inst_pointer; inst_pointer <= nInstructions; inst_pointer++)                //loop do ponteiro de instruções
    {
        struct command current_command = instruction_list[inst_pointer - 1];        //copia instrução para variável da inst atual

        if(strcmp(current_command.instruction_part[0], "MOVE") == 0)                //coloca inteiro no registrador
        {            
            int reg_id = current_command.instruction_part[1][1] - 48;               //pega o número depois do r no arquivo de texto e converte para int
            int value = (int)strtol(current_command.instruction_part[2], &pEnd, 10);//converte o segundo argumento para int 

            r[reg_id] = value;            
        }
        else if(strcmp(current_command.instruction_part[0], "LOAD") == 0)           //coloca valor da memória no registrador;
        {
            int reg_id = current_command.instruction_part[1][1] - 48;               //pega o número depois do r no arquivo de texto e converte para int
            //int adress = (int)strtol(current_command.instruction_part[2], &pEnd, 10);
            int adress;
            
            if(current_command.instruction_part[2][0] == 'r')
            {
                int reg_id2 = current_command.instruction_part[2][1] - 48;
                adress = r[reg_id2];                
            }
            else{
                adress = (int)strtol(current_command.instruction_part[2], &pEnd, 10);

                //r[reg_id] += value;
            }

            if(memory_list[adress-1].status)
            {
                r[reg_id] = memory_list[adress-1].content;
            }
            else
            {
                printf("\nInvalid memory access\n");
                return -1;
            }
            //TODO
        }
        else if(strcmp(current_command.instruction_part[0], "STORE") == 0)          //coloca valor do registrador na memoria;
        {
            int reg_id = current_command.instruction_part[1][1] - 48;               //pega o número depois do r no arquivo de texto e converte para int
            //int adress = (int)strtol(current_command.instruction_part[2], &pEnd, 10);
            
            int adress;

            if(current_command.instruction_part[2][0] == 'r')
            {
                int reg_id2 = current_command.instruction_part[2][1] - 48;
                adress = r[reg_id2];                
            }
            else{
                adress = (int)strtol(current_command.instruction_part[2], &pEnd, 10);

                //r[reg_id] += value;
            }

            memory_list[adress-1].content = r[reg_id];
            memory_list[adress-1].status = 1;
            //TODO
        }
        else if(strcmp(current_command.instruction_part[0], "COPY") == 0)           //copia valor do registrador 2 no registrador 1
        {
            int reg_id1 = current_command.instruction_part[1][1] - 48;
            int reg_id2 = current_command.instruction_part[2][1] - 48;                
            
            r[reg_id1] = r[reg_id2];            
        }
        else if(strcmp(current_command.instruction_part[0], "ADD") == 0)
        {
            int reg_id = current_command.instruction_part[1][1] - 48;
            if(current_command.instruction_part[2][0] == 'r')
            {
                int reg_id2 = current_command.instruction_part[2][1] - 48;

                r[reg_id] += r[reg_id2];
            }
            else{
                int value = (int)strtol(current_command.instruction_part[2], &pEnd, 10);

                r[reg_id] += value;
            }
        }
        else if(strcmp(current_command.instruction_part[0], "SUB") == 0)
        {
            int reg_id = current_command.instruction_part[1][1] - 48;
            if(current_command.instruction_part[2][0] == 'r')
            {
                int reg_id2 = current_command.instruction_part[2][1] - 48;

                r[reg_id] -= r[reg_id2];
            }
            else{
                int value = (int)strtol(current_command.instruction_part[2], &pEnd, 10);

                r[reg_id] -= value;
            }
        }
        else if(strcmp(current_command.instruction_part[0], "MUL") == 0)
        {
            int reg_id = current_command.instruction_part[1][1] - 48;
            if(current_command.instruction_part[2][0] == 'r')
            {
                int reg_id2 = current_command.instruction_part[2][1] - 48;

                r[reg_id] *= r[reg_id2];
            }
            else{
                int value = (int)strtol(current_command.instruction_part[2], &pEnd, 10);

                r[reg_id] *= value;
            }
        }
        else if(strcmp(current_command.instruction_part[0], "DIV") == 0)
        {
            int reg_id = current_command.instruction_part[1][1] - 48;
            if(current_command.instruction_part[2][0] == 'r')
            {
                int reg_id2 = current_command.instruction_part[2][1] - 48;
                
                resto = r[reg_id] % r[reg_id2];
                r[reg_id] /= r[reg_id2];
            }
            else{
                int value = (int)strtol(current_command.instruction_part[2], &pEnd, 10);
                
                resto = r[reg_id] % value;
                r[reg_id] /= value;                
            }
        }
        else if(strcmp(current_command.instruction_part[0], "COMP") == 0)
        {   
            //flags zeradas para facilitar
            flag_z = 0;     //zero
            flag_g = 0;     //greater
            flag_ge = 0;     
            flag_l = 0;     //lesser
            flag_le = 0;     
            flag_e = 0;     //equals    
            flag_d = 0;     //different

            int reg_id1 = current_command.instruction_part[1][1] - 48;
            int arg1 = r[reg_id1];
            int arg2;

            if(current_command.instruction_part[2][0] == 'r')
            {
                int reg_id2 = current_command.instruction_part[2][1] - 48;
                arg2 = r[reg_id2];               
            }
            else{
                int value = (int)strtol(current_command.instruction_part[2], &pEnd, 10);
                arg2 = value;                             
            }

            //comparações começam aqui
            if(arg1 == 0) flag_z = 1;
            if(arg1 > arg2) flag_g = 1;
            if(arg1 >= arg2) flag_ge = 1;
            if(arg1 < arg2) flag_l = 1;
            if(arg1 <= arg2) flag_le = 1;
            if(arg1 == arg2) flag_e = 1;
            if(arg1 != arg2) flag_d = 1;
            
        }
        else if(strcmp(current_command.instruction_part[0], "JUMP") == 0)
        {            
                int inst_adress = (int)strtol(current_command.instruction_part[1], &pEnd, 10);
                inst_pointer = inst_adress - 1;         //subtrai 1 para compensar o incremento da variável de controle inst_pointer                
        }
        else if(strcmp(current_command.instruction_part[0], "JZ") == 0)
        {
            if(flag_z == 1)
            {
                int inst_adress = (int)strtol(current_command.instruction_part[1], &pEnd, 10);
                inst_pointer = inst_adress - 1;         //subtrai 1 para compensar o incremento da variável de controle inst_pointer                
            }
        }
        else if(strcmp(current_command.instruction_part[0], "JG") == 0)
        {
            if(flag_g == 1)
            {
                int inst_adress = (int)strtol(current_command.instruction_part[1], &pEnd, 10);
                inst_pointer = inst_adress - 1;                
            }
        }
        else if(strcmp(current_command.instruction_part[0], "JGE") == 0)
        {
            if(flag_ge == 1)
            {
                int inst_adress = (int)strtol(current_command.instruction_part[1], &pEnd, 10);
                inst_pointer = inst_adress - 1;                
            }
        }
        else if(strcmp(current_command.instruction_part[0], "JL") == 0)
        {
            if(flag_l == 1)
            {
                int inst_adress = (int)strtol(current_command.instruction_part[1], &pEnd, 10);
                inst_pointer = inst_adress - 1;                
            }
        }
        else if(strcmp(current_command.instruction_part[0], "JLE") == 0)
        {
            if(flag_le == 1)
            {
                int inst_adress = (int)strtol(current_command.instruction_part[1], &pEnd, 10);
                inst_pointer = inst_adress - 1;                
            }
        }
        else if(strcmp(current_command.instruction_part[0], "JE") == 0)
        {
            if(flag_e == 1)
            {
                int inst_adress = (int)strtol(current_command.instruction_part[1], &pEnd, 10);
                inst_pointer = inst_adress - 1;                
            }
        }
        else if(strcmp(current_command.instruction_part[0], "JD") == 0)
        {
            if(flag_d == 1)
            {
                int inst_adress = (int)strtol(current_command.instruction_part[1], &pEnd, 10);
                inst_pointer = inst_adress - 1;                
            }
        }
        else if(strcmp(current_command.instruction_part[0], "GETINT") == 0)     //recebe valor inteiro do usuário e coloca num reg
        {
            int reg_id = current_command.instruction_part[1][1] - 48;
            scanf("%d", &r[reg_id]);
        }
        else if(strcmp(current_command.instruction_part[0], "ESCREVAL") == 0)   //imprime valor de reg na tela
        {
            int reg_id = current_command.instruction_part[1][1] - 48;
            printf("%d\n", r[reg_id]);
        }
        else if(strcmp(current_command.instruction_part[0], "PRINT") == 0)   //imprime valor de reg na tela
        {
            //int reg_id = current_command.instruction_part[1][1] - 48;
            printf("%s\n", current_command.instruction_part[1]);
        }
        else if(strcmp(current_command.instruction_part[0], "RET") == 0)        //sai do programa
        {
            int status = (int)strtol(current_command.instruction_part[1], &pEnd, 10);
            printf("Program ended with status %d\n", status);

            fpMemory = fopen("memory.txt", "w");
            if (fpMemory == NULL)
            {
                exit(EXIT_FAILURE);
            }

            for(int i = 0; i < 100; i++)                                        //Imprime toda a saída no arquivo txt de dados
            {
                if(memory_list[i].status)
                {
                    fprintf(fpMemory, "%d\n", memory_list[i].content);
                }
                else
                {
                    fprintf(fpMemory, "\n");
                }
            }

            break;
        }
        else if(strcmp(current_command.instruction_part[0], "ESCREVA") == 0)   //imprime valor de reg na tela
        {
            int reg_id = current_command.instruction_part[1][1] - 48;
            printf("%d", r[reg_id]);
        }
        
        //printf("%2d - %4d   %4d   %4d   %4d\n",inst_pointer, r[0], r[1], r[2], r[3]);            //printa registradores teste
        //printf("%d  ", inst_pointer);
    }


    fclose(fp);
    fclose(fpMemory);
    free(line);
    free(lineMemory);
    exit(EXIT_SUCCESS);
}