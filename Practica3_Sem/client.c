/*Sergio Lopez Alejandro*/

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <ctype.h>
#include <sys/shm.h>


// idSem = semget(SEM_KEY,cantidad_semaforos,0666 | IPC_CREAT) --> Crear semaforo
// idSem = semget(SEM_KEY,cantidad_semaforos,0666 | IPC_CREAT) --> Acceder a semaforos
// semctl(idSem, numero_semaforo(0/1/2),comando,opciones(Opcional))
// semop(idSem,*sops,catidad de operaciones)

//ej: {{0,1,0},{1,-1,0}

//Ver semaforos desde consola:
//ipcs -s

//Borrar semaforo desde consola
//ipcrm -s <sem_id> 

//Limpiar buffer para cuando querramos hacer un scanf y luego un fgets:
//scanf
//fgetc(stdin);
//fgets

//IMPORTANTE

//User sscanf para pasar de string a int con cierto formato:

//sscanf(str,"<%d>",&num) --> Guardamos en num el intero que esta entre las dos flechas

//Usar sprintf para pasar de int a string con cierto formato:

//sprintf(str,<%d>,num); --> Guardamos en el string, el entero junto con dos flechas


#define SEM_KEY 0x78997795L
#define CLAVE 0x78997795L

//Estructura donde identificaremos la operacion

 /*
 struct sembuf{
     short sem_num;
     short sem_op;
     short sem_flag;
 };
  */
 
 //Sube el semaforo 0 en 1 valor (0,1,0)
 
 
//Podemos decalrar las operaciones asi

 
 
 union semun{
     int val;
     struct semid_ds *buf;
     unsigned short *array;
 };
 
 void ejemplosem();
 void ejerciciosem();

int main(int argc, char** argv) {
    
    //ejemplosem();
    ejerciciosem();
    
    
    return (EXIT_SUCCESS);
}

void ejemplosem(){
   int idSem;
   
   
   struct sembuf down0 = {0, -1, 0};
   struct sembuf up0 = {0, 1, 0};
   
   struct sembuf down1 = {1, -1, 0};
   struct sembuf up1 = {1, 1, 0};
   
   //struct sembuf sembufs[2] = {{0,1,0},{1,-1,0}};
   //o
   struct sembuf sembufs[2] = {up0,down1};
    
   
   if((idSem = semget(SEM_KEY,2,0666|IPC_CREAT))==-1) {
       perror("Error: semget 1");
       exit(1);
   }
   printf("Mi semaforo es %d\n",idSem);
   
   //Inicializamos semaforos
   union semun arg;
   //Iniciamos el semaforo 0 a 1
   if(semctl(idSem,0,SETVAL,1)==-1){
       perror("Error: semctl 0");
       exit(1);
   }
   
   //Iniciamos el semaforo 1 a 0
   if(semctl(idSem,1,SETVAL,1)==-1){
       perror("Error: semctl 1");
       exit(1);
   }
   
   printf("Valor primer semaforo: %d\n",semctl(idSem,0,GETVAL));
   printf("Valor segundo semaforo: %d\n",semctl(idSem,1,GETVAL));
   
   if(semop(idSem,sembufs,2)==-1){
       perror("Error: semop");
       exit(1);
   }
   
   printf("Valor primer semaforo: %d\n",semctl(idSem,0,GETVAL));
   printf("Valor segundo semaforo: %d\n",semctl(idSem,1,GETVAL));
   
   if(semctl(idSem,0,IPC_RMID)==-1){
       perror("Error: IPC_RMID 0");
       exit(1);
   }
}

void ejerciciosem(){
    
    int idSem;
    int idMem;
    
    char frase_normal[128];
    char frase_mayus[128];
    
    char *punteroChar;
    
    struct sembuf up = {0,1,0};
    struct sembuf down = {0,-1,0};
    
    idMem = shmget(CLAVE,1024,0666);
    punteroChar = shmat(idMem,NULL,0);
    if((idSem=semget(SEM_KEY,1,0666))==-1){
        perror("Error: semget");
    }
        
    //---------- Cliente ----------
    //Obtengo frase del teclado
    printf("Introduce tu frase: ");
    fgets(frase_normal,128,stdin);
    
    //Guardo en memoria
    if(frase_normal[strlen(frase_normal)-1]=='\n'){
        frase_normal[strlen(frase_normal)-1]='\0';
    }
    strcpy(punteroChar,frase_normal);
    //up()
    if((semop(idSem,&up,1))==-1){
        perror("Error: up");
    }
    
    //down()
    if((semop(idSem,&down,1))==-1){
        perror("Error: down");
    }
    
    //Lee Mem
    strcpy(frase_mayus,punteroChar+sizeof(frase_normal));
    
    //Hace print
    printf("La frase es: %s\n",frase_mayus);
}

