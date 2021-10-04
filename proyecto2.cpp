/*Proyecto 2 - Calculo de Pi
* 
* Contribuyentes:
* Dariel Villatoro - 20776
* Javier Hernandez - 
*
* Este programa emplea pthreads y mutex para un claculo aproximado de Pi.
*/

#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<iostream>
#include<math.h>

using namespace std;

#define N 1e5       // Se efectuara el calculo con 10000 segmentos
#define threads 8   // Se emplearan 8 hilos

pthread_t tid[threads]; // Se declaran los hilos
pthread_mutex_t lock;   // Se declara la variable mutex

double Pi;                // Valor de Pi aproximado

void* CalcPi (void* arg){

    pthread_mutex_lock(&lock); // Bloqueo de mutex
    
    int seq = (uintptr_t) arg; // Desface de las sumas parciales

    double x, y, z; //El calculo se realiza en 3 fases

    // Sumatoria parcial
    for (int i = 0; i < N/threads; i++) { 
        x = ((i * 8) + 1) + seq;
        y = (1/N)*(x - 0.5);
        z = sqrt(1 - pow(y,2));
        Pi += 4*(z/N);
    }

    pthread_mutex_unlock(&lock); // Desbloqueo de mutex

    pthread_exit(NULL);
}

int main () {

    cout<<">Calculo de Pi\n>"<<endl;

    int rc; // Variable de errores

    // Inicializacion de mutex
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

    //Creacion de los hilos
    for (uintptr_t j = 0; j < threads; j++){
        rc = pthread_create(&tid[j],NULL,CalcPi,(void*) j);
        if(rc)
        {
            cout<<"No se pudo crear el hilo "<<j<<endl;
            exit(-1);
        }
    }

    //Se unen todos los hilos que hayan terminado el calculo
    for (int j = 0; j < threads; j++) {
        rc = pthread_join(tid[j],NULL);
        if(rc)
        {
            cout<<"No se pudo unir el hilo "<<j<<endl;
            exit(-1);
        }

    }

    printf(">El valor estimado de Pi es: %lf\n", Pi);

    pthread_exit(NULL);

    pthread_mutex_destroy(&lock);
    
    return 0;
}
