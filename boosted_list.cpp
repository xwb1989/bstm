/*
 * =====================================================================================
 *
 *       Filename:  boosted_list.cpp
 *
 *    Description:  definition of BoostedList
 *
 *        Version:  1.0
 *        Created:  10/15/2014 07:44:05 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wenbin Xiao (github.com/xwb1989)
 *   Organization:  
 *
 * =====================================================================================
 */
#include "boosted_list.h"


using namespace std;
bool_t BoostedList::tm_insert(void* data) {
    //Lock the whole list
    lock_guard<mutex> lock(list_lock);

    //Lock the semantic lock
    long key = (long) data;
    locks.lock(key, AbstractLock::Mode::WRITE);

    bool_t result = list_insert(base_list, data);
    if (result) {
        LogMap::accessor writer;
        undo_logs.insert(writer, thread_getId());
        auto& undo_log = writer->second;
        undo_log.push_back([this, data]() {
                lock_guard<mutex> lock(list_lock);
                return list_remove(base_list, data);
                } );
    }
    return result;
}

bool_t BoostedList::tm_remove(void* data) {
    //Lock the whole list
    lock_guard<mutex> lock(list_lock);

    //Lock the semantic lock
    long key = (long) data;
    locks.lock(key, AbstractLock::Mode::WRITE);

    bool_t result = list_remove(base_list, data);
    if (result) {
        LogMap::accessor writer;
        undo_logs.insert(writer, thread_getId());
        auto& undo_log = writer->second;
        undo_log.push_back([this, data]() {
                lock_guard<mutex> lock(list_lock);
                return list_insert(base_list, data);
                } );
    }

    return result;
}

void* BoostedList::tm_find(void* data) {
    //Lock the whole list
    lock_guard<mutex> lock(list_lock);

    //Lock the semantic lock
    long key = (long) data;
    locks.lock(key, AbstractLock::Mode::WRITE);

    void* result = list_find(base_list, data);
    return result;
}

bool_t BoostedList::tm_is_empty() {
    lock_guard<mutex> lock(list_lock);
    return list_isEmpty(base_list);
}

bool_t BoostedList::tm_has_next() {
    lock_guard<mutex> lock(list_lock);
    //TO-DO
    

}

void* BoostedList::tm_next() {

}

void BoostedList::tm_reset() {

}



