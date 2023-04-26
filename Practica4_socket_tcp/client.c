/*Sergio Lopez Alejandro-  Practica4_socket_tcp*/

#include <stdio.h>
#include <stdlib.h>


#include <sys/socket.h>
#include <netinet/in.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>

#define SIZE 1024
#define PORT 5000
    

int main(int argc, char** argv) {
    
    //Declaraciones
    
    int sockfd, n; ///socket para lectura y escritura + longitud de texto leida
    struct sockaddr_in serverAddr; //structura para esecificar TCP, Puerto, Direccion
    char buffer[SIZE]; //buffer para almacenar informacion temporal
    
    //Configuramos el destino
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT); //Host to nework
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Destino
    
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    
    if(connect(sockfd,(struct sockaddr *) &serverAddr,sizeof(serverAddr)) < 0){
        perror("Error connect:");
        exit(EXIT_FAILURE);
    }
    
    printf("Connected to server at %s:%d\n",inet_ntoa(serverAddr.sin_addr),ntohs(serverAddr.sin_port));
    
    printf("Enter text: ");
    fgets(buffer,SIZE,stdin);
    buffer[sizeof(buffer)-1]='\0';
    
    write(sockfd,buffer,strlen(buffer));//He quitado un 0 de parametro final
    
    n = read(sockfd,buffer,SIZE-1);
            
    buffer[n-1]='\0';
    
    printf("Frase %s obtenida.\n",buffer);

    close(sockfd);
    
    return (EXIT_SUCCESS);
}
