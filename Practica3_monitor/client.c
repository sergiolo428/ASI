/*Sergio Lopez Alejandro - Practica3_monitor*/

#include <stdio.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define CLAVE 0x78997795L

void espera(int n);
void ejer1();
void ejer2();
void ejer3();
void ejer4();
void ejer5();
void ejer6();
void ejer7();

int clave1;
char clave2[10];

int main(int argc, char** argv) {
    
    char c1;
    char c2;
    
    
    /*espera(1);
    ejer1();
    espera(2);
    ejer2();
    espera(3);
    ejer3();
    espera(4);
    ejer4();
    espera(5);
    ejer5();
    espera(6);
    ejer6();*/
    espera(7);
    ejer7();
    
    return (EXIT_SUCCESS);
}

void espera(int n){
    char a[128];
    printf("Enter para continuar con %d: ",n);
    fgets(a,sizeof(a),stdin);
}

void ejer1(){
    int idMem;
    
    if((idMem=shmget(CLAVE,1024,0666))==-1){
        printf("Error shmget: ");
        exit(EXIT_FAILURE);
    }
    
    int *punteroInt;
    char *punteroChar = (char *)shmat(idMem,NULL,0);
    punteroInt = (int *)punteroChar;
    
    int code1 = punteroInt[0];
    int offset = punteroInt[1];
    char code2[10];
    
    printf("Codigo 1: %d.\n",code1);
    printf("Offset: %d.\n",offset);
    
    strcpy(code2,punteroChar+offset);
    
    printf("Cerranbdo...\n");
    sleep(3);
    shmdt(punteroChar);
    
    clave1 = code1;
    strcpy(clave2,code2);
    
    printf("%d y %s\n",clave1,clave2);
    
}

void ejer2(){
    printf("Iniciando Parte 2...\n");
    int idMem;
    
    if((idMem=shmget(CLAVE,1024,0666|IPC_CREAT))==-1){
        printf("Error shmget: ");
        exit(EXIT_FAILURE);
    }
    printf("Obteniendo puntero...\n");
    
    char *punteroChar, *punteroChar2;
    if((punteroChar = (char *)shmat(idMem,NULL,0))==(char*)-1){
        printf("Error shmat: ");
        exit(EXIT_FAILURE);
    }
    
    char code1[10];
    int code2;
    
    char a[10];
    
    strcpy(punteroChar,clave2);
    
    sprintf(code1,"<%d>",clave1);
    
    printf("%s y %s\n",code1,clave2);
    
    code2 = (clave2[1]-'0')*100+(clave2[2]-'0')*10+(clave2[1]-'0');
    
    int punt2 = code2 + 16;
    
    punteroChar2 = punteroChar + punt2;
    
    strcpy(punteroChar2,code1);
    
    sleep(1);
    
    printf("Cerranbdo...\n");
    shmdt(punteroChar);
    
}

void ejer3(){
    int idSem = semget(CLAVE,1,0666 | IPC_CREAT);
    int cifras = 795;
    
    semctl(idSem,0,SETVAL,cifras);
    printf("Cerranbdo...\n");
    sleep(3);
    semctl(idSem,0,IPC_RMID);
}

void ejer4(){
    int idSem = semget(CLAVE,2,0666 | IPC_CREAT);
    
    semctl(idSem,0,SETVAL,1);
    semctl(idSem,1,SETVAL,2);
    printf("Cerranbdo...\n");
    sleep(3);
    semctl(idSem,0,IPC_RMID);
}

void ejer5(){
    int idSem = semget(CLAVE,1,0666);
    int idMem = shmget(CLAVE,1024,0666);
    
    int *punteroInt = (int *)shmat(idMem,NULL,0);
    int clave8;
    int cambio;
    
    struct sembuf up = {0,1,0};
    struct sembuf down = {0,-1,0};
    
    semop(idSem,&down,1);
    clave8 = punteroInt[0];
    printf("Clave: %d\n",clave8);
    cambio = -clave8;
    punteroInt[0]=cambio;
    printf("Saliendo seccion critica...\n");
    sleep(3);
    semop(idSem,&up,1);
}

void ejer6(){
    int idSem = semget(CLAVE,2,0666);
    int idMem = shmget(CLAVE,1024,0666);
    
    int *punteroInt = (int *)shmat(idMem,NULL,0);
    int clave10;
    int cambio;
    
    //sem
    struct sembuf up0 = {0,1,0};
    struct sembuf down0 = {0,-1,0};
    
    struct sembuf up1 = {1,1,0};
    struct sembuf down1 = {1,-1,0};
    
    struct sembuf down[2]={{0,-1,0},{1,-1,0}};
    struct sembuf up[2]={{0,1,0},{1,1,0}};
    
    semop(idSem,down,2);
    clave10 = punteroInt[0];
    cambio = -clave10;
    punteroInt[0]=cambio;
    printf("Saliendo seccion critica...\n");
    sleep(3);
    semop(idSem,up,2);
    
}

void ejer7(){
    int idMem = shmget(CLAVE,1024,0666);
    int idSem = semget(CLAVE,1,0666 | IPC_CREAT);
    
    struct sembuf down = {0,-1,0};
    struct sembuf up = {0,1,0};
    
    semop(idSem,&down,1);
    printf("Eliminando...\n");
    
    semctl(idSem,0,IPC_RMID);
    
    shmctl(idMem,IPC_RMID,NULL);
}
