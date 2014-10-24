/*
 * =====================================================================================
 *
 *       Filename:  list_workload.cpp
 *
 *    Description:  list workload
 *
 *        Version:  1.0
 *        Created:  10/17/2014 11:47:26 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wenbin Xiao (github.com/xwb1989)
 *   Organization:  
 *
 * =====================================================================================
 */
#include "work_load.h"

void set_initialize(SET_T* set, long size) {
    long i;
    for (i = size; i > 0; i--) {
        assert(SET_INSERT(set, i));
    }
}

void build_configs(Config** configs, int numThread, SET_T* set, long range) {
    long curr_low = 0;
    for (int i = 0; i < numThread; i++) {
        configs[i] = new Config(set, curr_low, curr_low+range);
        curr_low += range;
    }
}

void run(void* configs) {

}

int main(int argc, char** args) {
    TIMER_T start;
    TIMER_T stop;

    GOTO_REAL();

    /* Initialization */
    SIM_GET_NUM_CPU(global_params[PARAM_CLIENTS]);
    long numThread = 4;
    long size = 1024;
    long range = size / numThread;
    SET_T* set = SET_ALLOC();

    set_initialize(set, size);

    Config* configs[numThread];
    build_configs(configs, numThread, set, range);

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


    SET_FREE(set);

    puts("done.");
    fflush(stdout);

    TM_SHUTDOWN();
    P_MEMORY_SHUTDOWN();

    GOTO_SIM();

    thread_shutdown();
    return 0;
}
