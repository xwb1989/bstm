/*
 * =====================================================================================
 *
 *       Filename:  rwlock_test.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/06/2014 11:52:55 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wenbin Xiao (github.com/xwb1989), xwb1989@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "RWLock.h"
#include <iostream>
#include <thread>
using namespace std;
void read_only(RWLock* rwlock, int id) {
    if (rwlock->try_read_lock_for(MS(100)))
        cout << "Thread: " << id << " get read lock" << endl;
    if (rwlock->try_read_lock_for(MS(100)))
        cout << "Thread: " << id << " get read lock again" << endl;
}

void write_only(RWLock* rwlock, int id) {
    if (rwlock->try_write_lock_for(MS(100)))
        cout << "Thread: " << id << " get write lock" << endl;
    if (rwlock->try_write_lock_for(MS(100)))
        cout << "Thread: " << id << " get write lock again" << endl;

}

void read_write(RWLock* rwlock, int id) {
    if (rwlock->try_read_lock_for(MS(100)))
        cout << "Thread: " << id << " get read lock" << endl;
    if (rwlock->try_write_lock_for(MS(100)))
        cout << "Thread: " << id << " get write lock again" << endl;
}
int main() {
    RWLock rwlock;
    thread th1(read_only, &rwlock, 1);
    thread th2(write_only, &rwlock, 2);
    thread th3(read_write, &rwlock, 3);
    th3.join();
    th2.join();
    th1.join();
    return 0;
}

