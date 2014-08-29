/*
 * =====================================================================================
 *
 *       Filename:  common.h
 *
 *    Description:  jj
 *
 *        Version:  1.0
 *        Created:  08/27/2014 03:08:23 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef __COMMON_H__
#define __COMMON_H__

#include <event2/bufferevent.h>
#include <string>
using namespace std;


typedef void (*MsgDispatchFunc) (struct bufferevent * bev, string data);
typedef void* (*ThreadFunc) (void * arg);


#endif


