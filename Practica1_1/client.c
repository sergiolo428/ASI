/*Sergio Lopez Alejandro - Practica1_1 */

#include <stdio.h>
#include <stdlib.h>

//Librerias extra
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

#define SIZE 128

/*Como compilamosf=fopen("texto.txt","r");
        fread(buffer1,sizeof(char),SIZE,f);
        fclose(f);
        
        i=0;
        while(buffer1[i]!='\0'){
            buffer1[i]=toupper(buffer1[i]);
            i++;
        }
        
        printf("Tu frase en mayusculas es: %s\n", buffer1);
 1. Vamos al directorio
 2. Ejecutamos "gcc nombre.c -o nombre"
 3. Para ejecutar, usamos "/.nombre" */

/*El hijo comenzara ejecutando desde el fork que lo ha creado, 
guardando este valor en el contador de programa*/

/*Planificador, sera el que decida que copia de dicho programa
 sera el que se siga ejecutando*/


/*pthread_create(puntero ,NULL, funcion, paramatros de funcion)*/

void handler(int);
void mayus();
void readSave();

int main(int argc, char** argv) {
    
    int pid1,pid2;
    int status;
    
    pid1=fork();
    if(pid1==-1){
        printf("Error creando Hijo 1.");
        
    }else if(pid1==0){
        /*-----Soy el hijo1-----*/
        
        
        /*-----Hacemos signal y esperamos-----*/
        signal(SIGUSR1,handler);
        pause();
        printf("Soy el hijo1 con ID: %d\n",getpid());
        
        /*-----Leemos del archivo y pasamos a mayusculas-----*/
        mayus();
        _exit(0);
        
    }else{
        pid2=fork();
        if(pid2==-1){
        printf("Error creando Hijo 2.");
        
        }else if(pid2==0){
            /*-----Soy el hijo2-----*/
            printf("Soy el hijo2 con ID: %d\n",getpid());
            
            /*-----Escribimos en el archivo la frase introducida-----*/
            readSave();
            /*-----Enviamos la señal-----*/
            kill(pid1,SIGUSR1);
            _exit(0);
            
        }else{
            /*Soy el padre*/            
            wait(&status);
            wait(&status);
            printf("Soy el Padre con ID: %d\n",getpid());
            _exit(0);
        }
    }

    return (EXIT_SUCCESS);
}

void handler(int n){
    printf("Señal %d Obtenida\n",n);
}

void mayus(){
    FILE *g;
    char buffer1[SIZE];
    int i=0;
    
    g=fopen("texto.txt","r");
    fread(buffer1,sizeof(char),SIZE,g);
    fclose(g);

    while(buffer1[i]!='\0'){
        buffer1[i]=toupper(buffer1[i]);
        i++;
    }
        
    printf("Tu frase en mayusculas es: %s\n", buffer1);
    memset(buffer1,0,sizeof(buffer1));
}

void readSave(){
    
    char buffer2[SIZE];
    int len=0;
    FILE *f;
    
    f=fopen("texto.txt","w");        
    printf("Escribe la frase: ");
    fgets(buffer2,sizeof(buffer2),stdin);

    len=strlen(buffer2);

    if(buffer2[len-1]=='\n'){
        buffer2[len-1]='\0';
    }

    fwrite(buffer2,sizeof(char),strlen(buffer2),f);
    fclose(f);
    memset(buffer2,0,sizeof(buffer2));
}