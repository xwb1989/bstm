/*
 * =====================================================================================
 *
 *       Filename:  list_set.h
 *
 *    Description:  set implemented by list
 *
 *        Version:  1.0
 *        Created:  10/23/2014 05:56:13 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wenbin Xiao (github.com/xwb1989)
 *   Organization:  
 *
 * =====================================================================================
 */

#if defined(SET_USE_TL2)

#include "list.h"

#define SET_T                             list_t
#define SET_ALLOC()                       list_alloc(NULL)
#define SET_FREE(set)                     list_free(set)

//normal operations
#define SET_INSERT(set, data)             list_insert(set, (void*) ( data )) 
#define SET_FIND(set, data)               list_find(set, (void*) ( data ))
#define SET_REMOVE(set, data)             list_remove(set, (void*) ( data ))

//TM operations
#define TMSET_INSERT(set, data)           TMLIST_INSERT(set, data)
#define TMSET_FIND(set, data)             TMLIST_FIND(set, data)
#define TMSET_REMOVE(set, data)           TMLIST_REMOVE(set, data)

#elif defined(SET_USE_BOOST)

#include "boosted_list.h"

#define SET_T                             BoostedList
#define SET_ALLOC()                       new BoostedList()
#define SET_FREE(set)                     delete( set )

//normal operations
#define SET_INSERT(set, data)             set->insert((void*) ( data )) 
#define SET_FIND(set, data)               set->find((void*) ( data )) 
#define SET_REMOVE(set, data)             set->remove((void*) ( data )) 

//TM operations
#define TMSET_INSERT(set, data)       \
    ({ \
     tx->add_container(set); \
     set->tm_insert((void*) (data));  \
     })

#define TMSET_FIND(set, data)    \
    ({ \
     tx->add_container(set); \
     set->tm_find((void*) (data)); \
     })

#define TMSET_REMOVE(set, data)   \
    ({ \
     tx->add_container(set); \
     set->tm_remove((void*) (data)); \
     })

#endif


