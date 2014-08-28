#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <signal.h>

#include <event2/bufferevent.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>

#include "../common/common.h"
#include "../common/Queue.h"
#include "../common/protobuf_codec.h"
#include "../protos/query.pb.h"

#include "RedisOperator.h"
#include "ThreadHandler.h"
#include "InterfaceServer.h"


using namespace std;

Queue q;

void doEnqueue(struct bufferevent * bev, string data)
{
    q.enqueue(bev, data);
    printf("q.size()=%d\n",q.size());
}

void* handler(void* arg)
{
    printf("running thread..\n");
    RedisOperator redis("127.0.0.1", 6379, "");

    Queue* q = (Queue*)arg;
    //TODO: should use pthread_cond instead of busying waiting
    while (1)
    {
        if (q->size())
        {
            QueueNode node = q->dequeue();

            google::protobuf::Message * message = decode(node.data);
            if (!message)
            {
                printf("decode failed\n");
            }
            else
            {
                Query * q = dynamic_cast<Query*>(message);
                printf("Query received(id:%d,from:%s)\n", q->id(), q->from().c_str());
            
                char getIpCmd[64], getPortCmd[64];
                memset(getIpCmd, '\0', sizeof(getIpCmd));
                memset(getPortCmd, '\0', sizeof(getPortCmd));
                snprintf(getIpCmd, sizeof(getIpCmd)-1, "HGET SERVERIP_HT %d", q->id());
                snprintf(getPortCmd, sizeof(getPortCmd)-1, "HGET SERVERPORT_HT %d", q->id());

                printf("cmd is %s,%s\n",getIpCmd, getPortCmd);
                
                redis.selectDatabase("0");
                printf("select database\n");
                
                string ip = redis.queryString(string(getIpCmd));
                string port = redis.queryString(getPortCmd);
                printf("ip is %s, port is %s\n", ip.c_str(), port.c_str());

                char output[100];
                memset(output, '\0', sizeof(output));
                snprintf(output, sizeof(output)-1, 
                    "hi %s,requese id %d accepte.\nfrom redis:ip:%s,port:%s\n",
                        q->from().c_str(), q->id(), ip.c_str(), port.c_str());
                bufferevent_write(node.bev, (void*)output, strlen(output));
            }
            delete message;
            
        }
        else
        {
            usleep(300);
        }
    }
    return NULL;
}

int main()
{

    RedisOperator redis("127.0.0.1", 6379, "");

    ThreadFunc pThreadFunc = handler;
    ThreadHandler thhandler(&pThreadFunc, (void*)&q);
    
    readCallBackFun func = doEnqueue;

	InterfaceServer server = InterfaceServer(9999);
    
	server.init(&func);
	
    server.start();


    cout << "about to exit" <<endl;
    sleep(3);
	
    return 0;
}


