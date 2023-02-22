
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
/*
 --------------------NOTAS--------------------
 */
/*

 * PIPES:
 * COnsiste en una tuberia dentro del sistema operativo, mediante la cual
 * los procesos se intercambian informacion
 * 
 * Si un proceso quiere leer pero no hay nada en el pipe, el proceso se bloquearia
 * 
 * Como generamos el pipe:
 * 
 * int fd[2]
 * 
 * pipe( fd[array de dos enteros]) //Donde obtenemos un fd[0] y fd[1]
 * 
 * fd[0] usaremos cuando queramos realizar operaciones de lectura
 * fd[1] usaremos cuando queramos realziar operaciones de escritura
 * 
 * Debemos cerrar los fd los cuales no usaremos para evitar problemas: usaremos close(fd[])
 * Es decir, si un proceso solo va a actuar como consumidor,
 * deberemos cerrar el f[1] ya que no va a producir info en el pipe
 * 
 * 
 * Escritura:
 * 
 * write(fd[1],buffer,size of datos a enviar)
 * 
 * 
 * Lectura:
 * 
 * read(fd[0],buffer, sizeof(buffer))
 * 
 * 
 * -----IMPORTANTE-----:
 * 
 * >> Esta counicacion solo vale para procesos que compartan el mismo padre!!!
 * 
 * >> Debemos de crear este pipe(fd) en el proceso padre para que sea un recurso compartido por los hijos
 * 
 * 
 * 
 * PIPES CON NOMBRE (FIFO)
 * 
 * mkfifo("nombre",permisos (0666))
 * Si hacemos un ls -l en el directorio de trabajo, veremos que se ha creado un archivo con el nombre indicado
 * 
 * fd=open("nombre",0_RDWR)
 * 
 * Escritura:
 * 
 * write(fd,buffer,size)
 * 
 * Lectura:
 * 
 * read(fd,buffer,sizeof(buffer))
 * 
 * 
 * -----IMPORTANTE-----
 * 
 * >>Los procesos en FIFO pueden ser de jerarquias diferentes
 * 
 *  
 
*/
/*
 ^^--------------------NOTAS--------------------^^
 */ 

#define SIZE 128

void funcionPipe();
void funcionFifo();

int main(int argc, char** argv) {
        
    funcionFifo();
    
    return (EXIT_SUCCESS);
}

void funcionPipe(){
    
    int pid1,pid2;
    
    //PIPE
    int fd[2];
    pipe(fd);
    
    pid1=fork();
    if(pid1==0){
        //Hijo1
        char buffer1[SIZE];
        //Config PIPE + Leo frase del pipe
        close(fd[1]);
        read(fd[0],buffer1,sizeof(buffer1));
        close(fd[1]);
        //print de la frase
        printf("Mi frase es: %s.\n",buffer1);
        exit(0);
    }else{
        pid2=fork();
        if(pid2==0){
            //Hijo2
            char buffer2[SIZE];
            // Leo frase del teclado
            printf("Introduce tu frase: ");
            fgets(buffer2,sizeof(buffer2),stdin);
            
            if(buffer2[strlen(buffer2)-1]=='\n'){
                buffer2[strlen(buffer2)-1]='\0';
            }
            
            //Config PIPE + Escribo frase en el pipe
            close(fd[0]);
            write(fd[1],buffer2,strlen(buffer2));
            close(fd[1]);
            exit(0);
            
        }else{
            //Padre
            int status;
            wait(&status);
            exit(0);
        }
    }
}

void funcionFifo(){
    
    int pid1,pid2;
    int fd;
    
    mkfifo("miFifo",0666);
    fd=open("miFifo",O_RDWR);
    
    
    pid1=fork();
    if(pid1==0){
        //Hijo1
        char buffer1[SIZE];
        read(fd,buffer1,sizeof(buffer1));
        printf("Mi frase es: %s.",buffer1);
        exit(0);
    }else{
        pid2=fork();
        if(pid2==0){
            //Hijo2
            char buffer2[SIZE];
            printf("Introduce tu frase: ");
            fgets(buffer2,sizeof(buffer2),stdin);
            
            if(buffer2[strlen(buffer2)-1]=='\n'){
                buffer2[strlen(buffer2)-1]='\0';
            }
            write(fd,buffer2,strlen(buffer2));
            exit(0);
        }else{
            //Padre
            int status;
            wait(&status);
            close(fd);
            exit(0);
        }
    }
}
