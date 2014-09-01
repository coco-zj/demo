/*
 * =====================================================================================
 *
 *       Filename:  ThreadPool.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/29/2014 10:41:39 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include <vector>
#include <list>

#include "../common/common.h"
#include "../common/Consumer.h"
#include "RedisOperator.h"

using namespace std;

class ThreadPool;

struct ThreadInfo
{
    bool existing;
    RedisOperator * redis;
    void*  arg;
    ThreadPool* pool;
    pthread_t th;
    pthread_cond_t cond;
    ThreadInfo(){existing = false; redis = NULL; arg = pool = NULL;}
};



class ThreadPool: public Consumer
{
    public:
//        ThreadPool();
//        virtual ~ThreadPool();

        void init(int min, int max);
        void destroy();
        void start(ThreadFunc * func);
        virtual void dispatch(void * arg);

    private:
        int addWorker();
        int deleteWorker();

    public:
        ThreadFunc * thread_func;//user surply function
        friend void* thread_ready(void* arg);

    private:
        vector<ThreadInfo*> thread_list;
        list<ThreadInfo*> idle_list;
        bool destroying;

        //lock for ThreadPool
        pthread_mutex_t lock;
    private:
        unsigned int minThreads;
        unsigned int maxThreads;
    public:
        unsigned int currThreads;

    public:
        inline void lockPool()
        {
            pthread_mutex_lock(&this->lock);
        }
        inline void unlockPool()
        {
            pthread_mutex_unlock(&this->lock);
        }
        inline pthread_mutex_t * get_lock()
        {
            return &this->lock;
        }
        inline bool isDestroying()
        {
            return destroying;
        }
};


#endif

