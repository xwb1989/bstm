/*
 * =====================================================================================
 *
 *       Filename:  transaction.h
 *
 *    Description:  define transaction which will be managed by transaction manager
 *
 *        Version:  1.0
 *        Created:  07/28/2014 12:01:27 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wenbin Xiao (github.com/xwb1989), xwb1989@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <set>

#include "base_container.h"

/* A transaction will only be accessed by a thread; thus it's thread-safe by nature. */

class Transaction {
    public:
        void start();
        void abort();
        void commit();
        void add_container(BaseContainer* container);
    private:
        std::set<BaseContainer*> containers;  //the containers accessed by this transaction
};
#endif
