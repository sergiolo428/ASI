/*Server TCP*/

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
#include <ctype.h>

#define SIZE 1024
#define PORT 5000

void serverCola();
void serverMulti();

int main(int argc, char** argv) {
    
    serverMulti();
    
    return (EXIT_SUCCESS);
}

void serverCola(){
    
    int sockfd, clientfd,n;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    char buffer[SIZE];
    
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    
    //memset(&serverAddr,0,sizeof(serverAddr));
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY; //Lo mismo que poner la funcion de antes con 127.0.0.1
    
    bind(sockfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    
    listen(sockfd,3); //Indicamos el maximo de usuarios que se pueden poner en cola
    
    while(1){
        clientfd = accept(sockfd, (struct sockaddr *) &clientAddr, &clientAddrLen);
        
    //Podemos hacer el while anterior usando multiprocesos:
    
    //while(1){ //clientfd = accept //Proceso hijo //Operacion //exit(EXIT_SUCCESS) //proceso padre //wait????? //close(clientfd);
    
        while((n=read(clientfd,buffer,SIZE))>0){
            buffer[n-1]='\0';
            printf("Mensaje %s.\n",buffer);
            sleep(3);
            for(int i = 0;i<n;i++){
                buffer[i] = toupper(buffer[i]);
            }

            write(clientfd,buffer,strlen(buffer)+1);
        }
    }
    
}

void serverMulti(){
    int sockfd, clientfd, n;
    
    struct sockaddr_in serverAddr,clientAddr;
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    
    socklen_t clientAddrLen = sizeof(clientAddrLen);
    
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    
    char buffer[SIZE];
    
    bind(sockfd,(struct sockaddr *) &serverAddr,sizeof(serverAddr));
    
    
    
    listen(sockfd,3);
    
    while(1){
        clientfd = accept(sockfd, (struct sockaddr *) &clientAddr, &clientAddrLen);
        
        if(fork()==0){
            //Hijo
            while((n=read(clientfd,buffer,SIZE))>0){
            buffer[n-1]='\0';
            printf("Mensaje %s.\n",buffer);
            sleep(3);
            for(int i = 0;i<n;i++){
                buffer[i] = toupper(buffer[i]);
            }

            write(clientfd,buffer,strlen(buffer)+1);
            
        }
            close(clientfd);
            exit(EXIT_SUCCESS);
        }else{
            //Padre
            close(clientfd);
        }
    }
    
    
}