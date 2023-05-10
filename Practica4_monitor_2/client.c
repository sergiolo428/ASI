/*Sergio Lopez Alejandro Practica4_monitor_2*/

#include <stdio.h>
#include <stdlib.h>

#include <netinet/in.h>

#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/types.h>

void espera();
void ejer5();
void ejer4();
void ejer3();
void ejer2();
void ejer1();

int main(int argc, char** argv) {

    //ejer1();
    //Funciona 2/3s
    //ejer2();
    //FUNCIONA
    //ejer3();
    //FUNCIONA
    //ejer4();
    //ejer5();
    return (EXIT_SUCCESS);
}

void espera(int n){
    char m[128];
    printf("Pulsa enter para continuar con el ejercicio %d",n);
    fgets(m,sizeof(m),stdin);
}

void ejer4(){
    espera(4);
    int sockfd4;
    struct sockaddr_in serverAddr4, clientAddr4;
    int clientTam = sizeof(clientAddr4);
    
    sockfd4 = socket(AF_INET,SOCK_DGRAM,0);

    serverAddr4.sin_family=AF_INET;
    serverAddr4.sin_addr.s_addr=INADDR_ANY;
    serverAddr4.sin_port=htons(3001);
    
    bind(sockfd4,(struct sockaddr *) &serverAddr4,sizeof(serverAddr4));
    
    int clave;
    char str[10];
    int tempint;
    char tempchar[10];
    
    recvfrom(sockfd4,&clave,sizeof(clave),0,(struct sockaddr *)&clientAddr4,&clientTam);
    
    printf("Clave 10 es: %d.\n",clave);
    
    sprintf(str,"<%d>",clave);
    
    sendto(sockfd4,str,sizeof(str),0,(struct sockaddr *)&clientAddr4,clientTam);
    
    while(1){
        recvfrom(sockfd4,&tempint,sizeof(tempint),0,(struct sockaddr *)&clientAddr4,&clientTam);
        
        printf("Obtenido: %d.\n",tempint);
    
        sprintf(tempchar,"<%d>",tempint);
    
        sendto(sockfd4,tempchar,sizeof(tempchar),0,(struct sockaddr *)&clientAddr4,clientTam);
        
    }
    
}

void ejer3(){
    espera(3);
    int sockfd3;
    struct sockaddr_in serverAddr3;
    int tam = sizeof(serverAddr3);
    
    sockfd3 = socket(AF_INET,SOCK_DGRAM,0);
    
    serverAddr3.sin_port=htons(3000);
    serverAddr3.sin_addr.s_addr=inet_addr("127.0.0.1");
    serverAddr3.sin_family=AF_INET;
    
    char cadena[]="<??\?>";
    char cadena2[10];
    int num;
    
    sendto(sockfd3,cadena,strlen(cadena),0,(struct sockaddr *)&serverAddr3,tam);
    
    recvfrom(sockfd3,cadena2,sizeof(cadena2),0,(struct sockaddr *)&serverAddr3,&tam);
    
    printf("Clave8: %s.\n",cadena2);
    
    sscanf(cadena2,"<%d>",&num);
    
    sendto(sockfd3,&num,sizeof(num),0,(struct sockaddr *)&serverAddr3,tam);
    
    sleep(1);
    
}


void ejer2(){
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
            printf("Enviando: %s.\n",charCode3);
            
            write(clientfd2,charCode3,strlen(charCode3));
            
            sleep(1);
            
            close(clientfd2);
            
            sleep(3);
            
            exit(EXIT_SUCCESS);
            
        }else{
            //Padre
            close(clientfd2);
            sleep(1);
        }
        
    }
    sleep(1);
    close(clientfd2);
}

void ejer1(){
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
}
