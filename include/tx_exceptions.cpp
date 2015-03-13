/*
 * =====================================================================================
 *
 *       Filename:  tx_exceptions.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/20/2014 03:57:36 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wenbin Xiao (github.com/xwb1989), xwb1989@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "tx_exceptions.h"

TxAbortException::TxAbortException(std::string m) : runtime_error(m) {}
