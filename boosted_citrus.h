/*
 * =====================================================================================
 *
 *       Filename:  boosted_citrus.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/18/2015 04:40:04 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wenbin Xiao (github.com/xwb1989) 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef BOOSTED_CITRUS_H
#define BOOSTED_CITRUS_H

#include "include/base_container.h"
#include "citrus.h"

class BoostedCitrus: public BaseContainer {
    private:
        citrus_node citrus_root;

    public:
        //Constructor
        BoostedCitrus(int);

        //Destructor
        ~BoostedCitrus();

        //tx operations
        bool_t tm_insert(long key, void* val);
        bool_t tm_remove(long key);
        bool_t tm_contains(long key);
        void* tm_find(long key);

        //standard operations
        bool_t insert(long key, void* val);
        bool_t remove(long key);
        bool_t contains(long key);
        void* find(long key);

};

#endif
