/*
 * =====================================================================================
 *
 *       Filename:  list_workload.h
 *
 *    Description:  workload for list
 *
 *        Version:  1.0
 *        Created:  10/17/2014 11:40:30 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wenbin Xiao (github.com/xwb1989) 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef LIST_WORKLOAD_H
#define LIST_WORKLOAD_H


#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include "list.h"
#include "list_set.h"
#include "memory.h"
#include "random.h"
#include "thread.h"
#include "timer.h"
#include "tm.h"
#include "types.h"
#include "utility.h"

enum ListType {TL2, BOOST};
class Config {
    public:
        SET_T** sets_ptr;
        long low;
        long high;
        long n_op;
        long n_tx;
        random_t* random_ptr;

        Config(SET_T** _sets_ptr, long _low, long _high, long _n_op, long _n_tx, random_t* _random_ptr) {
            sets_ptr = _sets_ptr;
            low = _low;
            high = _high;
            n_op = _n_op;
            n_tx = _n_tx;
            random_ptr = _random_ptr;
        }
};

void initialize(void* list_ptr, int size);
static void shuffle(SET_T** array, size_t n);

#endif
