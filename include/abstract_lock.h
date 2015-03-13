/*
 * =====================================================================================
 *
 *       Filename:  abstract_lock.h
 *
 *       Description:  abstract locks
 *
 *       Version:  1.0
 *       Created:  07/31/2014 01:52:24 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Author:  Wenbin Xiao (github.com/xwb1989), xwb1989@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef ABSTRACT_LOCK_H
#define ABSTRACT_LOCK_H

#include <mutex>
#include <set>
#include <map>
#include <sstream>
#include <mutex>
#include "thread.h"
#include "tx_exceptions.h"
//concurrent hash map
#include "tbb/concurrent_hash_map.h"

using namespace std;
//customed type
typedef timed_mutex Lock;
typedef tbb::concurrent_hash_map<long, Lock*> LockMap;
typedef tbb::concurrent_hash_map<long, set<Lock*>> LockSetMap;
typedef std::chrono::nanoseconds NS;

class AbstractLock {
    public:
        enum Mode {READ, WRITE}; 

        //destructor
        ~AbstractLock();
        //lock the access of the key
        void lock(long key, Mode mode);
        void unlock(long key);
        void release_locks();
    private:
        LockMap lock_map;
        LockSetMap lock_set_map;
};

#endif
