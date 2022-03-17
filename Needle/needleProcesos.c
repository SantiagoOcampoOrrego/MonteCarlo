#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

#define MAX_PROCESSES 8

struct Floor{
    double l;
};

struct Needle{
    double x;
    double tetha;
    double L;
};

struct args{
    int index;
    int operations;
};

double L = 1;
struct Floor f = {2};
int nb_crosses[MAX_PROCESSES];

double rand_double(){
    return ((double) rand() / (double) RAND_MAX);
}

struct Needle toss_needle(){
    double x = rand_double() * f.l;
    double tetha = rand_double() * M_PI;

    //printf("%f %f\n", x, tetha);

    struct Needle needle = {x, tetha, L};

    return needle;
}

int cross_line(struct Needle needle){
    double x_right_tip = needle.x + needle.L / 2 * sin(needle.tetha);
    double x_left_tip = needle.x - needle.L / 2 * sin(needle.tetha);
    return x_right_tip > f.l || x_left_tip < 0.0;
}

void *calc(void *arguments){
    struct args *data = arguments;
    int i;

    for(i=0; i<data->operations; i++){
        struct Needle needle = toss_needle();
        if(cross_line(needle)){
            nb_crosses[data->index]++;
        }
    }
}

int main(int argc, char* argv[]){

    int nb_tosses = 100000000;

    int i;
    struct args arguments[MAX_PROCESSES];
    pthread_t tid[MAX_PROCESSES];
    int accum = 0;
    srand(time(NULL));

    struct timespec tstart={0,0}, tend={0,0};

    clock_gettime(CLOCK_MONOTONIC, &tstart);

    for(int i=0; i<MAX_PROCESSES; i++){
        arguments[i].index = i;
        if(i == MAX_PROCESSES-1 && nb_tosses % MAX_PROCESSES != 0){
            arguments[i].operations = nb_tosses - (nb_tosses / MAX_PROCESSES) * i;
        }
        else arguments[i].operations = nb_tosses / MAX_PROCESSES;

        pthread_create(&tid[i], NULL, &calc, &arguments[i]);
    }

    for(i = 0; i < MAX_PROCESSES; i++){
        pthread_join(tid[i], NULL);
    }

    for(i = 0; i < MAX_PROCESSES; i++){
        accum += nb_crosses[i];
    }

    clock_gettime(CLOCK_MONOTONIC, &tend);

    printf("Tiempo de ejecucion: %.5f segundos\n",
           ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
           ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));

    //printf("%f\n", (double)nb_tosses / accum);

    return 0;
}
