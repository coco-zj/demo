/*
 * =====================================================================================
 *
 *       Filename:  ThreadHandler.h
 *
 *    Description:  thread handler
 *
 *        Version:  1.0
 *        Created:  08/27/2014 10:31:22 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef __THREAD_HANLDER_H__
#define __THREAD_HANDLER_H__

#include "../common/common.h"

class ThreadHandler{
    public:
        ThreadHandler(ThreadFunc* func, void* arg);
        ~ThreadHandler();

    private:
        pthread_t th;

};



#endif


