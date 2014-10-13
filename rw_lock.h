/*
 * =====================================================================================
 *
 *       Filename:  RWLock.h
 *
 *    Description:  RWLock implemented with one unique_mutex and std::set. 
 *                  It supports lock ownship promotion:
 *                      If the caller already owns the read ownership, it can promote it to write ownership
 *                      if there is no other reader/writer currently or after the waiting duration passed.
 *
 *                  Also, it allows re-lock the lock:
 *                      If the caller already owns the read/write ownership, another attempt on read ownership 
 *                      would return true.
 *                      If the caller already owns the write ownership, another attempt on write ownership
 *                      would return true.
 *
 *                  It's writer-preferred:
 *                      If the caller issue an attempt on write ownership, no furthur read ownership request
 *                      will be permitted as long as the caller is still waiting.
 *
 *                  Unlock both ownerships:
 *                      The unlock operation would not distinguish the type of ownership. It will release both
 *                      if the caller owns both. However, if it's writer ownership the caller gonna release or
 *                      the caller is the last reader, it will set the lock available for upcoming read/write 
 *                      requests and notify the waiting requesters.
 *
 *        Version:  1.0
 *        Created:  08/06/2014 05:14:18 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wenbin Xiao (github.com/xwb1989), xwb1989@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef RWLOCK_H
#define RWLOCK_H

#include <set>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <iostream>

#include "thread.h"
#include "debug.h"
typedef std::chrono::milliseconds MS;
typedef std::unique_lock<std::mutex> Lock;

class RWLock {
    private:
        std::condition_variable cv;
        std::mutex cv_m;
        std::set<long> owners;
        bool write_only;
    public:
        RWLock() : write_only(false) {}

        template<class Rep, class Per>
            bool try_read_lock_for(const std::chrono::duration<Rep, Per> &rel_time) {
                Lock l(cv_m);
                bool own_read_lock = owners.find(thread_getId()) != owners.end();
                if (own_read_lock) { //already owned the lock, no matter read or write 
                    return true;
                }
                //has not owned the lock and there is already a writer or a writer waiting, wait for duration to see whether the writer leaves.
                if (write_only && cv.wait_for(l, rel_time, [this](){return write_only;})) { 
                    //the writer is still there, attempt fail
                    return false;
                }
                owners.insert(thread_getId());
                log("Thread %ld: readlock owners size %d\n", thread_getId(), owners.size());
                return true;
            }

        template<class Rep, class Per>
            bool try_write_lock_for(const std::chrono::duration<Rep, Per> &rel_time) {
                Lock l(cv_m);
                write_only = true;  //attempt to be the writer
                bool own_read_lock = owners.find(thread_getId()) != owners.end();
                if (own_read_lock) {
                    if (owners.size() == 1) {   //already the writer, or the only reader. 
                        log("Thread %ld: writelock owners size %d\n", thread_getId(), owners.size());
                        return true;
                    }
                    if (cv.wait_for(l, rel_time, [this](){return owners.size() == 1;})){ //wait for duration and becomes the only reader
                        log("Thread %ld: writelock owners size %d\n", thread_getId(), owners.size());
                        return true;
                    }
                } else {
                    //if there is other reader or writer, wait for duration for them to leave.
                    if (cv.wait_for(l, rel_time, [this](){return owners.size() == 0; })) { 
                        owners.insert(thread_getId());
                        log("Thread %ld: writelock owners size %d\n", thread_getId(), owners.size());
                        return true;
                    }
                }
                write_only = false;  //this attempt fail
                return false;
            }


        void unlock() {
            Lock l(cv_m);
            owners.erase(thread_getId());
            if (owners.empty()) {
                write_only = false;
                log("Thread %ld last one.\n", thread_getId());
                cv.notify_all();
            }
            log("Thread %ld unlocks.\n", thread_getId());
        }      
};

#endif
