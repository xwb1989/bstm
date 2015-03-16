/*
 * =====================================================================================
 *
 *       Filename:  boosted_skiplist.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/12/2015 07:33:33 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wenbin Xiao (github.com/xwb1989) 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef BOOST_SKIPLIST_H 
#define BOOST_SKIPLIST_H



#include "include/base_container.h"
#include "include/skiplist/sl-map.h"

class BoostedSkiplist: public BaseContainer {
    private:
        sl_map_t* map;

    public:
        //Constructor
        BoostedSkiplist():BaseContainer(), map(sl_map_new()) {}

        //Destructor
        ~BoostedSkiplist();

        //tx operations
        bool_t tm_insert(long key, void* val);
        bool_t tm_remove(long key);
        bool_t tm_contains(long key);
        void* tm_find(long key);

        //standard operations
        bool_t insert(long key, void* val);
        bool_t remove(long key);
        bool_t contains(long key);
        void* find(long key);
};



#endif
