/*
 * =====================================================================================
 *
 *       Filename:  skiplist-map.h
 *
 *    Description:  MAP interface for skiplist
 *
 *        Version:  1.0
 *        Created:  03/19/2015 10:02:04 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wenbin Xiao
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef SKIPLIST_H
#define SKIPLIST_H


#  include "sl-map.h"

#  define MAP_T                       sl_map_t
#  define MAP_ALLOC(hash, cmp)        sl_map_new()
#  define MAP_FREE(map)               sl_map_delete(map)
#  define MAP_CONTAINS(map, key)      sl_contains(map, key)
#  define MAP_FIND(map, key)          sl_find(key)
#  define MAP_INSERT(map, key, data)  sl_insert(map, key, (void*) data)
#  define MAP_REMOVE(map, key)        sl_remove(map, key)


//TM operations

#  define TMMAP_CONTAINS(map, key)    \
    TM_sl_contains(TM_ARG map, key)
#  define TMMAP_FIND(map, key)      \
    TM_sl_find(TM_ARG map, key)
#  define TMMAP_INSERT(map, key, data) \
    TM_sl_insert(TM_ARG map, key, (void*)data)
#  define TMMAP_REMOVE(map, key)  \
    TM_sl_remove(TM_ARG map, key)

#endif
