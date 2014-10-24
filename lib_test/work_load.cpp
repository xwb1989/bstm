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
    for (; size > 0; size--) {
        assert(SET_INSERT(set, size));
    }
}

void build_configs(Config** configs, int numThread, SET_T* set, long n_op, long n_tx, long range) {
    long curr_low = 0;
    random_t* random_ptr = random_alloc();
    for (int i = 0; i < numThread; i++) {
        configs[i] = new Config(set, curr_low, curr_low+range, n_op, n_tx, random_ptr);
        curr_low += range;
    }
}

void run(void* configs) {
    TM_THREAD_ENTER();
    int myId = thread_getId();
    Config* config = ((Config**)configs)[myId];

    random_t* random_ptr = config->random_ptr;
    int range = config->high - config->low;
    int n_op = config->n_op;
    int n_tx = config->n_tx;
    SET_T* set_ptr = config->set_ptr;

    ulong_t data;
    for (; n_tx > 0; n_tx--) {
        TM_BEGIN();
        for (int i = 0; i < n_op; i++) {
            data = random_generate(random_ptr) % range + config->low +1;
            printf("%i onfire: %ld\n", myId, data);
            assert(TMSET_FIND(set_ptr, data));
            assert(TMSET_REMOVE(set_ptr, data));
            assert(TMSET_INSERT(set_ptr, data));
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
    SIM_GET_NUM_CPU(global_params[PARAM_CLIENTS]);
    long numThread = 4;
    long size = 1024;
    long range = size / numThread;
    long n_op = 4;
    long n_tx = 1024;
    SET_T* set = SET_ALLOC();

    set_initialize(set, size);

    Config* configs[numThread];
    build_configs(configs, numThread, set, n_op, n_tx, range);

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
