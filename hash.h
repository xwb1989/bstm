/*
 * =====================================================================================
 *
 *       Filename:  hash.h
 *
 *    Description:  hash function/structures
 *
 *        Version:  1.0
 *        Created:  08/03/2014 04:43:34 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wenbin Xiao (github.com/xwb1989), xwb1989@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef HASH_H
#define HASH_H
#include "types.h"

//compare and hash for concurrent_hash_map

namespace Hash {

    struct HashCompare {
        template<class KeyType>
        static ulong_t hash(const KeyType key) {
            return (ulong_t) key;
        }

        template<class KeyType>
        static bool_t equal(const KeyType key1, const KeyType key2) {
            return (ulong_t) key1 == (ulong_t) key2;
        }
    };

}
#endif
