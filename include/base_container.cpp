/*
 * =====================================================================================
 *
 *       Filename:  base_container.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/19/2014 02:17:15 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wenbin Xiao (github.com/xwb1989), xwb1989@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "base_container.h"
#include "debug.h"
#include "thread.h"

#include <map>
#include <vector>
#include <sstream>
#include <assert.h>

BaseContainer::BaseContainer() {
    key = (THREAD_LOCAL_T) (long) this;
    assert(key);
    THREAD_LOCAL_INIT(key);   
}

void BaseContainer::tx_start() {
//    LogMap::accessor writer;
//    assert(undo_logs.insert(writer, thread_getId()));
    THREAD_LOCAL_SET(key, new Log());
    assert(THREAD_LOCAL_GET(key));
    return;
}

void BaseContainer::tx_abort() {
    Log* undo_log = (Log*)THREAD_LOCAL_GET(key);
    if (undo_log) {
        printf("1\n");
        for (auto op = undo_log->rbegin(); op != undo_log->rend(); op++) {
            //invoke callbacks in reverse order
            assert((*op)());
        }
        delete(undo_log);
    }
    THREAD_LOCAL_SET(key, NULL);




//    LogMap::accessor writer;
//    if (undo_logs.find(writer, thread_getId())) {
//        auto& undo_log = writer->second;
//        for (auto op = undo_log.rbegin(); op != undo_log.rend(); op++) {
//            //invoke callbacks in reverse order
//            assert((*op)());
//        }
//        assert(undo_logs.erase(writer));
//    }
    locks.release_locks();
}

void BaseContainer::tx_commit() {
    Log* undo_log = (Log*) THREAD_LOCAL_GET(key);
    if (undo_log) {
        printf("2\n");
        delete(undo_log);
        THREAD_LOCAL_SET(key, NULL);
    }

//    LogMap::accessor writer;
//    if (undo_logs.find(writer, thread_getId())) {
//        assert(undo_logs.erase(writer));
//    }
    locks.release_locks();
}
