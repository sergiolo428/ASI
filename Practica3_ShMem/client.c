/*Sergio Lopez Alejandro Practica 3_Mem_Sem*/


/*Gestion de memria*/

//shm --> Share memory

//idMem = shmget(CLAVE,Tamaño,Permisos) --> idMem da menor que 0 si hay error
//^^ Permisos pueden ser IPC_CREAT | 0666 o 0666

//dirMem = shmat(idMem,NULL,0) --> Nos da la direccion donde esta la memoria compartida
//shmdet(dirMem) --> Cerramos el recurso

//shmctl(idMem,IPC_RMID) --> Elimina el recurso

//RECOMENDACIONES

//c
//Lee frase
//escribe la frase en la posicion 0
//s
//obtiene frase
//pone en mayusculas frase
//escribe la frase en una posicion 0 + offset
//c
//Obtiene la frase
//la imprime

//COMO GUARDAMOS VALORES

//char* puntMem
//char miTexto[256]
//puntMem=(char*)shmat()
//strcpy(puntMem,miTexto)


//Como leemos estructura

//struct Msg *mensaje1

//struct Msg{
//  char a
//  int b
//  int c
//};

//mensaje1 = (Mesg *)shmat(id,NULL,0)

//int b = mensaje1[0].b --> Hay que poner [0] al ser un puntero???

#include <stdio.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>


#define CLAVE 0x78997795L
#define SIZE 128


int main(int argc, char** argv) {
    int a;
    int idMem;
    char *dirMemChar;
    char texto[SIZE];
    
    while(1){
        printf("\nMENU\n");
        printf("0 --> Salir\n");
        printf("1 --> Leer frase + escribir en memoria\n");
        printf("2 --> Obtener frase en mayus + print\n\n");
        
        printf("Introduce la opcion: ");
        scanf("%d",&a);
        fgetc(stdin);
        
        switch(a){
            case 0:
                shmdt(dirMemChar);
                exit(0);
                
                break;
            case 1:
                idMem = shmget(CLAVE,1024,0600);
                dirMemChar = shmat(idMem,NULL,0);
                
                if(idMem==-1){
                    printf("ERROR ACCEDIENDO MEMORIA\n");
                    exit(0);
                }
                
                if(dirMemChar == (char*)-1){
                    printf("ERROR OBTENIENDO DIRECCION\n");
                }
                
                printf("Introduce tu frase: ");
                
                fgets(texto,SIZE,stdin);

                if(texto[strlen(texto)-1]=='\n'){
                    texto[strlen(texto)-1]='\0';
                }

                printf("Tu frase es: %s.\n",texto);
                printf("Escribiendo frase en memoria...\n");

                strcpy(dirMemChar,texto);

                printf("Frase escrita correctamente.\n");

                break;
            case 2:
                printf("Obteniendo frase nueva de la memoria...\n");

                strcpy(texto,(dirMemChar+SIZE));

                printf("La nueva frase es: %s\n",texto);
                strcpy(texto,(dirMemChar));
                printf("La frase antigua es: %s\n",texto);
                printf("Pulsa 0 para salir.\n");
                break;
            default:
                shmdt(dirMemChar);
                exit(0);
                break;
        }
    }
    return (EXIT_SUCCESS);
}

