
#include <string>

#include "Queue.h"
#include "common.h"


Queue::Queue()
{
    QueueNode * n = new QueueNode();
    this->tail = this->head = n;
    this->_size = 0;
    this->mode = 0;
}

Queue::~Queue()
{
    while (this->head)
    {
        QueueNode * p = this->head;
        this->head = head->next;
        delete p;
    }
    if (this->mode | HAS_PRODUCER)
        pthread_mutex_destroy(&this->producerLock);
    if (this->mode | HAS_CONSUMER)
        pthread_mutex_destroy(&this->consumerLock);
}

void Queue::registerProducer(Producer* producer, bool multi)
{
    if(producer)
    {
        this->_producer = producer;
        this->mode |= HAS_PRODUCER;
        this->mode |=  multi ? MULTI_PRODUCER : 0;
        if (multi)
            pthread_mutex_init(&this->producerLock, NULL);
    }
}

void Queue::registerConsumer(Consumer* consumer, bool multi)
{
    if (consumer)
    {
        this->_consumer = consumer;
        this->mode |= HAS_CONSUMER;
        this->mode |= multi ? MULTI_CONSUMER : 0;
        if (multi)
            pthread_mutex_init(&this->consumerLock, NULL);
    }
}
void Queue::lockConsumer()
{
    pthread_mutex_lock(&this->consumerLock);
}
void Queue::unlockConsumer()
{
    pthread_mutex_unlock(&this->consumerLock);
}
void Queue::lockProducer()
{
    pthread_mutex_lock(&this->producerLock);
}
void Queue:: unlockProducer()
{
    pthread_mutex_unlock(&this->producerLock);
}
//Dont know how to do with consumer and producer
void Queue::clear()
{
    while(this->head->next)
    {
        QueueNode * p = this->head;
        this->head = this->head->next;
        delete p;
    }
}
void Queue::enqueue(struct bufferevent* bev, string str)
{
    QueueNode * n = new QueueNode(bev, str);
    n->data = str;
    
    if (this->mode | MULTI_PRODUCER)
    {
        pthread_mutex_lock(&this->producerLock);
        this->tail->next = n;
        this->tail = this->tail->next;
        this->_size++;
        pthread_mutex_unlock(&this->producerLock);
    }
    else
    {
        this->tail->next = n;
        this->tail = this->tail->next;
        this->_size++;
    }
    if (this->mode | HAS_CONSUMER)
        this->_consumer->dispatch((void*)this);
}
void Queue::set(void * arg, string arg2)
{
    struct bufferevent * bev = (bufferevent*)arg;
    this->enqueue(bev, arg2);
}
QueueNode Queue::dequeue()
{
    string data = "";
    struct bufferevent * b = NULL;
    if (this->mode | MULTI_CONSUMER)
    {
        pthread_mutex_lock(&this->consumerLock);
        if (this->head->next != NULL)
        {
            data = this->head->next->data;
            b = this->head->next->bev;

            QueueNode * h = this->head;
            this->head = this->head->next;
            delete h;
            this->_size--;
        }
        pthread_mutex_lock(&this->consumerLock);
    }
    else
    {
        if (this->head->next != NULL)
        {
            data = this->head->next->data;
            b = this->head->next->bev;

            QueueNode * h = this->head;
            this->head = this->head->next;
            delete h;
            this->_size--;
        }
    }
    if (this->mode | HAS_PRODUCER)
        this->_producer->signal();
    return QueueNode(b,data);
}

int Queue::size()
{
    return this->_size;    
}

