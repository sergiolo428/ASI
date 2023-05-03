/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cFiles/main.c to edit this template
 */

/* 
 * File:   server.c
 * Author: ubuntu
 *
 * Created on 3 de mayo de 2023, 8:04
 */

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

void serverMulti();

int main(int argc, char** argv) {
    
    serverMulti();
    
    return (EXIT_SUCCESS);
}

void serverMulti(){
    int sockfd, clientfd, n;
    
    struct sockaddr_in serverAddr,clientAddr;
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    
    socklen_t clientAddrLen = sizeof(clientAddrLen);
    
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    
    char buffer[SIZE];
    
    bind(sockfd,(struct sockaddr *) &serverAddr,sizeof(serverAddr));
    
    
    while(1){
        
            n=recvfrom(sockfd,buffer,SIZE,0,(struct sockaddr *) &clientAddr,&clientAddrLen);
            
            buffer[strlen(buffer)-1]='\0';
            
            printf("Mensaje %s.\n",buffer);
            sleep(3);
            for(int i = 0;i<n;i++){
                buffer[i] = toupper(buffer[i]);
            }

            write(clientfd,buffer,strlen(buffer)+1);
            sendto(sockfd,buffer,strlen(buffer)+1,0,(struct sockaddr *) &clientAddr,clientAddrLen);
            
            close(clientfd);

    }
    
    
}
