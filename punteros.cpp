#include <iostream>

int main(){
    /*
     * Los punteros almacenan la direccion de memoria de otra variable.
     * Si la variable cambia de valor, el puntero también cambia; a diferencia de las variables.
     */

    int a = 3;
    int b = a;
    int *p = &a;

    printf("El valor de b es: %d\n", b);
    printf("Valor almacenado en la dirMem de la variable 'a' a quien apunta p es: %d\n", *p);

    a = 100;
    printf("El valor de b es: %d\n", b);
    printf("Valor almacenado en la dirMem de la variable 'a' a quien apunta p es: %d\n", *p);

    return 0;
}
