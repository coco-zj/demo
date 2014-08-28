/*
 * =====================================================================================
 *
 *       Filename:  RedisOperator.cpp
 *
 *    Description:  redis operator
 *
 *        Version:  1.0
 *        Created:  08/26/2014 05:42:57 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  zj, 
 *        Company:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "RedisOperator.h"


RedisOperator::RedisOperator()
{
    //TODO shoud use a smarter way to do initialise
    this->host = string("127.0.0.1");
    this->port = 6379;
    this->password = string("(empty)");

    this->ctx = redisConnect(this->host.c_str(), this->port);
    if (!this->ctx)
    {
        printf("init redis connection failed\n");
    }
    
}

RedisOperator::RedisOperator(string h, int p, string pass)
{
    //TODO shoud use a smarter way to do initialise
    this->host = h;
    this->port = p;
    this->password = pass;

    this->ctx = redisConnect(this->host.c_str(), this->port);
    if (!this->ctx)
    {
        printf("init redis connection failed\n");
    }
}

RedisOperator::~RedisOperator()
{
    if(this->ctx)
    {
        redisFree(this->ctx);
        this->ctx = NULL;
    }
    this->host.resize(0);
    this->port = -1;
    this->password.resize(0);
}

void RedisOperator::selectDatabase(string db)
{
    int dbIndex = atoi(db.c_str());
    if (dbIndex > 16 || dbIndex < 0)
    {
        printf("out of database %d\n",dbIndex);
        return;
    }
    if (0 == dbIndex && db.compare("0"))
    {
        printf("atoi(%s) failed\n", db.c_str());
        return;
    }
    struct redisReply * reply = static_cast<redisReply*>
           (redisCommand(this->ctx, "SELECT %d", dbIndex));

    if (!reply || reply->type == REDIS_REPLY_ERROR)
    {
        printf("Redis select Database failed:%s\n", db.c_str());
    }
    freeReplyObject(reply);
    return ;
}

void RedisOperator::query(string queryString)
{
    struct redisReply * reply = static_cast<redisReply*>
        (redisCommand(this->ctx, queryString.c_str()));
    if( !reply )
    {
        printf("RedisCommand failed:%s\n", queryString.c_str());
        return ;
    }
    freeReplyObject(reply);
    return ;
}

long RedisOperator::queryInt(string queryString)
{
    struct redisReply * reply = static_cast<redisReply*>
        (redisCommand(this->ctx, queryString.c_str()));

    long result = -1;
    if (reply && reply->type == REDIS_REPLY_INTEGER)
    {
        result = static_cast<long>(reply->integer);
    }
    else
    {
        printf("RedisQuery failed:%s, ErrorMsg:%s\n", 
                queryString.c_str(), reply->str);
    }
    freeReplyObject(reply);
    return result;
}

string RedisOperator::queryString(string queryString)
{
    struct redisReply * reply = static_cast<redisReply*>
        (redisCommand(this->ctx, queryString.c_str()));
 
    printf("reply->type:%d\n",reply->type);

    string result("");
    if (!reply)
    {
        printf("execute redisCommand failed\n");
    }
    if (reply->type == REDIS_REPLY_ERROR)
    {
        printf("redisCommand error:%s\n", reply->str);
    }
    if (reply->type == REDIS_REPLY_STRING)
    {
        result = string(reply->str);
    }
    else
    {
        printf("RedisCommand failed:%s, ErrorMsg:%s(%d)\n", 
                queryString.c_str(),reply->str,reply->type);
    }
    freeReplyObject(reply);
    return result;
}




