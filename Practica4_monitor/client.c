/*Sergio Lopez Alejandro Practica4_monitor*/

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>

#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void espera();

int main(int argc, char** argv) {
    
    espera(1);
    //------------------------------Ejercicio1------------------------------
    
    int sockfd,n;
    struct sockaddr_in serverAddr;
    char buffer[1024];
    
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(3000);
    serverAddr.sin_family = AF_INET;
    
    
    sockfd = socket(AF_INET,SOCK_STREAM,0);
                
    connect(sockfd,(struct sockaddr *) &serverAddr,sizeof(serverAddr));
    
    printf("Connected to server at %s:%d\n",inet_ntoa(serverAddr.sin_addr),ntohs(serverAddr.sin_port));
    
    espera(2);
    //------------------------------Ejercicio2------------------------------
    read(sockfd,buffer,sizeof(buffer));
    //buffer[strlen(buffer)-1]='\0';
    printf("Obtenido code2: %s.\n",buffer);
    
    espera(3);
    //------------------------------Ejercicio3------------------------------
    
    write(sockfd,buffer,strlen(buffer));
    
    read(sockfd,buffer,sizeof(buffer));
    printf("Obtenido y mandando code3: %s.\n",buffer);
    
    write(sockfd,buffer,strlen(buffer));
    sleep(1);
    close(sockfd);
    
    espera(4);
    //------------------------------Ejercicio4------------------------------
    
    int sockfd2,clientfd2;
    struct sockaddr_in serverAddr2, clientAddr2;
    int cliTam = sizeof(clientAddr2);
    
    serverAddr2.sin_addr.s_addr=inet_addr("127.0.0.1");
    serverAddr2.sin_port=htons(3001);
    serverAddr2.sin_family=AF_INET;
    
    sockfd2 = socket(AF_INET,SOCK_STREAM,0);
    
    bind(sockfd2,(struct sockaddr *)&serverAddr2,sizeof(serverAddr2));
    
    listen(sockfd2,3);
    
    clientfd2 = accept(sockfd2,(struct sockaddr *)&clientAddr2,&cliTam);
    espera(5);
    //------------------------------Ejercicio5------------------------------
    
    read(clientfd2,buffer,sizeof(buffer));
    
    printf("Obtenido code4: %s\n",buffer);
    
    write(clientfd2,buffer,strlen(buffer));
    
    espera(6);
    //------------------------------Ejercicio6------------------------------
    
    strcpy(buffer,"HOLA!");
    int sockfd3;
    struct sockaddr_in serverAddr3;
    int servertam = sizeof(serverAddr3);
    
    sockfd3 = socket(AF_INET,SOCK_DGRAM,0);
    
    serverAddr3.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr3.sin_family = AF_INET;
    serverAddr3.sin_port = htons(3000);
    
    sendto(sockfd3,buffer,strlen(buffer),0,(struct sockaddr *) &serverAddr,servertam);
    
    espera(7);
    //------------------------------Ejercicio7------------------------------
    
    int sockfd4;
    struct sockaddr_in serverAddr4, clientAddr4;
    int clientTam4 = sizeof(clientAddr4);
    char buffer4[1024];
    
    serverAddr4.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr4.sin_family = AF_INET;
    serverAddr4.sin_port = htons(3001);
    
    sockfd4 = socket(AF_INET,SOCK_DGRAM,0);
    
    bind(sockfd4,(struct sockaddr *) &serverAddr4,sizeof(serverAddr4));
    
    recvfrom(sockfd4,buffer4,sizeof(buffer4),0,(struct sockaddr *)&clientAddr4,&clientTam4);
    
    printf("Obtenido code7: %s\n",buffer4);
    
    sendto(sockfd4,buffer4,strlen(buffer4),0,(struct sockaddr *) &clientAddr4, clientTam4);
    
    sleep(2);
    return (EXIT_SUCCESS);
}

void espera(int n){
    char a[128];
    printf("Enter para continuar con el ejercicio %d: ",n);
    fgets(a,sizeof(a),stdin);
}

