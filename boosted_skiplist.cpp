/*
 * =====================================================================================
 *
 *       Filename:  boosted_skiplist.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/12/2015 07:36:58 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wenbin Xiao (github.com/xwb1989)
 *   Organization:  
 *
 * =====================================================================================
 */
#include "boosted_skiplist.h"
#include "debug.h"

BoostedSkiplist::BoostedSkiplist() {
    map = sl_map_new();
}

BoostedSkiplist::~BoostedSkiplist() {
    sl_map_delete(map);
}

bool_t BoostedSkiplist::tm_insert(long key, void* val) {
    locks.lock(key, AbstractLock::Mode::WRITE);

    log("Skiplist Insert: key - %ld\n", key);
    
    bool result = sl_insert(map, key, val);
    
    if(result) {
        LogMap::accessor writer;
        undo_logs.insert(writer, thread_getId());
        auto& undo_log = writer->second;
        undo_log.push_back([this, key]() {
                
                bool result = sl_delete(map, key);
                
                return result;
                } );
    }
    log("Skiplist Insert %s: key - %ld\n", result?"true":"false", key);
    return result;
}

bool_t BoostedSkiplist::tm_remove(long key) {
    locks.lock(key, AbstractLock::Mode::WRITE);
    log("Skiplist Remove: key - %ld\n", key);
    //get the val associated with the key
    
    sl_val_t val = sl_get(map, key);
    
    if(val) {
        //get the log or create a new one if it does not exist
        LogMap::accessor writer;
        undo_logs.insert(writer, thread_getId());
        auto& undo_log = writer->second;

        
        assert(sl_delete(map, key));
        undo_log.push_back([this, key, val]() {
                
                bool result = sl_insert(map, key, val);            
                
                return result;
                });
    }
    log("Skiplist Remove %s: key - %ld\n", val ? "true":"false", key);
    return val != NULL;
}

bool_t BoostedSkiplist::tm_contains(long key) {
    locks.lock(key, AbstractLock::Mode::READ);
    log("Skiplist Contains: key - %ld\n", key);
    
    bool result = sl_contains(map, key);
    
    return result;
}

void* BoostedSkiplist::tm_find(long key) {
    locks.lock(key, AbstractLock::Mode::READ);
    log("Skiplist Find: key - %ld\n", key);
    
    sl_val_t val = sl_get(map, key);
    
    if (val) {
        log("Thread %ld Skiplist Find: key - %ld, val - %ld\n", thread_getId(), key, (long) val);
        return val;
    }
    log("Thread %ld Skiplist Find failed: key - %ld\n", thread_getId(), key);
    return nullptr;
}

bool_t BoostedSkiplist::insert(long key, void* val) {
    
    log("Skiplist Insert: map - %ld, key - %ld, val - %ld\n", (long)map, key, (long)val);
    assert(map);
    bool result = sl_insert(map, key, val);
    
    log("Skiplist Insert %s: key - %ld\n", result?"true":"false", key);
    assert(result);
    return result;
}

bool_t BoostedSkiplist::remove(long key) {
    
    bool result = sl_delete(map, key);
    
    return result;
}

bool_t BoostedSkiplist::contains(long key) {
    
    bool result = sl_contains(map, key);
    
    return result;
}

void* BoostedSkiplist::find(long key) {
    
    sl_val_t val = sl_get(map, key);
    return val;
}
