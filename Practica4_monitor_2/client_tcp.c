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

#define BUFFER_SIZE 128

int main(int argc, char** argv) {
    
    char direccion[] = "123456789@dominio:8888";
    
    char *dni,*dominio,*puerto;
    int dniInt;
    int puertoInt;
    char userInfo[128];
    char answer[128];
    char clave14[128],clave14v2[128];
    char buffer[128];
    
    dni = strtok(argv[1],"@");
    dominio = strtok(NULL,":");
    puerto = strtok(NULL,"");
    
    sscanf(puerto,"%d",&puertoInt);
    sscanf(dni,"%d",&dniInt);
    
    struct hostent *h;
    
    h = gethostbyname(dominio);
    
    
    printf("Conectando: %s@%s:%s.\n",dni,dominio,puerto);   
    
    
    int sockfd;
    struct sockaddr_in serverAddr;
    
    serverAddr.sin_port = htons(puertoInt);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr= *(struct in_addr *) h -> h_addr_list[0];
    
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    
    connect(sockfd,(struct sockaddr *) &serverAddr,sizeof(serverAddr));
    
    sprintf(userInfo,"user %s\n",dni);
    
    write(sockfd,userInfo,strlen(userInfo));
    printf("TX:%s\n",userInfo);
    
    read(sockfd,answer,sizeof(answer));
    printf("RX:%s.\n",answer);

    strcpy(userInfo,"get clave\n");

    write(sockfd,userInfo,strlen(userInfo));
    printf("TX:%s.\n",userInfo);

    read(sockfd,clave14,sizeof(clave14));
    printf("RX:%s.\n",clave14);

    sscanf(clave14,"CLAVE:%s",clave14v2);

    write(sockfd,clave14v2,strlen(clave14v2));
    printf("TX:%s.\n",clave14v2);

    
    return (EXIT_SUCCESS);
}