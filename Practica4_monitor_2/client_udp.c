
#include <stdio.h>
#include <stdlib.h>

#include <netinet/in.h>

#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/types.h>
#include <netdb.h>

int main(int argc, char** argv) {

    char *dni, *puerto, *dominio;
    int sockfd;
    struct sockaddr_in serverAddr;
    int PORT;
    char buffer[128];
    char answer[128];
    char clave[128];
    char clavev2[128];
    int tam = sizeof(serverAddr);
    
    struct hostent *h;
    
    dni = strtok(argv[1],"@");
    dominio = strtok(NULL,":");
    puerto = strtok(NULL,"");
    sscanf(puerto,"%d",&PORT);
    
    h = gethostbyname(dominio);
    
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    
    serverAddr.sin_addr= *(struct in_addr *) h -> h_addr_list[0];;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    
    
    sprintf(buffer,"GET CLAVE %s",dni);
    
    sendto(sockfd,buffer,strlen(buffer),0,(struct sockaddr *)&serverAddr,tam);
    printf("TX:%s\n",buffer);
    
    recvfrom(sockfd,answer,sizeof(answer),0,(struct sockaddr *)&serverAddr,&tam);
    printf("RX:%s\n",answer);
    
    sscanf(answer,"CLAVE:%s",clave);
    
    sendto(sockfd,clave,strlen(clave),0,(struct sockaddr *)&serverAddr,tam);
    printf("TX:%s\n",clave);
    
    return (EXIT_SUCCESS);
}

