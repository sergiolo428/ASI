/*Sergio Lopez Alejandro - Practica2_ColasCliente */

/*
 ------------------------------COSAS IMPORTANTES---------------------------
 
 * 1. Solo el servidor debera hacer IPC_CREAT en el msgget
 * 2. La forma de declarar y posteriormente crear estructuras es:
 * *Recordando que la declaracion se hace fuera del main
 * 
 * struct Nombre{
 *      int a;
 *      char b[SIZE];
 * };
 * 
 * struct Nombre estructura;
 * 
 */

#include <stdio.h>
#include <stdlib.h>

#include <sys/msg.h>
#include <ctype.h>
#include <string.h>

#define CLAVE 0x78997795L
#define SIZE 128

struct Msg{
        long canal;
        char text[SIZE];
    };

int main(int argc, char** argv) {
    
    
    int idCola;
    struct Msg mensaje1;
    int canalEscritura=1;
    int canalLectura=2;
    
    idCola = msgget(CLAVE,0666);
    
    printf("Introduce la frase: ");
    fgets(mensaje1.text,sizeof(mensaje1.text),stdin);
    mensaje1.text[strlen(mensaje1.text)-1]='\0';
    mensaje1.canal=canalEscritura;
    
    printf("Mandando...\n");
    msgsnd(idCola,&mensaje1,sizeof(mensaje1.text),0);
    
    printf("Escuchando...\n");
    msgrcv(idCola,&mensaje1,sizeof(mensaje1),canalLectura,0);
    
    printf("Mi nueva frase es: %s.\n",mensaje1.text);
    
    return (EXIT_SUCCESS);
}

