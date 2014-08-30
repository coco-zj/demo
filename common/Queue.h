/*
 * =====================================================================================
 *
 *       Filename:  Queue.h
 *
 *    Description:  no lock queue
 *
 *        Version:  1.0
 *        Created:  08/27/2014 12:04:12 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <string>
#include <event2/bufferevent.h>
#include <pthread.h>

#include "Producer.h"
#include "Consumer.h"
#include "MsgContainer.h"

using namespace std;

struct QueueNode
{
    QueueNode * next;
    string data;
    struct bufferevent * bev;
    QueueNode(){next = NULL; bev = NULL;};
    QueueNode(struct bufferevent *b, string item)
    {
        bev = b;
        data = item;
        next = NULL;
    };
};
enum QueueType 
{
    PURE = 0,
    HAS_PRODUCER = 1 << 1,
    HAS_CONSUMER = 1 << 2,
    MULTI_PRODUCER = 1 << 3,
    MULTI_CONSUMER = 1 << 4,
};

class Queue : public MsgContainer
{
    public:
        Queue();
        virtual ~Queue();
        void registerProducer(Producer* producer, bool multi);
        void registerConsumer(Consumer* consumer, bool multi);

        void lockConsumer();
        void unlockConsumer();
        void lockProducer();
        void unlockProducer();

        int size();
        void clear();
        virtual void set(void * arg, string arg2);
        void enqueue(struct bufferevent * bev, string item);
        QueueNode dequeue();

    private:
        int _size;
        QueueNode * head;
        QueueNode * tail;

        Producer * _producer;
        Consumer * _consumer;
    private:
        pthread_mutex_t producerLock;
        pthread_mutex_t consumerLock;
        int mode;
};


#endif

