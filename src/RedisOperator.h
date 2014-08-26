/*
 * =====================================================================================
 *
 *       Filename:  RedisOperator.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/26/2014 05:46:07 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef __REDIS_OPERATOR_H__
#define __REDIS_OPERATOR_H__

#include <string>
#include <hiredis.h>

#include "DbOperator.h"


class RedisOperator : public DbOperator
{

    private:
        redisContext * ctx;

    public:
        RedisOperator();
        RedisOperator(string h, int p, string pass);
        virtual ~RedisOperator();
        
        virtual void selectDatabase(string db);

        long queryInt(string queryString);
        string queryString(string queryString);

        //TODO: maybe we need to get array
//        vector<string> queryArray(string queryString);


        virtual void query(string queryString);
};

#endif


