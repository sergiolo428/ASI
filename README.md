# ASI_22-23 apuntes

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
pthread_create(&mythread,NULL,funcionThread,&i); //Creamos el Thread
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
f=fopen("file.txt","w");
```

Editar fichero
```
fwrite(variableAEscribir,sizeof(char),strlen(variableAEscribir),f);
```

Leer fichero
```
fread(variableDendeGuardar,sizeof(char),SIZE,f);
```

Cerrar fichero
```
fclose(f);
```

### 5 Leer del teclado

Strings
```
char frase[SIZE];
fgets(frase,sizeof(frase),stdin);
```

Ints
```
int val;
scanf("%d",&val);
```

## Practica 2 - Pipe, Fifo y Colas de mensajes

## Pipe

### 1 Creamos Pipe
```
int fd[2];
pipe(fd);
```


### 2 Escritura
```
write(fd[1],buffer,strlen(buffer));
```

### 3 Lectura
```
read(fd[0],buffer,SIZE);
```


### IMPORTANTE

**Solo vale para procesos con un mismo padre.**

**Es buena idea declarar el fifo bien como global, o si no, en el padre**


## Fifo

### 1 Creamos Fifo

fd=mkfifo("Nombre",0_RDWR); //Read & Write

### 2 Escritura
```
write(fd,buffer,strlen(buffer));
```

### 3 Lectura
```
read(fd,buffer,SIZE);
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
idCola = msgget(CLAVE,0666|IPC_CREAT) //--> Crear y Usar cola
```

or

```
idCola = msgget(CLAVE,0666) //--> Usar cola
```

### 4 Escritura
```
buffer.type = CanalEscritura;
msgsnd(idCola,&buffer,strlen(buffer),0);
```

### 5 Lectura
```
msgrcv(idCola,&buffer,sizeof(buffer),canalLectura,0);
```

### 6 Eliminar cola
```
msgctl(idCola,IPC_RMID,NULL); //Elimina Cola (Hacer donde se creo)
```

## Practica 3 - //TO DO
