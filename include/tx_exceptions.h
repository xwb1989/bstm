/*
 * =====================================================================================
 *
 *       Filename:  tx_exceptions.h
 *
 *    Description:  transaction exceptions
 *
 *        Version:  1.0
 *        Created:  08/04/2014 03:35:48 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wenbin Xiao (github.com/xwb1989), xwb1989@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef TX_EXCEPTIONS_H
#define TX_EXCEPTIONS_H

#include <iostream>
#include <stdexcept>
#include <string>

class TxAbortException: public std::runtime_error {
    public:
        TxAbortException(std::string m);
};

#endif
