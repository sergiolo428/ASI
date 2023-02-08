/*Sergio Lopez Alejandro Practica1_Threads*/

#include <stdio.h>
#include <stdlib.h>


#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>

#define SIZE 128

void readAndSave();
void handlerHilo();
void handler();
void *threadFun(void *arg);

int main(int argc, char** argv) {
    
    int pid1,pid2,ppid;
    
    pid1=fork();
    if(pid1==0){
        /*Hijo1*/
        ppid=getppid();
        signal(SIGUSR1,handlerHilo);
        pause();
        printf("Hijo1 Muere.\n");
        kill(ppid,SIGUSR1);
        exit(0);
    }else{
     pid2=fork();
     if(pid2==0){
         /*Hijo2*/
         ppid=getppid();
         readAndSave();

         kill(pid1,SIGUSR1);
         printf("Hijo 2 Muere.\n");
         exit(0);         
     }else{
         /*Padre*/
         signal(SIGUSR1,handler);
         pause();
         printf("Padre Muere.\n");
         exit(0);
     }
    }
        
    return (EXIT_SUCCESS);
}

void readAndSave(){
    
    FILE *f;
    int len;
    char buffer[SIZE];
    f=fopen("texto.txt","w");
    printf("Introduce tu frase: ");
    fgets(buffer,sizeof(buffer),stdin);
    len=strlen(buffer);
    f=fopen("texto.txt","w");
    
    if(buffer[len-1]=='\n'){
        buffer[len-1]='\0';
    }
    
    fwrite(buffer,sizeof(char),strlen(buffer),f);
    fclose(f);
}

void handlerHilo(){
    printf("handlerHilo ejecutado.\n");
    pthread_t thread;
    
    pthread_create(&thread,NULL,threadFun,NULL);
    pthread_join(thread,NULL);
}

void handler(){
    printf("Señal obtenida.\n");
}

void* threadFun(void *arg){
    printf("This is a thread.\n");
    printf("Paso a mayusculas e imprimo.\n");
    pthread_exit(NULL);
    printf("Sal.\n");
}