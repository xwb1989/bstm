/*
 * =====================================================================================
 *
 *       Filename:  boosted_citrus.cpp
 *
 *    Description:  boosted citrus. It uses the same interface as boosted_map.cpp
 *
 *        Version:  1.0
 *        Created:  01/18/2015 04:29:19 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wenbin Xiao(github.com/xwb1989) 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "boosted_citrus.h"
#include "urcu.h"
#include "debug.h"

BoostedCitrus::BoostedCitrus(int numThread) {
    citrus_root = citrus_init();
    initURCU(numThread);
}

BoostedCitrus::~BoostedCitrus() {
    free(citrus_root);
}

bool_t BoostedCitrus::tm_insert(long key, void* val) {
    locks.lock(key, AbstractLock::Mode::WRITE);

    log("Citrus Insert: key - %ld\n", key);
    urcu_register(thread_getId());
    bool result = citrus_insert(citrus_root, key, val);
    urcu_unregister();
    if(result) {
        LogMap::accessor writer;
        undo_logs.insert(writer, thread_getId());
        auto& undo_log = writer->second;
        undo_log.push_back([this, key]() {
                urcu_register(thread_getId());
                bool result = citrus_delete(citrus_root, key);
                urcu_unregister();
                return result;
                } );
    }
    log("Citrus Insert %s: key - %ld\n", result?"true":"false", key);
    return result;
}

bool_t BoostedCitrus::tm_remove(long key) {
    locks.lock(key, AbstractLock::Mode::WRITE);
    log("Citrus Remove: key - %ld\n", key);
    void* val;
    //get the val associated with the key
    urcu_register(thread_getId());
    bool result = citrus_find(citrus_root, key, &val);
    urcu_unregister();
    if(result) {
        //get the log or create a new one if it does not exist
        LogMap::accessor writer;
        undo_logs.insert(writer, thread_getId());
        auto& undo_log = writer->second;

        urcu_register(thread_getId());
        assert(citrus_delete(citrus_root, key));
        undo_log.push_back([this, key, val]() {
                urcu_register(thread_getId());
                bool result = citrus_insert(citrus_root, key, val);            
                urcu_unregister();
                return result;
                });
    }
    log("Citrus Remove %s: key - %ld\n", result?"true":"false", key);
    return result;
}

bool_t BoostedCitrus::tm_contains(long key) {
    locks.lock(key, AbstractLock::Mode::READ);
    log("Citrus Contains: key - %ld\n", key);
    urcu_register(thread_getId());
    bool result = citrus_contains(citrus_root, key);
    urcu_unregister();
    return result;
}

void* BoostedCitrus::tm_find(long key) {
    locks.lock(key, AbstractLock::Mode::READ);
    log("Citrus Find: key - %ld\n", key);
    void* val;
    urcu_register(thread_getId());
    bool result = citrus_find(citrus_root, key, &val);
    urcu_unregister();
    if (result) {
        log("Thread %ld Citrus Find: key - %ld, val - %ld\n", thread_getId(), key, (long) val);
        return (void*) val;
    }
    log("Thread %ld Citrus Find failed: key - %ld\n", thread_getId(), key);
    return nullptr;
}

bool_t BoostedCitrus::insert(long key, void* val) {
    urcu_register(thread_getId());
    bool result = citrus_insert(citrus_root, key, val);
    urcu_unregister();
    assert(result);
    return result;
}

bool_t BoostedCitrus::remove(long key) {
    urcu_register(thread_getId());
    bool result = citrus_delete(citrus_root, key);
    urcu_unregister();
    return result;
}

bool_t BoostedCitrus::contains(long key) {
    urcu_register(thread_getId());
    bool result = citrus_contains(citrus_root, key);
    urcu_unregister();
    return result;
}

void* BoostedCitrus::find(long key) {
    void* val;
    urcu_register(thread_getId());
    bool result = citrus_find(citrus_root, key, &val);
    urcu_unregister();
    if (result) {
        return val;
    }
    return nullptr;
}
