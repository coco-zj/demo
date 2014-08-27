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


class Queue
{
    public:
        Queue();
        ~Queue();

        int size();
        void clear();
        void enqueue(struct bufferevent * bev, string item);
        QueueNode dequeue();

    private:
        int _size;
        QueueNode * head;
        QueueNode * tail;
};


#endif

