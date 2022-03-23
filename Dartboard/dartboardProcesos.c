#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

#define MAX_PROCESSES 4

struct args{
    int index;
    int operations;
};

int in_circle[MAX_PROCESSES];

double rand_double(){
    return ((double) rand() / (double) RAND_MAX);
}

void *calc(void *arguments){
    struct args *data = arguments;
    int i;

    for(i=0; i<data->operations; i++){
        double x = rand_double();
        double y = rand_double();

        if(x*x + y*y < 1.0){
            in_circle[data->index]++;
        }
    }
}

int main(int argc, char* argv[]){

    int darts = atoi(argv[1]);

    int i;
    struct args arguments[MAX_PROCESSES];
    pthread_t tid[MAX_PROCESSES];
    int accum = 0;

    srand(time(NULL));

    struct timespec tstart={0,0}, tend={0,0};

    clock_gettime(CLOCK_MONOTONIC, &tstart);

    for(int i=0; i<MAX_PROCESSES; i++){
        arguments[i].index = i;
        if(i == MAX_PROCESSES-1 && darts % MAX_PROCESSES != 0){
            arguments[i].operations = darts - (darts / MAX_PROCESSES) * i;
        }
        else arguments[i].operations = darts / MAX_PROCESSES;

        pthread_create(&tid[i], NULL, &calc, &arguments[i]);
    }

    for(i = 0; i < MAX_PROCESSES; i++){
        pthread_join(tid[i], NULL);
    }

    for(i = 0; i < MAX_PROCESSES; i++){
        accum += in_circle[i];
    }

    clock_gettime(CLOCK_MONOTONIC, &tend);

    printf("Tiempo de ejecucion: %.5f segundos\n",
           ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
           ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));

    //printf("%f\n", 4 * (double) accum / darts);

    return 0;
}
