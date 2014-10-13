/*
 * =====================================================================================
 *
 *       Filename:  bstm.h
 *
 *    Description:  interface integrating Boosted Transational Memory Containers and TL2
 *
 *        Version:  1.0
 *        Created:  08/11/2014 06:17:47 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wenbin Xiao (github.com/xwb1989), xwb1989@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef BSTM_H
#define BSTM_H

#include "stm.h"
#include "transaction.h"
#include "tx_exceptions.h"
#include "thread.h"
#include "debug.h"

#define BSTM_ARG                        STM_SELF, tx,      
#define BSTM_ARG_ALONE                  STM_SELF, tx
#define BSTM_ARGDECL                    STM_THREAD_T* STM_SELF, Transaction* tx, 
#define BSTM_ARGDECL_ALONE              STM_THREAD_T* STM_SELF, Transaction* tx
#define BSTM_THREAD_ENTER()        \
    STM_THREAD_T* STM_SELF = STM_NEW_THREAD(); \
    STM_INIT_THREAD(STM_SELF, thread_getId());  \
    Transaction* tx = new Transaction()

#define BSTM_THREAD_EXIT()              \
    STM_FREE_THREAD(STM_SELF);           \
    delete tx

#define BSTM_BEGIN()                              \
    tx->start();                                  \
    log("Thread: %ld begins a BSTM transaction\n", thread_getId()); \
    try {
    

#define BSTM_END()                               \
    } catch (TxAbortException& e) {              \
        log("%s\n", e.what());                \
        tx->abort();                             \
        log("Thread %ld BSTM aborted successfully\n", thread_getId()); \
        BSTM_RESTART();                         \
        assert(0);                                  \
    }                                            \
    STM_END();                                   \
    tx->commit();                                 \
    log("Thread: %ld commits a transactionn\n", thread_getId())

#define BSTM_RESTART()          \
    log("Thread: %ld RESTART the transactionn\n", thread_getId()); \
    STM_RESTART()              

#define BSTM_STARTUP()          \
    STM_STARTUP()


#define BSTM_BEGIN_WR()          \
    STM_BEGIN_WR();              \
    BSTM_BEGIN()
#define BSTM_BEGIN_RD()          \
    STM_BEGIN_RD();              \
    BSTM_BEGIN()

#define BSTM_SHUTDOWN()          \
    STM_SHUTDOWN()
#endif

