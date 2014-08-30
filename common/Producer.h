/*
 * =====================================================================================
 *
 *       Filename:  Producer.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/30/2014 10:46:51 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef __PRODUCER_H__
#define __PRODUCER_H__

class Producer
{
    public:
        virtual ~Producer() = 0;
        virtual void signal() = 0;
};

#endif

