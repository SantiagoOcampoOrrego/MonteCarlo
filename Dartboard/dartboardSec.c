#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

double rand_double(){
    return ((double) rand() / (double) RAND_MAX);
}

int main(int argc, char* argv[]){

    int darts = atoi(argv[1]);
    int in_circle = 0;
    int i;

    srand(time(NULL));

    struct timespec tstart={0,0}, tend={0,0};

    clock_gettime(CLOCK_MONOTONIC, &tstart);

    for(i=0; i<darts; i++){
        double x = rand_double();
        double y = rand_double();

        if(x*x + y*y < 1.0){
            in_circle++;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &tend);

    printf("Tiempo de ejecucion: %.5f segundos\n",
           ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
           ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));

    //printf("%f\n", 4 * (double) in_circle / darts);

    return 0;
}
