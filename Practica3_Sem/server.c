
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <ctype.h>
#include <sys/shm.h>

#define SEM_KEY 0x78997795L
#define CLAVE 0x78997795L

int main(int argc, char** argv) {
    //---------- Servidor ----------
    //Inicializamos el semaforo a 0
    int idSem;
    int idMem;
    int i=0;
    
    char frase[128];
    
    char *punteroChar;
    char *punteroChar2;
    
    struct sembuf up = {0,1,0};
    struct sembuf down = {0,-1,0};
    
    
    idMem = shmget(CLAVE,1024,0666|IPC_CREAT);
    punteroChar = shmat(idMem,NULL,0);
    punteroChar2 = punteroChar + 128;
    if((idSem=semget(SEM_KEY,1,0666 | IPC_CREAT))==-1){
        perror("Error: semget");
    }
    
    if(semctl(idSem,0,SETVAL,0)==-1){
        perror("Error: semctl - Init");
    }
    
    printf("Servidor iniciado.\n");
    
    //While{
    while(1){  
    
    
    //down
    if(semop(idSem,&down,1)==-1){
        perror("Error: down");
    }
    //Lee Mem
    strcpy(frase,punteroChar);
    
    printf("Frase obtenida: %s\n",frase);
    
    //Pasa a mayus
    i=0;
    while(frase[i]!='\0'){
        frase[i]=toupper(frase[i]);
        i++;
    }
    //Guarda en Mem
    strcpy(punteroChar2,frase);
    //up
    if(semop(idSem,&up,1)==-1){
        perror("Error: up");
    }
    //Cierre while
    }
    
    return (EXIT_SUCCESS);
}

