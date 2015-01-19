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

void BoostedCitrus::register_rcu() {
    urcu_register(thread_getId());
}

BoostedCitrus::BoostedCitrus(int numThread) {
    citrus_root = citrus_init();
    initURCU(numThread);

}

BoostedCitrus::~BoostedCitrus() {
    free(citrus_root);
}

bool_t BoostedCitrus::tm_insert(long key, void* val) {
    locks.lock(key, AbstractLock::Mode::WRITE);
    bool result = citrus_insert(citrus_root, key, val);
    if(result) {
        LogMap::accessor writer;
        undo_logs.insert(writer, thread_getId());
        auto& undo_log = writer->second;
        undo_log.push_back([this, key]() {
                return citrus_delete(citrus_root, key);
                } );
    }
    return result;
}

bool_t BoostedCitrus::tm_remove(long key) {
    locks.lock(key, AbstractLock::Mode::WRITE);
    void* val;
    //get the val associated with the key
    bool result = citrus_find(citrus_root, key, &val);
    if(result) {
        //get the log or create a new one if it does not exist
        LogMap::accessor writer;
        undo_logs.insert(writer, thread_getId());
        auto& undo_log = writer->second;

        assert(citrus_delete(citrus_root, key));
        undo_log.push_back([this, key, val]() {
                return citrus_insert(citrus_root, key, val);            
                });
    }
    return result;
}

bool_t BoostedCitrus::tm_contains(long key) {
    locks.lock(key, AbstractLock::Mode::READ);
    return citrus_contains(citrus_root, key);
}

void* BoostedCitrus::tm_find(long key) {
    locks.lock(key, AbstractLock::Mode::READ);
    void* val;
    if (citrus_find(citrus_root, key, &val)) {
        return val;
    }
    return nullptr;
}

bool_t BoostedCitrus::insert(long key, void* val) {
    bool result = citrus_insert(citrus_root, key, val);
    return result;
}

bool_t BoostedCitrus::remove(long key) {
    return citrus_delete(citrus_root, key);
}

bool_t BoostedCitrus::contains(long key) {
    return citrus_contains(citrus_root, key);
}

void* BoostedCitrus::find(long key) {
    void* val;
    if (citrus_find(citrus_root, key, &val)) {
        return val;
    }
    return nullptr;
}
