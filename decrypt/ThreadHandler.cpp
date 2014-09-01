/*
 * =====================================================================================
 *
 *       Filename:  ThreadHandler.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/28/2014 09:32:45 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <string>
/*
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <event2/bufferevent.h>
#include "../protos/query.pb.h"
#include "../common/Queue.h"
#include "../common/protobuf_codec.h"
*/
#include "../common/common.h"

#include "ThreadHandler.h"
using namespace std;



ThreadHandler::ThreadHandler(ThreadFunc* func, void* arg)
{
    pthread_create(&this->th, NULL, *func, arg);
    pthread_detach(this->th);
}


ThreadHandler::~ThreadHandler()
{
    pthread_cancel(this->th);
}

