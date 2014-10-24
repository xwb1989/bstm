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

#include "list_set.h"
#include "random.h"
#include "timer.h"

enum ListType {TL2, BOOST};
class Config {
    public:
        SET_T* set_ptr;
        long low;
        long high;

        Config(SET_T* _set_ptr, long _low, long _high) {
            set_ptr = _set_ptr;
            low = _low;
            high = _high;
        }
};

void initialize(void* list_ptr, int size);

#endif
