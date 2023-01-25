/*Sergio Lopez Alejandro - Practica1_1 */

#include <stdio.h>
#include <stdlib.h>

//Librerias extra
#include <sys/types.h>
#include <unistd.h>
 #include <sys/wait.h>

/*Como compilamos
 1. Vamos al directorio
 2. Ejecutamos "gcc nombre.c -o nombre"
 3. Para ejecutar, usamos "/.nombre" */

/*El hijo comenzara ejecutando desde el fork que lo ha creado, 
guardando este valor en el contador de programa*/

/*Planificador, sera el que decida que copia de dicho programa
 sera el que se siga ejecutando*/

int main(int argc, char** argv) {
    
    int pid1,pid2;
    int status;
    
    pid1=fork();
    if(pid1==-1){
        printf("Error creando Hijo 1.");
    }else if(pid1==0){
        /*Soy el hijo1*/
        printf("Soy el hijo1 con ID: %d\n",getpid());
        _exit(0);
    }else{
        pid2=fork();
        if(pid2==-1){
        printf("Error creando Hijo 2.");
        }else if(pid2==0){
            /*Soy el hijo2*/
            printf("Soy el hijo2 con ID: %d\n",getpid());
            _exit(0);
        }else{
            /*Soy el padre*/            
            wait(&status);
            printf("Soy el Padre con ID: %d\n",getpid());
            _exit(0);
        }
    }

    return (EXIT_SUCCESS);
}

