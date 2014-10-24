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

double global_params[256]; /* 256 = ascii limit */

void set_initialize(SET_T* set, long size) {
    for (; size > 0; size--) {
        assert(SET_INSERT(set, size));
    }
}

void build_configs(Config** configs, long numThread, SET_T** sets, long n_op, long n_tx, long size, long per) {
    for (int i = 0; i < n_op; i++) {
        sets[i] = SET_ALLOC();
        set_initialize(sets[i], size);
    }
    long curr_low = 0;
    long range = (long)(size * 1.0 / (100/per));
    long step = (long)((size - range)*1.0 / (numThread-1));

    printf("Configurations: \n");
    printf("    Transactions        = %li\n", n_tx);
    printf("    Clients             = %li\n", numThread);
    printf("    Transactions/client = %li\n", n_tx/numThread);
    printf("    Sets/transaction    = %li\n", n_op);
    printf("    Relations           = %li\n", size);
    printf("    Query percent       = %li\n", per);
    printf("    Query range         = %li\n", range);

    random_t* random_ptr = random_alloc();
    printf("Query Range:\n");
    for (int i = 0; i < numThread; i++) {
        printf("    Client %d   = %li - %ld\n", i, curr_low+1, curr_low+range); 
        configs[i] = new Config(sets, curr_low, curr_low+range, n_op, n_tx/numThread, random_ptr);
        curr_low += step;
    }
    fflush(stdout);
}

void run(void* configs) {
    TM_THREAD_ENTER();
    int myId = thread_getId();
    Config* config = ((Config**)configs)[myId];

    random_t* random_ptr = config->random_ptr;
    int range = config->high - config->low;
    int n_op = config->n_op;
    int n_tx = config->n_tx;
    SET_T** sets_ptr = config->sets_ptr;

    ulong_t data[n_op];
    for (int i = 0; i < n_op; i++) {
        data[i] = random_generate(random_ptr) % range + config->low +1;
    }
    for (; n_tx > 0; n_tx--) {
        TM_BEGIN();
        for (int i = 0; i < n_op; i++) {
            assert(TMSET_FIND(sets_ptr[i], data[i]));
            assert(TMSET_REMOVE(sets_ptr[i], data[i]));
            assert(TMSET_INSERT(sets_ptr[i], data[i]));
        }
        TM_END();
    }
    TM_THREAD_EXIT();
}


enum param_types {
    PARAM_CLIENTS      = (unsigned char)'c',
    PARAM_NUMBER       = (unsigned char)'n',
    PARAM_RELATIONS    = (unsigned char)'r',
    PARAM_PERCENTAGE   = (unsigned char)'p',
    PARAM_TRANSACTIONS = (unsigned char)'t',
};


/* =============================================================================
 * parseArgs
 * =============================================================================
 */
static void
parseArgs (long argc, char* const argv[])
{
    long i;
    long opt;

    opterr = 0;

    while ((opt = getopt(argc, argv, "c:n:r:p:t:")) != -1) {
        switch (opt) {
            case 'c':
            case 'n':
            case 'r':
            case 'p':
            case 't':
                global_params[(unsigned char)opt] = atol(optarg);
                break;
            case '?':
            default:
                opterr++;
                break;
        }
    }

    for (i = optind; i < argc; i++) {
        fprintf(stderr, "Non-option argument: %s\n", argv[i]);
        opterr++;
    }

    if (opterr) {
        printf("wrong command.\n");
    }
}



int main(int argc, char** args) {
    parseArgs(argc, (char** const)args);

    TIMER_T start;
    TIMER_T stop;

    GOTO_REAL();

    /* Initialization */
//    SIM_GET_NUM_CPU(global_params[PARAM_CLIENTS]);
    long numThread = global_params[PARAM_CLIENTS];
    long size = global_params[PARAM_RELATIONS];
    long n_op = global_params[PARAM_NUMBER];
    long n_tx = global_params[PARAM_TRANSACTIONS];
    long per = global_params[PARAM_PERCENTAGE];

    SET_T* sets[n_op];

    Config* configs[numThread];
    build_configs(configs, numThread, sets, n_op, n_tx, size, per);

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


    for (int i = 0; i < n_op; i++) {
        SET_FREE(sets[i]);
    }

    puts("done.");
    fflush(stdout);

    TM_SHUTDOWN();
    P_MEMORY_SHUTDOWN();

    GOTO_SIM();

    thread_shutdown();
    return 0;
}
