/*
 * =====================================================================================
 *
 *       Filename:  transaction.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/20/2014 10:02:56
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wenbin Xiao (github.com/xwb1989), xwb1989@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "transaction.h"

void Transaction::start() {
    //if there is an partially completed transaction, abort it.
    if (!containers.empty()) {
        abort();
    }
    return;
}

void Transaction::abort() {
    for(auto container = containers.begin(); container != containers.end(); container++) {
        (*container)->tx_abort();
    }
    containers.clear();
    return;
}

void Transaction::commit() {
    for(auto container = containers.begin(); container != containers.end(); container++) {
        (*container)->tx_commit();
    }
    containers.clear();
    return;
}

void Transaction::add_container(BaseContainer* container) {
    if (containers.insert(container).second) {
        container->tx_start();
    }
}
