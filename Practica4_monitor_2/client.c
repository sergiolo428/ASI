/*Sergio Lopez Alejandro Practica4_monitor_2*/

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
    
    /*
    espera(1);
    //------------------------------Ejercicio1------------------------------
    int sockfd1;
    struct sockaddr_in serverAddr1;
    int intCode2;
    char charCode2[128]; 
    
    sockfd1 = socket(AF_INET,SOCK_STREAM,0);
    
    serverAddr1.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr1.sin_family = AF_INET;
    serverAddr1.sin_port = htons(3000);
    
    connect(sockfd1,(struct sockaddr *) &serverAddr1,sizeof(serverAddr1));
    
    read(sockfd1,&intCode2,sizeof(intCode2));
    
    printf("Obtenido el code2: %d\n",intCode2);
    
    sprintf(charCode2,"<%d>",intCode2);
    
    write(sockfd1,charCode2,strlen(charCode2));
    
    close(sockfd1);
    */
    espera(2);
    //------------------------------Ejercicio2------------------------------
    int sockfd2, clientfd2;
    struct sockaddr_in serverAddr2, clientAddr2;
    int tamClient2 = sizeof(clientAddr2);
    int intCode3;
    char charCode3[128];
    
    serverAddr2.sin_addr.s_addr = INADDR_ANY;
    serverAddr2.sin_port = htons(3001);
    serverAddr2.sin_family = AF_INET;
    
    sockfd2 = socket(AF_INET,SOCK_STREAM,0);
    
    bind(sockfd2,(struct sockaddr *) &serverAddr2, sizeof(serverAddr2));
    
    listen(sockfd2,20);
    
    while(1){
        clientfd2 = accept(sockfd2,(struct sockaddr *) &clientAddr2, &tamClient2);
        
        if(fork() == 0){
            //Hijo
            
            close(sockfd2);
            
            read(clientfd2,&intCode3,sizeof(intCode3));
    
            printf("Obtenido el code2: %d\n",intCode3);
    
            sprintf(charCode3,"<%d>",intCode3);
            
            write(clientfd2,charCode3,strlen(charCode3));
             
            close(clientfd2);
            
            exit(EXIT_SUCCESS);
            
        }else{
            //Padre
            close(clientfd2);
        }
        
    }
    //close(clientfd2);
    
    espera(3);
    return (EXIT_SUCCESS);
}

void espera(int n){
    char m[128];
    printf("Pulsa enter para continuar con el ejercicio %d",n);
    fgets(m,sizeof(m),stdin);
}
