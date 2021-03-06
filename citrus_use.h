/*
 * =====================================================================================
 *
 *       Filename:  citrus_use.h
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

#if defined(SET_USE_CITRUS)

#include "boosted_citrus.h"

#define SET_T                             BoostedCitrus
#define SET_ALLOC()                       new BoostedCitrus(32)
#define SET_FREE(set)                     delete( set )

//normal operations
#define SET_INSERT(set, data)             set->insert(data,(void*)(data))
#define SET_FIND(set, data)               set->find(data)
#define SET_REMOVE(set, data)             set->remove(data) 

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

#endif

#if defined(MAP_USE_CITRUS)

#  include "boosted_citrus.h"

#  define MAP_T                       BoostedCitrus
#  define MAP_ALLOC(hash, cmp)        new BoostedCitrus(32)
#  define MAP_FREE(map)               delete(map)
#  define MAP_CONTAINS(map, key)      map->contains(key)
#  define MAP_FIND(map, key)          map->find(key)
#  define MAP_INSERT(map, key, data)  map->insert(key, (void*) data)
#  define MAP_REMOVE(map, key)        map->remove(key)

#ifdef BSTM
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

#else
#  define TMMAP_CONTAINS(map, key)    \
    MAP_CONTAINS(map, key)
#  define TMMAP_FIND(map, key)      \
    MAP_FIND(map, key)
#  define TMMAP_INSERT(map, key, data) \
    MAP_INSERT(map, key, data)
#  define TMMAP_REMOVE(map, key)  \
    MAP_REMOVE(map, key)
#endif



#endif

