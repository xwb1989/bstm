/*
 * =====================================================================================
 *
 *       Filename:  test-stm.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/19/2015 09:27:38 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "skiplist.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include "../../../memory.h"
#include "../../../random.h"
#include "../../../thread.h"
#include "../../../timer.h"
#include "../../../tm.h"
#include "../../../types.h"
#include "../../../utility.h"

#define NTHREAD 4

double global_params[256]; /* 256 = ascii limit */
typedef struct config {
        MAP_T* map_ptr;
        long low;
        long high;
        long n_op;
        long n_tx;
        random_t* random_ptr;
} config_t;



void map_initialize(MAP_T* map, long size) {
    for (; size > 0; size--) {
        assert(MAP_INSERT(map, size, size));
    }
}


config_t* config_new(MAP_T* map_ptr, long curr_low, long curr_high, int n_op, int n_tx) {
    config_t* config = malloc(sizeof(config_t)); 
    config->map_ptr = map_ptr;
    config->low = curr_low;
    config->high = curr_high;
    config->n_op = n_op;
    config->n_tx = n_tx;
    config->random_ptr = random_alloc();

    return config;
}
void build_configs(config_t** configs, long numThread, MAP_T* map_ptr) {
    long size = 2 << 16;
    long n_op = 1;
    long n_tx = 2 << 16;


    long curr_low = 0;
    long curr_high = size-1;

    map_initialize(map_ptr, size);

    for (int i = 0; i < numThread; i++) {
        configs[i] = config_new(map_ptr, curr_low, curr_high, n_op, n_tx/numThread);
    }
    fflush(stdout);
}


void run(void* configs) {
    TM_THREAD_ENTER();
    int myId = thread_getId();
    config_t* config = ((config_t**)configs)[myId];

    random_t* random_ptr = config->random_ptr;
    int range = config->high - config->low;
    int n_op = config->n_op;
    int n_tx = config->n_tx;
    MAP_T* map_ptr = config->map_ptr;

    ulong_t data[n_op];
    for (int i = 0; i < n_op; i++) {
        data[i] = random_generate(random_ptr) % range + config->low +1;
    }
    for (; n_tx > 0; n_tx--) {
        TM_BEGIN();
        for (int i = 0; i < n_op; i++) {
            if (!TMMAP_FIND(map_ptr, data[i])) {
                TM_RESTART();
            }
            if (!TMMAP_REMOVE(map_ptr, data[i])) {
                TM_RESTART();
            }
            if (!TMMAP_INSERT(map_ptr, data[i], data[i])) {
                TM_RESTART();
            }
        }
        TM_END();
    }
    TM_THREAD_EXIT();
}

int main(int argc, char** args) {
    TIMER_T start;
    TIMER_T stop;

    GOTO_REAL();

    /* Initialization */
//    SIM_GET_NUM_CPU(global_params[PARAM_CLIENTS]);

    long numThread = NTHREAD;
    config_t* configs[numThread];
    MAP_T* map_ptr = (MAP_T*) malloc(sizeof(MAP_T));
    build_configs(configs, numThread, map_ptr);


    TM_STARTUP(numThread);
    P_MEMORY_STARTUP(numThread);
    thread_startup(numThread);

    /* Run transactions */
    printf("Running clients... ");
    fflush(stdout);
    TIMER_READ(start);
    GOTO_SIM();
#ifdef OTM
#pragma omp parallel
    {
        run(configs);
    }
#else
    thread_start(run, (void*) configs);
#endif
    GOTO_REAL();
    TIMER_READ(stop);
    puts("done.");
    printf("Time = %0.6lf\n",
           TIMER_DIFF_SECONDS(start, stop));
    fflush(stdout);

    /* Clean up */
    printf("Deallocating memory... ");
    fflush(stdout);


    MAP_FREE(map_ptr);

    puts("done.");
    fflush(stdout);

    TM_SHUTDOWN();
    P_MEMORY_SHUTDOWN();

    GOTO_SIM();

    thread_shutdown();
    return 0;
}
