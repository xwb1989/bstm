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
#include <mutex>

class BoostedList: public BaseContainer {
    private:
        list_t* base_list;
        list_iter_t* list_iter;
        std::mutex list_lock;


    public:
        //Constructor
        BoostedList() {
            base_list = list_alloc(NULL);
            list_iter_reset(list_iter, base_list);
        }

        ~BoostedList() {
            list_free(base_list);
        }

        //TM operations
        void tm_clear();
        long tm_size();
        void* tm_find(void* data);
        bool_t tm_insert(void* data);
        bool_t tm_remove(void* data);
        void* tm_next(list_iter_t* itr_ptr);
        bool_t tm_has_next(list_iter_t* itr_ptr);
        void tm_reset(list_iter_t* itr_ptr);
        bool_t tm_is_empty();



        //Non TM operations
        void clear();
        long size();
        void* find(void* data);
        bool_t insert(void* data);
        bool_t remove(void* data);
        list_t next();
        bool_t has_next();
        list_t reset();
        bool_t is_empty();


};

#endif
