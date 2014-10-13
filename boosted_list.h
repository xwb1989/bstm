/*
 * =====================================================================================
 *
 *       Filename:  boosted_list.h
 *
 *    Description:  boosted list
 *
 *        Version:  1.0
 *        Created:  10/13/2014 01:45:12 AM
 *       Compiler:  gcc
 *
 *         Author:  Wenbin Xiao (github.com/xwb1989)
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef BOOSTED_LIST_H
#define BOOSTED_LIST_H

#include "base_container.h"

#include "list.h"
#include "thread.h"
#include "types.h"


class BoostedList: public BaseContainer {
    private:
        list_t base_list;

    public:
        //TM operations
        bool_t tm_insert();
        void tm_clear();



        //Non TM operations
        void* find(void* data);
        long size();
        bool_t insert(void* data);
        bool_t is_empty();
        bool_t has_next();
        list_t next();
        list_t reset();
        bool_t remove(void* data);
        




};

#endif
