/*Sergio Lopez Alejandro practica4_socket_udp*/

#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>

#define IP "127.0.0.1"
#define PORT 5000
#define SIZE 1024


/*Comunicacion con servidor remoto*/
/*

char cadena[]="12345678@example.eus:5000";
char *dni, *dominio,*puerto;

dni = strtok(argv[1],"@");
dominio = strtok(NULL,":");
puerto = strtok(NULL,"");

//obtener ip:
struct hostent *ho;

ho = gethostname(dominio);

serverAddr.sinaddr = ((struct in_addr *) ho->h_addr_list[0]);

 */

int main(int argc, char** argv) {
    
    int sockfd, n; ///socket para lectura y escritura + longitud de texto leida
    struct sockaddr_in serverAddr; //structura para esecificar TCP, Puerto, Direccion
    char buffer[SIZE]; //buffer para almacenar informacion temporal
    int tam = sizeof(serverAddr);
    
    //Configuramos el destino
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT); //Host to nework
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Destino
    
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    
    printf("Enter text: ");
    fgets(buffer,SIZE,stdin);
    buffer[sizeof(buffer)-1]='\0';
    
    sendto(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *) &serverAddr,tam);

    n = recvfrom(sockfd,buffer,SIZE,0,(struct sockaddr *) &serverAddr,&tam);
            
    buffer[n-1]='\0';
    
    printf("Frase %s obtenida.\n",buffer);

    close(sockfd);
    
    return (EXIT_SUCCESS);
}

