/*
 * =====================================================================================
 *
 *       Filename:  MsgContainer.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/30/2014 11:37:01 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef __MSGCONTAINER_H__
#define __MSGCONTAINER_H__

#include <string>
using namespace std;

class MsgContainer
{
    public:
        virtual ~MsgContainer() = 0;
        virtual void set(void* arg, string arg2) = 0;
};

#endif

