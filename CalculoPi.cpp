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

pthread_t tid [threads];

void* CalcPi (void* arg){
    int seq = (uintptr_t) arg;
    float P;
    float x;
    float y;

    for (int i = 1; i <= N/threads; i++) { 
        x = (1/N)*((i + seq) - 0.5);
        y = sqrt(1 - pow(x,2));
        P += 4*(y/N);
    }

    pthread_exit((void*) P);
}

int main () {

    cout<<"Calculo de Pi\n"<<endl;

    int rc;
    void* retorno;
    long resultado;
    long total;

    pthread_t tid [8];

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


    printf("El valor aproximado de Pi es: %ld\n", total);

    pthread_exit(NULL);

    return 0;
}