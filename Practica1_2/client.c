/*Sergio Lopez Alejandro - Practica1_2 */

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
int main(int argc, char** argv) {
    int val;
    
    while(1){
        printf("Introduce el ejercicio a realizar: ");
        scanf("%d",&val);
        switch(val){
            case 1:
                printf("Ejercicio 1: \n");
                break;
            case 2:
                printf("Ejercicio 2: \n");
                break;
            case 3:
                printf("Ejercicio 3: \n");
                break;
            case 4:
                printf("Ejercicio 4: \n");
                break;
            case 5:
                printf("Ejercicio 5: \n");
                break;
            case 6:
                printf("Ejercicio 6: \n");
                break;
            default:
                printf("Programa finalizado.\n");
                _exit(0);
        }
    }
    return (EXIT_SUCCESS);
}

