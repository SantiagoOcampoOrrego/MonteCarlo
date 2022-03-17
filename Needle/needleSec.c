#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

struct Floor{
    double l;
};

struct Needle{
    double x;
    double tetha;
    double L;
};

double rand_double(){
    return ((double) rand() / (double) RAND_MAX);
}

struct Needle toss_needle(double L, struct Floor floor){
    double x = rand_double() * floor.l;
    double tetha = rand_double() * M_PI;

    //printf("%f %f\n", x, tetha);

    struct Needle needle = {x, tetha, L};

    return needle;
}

int cross_line(struct Needle needle, struct Floor floor){
    double x_right_tip = needle.x + needle.L / 2 * sin(needle.tetha);
    double x_left_tip = needle.x - needle.L / 2 * sin(needle.tetha);
    return x_right_tip > floor.l || x_left_tip < 0.0;
}

int main(int argc, char* argv[]){

    int nb_tosses = 100000000;

    srand(time(NULL));
    double L = 1;
    struct Floor floor = {2};
    int nb_crosses = 0;
    int i;

    struct timespec tstart={0,0}, tend={0,0};

    clock_gettime(CLOCK_MONOTONIC, &tstart);

    for(i=0; i<nb_tosses; i++){
        struct Needle needle = toss_needle(L, floor);
        if(cross_line(needle, floor)){
            nb_crosses++;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &tend);

    printf("Tiempo de ejecucion: %.5f segundos\n",
           ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
           ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));

    //printf("%f\n", (double)nb_tosses / nb_crosses);

    return 0;
}
