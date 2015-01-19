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

