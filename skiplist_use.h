/*
 * =====================================================================================
 *
 *       Filename:  skiplist_use.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/18/2015 08:39:57 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wenbin Xiao(github.com/xwb1989)
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef SKIPLIST_USE_H
#define SKIPLIST_USE_H


#if defined(SET_USE_SKIPLIST)
#include "boosted_skiplist.h"

#define SET_T                             BoostedSkiplist
#define SET_ALLOC()                       new BoostedSkiplist()
#define SET_FREE(set)                     delete( set )

//normal operations
#define SET_INSERT(set, data)             set->insert(data,(void*)(data))
#define SET_FIND(set, data)               set->find(data)
#define SET_REMOVE(set, data)             set->remove(data) 

#ifdef BSTM
//TM operations
#define TMSET_INSERT(set, data)       \
    ({ \
     tx->add_container(set); \
     set->tm_insert(data, (void*)(data));  \
     })

#define TMSET_FIND(set, data)    \
    ({ \
     tx->add_container(set); \
     set->tm_find(data); \
     })

#define TMSET_REMOVE(set, data)   \
    ({ \
     tx->add_container(set); \
     set->tm_remove(data); \
     })

#elif defined(STM)
#endif

#endif

#if defined(MAP_USE_SKIPLIST)

#ifdef BSTM

#  include "boosted_skiplist.h"

#  define MAP_T                       BoostedSkiplist
#  define MAP_ALLOC(hash, cmp)        new BoostedSkiplist()
#  define MAP_FREE(map)               delete(map)
#  define MAP_CONTAINS(map, key)      map->contains(key)
#  define MAP_FIND(map, key)          map->find(key)
#  define MAP_INSERT(map, key, data)  map->insert(key, (void*) data)
#  define MAP_REMOVE(map, key)        map->remove(key)


//Transactional Ops
#  define TMMAP_CONTAINS(map, key)    \
    ({ \
     tx->add_container(map); \
     map->tm_contains(key); \
     })
                                                                 
#  define TMMAP_FIND(map, key)      \
    ({ \
     tx->add_container(map); \
     map->tm_find(key); \
     })
#  define TMMAP_INSERT(map, key, data) \
    ({ \
     tx->add_container(map); \
     map->tm_insert(key, (void*) data); \
     })
#  define TMMAP_REMOVE(map, key)  \
    ({ \
     tx->add_container(map); \
     map->tm_remove(key); \
     })

#elif defined(STM)

#  include "include/skiplist-stm/sl-map.h"

#  define MAP_T                       sl_map_t
#  define MAP_ALLOC(hash, cmp)        sl_map_new()
#  define MAP_FREE(map)               sl_delete(map)
#  define MAP_CONTAINS(map, key)      sl_contains(map, key)
#  define MAP_FIND(map, key)          sl_find(key)
#  define MAP_INSERT(map, key, data)  sl_insert(map, key, (void*) data)
#  define MAP_REMOVE(map, key)        sl_remove(map, key)


//TM operations

#  define TMMAP_CONTAINS(map, key)    \
    TM_sl_contains(TM_ARG map, key)
#  define TMMAP_FIND(map, key)      \
    TM_sl_find(TM_ARG, map, key)
#  define TMMAP_INSERT(map, key, data) \
    TM_sl_insert(TM_ARG map, key, (void*)data)
#  define TMMAP_REMOVE(map, key)  \
    TM_sl_remove(TM_ARG map, key)
#endif



#endif
#endif
