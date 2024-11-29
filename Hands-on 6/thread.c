#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Estructura para pasar múltiples argumentos a un hilo
typedef struct {
    int thread_id;
    char* message;
} thread_data;

// Función que será ejecutada por los hilos
void* thread_function(void* arg) {
    // Convertir el argumento al tipo correcto
    thread_data* data = (thread_data*)arg;

    printf("Hilo %d iniciado. Mensaje: %s\n", data->thread_id, data->message);

    // Simular trabajo
    sleep(data->thread_id);

    printf("Hilo %d completado.\n", data->thread_id);

    // Devolver un valor de retorno (en este caso, el ID del hilo)
    pthread_exit((void*)(long)data->thread_id);
}

int main() {
    // Número de hilos a crear
    #define NUM_THREADS 3

    // Arreglo de identificadores de hilos
    pthread_t threads[NUM_THREADS];

    // Arreglo de estructuras de datos para hilos
    thread_data thread_args[NUM_THREADS];

    int rc;
    long t;

    printf("Iniciando creación de hilos...\n");

    // Crear hilos
    for(t = 0; t < NUM_THREADS; t++) {
        // Preparar datos para este hilo
        thread_args[t].thread_id = t;
        
        char buffer[50];
        snprintf(buffer, sizeof(buffer), "Mensaje del hilo %ld", t);
        thread_args[t].message = buffer;

        printf("Creando hilo %ld\n", t);
        
        // Crear hilo
        rc = pthread_create(&threads[t], NULL, 
                            thread_function, 
                            (void*)&thread_args[t]);
        
        if (rc) {
            printf("Error al crear hilo %ld. Código: %d\n", t, rc);
            exit(1);
        }
    }

    // Esperar a que terminen todos los hilos
    void* status;
    for(t = 0; t < NUM_THREADS; t++) {
        rc = pthread_join(threads[t], &status);
        
        if (rc) {
            printf("Error al unir hilo %ld. Código: %d\n", t, rc);
            exit(1);
        }
        
        printf("Hilo %ld terminado. Estado de salida: %ld\n", 
               t, (long)status);
    }

    printf("Todos los hilos han sido completados.\n");
    pthread_exit(NULL);
}
