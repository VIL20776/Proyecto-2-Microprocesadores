#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<iostream>
#include<math.h>

using namespace std;

#define N 1e5
#define threads 8

pthread_t tid[threads];

pthread_mutex_t lock;
double P;

void* CalcPi (void* arg){
    pthread_mutex_lock(&lock);
    
    int seq = (uintptr_t) arg;
    float x;
    float y;

    for (int i = 1; i <= N/threads; i++) { 
        x = (1/N)*((i + seq) - 0.5);
        y = sqrt(1 - pow(x,2));
        P += 4*(y/N);
    }

    pthread_mutex_unlock(&lock);
    return NULL;
}

int main () {

    cout<<"Calculo de Pi\n"<<endl;

    int rc;
    void* retorno;
    long resultado;
    long total;

    pthread_t tid[8];

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

    for (int j = 0; j < 8; j++){
        rc = pthread_create(&tid[j],NULL,CalcPi,(void*)j);
        if(rc)
        {
            cout<<"No se pudo crear el hilo "<<j<<endl;
            exit(-1);
        }
    }

    for (int j = 0; j < 8; j++) {
        //Podemos recibir del hilo un valor mediante join
        rc = pthread_join(tid[j],&retorno);
        if(rc)
        {
            cout<<"No se pudo unir el hilo "<<j<<endl;
            exit(-1);
        }

        resultado = (long) retorno;
        total += resultado;
    }


    printf("\n El valor estimado de Pi es: %f\n", P);

    pthread_exit(NULL);

    pthread_mutex_destroy(&lock);
    
    return 0;
}
