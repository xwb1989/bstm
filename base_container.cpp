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

void BaseContainer::tx_start() {
    LogMap::accessor writer;
    assert(undo_logs.insert(writer, thread_getId()));
    return;
}

void BaseContainer::tx_abort() {
    LogMap::accessor writer;
    if (undo_logs.find(writer, thread_getId())) {
        auto& undo_log = writer->second;
        for (auto op = undo_log.rbegin(); op != undo_log.rend(); op++) {
            //invoke callbacks in reverse order
            assert((*op)());
        }
        assert(undo_logs.erase(writer));
    }
    locks.release_locks();
}

void BaseContainer::tx_commit() {
    LogMap::accessor writer;
    if (undo_logs.find(writer, thread_getId())) {
        assert(undo_logs.erase(writer));
    }
    locks.release_locks();
}
