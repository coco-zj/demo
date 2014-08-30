/*
 * =====================================================================================
 *
 *       Filename:  Consumer.h
 *
 *    Description:  jjjj
 *
 *        Version:  1.0
 *        Created:  08/30/2014 10:57:26 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef __CONSUMER_H__
#define __CONSUMER_H__

class Consumer
{
    public:
        virtual ~Consumer() = 0;
        virtual void dispatch(void* arg) = 0;
};

#endif
