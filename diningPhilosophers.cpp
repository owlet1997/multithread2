//
// Created by owlet on 25.11.2019.
//
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <ctime>

#include "omp.h"

#define PHILO_COUNT 5
#define FORKS_COUNT 5

FILE *file;

omp_lock_t forks[FORKS_COUNT];

void think(int i) {
    int secs = 1 + (rand() % PHILO_COUNT);
    printf("Философ %d думает %d секунд...\n", i, secs);
    fprintf(file, "Философ %d думает %d секунд...\n", i, secs);
    sleep(secs);
}

void eat(int i) {
    int fork_first;
    int fork_second;

    if (i == PHILO_COUNT - 1) {
        fork_first = 0;
        fork_second = i;
    } else {
        fork_first = i;
        fork_second = i + 1;
    }
    fprintf(file, "Философ %d просит вилку %d и %d\n", i, fork_first, fork_second);
    fflush(file);
    printf("Философ %d просит вилку %d и %d\n", i, fork_first, fork_second);
    omp_set_lock(&forks[fork_first]);
    omp_set_lock(&forks[fork_second]);

    //симуляция процесса поедания пищи
    int secs = rand() % PHILO_COUNT;
    printf("Философ %d ест %d секунд\n...", i, secs);
    fprintf(file, "Философ %d ест %d секунд\n...", i, secs);
    fflush(file);

    omp_unset_lock(&forks[fork_second]);
    omp_unset_lock(&forks[fork_first]);
}

void simulation(int i) {
    while (true) {
        think(i);
        eat(i);
    }
}

void diningPhilosophers() {

    int i;

    srand(time(NULL));
    omp_set_num_threads(PHILO_COUNT);

    file = fopen("/home/owlet/CLionProjects/multithread5/log2.txt", "w");

    for (i = 0; i < PHILO_COUNT; i++) {
        omp_init_lock(&forks[i]);
    }

#pragma omp parallel for private(i)
    for (i = 0; i < PHILO_COUNT; i++) {
        simulation(i);
    }

    fclose(file);
}