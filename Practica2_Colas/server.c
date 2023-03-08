/*Sergio Lopez Alejandro - Practica2_ColasServer */

#include <stdio.h>
#include <stdlib.h>

#include <sys/msg.h>
#include <ctype.h>

#include <stdio_ext.h>


#define SIZE 128
#define CLAVE 0x78997795L

struct Msg{
        long canal;
        char text[SIZE];
    };
    
    
int main(int argc, char** argv) {
    
    
    int idCola;
    
    int canalLectura=1;
    int canalEscritura=2;
    int i=0;
    struct Msg mensaje1;
    
    idCola= msgget(CLAVE,0666|IPC_CREAT);
    
    printf("Escuchando...\n");
    msgrcv(idCola,&mensaje1,sizeof(mensaje1.text),canalLectura,0);
    printf("He obtenido: %s.\n",mensaje1.text);
    
    while(mensaje1.text[i]!='\0'){
        mensaje1.text[i]=toupper(mensaje1.text[i]);
        i++;
    }
    
    mensaje1.canal=canalEscritura;
    printf("Mandando en mayusculas...\n");
    msgsnd(idCola,&mensaje1,sizeof(mensaje1.text),0);

    return (EXIT_SUCCESS);
}

