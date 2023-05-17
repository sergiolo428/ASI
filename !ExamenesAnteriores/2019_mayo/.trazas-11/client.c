#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


#define FIFO_RD "/tmp/fifo_rd"
#define FIFO_WR "/tmp/fifo_wr"

#define PORT 10000

#define KEY 0x78997795L

#define SIZE 1024

#define OFF_AC_COUNTER 10
#define N_COUNTERS 4


char *tbl_cmd[]={"HE","AC","OK","SC","SQ","NK",0};
char *tbl_dev_name[]={ "FIFO","TCP","UDP" };

int mid,o,c,sigout=0;
char *start;
void fifo(char *);
void tcp(char *);
void udp(char *);
void handler1();
void handler2();

int main(int argc,char *argv[]){
    
    char buf[256];
    int pid1,pid2,pid3;
    int status1,status2,status3;mid = atoi(argv[1]);
    
    if(argc != 2) {
            fprintf(stderr,"Missing parameters.\nFormat: %s <pidMonitor>\n",argv[0]);
            exit(20);
    }
    
    printf("Press Enter to start exercise:\n");
    fgets(buf,sizeof(buf)-1,stdin);
    
    int idMem = shmget(KEY,1024,0666);
    start = (char *)shmat(idMem,NULL,0);
    
    if((pid1=fork())==0){
        printf("----- Iniciado FIFO:\n");
        fifo(start);
        printf("----- Terminando FIFO:\n");
        pause();
    }
    
    if((pid2=fork())==0){
        printf("----- Iniciado TCP:\n");
        tcp(start);
        printf("----- Terminando TCP:\n");
        pause();
    }
    
    if((pid3=fork())==0){
        printf("----- Iniciado UDP:\n");
        udp(start);
        printf("----- Terminando UDP:\n");
        pause();
    }
    
    o=0;c=0;
    signal(SIGUSR1,handler1);
    signal(SIGUSR2,handler2);
    kill(mid,SIGUSR1);
    
    printf("Dale para continuar:\n");
    getchar();
    
    while(1){
        if(sigout !=0){
            break;
        }
    }
    
    kill(pid1,SIGKILL);
    kill(pid2,SIGKILL);
    kill(pid3,SIGKILL);
    
    sleep(5);
    printf("END EXERCISE\n");
}

void handler1(){
    if(o==1){
        o=0;
        kill(mid,SIGUSR2);
    }else if(o==0){
        o=1;
    }
}

void handler2(){
    if(o==1){
        c++;
        memcpy(start+OFF_AC_COUNTER+3*sizeof(int),&c,sizeof(int));
    }
}

void logout(){
    sigout=1;
}

void fifo(char *start){
    int fdwr,fdrd;
    fdwr = open(FIFO_WR,O_RDWR);
    fdrd = open(FIFO_RD,O_RDWR);
       
    char buffer_rx[SIZE];
    int buffer_tx[SIZE];
    int countAC=0;
    int num,valor,sum=0;
    char buf[SIZE];
    
    buffer_tx[0]=0;
    buffer_tx[1]=getpid();
    write(fdwr,buffer_tx,2*sizeof(int));
    printf("TX:%d-%d\n",buffer_tx[0],buffer_tx[1]);
    
    read(fdrd,buffer_rx,sizeof(buffer_rx));
    
    if(buffer_rx[0]=='H' && buffer_rx[1]=='E'){
        printf("HE - Correcto.\n");
    }else{
        printf("------ERROR: Solicitud de inicio de negado (HE:FIFO).----\n");
    }
    
    while(1){
        read(fdrd,buffer_rx,sizeof(buffer_rx));
        
        if(buffer_rx[0]=='A' && buffer_rx[1]=='C'){
            countAC++;
            printf("FIFO:RX: %s\n",buffer_rx);
            
            memcpy(start+OFF_AC_COUNTER,&countAC,sizeof(int));
            
            memcpy(&valor,buffer_rx + 2*sizeof(char),sizeof(int));
            sum+=valor;
            memcpy(start+OFF_AC_COUNTER+4*sizeof(int),&sum,sizeof(int));
            
            
            buffer_tx[0]=1;
            buffer_tx[1]=countAC;
            write(fdwr,buffer_tx,2*sizeof(int));
            printf("FIFO:TX: %d-%d\n",buffer_tx[0],buffer_tx[1]);
            
        }
        
        if(buffer_rx[0]=='S' && buffer_rx[1]=='C'){            
            
            printf("FIFO:RX: %s\n",buffer_rx);
            
            memcpy(&num,buffer_rx + 2*sizeof(char),sizeof(int));
            printf("Secreto: %d\n",num);
            
            buffer_tx[0]=2;
            buffer_tx[1]=2;
            buffer_tx[2]=num;
            
            write(fdwr,buffer_tx,3*sizeof(int));
            printf("FIFO:TX: %d-%d-%d\n",buffer_tx[0],buffer_tx[1],buffer_tx[2]);
        }
        
        if(buffer_rx[0]=='S' && buffer_rx[1]=='Q'){            
            
            printf("FIFO:RX: %s\n",buffer_rx);
            
            buffer_tx[0]=3;
            write(fdwr,buffer_tx,sizeof(int));
            printf("FIFO:TX: %d\n",buffer_tx[0]);
        }
    }
    
}

void tcp(char *start){
    
    
    int sockfd;
    char buffer_rx[SIZE];
    int buffer_tx[SIZE];
    int countAC=0;
    int num,valor,sum=0;
    char buf[SIZE];
    struct sockaddr_in serverAddr;
    
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_family = AF_INET;
    
    connect(sockfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr));

    buffer_tx[0]=0;
    buffer_tx[1]=getpid();
    write(sockfd,buffer_tx,2*sizeof(int));
    printf("TX:%d-%d\n",buffer_tx[0],buffer_tx[1]);
    
    read(sockfd,buffer_rx,sizeof(buffer_rx));
    
    if(buffer_rx[0]=='H' && buffer_rx[1]=='E'){
        printf("HE - Correcto.\n");
    }else{
        printf("------ERROR: Solicitud de inicio de negado (HE:TCP).----\n");
    }
    
    while(1){
        read(sockfd,buffer_rx,sizeof(buffer_rx));
        
        if(buffer_rx[0]=='A' && buffer_rx[1]=='C'){
            countAC++;
            printf("TCP:RX: %s\n",buffer_rx);
            
            memcpy(start+OFF_AC_COUNTER+sizeof(int),&countAC,sizeof(int));
            
            memcpy(&valor,buffer_rx + 2*sizeof(char),sizeof(int));
            sum+=valor;
            memcpy(start+OFF_AC_COUNTER+5*sizeof(int),&sum,sizeof(int));
            
            buffer_tx[0]=1;
            buffer_tx[1]=countAC;
            write(sockfd,buffer_tx,2*sizeof(int));
            printf("TCP:TX: %d-%d\n",buffer_tx[0],buffer_tx[1]);
            
        }
        
        if(buffer_rx[0]=='S' && buffer_rx[1]=='C'){            
            
            printf("TCP:RX: %s\n",buffer_rx);
            
            memcpy(&num,buffer_rx + 2*sizeof(char),sizeof(int));
            printf("Secreto: %d\n",num);
            
            buffer_tx[0]=2;
            buffer_tx[1]=5;
            buffer_tx[2]=num;
            
            write(sockfd,buffer_tx,3*sizeof(int));
            printf("TCP:TX: %d-%d-%d\n",buffer_tx[0],buffer_tx[1],buffer_tx[2]);
        }
        
        if(buffer_rx[0]=='S' && buffer_rx[1]=='Q'){            
            
            printf("TCP:RX: %s\n",buffer_rx);
            
            buffer_tx[0]=3;
            write(sockfd,buffer_tx,sizeof(int));
            printf("TCP:TX: %d\n",buffer_tx[0]);
        }
    }
}

void udp(char *start){
    
    int sockfd = socket(AF_INET,SOCK_DGRAM,0);
    int countAC=0;
    int num,valor,sum;
    char buffer_rx[SIZE];
    int buffer_tx[SIZE];
    
    struct sockaddr_in serverAddr;
    int tam = sizeof(serverAddr);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_family = AF_INET;
    
    buffer_tx[0]=0;
    buffer_tx[1]=getppid();
    sendto(sockfd,buffer_tx,2*sizeof(int),0,(struct sockaddr *)&serverAddr,tam);
    printf("TX:%d-%d\n",buffer_tx[0],buffer_tx[1]);
    
    recvfrom(sockfd,buffer_rx,sizeof(buffer_rx),0,(struct sockaddr *)&serverAddr,&tam);
    
    if(buffer_rx[0]=='H' && buffer_rx[1]=='E'){
        printf("HE - Correcto.\n");
    }else{
        printf("------ERROR: Solicitud de inicio de negado (HE:UDP).----\n");
    }
    
    while(1){
        recvfrom(sockfd,buffer_rx,sizeof(buffer_rx),0,(struct sockaddr *)&serverAddr,&tam);
        
        if(buffer_rx[0]=='A' && buffer_rx[1]=='C'){
            countAC++;
            printf("TCP:RX: %s\n",buffer_rx);
            
            memcpy(start+OFF_AC_COUNTER+2*sizeof(int),&countAC,sizeof(int));
            
            memcpy(&valor,buffer_rx + 2*sizeof(char),sizeof(int));
            sum+=valor;
            memcpy(start+OFF_AC_COUNTER+6*sizeof(int),&sum,sizeof(int));
            
            buffer_tx[0]=1;
            buffer_tx[1]=countAC;
            sendto(sockfd,buffer_tx,2*sizeof(int),0,(struct sockaddr *)&serverAddr,tam);
            printf("UDP:TX: %d-%d\n",buffer_tx[0],buffer_tx[1]);
            
        }
        
        if(buffer_rx[0]=='S' && buffer_rx[1]=='C'){
            
            printf("UDP:RX: %s\n",buffer_rx);
            
            memcpy(&num,buffer_rx + 2*sizeof(char),sizeof(int));
            printf("Secreto: %d\n",num);
            
            buffer_tx[0]=2;
            buffer_tx[1]=8;
            buffer_tx[2]=num;
            
            sendto(sockfd,buffer_tx,3*sizeof(int),0,(struct sockaddr *)&serverAddr,tam);
            printf("UDP:TX: %d-%d-%d\n",buffer_tx[0],buffer_tx[1],buffer_tx[2]);
        }
        
        if(buffer_rx[0]=='S' && buffer_rx[1]=='Q'){
            
            printf("UDP:RX: %s\n",buffer_rx);
            
            buffer_tx[0]=3;
            sendto(sockfd,buffer_tx,sizeof(int),0,(struct sockaddr *)&serverAddr,tam);
            printf("UDP:TX: %d\n",buffer_tx[0]);
        }
    }
}

