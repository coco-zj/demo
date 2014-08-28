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
  
    Queue* q = (Queue*)arg;
    while (1)
    {
        if (q->size())
        {
            QueueNode node = q->dequeue();
            printf("%d msg left\n",q->size());
/*  
            printf("get queue node:%p, %s,handling...\n",
                    node.bev, node.data.c_str());

            google::protobuf::Message * message = decode(node.data);
            if (!message)
            {
                printf("decode failed\n");
            }
            else
            {
                Query * q = dynamic_cast<Query*>(message);
                printf("Query received(id:%d,from:%s)\n", q->id(), q->from().c_str());
                char output[64];
                memset(output, '\0', sizeof(output));
                snprintf(output,sizeof(output)-1, "hi %s,requese id %d accepted\n",
                        q->from().c_str(), q->id());
                bufferevent_write(node.bev, (void*)output, strlen(output));
            }
            delete message;
*/            
        }
        else
        {
            usleep(300);
        }
    }


}

int main()
{
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


