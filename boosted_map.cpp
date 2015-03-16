/*
 * =====================================================================================
 *
 *       Filename:  boosted_map.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/19/2014 02:19:42 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wenbin Xiao (github.com/xwb1989), xwb1989@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "boosted_map.h"

#include <assert.h>
bool_t BoostedMap::tm_insert(long key, void* val) {
    locks.lock(key, AbstractLock::Mode::WRITE);
    bool result = hash_map.insert(make_pair(key, val));
    if(result) {
        LogMap::accessor writer;
        undo_logs.insert(writer, thread_getId());
        auto& undo_log = writer->second;
        undo_log.push_back([this, key]() {
                return hash_map.erase(key);    
                } );
    }
    return result;
}

bool_t BoostedMap::tm_remove(long key) {
    locks.lock(key, AbstractLock::Mode::WRITE);
    HashMap::const_accessor reader;
    bool result = hash_map.find(reader, key);
    if(result) {
        //get the log or create a new one if it does not exist
        LogMap::accessor writer;
        undo_logs.insert(writer, thread_getId());
        auto& undo_log = writer->second;

        auto val = reader->second;
        assert(hash_map.erase(reader));
        undo_log.push_back([this, key, val]() {
                return hash_map.insert(make_pair(key, val));            
                });
    }
    return result;
}

bool_t BoostedMap::tm_contains(long key) {
    locks.lock(key, AbstractLock::Mode::READ);
    HashMap::const_accessor reader;
    return hash_map.find(reader, key);
}

void* BoostedMap::tm_find(long key) {
    locks.lock(key, AbstractLock::Mode::READ);
    HashMap::accessor writer;
    if (hash_map.find(writer, key)) {
        return writer->second;
    }
    return nullptr;
}

bool_t BoostedMap::insert(long key, void* val) {
    bool result = hash_map.insert(make_pair(key, val));
    return result;
}

bool_t BoostedMap::remove(long key) {
    return hash_map.erase(key);
}

bool_t BoostedMap::contains(long key) {
    HashMap::const_accessor reader;
    return hash_map.find(reader, key);
}

void* BoostedMap::find(long key) {
    HashMap::accessor writer;
    if (hash_map.find(writer, key)) {
        return writer->second;
    }
    return nullptr;
}
