/*Sergio Lopez Alejandro - Practica2_Pipes_Fifo_Colas */
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/msg.h>
#include <ctype.h>
/*
 --------------------NOTAS--------------------
 */
/*

 * --------------------------------PIPES-------------------------------------------
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
 * --------------------------------FIFO (Pipes con nombre)----------------------------------------------
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
 * 
 * 
 * -------------------------------------Colas de Mensajes--------------------------------
 * 
 * Ejercicio:
 * 
 * Proceso A coloca un mensaje en un "buzon" para que B despues obtenga
 * el mensaje, lo ponga en mayusculas, coloque de nuevo en el buzon, A lo
 * obtiene y lo imprime.
 * 
 * #define CLAVE 0x78997795L
 * 
 * struct Msg{
 *      long tipo;
 *      char mensaje[SIZE]
 * };
 * 
 * Llamadas a sistema:
 * 
 * int idCola = msgget(CLAVE,0666|IPC_CREAT) --> Creamos el buzon
 * 
 * struct Msg mensaje1;
 * struct Msg mensaje2;
 * 
 * msgsnd(idCola,&mensaje1,strlen(mensaje1.mensaje),0) --> 0 el proceso se bloquea si el buzon esta lleno
 * 
 * msgrcv(idCola,&mensaje2,sizeof(),canal,0) --> El canal debera corresponder con el tipo que ha puesto el otro proceso en su estructura
 * 
 * --> Ese tipo equivale a un identificador el cual debes dar para que el buzon te de el mensaje correspondiente
 * --> El 0 se utilizara para bloquera el proceso en caso de que la cola esta vacia
 * 
 * IMPORTANTE
 * 
 * En caso de que muchos procesos quieran comunicarse,
 * pueden mandar por un canal 1 por ejemplo, un mensaje con una structura de
 * long, int, msj, donde el int sera su pripio pid. Entonces a partir de ese mensaje, los dos procesos
 * pasarana comunicarse por el canal coincidente con ese pid.
 * 
 * 
 * 
 * 
 
*/
/*
 ^^--------------------NOTAS--------------------^^
 */ 


#define SIZE 128
#define CLAVE 0x78997795L

void funcionPipe();//Pipe sin nombre
void funcionFifo();//Pipe con nombre
void funcionColasMsg();

int main(int argc, char** argv) {
        
    funcionColasMsg();
    
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

struct Msg{
        long canal;
        char texto[SIZE];
    };

// OJO, ESTA FUNCION TIENE ERRORRES
void funcionColasMsg(){
    
    int pid1,pid2;
    
    
    pid1=fork();
    if(pid1==0){
        //Hijo1
        sleep(1);
        int idCola1=msgget(CLAVE,0666);
        char texto1[SIZE];
        printf("Soy el Hijo 1.\n");
        printf("Introduce tu frase: ");
        fgets(texto1,SIZE,stdin);
        
        
        struct Msg mensaje1;
        struct Msg mensaje11;
        int canalLectura=2;
        int canalEscritura=1;
        
                
        //Quitamos el salto de linea
        if(texto1[strlen(texto1)-1]=='\n'){
            texto1[strlen(texto1)-1]='\0';
        }
        
        strcpy(mensaje1.texto,texto1);
        mensaje1.canal=canalEscritura;
        printf("Hijo 1 manda: <%s> a Hijo 2.\n",mensaje1.texto);
        
        msgsnd(idCola1,&mensaje1,sizeof(mensaje1.texto),0);
        msgrcv(idCola1,&mensaje11,sizeof(mensaje11.texto),canalLectura,0);
        
        printf("La frase es %s.\n",mensaje11.texto);
        exit(0);
        
    }else{
        pid2=fork();
        if(pid2==0){
            //Hijo2
            int idCola2=msgget(CLAVE,0666|IPC_CREAT);
            printf("Soy el Hijo 2.\n");
            struct Msg mensaje2;
            int canalLectura=1;
            int canalEscritura=2;
            int i=0;
            int val;
            
            msgrcv(idCola2,&mensaje2,sizeof(mensaje2.texto),canalLectura,0);
            printf("Hijo 2 obtiene: <%s> de Hijo 1.\n",mensaje2.texto);
            
            while(mensaje2.texto[i]!='\0'){
                mensaje2.texto[i]=toupper(mensaje2.texto[i]);
                i++;
            }
            
            printf("Hijo 2 envia: <%s> a Hijo 1.\n",mensaje2.texto);
            
            mensaje2.canal=canalEscritura;
            msgsnd(idCola2,&mensaje2,strlen(mensaje2.texto),0);
            exit(0);
        }else{
            //Padre
            int status;
            printf("Soy el padre.\n");
            wait(&status);
            //msgctl(idCola2,IPC_RMID,NULL);
            printf("Finalizado.\n");
            exit(0);
        }
    }
}
