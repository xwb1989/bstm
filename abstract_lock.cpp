/*
 * =====================================================================================
 *
 *       Filename:  abstract_lock.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/19/2014 02:15:01 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wenbin Xiao (github.com/xwb1989), xwb1989@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "abstract_lock.h"
#include "debug.h"

AbstractLock::~AbstractLock() {
    //Free the locks
    for (auto itr = lock_map.begin(); itr != lock_map.end(); itr++) {
        delete itr->second;
    }
}
void AbstractLock::lock(long key, Mode mode) {
    //If the lock has not been initialized yet, insert a new one; otherwise get the lock
    Lock* lock_ptr;
    LockMap::accessor lock_writer;
    if(!lock_map.insert(lock_writer, key)) {
        lock_ptr = lock_writer->second;
    } else {
        lock_ptr = new Lock();
        lock_writer->second = lock_ptr;
    }

    //If the lockset of this thread has not been initialized yet, create one; otherwise get the lockset
    LockSetMap::accessor lock_set_writer;
    lock_set_map.insert(lock_set_writer, thread_getId());
    auto& lock_set = lock_set_writer->second;

    //the set will prevent duplicate record
    if(lock_set.insert(lock_ptr).second) {
        if (lock_ptr->try_lock_for(NS(10))) {
            log("Thread %ld succeeded to lock key: %d\n", thread_getId(), key);
        } else {
            stringstream msg;
            msg << "Thread " << thread_getId() << " failed to READ-lock key: " << key << ", transaction aborted."; 
            assert(lock_set.erase(lock_ptr));
            throw TxAbortException(msg.str());
        }
    }
}

void AbstractLock::unlock(long key) {
    LockMap::accessor writer;
    lock_map.find(writer, key);
    auto& lock_ptr = writer->second;
    lock_ptr->unlock();
}

void AbstractLock::release_locks() {
    LockSetMap::accessor writer;
    if (lock_set_map.find(writer, thread_getId())) {
        auto& lock_set = writer->second;
        for (auto lock_ptr_itr = lock_set.rbegin(); lock_ptr_itr != lock_set.rend(); lock_ptr_itr++) {
            (*lock_ptr_itr)->unlock();
        }
        assert(lock_set_map.erase(writer));
    }
}

