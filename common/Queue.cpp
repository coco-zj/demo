
#include <string>

#include "Queue.h"


Queue::Queue()
{
    QueueNode * n = new QueueNode();
    this->tail = this->head = n;
    this->_size = 0;
}

Queue::~Queue()
{
    while (this->head)
    {
        QueueNode * p = this->head;
        this->head = head->next;
        delete p;
    }
}
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
    

    this->tail->next = n;
    this->tail = this->tail->next;
    this->_size++;
}

QueueNode Queue::dequeue()
{
    string data = "";
    struct bufferevent * b = NULL;
    if (this->head->next != NULL)
    {
        data = this->head->next->data;
        b = this->head->next->bev;

        QueueNode * h = this->head;
        this->head = this->head->next;
        delete h;
        this->_size--;
    }
    return QueueNode(b,data);
}

int Queue::size()
{
    return this->_size;    
}

