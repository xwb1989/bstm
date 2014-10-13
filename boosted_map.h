/*
 * =====================================================================================
 *
 *       Filename:  bmap.h
 *
 *    Description:  boosted map
 *
 *        Version:  1.0
 *        Created:  08/02/2014 09:32:10 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wenbin Xiao (github.com/xwb1989), xwb1989@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef BOOSTED_MAP_H
#define BOOSTED_MAP_H

#include "base_container.h"

//concurrent hash map
#include "tbb/concurrent_hash_map.h"
#include "thread.h"
#include "types.h"

typedef tbb::concurrent_hash_map<long, void*> HashMap;

class BoostedMap: public BaseContainer {
    private: 
        HashMap hash_map;
    public:
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
