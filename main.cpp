#include <iostream>
#include <omp.h>

int main()
{
    // Region paralela, ie: PARALEL REGIONS
    printf("Paralell regions:\n");

#pragma omp parallel num_threads(3)
    {
        const int num_hilos = omp_get_num_threads();
        const int id_hilo = omp_get_thread_num();
        printf("Hola mundo desde el hilo %d de %d hilos\n", id_hilo, num_hilos);
    }


    /* Repartición de trabajo en bucles, ie: LOOP WORK SHARING
     * Cada hilo se encarga de una iteración del bucle hasta que se acaben las iteraciones.
     * La directiva "#pragma omp for" NO puede estar fuera de una región paralela. De ley debe estar dentro de una
     * región paralela "#pragma omp parallel".
     * Si dentro de la región paralela solo se va a ejecutar un bucle for, ahi puedo usar directamente la directiva
     * "#pragma omp parallel for". A esta forma de ejecutar un bucle se le llama "Parallel Regions and Work Sharing".
     */
    printf("\nLoop work sharing:\n");

#pragma omp parallel
    {
#pragma omp for
        for (int i = 0; i < 11; i++)
        {
            printf("Hilo %d: Iteracion %d\n", omp_get_thread_num(), i);
        }
    }


    /* Regiones paralelas y trabajo compartido, ie: PARALLEL REGIONS AND WORK SHARING
     * Como lo único que se ejecuta dentro de la región paralela es un bucle for, puedo usar la directiva "#pragma omp
     * parallel for" para ejecutar el bucle y la región paralela al mismo tiempo. No debo usar llaves "{}" para
     * delimitar el bucle for.
    */
    printf("\nParallel regions and work sharing:\n");

#pragma omp parallel for
    for (int i = 0; i < 11; i++)
    {
        printf("Hilo %d: Iteracion %d\n", omp_get_thread_num(), i);
    }

    /* Dentro de PARALLEL REGIONS AND WORK SHARING, puedo usar la directiva "reduction" para realizar operaciones
     * de reducción, como sumas, multiplicaciones, etc. La directiva "reduction" se usa para que cada hilo tenga su
     * propia copia de la variable y al final se realice la operación de reducción.
     * Es necesario usar la directiva "reduction" para evitar condiciones de carrera, que son errores que se dan
     * cuando dos o más hilos intentan modificar una variable al mismo tiempo.
     */
    printf("\nParallel regions and work sharing con reduction:\n");
    int mi_suma = 0;
#pragma omp parallel for reduction(+:mi_suma)
    for (int i = 0; i <= 10; i++)
    {
        mi_suma = mi_suma + i;
    }

    printf("La suma de los primeros 10 numeros es: %d\n", mi_suma);

    return 0;
}
