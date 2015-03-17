/*
 * =====================================================================================
 *
 *       Filename:  base_container.h
 *
 *    Description:  base container; all boosted container must inherit this class
 *
 *        Version:  1.0
 *        Created:  07/27/2014 09:54:30 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wenbin Xiao (github.com/xwb1989), xwb1989@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef BASE_CONTAINER_H
#define BASE_CONTAINER_H

#include <vector>

#include "abstract_lock.h"
#include "tbb/concurrent_hash_map.h"



class BaseContainer {
    protected:
        //cannot use transaction memory address as key because transactions are managed by container,
        //when they are reallocated, the addresses also change.

        typedef std::vector<std::function<bool()>> Log;
        typedef tbb::concurrent_hash_map<long, Log> LogMap;
        AbstractLock locks;

        //a set of vectors of callbacks
        LogMap undo_logs;

        //Key for logs
        THREAD_LOCAL_T key;

        Log* get_log();

        

    public:
        BaseContainer();
        void tx_start();

        void tx_abort();

        void tx_commit();
};

#endif
