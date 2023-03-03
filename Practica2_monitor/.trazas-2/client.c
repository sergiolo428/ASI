
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

void ejer1();
void ejer2();
void ejer3();
void ejer4();
void ejer5();
void ejer6();
void ejer7();

#define SIZE 128
#define CLAVE 0x78997795L

int main(int argc, char** argv) {
    
    int ejer=1;     
    
    while(ejer!=0){
        printf("Introduce el numero de ejercicio: ");
        scanf("%d",&ejer);
        
        switch(ejer){
            case 1:
                printf("Ejercicio 1:\n");
                ejer1();
                break;
            case 2:
                printf("Ejercicio 2:\n");
                printf("Seleccione el ejercicio 1 para realizar los ejercicios 1 y 2.\n");
                break;
            case 3:
                printf("Ejercicio 3:\n");
                ejer3();
                break;
            case 4:
                printf("Ejercicio 4:\n");
                break;
            case 5:
                printf("Ejercicio 5:\n");
                break;
            case 6:
                printf("Ejercicio 6:\n");
                break;
            case 7:
                break;
            default:
                exit(0);
                break;
        }
    }
    
    
    return (EXIT_SUCCESS);
}

void ejer1(){
    //EJER 1
    //FIFO
    //mkfifo("",0666)
    //fd=open("",0_RDWR)
    //write(fd,buffer,strlen(buffer))
    //read(fd,buffer,sizeof(buffer))
    
    int fd1,fd2,wait;
    char buffer1[SIZE];
    
    printf("Abriendo FIFO.\n");
    fd1 = open("/tmp/fifo_monitor_1",O_RDWR);
    printf("Leyendo FIF.\n");
    read(fd1,buffer1,sizeof(buffer1));
    printf("%s leido.\n",buffer1);
    
    printf("Pulsa 2 para continuar: ");
    scanf("%d",&wait);
    //fgets(wait,sizeof(wait),stdin);
    
    printf("Creando FIFO-2.\n");
    mkfifo("/tmp/fifo_monitor_2",0666);
    fd2=open("/tmp/fifo_monitor_2",O_RDWR);
    
    printf("Escribiendo en FIFO-2.\n");
    write(fd2,buffer1,strlen(buffer1));
    sleep(1);
    printf("Leyendo de FIFO-2.\n");
    read(fd1,buffer1,sizeof(buffer1));
    
    printf("Escribiendo en FIFO-2.\n");
    write(fd2,buffer1,strlen(buffer1));
    
    sleep(1);
    close(fd1);
    close(fd2);
}

struct Msg{
    long tipo;
    char mensaje[SIZE];
};

void ejer3(){
    
    struct Msg mensaje1;
    struct Msg mensaje2;
    int wait,idCola2;
    int idCola = msgget(CLAVE,0666);
    char str1[SIZE],str[SIZE];
    
    msgrcv(idCola,&mensaje1,sizeof(mensaje1.mensaje),0,0);//Por que es en el 0??
    
    printf("Leido %ld y %s.\n",mensaje1.tipo,mensaje1.mensaje);
    
    sprintf(str1,"%ld",mensaje1.tipo);    
    strcpy(str,"<");
    strcat(str,str1);
    strcat(str,">");
    strcat(str,mensaje1.mensaje);
    
    printf("Guardaremos %s.\n",str);
            
    printf("Pulsa 4 para continuar: ");
    scanf("%d",&wait);
    
    printf("Creabndo cola...\n");
    idCola2 = msgget(CLAVE,0666|IPC_CREAT);
    
    strcpy(mensaje2.mensaje,str);
    mensaje2.tipo=1;
    sleep(10);
    printf("Enviando mensaje...\n");    
    msgsnd(idCola2,&mensaje2,sizeof(mensaje2.mensaje),0);
    sleep(5);
    
    //msgctl(idCola2, IPC_RMID,0);
    
}
