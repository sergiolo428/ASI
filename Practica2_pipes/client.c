
#include <stdio.h>
#include <stdlib.h>

/*
 --------------------NOTAS--------------------
 */
/*

 * PIPES:
 * COnsiste en una tuberia dentro del sistema operativo, mediante la cual
 * los procesos se intercambian informacion
 * 
 * Si un proceso quiere leer pero no hay nada en el pipe, el proceso se bloquearia
 * 
 * Como generamos el pipe:
 * 
 * int fd[2]
 * 
 * pipe( fd[array de dos enteros]) //Donde obtenemos un fd[0] y fd[1]
 * 
 * fd[0] usaremos cuando queramos realizar operaciones de lectura
 * fd[1] usaremos cuando queramos realziar operaciones de escritura
 * 
 * Debemos cerrar los fd los cuales no usaremos para evitar problemas: usaremos close(fd[])
 * Es decir, si un proceso solo va a actuar como consumidor,
 * deberemos cerrar el f[1] ya que no va a producir info en el pipe
 * 
 * 
 * Escritura:
 * 
 * write(fd[1],buffer,size of datos a enviar)
 * 
 * 
 * Lectura:
 * 
 * read(fd[0],buffer, sizeof(buffer))
 * 
 * 
 * -----IMPORTANTE-----:
 * 
 * >> Esta counicacion solo vale para procesos que compartan el mismo padre!!!
 * 
 * >> Debemos de crear este pipe(fd) en el proceso padre para que sea un recurso compartido por los hijos
 * 
 * 
 
*/
/*
 ^^--------------------NOTAS--------------------^^
 */
int main(int argc, char** argv) {

    return (EXIT_SUCCESS);
}

