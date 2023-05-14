# Aarquitectura de sistemas de información - Resumen


## Practica 1 - Procesos, subprocesos, hilos y ficheros

### 1 Crear proceso

```
pid=fork();
```

### 2 Diferenciar entre codigo del proceso padre y proceso hijo

```
if(pid==0){
	//Proceso Hijo
}else{
	//Proceso Padre
	//*En caso de querer añadir mas hijos,
	//Colocariamos un nuevo if en el padre
}
```

### 3 Crear Thread


Declaramos funcion thread

```
void *funcionThread(void *arg);
```

Variables
```
pthread_t mythread; //Declaramos una variable pthread_t
pthread_create(&mythread, NULL, funcionThread, &i); //Creamos el Thread
```

Creamos funcion del Thread

```
void* funcionThread(void *args){
	int i=*(int *)arg; //Obtenemos el valor i del parametro
	printf("Soy el hilo %d.\n",i);
	//Varias operaciones
	pthread_exit(NULL); //Recordar salir del thread al terminar
}
```

### 4 Ficheros

Declaramos variables
```
FILE *f;
char variableAEscribir[SIZE];
char variableDondeGuardar[SIZE];
```

Creamos fichero

```
f=fopen("file.txt", "w");
```

Editar fichero
```
fwrite(variableAEscribir, sizeof(char), strlen(variableAEscribir), f);
```

Leer fichero
```
fread(variableDendeGuardar, sizeof(char), SIZE, f);
```

Cerrar fichero
```
fclose(f);
```

### 5 Leer del teclado

Strings
```
char frase[SIZE];
fgets(frase, sizeof(frase), stdin);
```

Ints
```
int val;
scanf("%d", &val);
```

## Practica 2 - Pipe, Fifo y Colas de mensajes

## PIPE

### 1 Creamos Pipe
```
int fd[2];
pipe(fd);
```


### 2 Escritura
```
write(fd[1], buffer, strlen(buffer));
```

### 3 Lectura
```
read(fd[0], buffer, SIZE);
```


### IMPORTANTE

**Solo vale para procesos con un mismo padre.**

**Es buena idea declarar el fifo bien como global, o si no, en el padre**


## FIFO

### 1 Creamos Fifo
```
fd=mkfifo("Nombre", 0_RDWR); //Read & Write
```
### 2 Escritura
```
write(fd, buffer, strlen(buffer));
```

### 3 Lectura
```
read(fd, buffer, SIZE);
```

### IMPORTANTE
**Puede ser usado por procesos con diferente padre**


## Colas


### 1 Declaramos estructura
```
struct Msg{
	long type;
	char msg[SIZE];
	}
```

### 2 Declaramos variables
```
struct Msg buffer;
int idCola;
```

### 3 Obtenemos identificador
```
idCola = msgget(CLAVE, 0666 | IPC_CREAT) //--> Crear y Usar cola
```

or

```
idCola = msgget(CLAVE, 0666) //--> Usar cola
```

### 4 Escritura
```
buffer.type = CanalEscritura;
msgsnd(idCola, &buffer, strlen(buffer), 0);
```

### 5 Lectura
```
msgrcv(idCola, &buffer, sizeof(buffer), canalLectura, 0);
```

### 6 Eliminar cola
```
msgctl(idCola, IPC_RMID, NULL); //Elimina Cola (Hacer donde se creo)
```

## Practica 3
## MEMORIA COMPARTIDA (Shared memory)

### 1 Acceso memoria compartida
```
Importante -> #define CLAVE 0x12345678L
int idCola;
```

Crear
```
idCola = shget(CLAVE, 1024, 0666 | IPC_CREAT);
```

Acceder
```
idCola = shget(CLAVE, 1024, 0666);
```

### 2 Obtener puntero de memoria

Como char
```
char *punteroChar;

punteroChar = (char *)shmat(idMem, NULL, 0);
```

Como Int
```
int *punteroInt;

punteroInt = (int *)shmat(idMem, NULL, 0);
```

Como estructura
```
struct Msg{
	char[5] a
	int b
	int c
};

struct Msg *punteroMsg

punteroMsg = (Msg *)shmat(idMem, NULL,0);
```

### 3 Escritura/Lectura en memoria
(Se mostrara como realizar la escritura, la lectura sera equivalente si intercambiamos las variables puntero y dato de lugar)

Como char
```
char dataChar[10]
strcpy(punteroChar, dataChar);
```

Como Int -> Usaremos [0] en el puntero para saltar de int en int en la memoria
```
Int dataInt;
punteroInt[0] = dataInt;
```

Como estructura
```
struct Msg dataMsg;

strcpy(punteroMsg[0].a, dataMsg.a)
punteroMsg[0].b = dataMsg.b
punteroMsg[0].c = dataMsg.c
```

### Cerrar y Borrar Memoria
```
shmdet(puntero)

shmctl(idMem,IPC_RMID)
```

## SEMAFOROS

### 1 - Definir operaciones
```
struct sembuf down0 = {0, -1, 0}; // {Nº Sem, operacion, 0}
struct sembuf up0 = {0, 1, 0};

struct sembuf down1 = {1, -1, 0};
struct sembuf up1 = {1, 1, 0};
```

OJO se pueden unir para hacer operaciones dobles
```
struct sembuf sembufs[2] = {up0,down1};
```

### 2 - Obtener identificadaor / Crear semaforo
```
General: semget(CLAVE,Nº Sems,0666)

idSem = semget(CLAVE,2,0666|IPC_CREAT) //Crear semaforo + obtener id

idSem = semget(CLAVE,2,0666) // Obtener id
```

### 3 - Inicializar semaforo
```
General: semctl(idSem,Nº de Sem,SETVAL,0/1)

semctl(idSem,0,SETVAL,1) //Inicializacion semaforo 0 a 1
```

### 4 - Up & down
```
General: semop(idSem,Operador,Nº de operaciones (Longitud array)) //"&" si es solo uno, nada si es un array

semop(idSem,&up0,1)

semop(idSem,sembufs,2)
```
## SOCKET TCP

### IMPORTANTE

**Hacer uso de **sprintf** para pasar de Int a String o de String a String mas completo

**Hacer uso de **sscanf** para extraer un int de un String

## CLIENT TCP

### 1 - Declaración
```
int sockfd;
struct sockaddr_in serverAddr;
char buffeer[128];
```

### 2 - Configuración serverAddr & sockfd
```
serverAddr.sin_family = AF_INET;
serverAddr.sin_port = htons(PORT); //Host to nework
serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Destino

+

sockfd = socket(AF_INET,SOCK_STREAM,0);
```

### 3 - Conexión
```
connect(sockfd,(struct sockaddr *) &serverAddr,sizeof(serverAddr))
```

### 4 - Envío de info
```
write(sockfdmbuffer,strlen(buffer))
```
### 5 - Leeer info
```
read(sockfd,buffer,sizeof(buffer))
```

## SERVIDOR TCP

### 1 - Declaración
int sockfd, clientfd;
struct sockaddr_in serverAddr, clientAddr;
char buffer[128];

### 2 - Configuración serverAddr & sockfd

### 3 - Conexión

### 4 - Envío de info

### 5 - Leeer info
