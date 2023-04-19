
#include <stdio.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <ctype.h>


#define SIZE 128
#define CLAVE 0x78997795L

int main(int argc, char** argv) {
    
    int a;
    char texto[SIZE];
    int idMem;
    char *dirMemChar;
    int i=0;
    
    idMem = shmget(CLAVE,1024,0666 | IPC_CREAT);
    dirMemChar = (char *)shmat(idMem,NULL,0);
        
    if(idMem==-1){
        printf("ERROR CREANDO MEMORIA");
        exit(0);
    }
    
    if(dirMemChar == (char*)-1){
        printf("ERROR OBTENIENDO DIRECCION\n");
    }
    
   
    while(1){
        printf("\nMENU\n");
        printf("0 --> Salir\n");
        printf("1 --> Obtener frase + mayus + mandar\n\n");
        printf("Introduce la opcion: ");
        scanf("%d",&a);
        fgetc(stdin);
        
        switch(a){
            case 0:
                shmdt(dirMemChar);
                shmctl(idMem,IPC_RMID,NULL);
                exit(0);
                break;
            case 1:
                printf("Obteniendo frase original...\n");
                strcpy(texto,dirMemChar);

                i=0;
                while(texto[i]!='\0'){
                    texto[i]=toupper(texto[i]);
                    i++;
                }
                printf("Nueva frase: %s.\n",texto);
                printf("Guardando frase nueva en memoria...\n");
                strcpy((dirMemChar + SIZE),texto);
                printf("Frase Guardadada");

                break;
            default:
                shmdt(dirMemChar);
                shmctl(idMem,IPC_RMID,NULL);
                exit(0);
                break;

        }
    }
    
    return (EXIT_SUCCESS);
}