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

#include "InterfaceServer.h"


using namespace std;



Queue q;

void doEnqueue(struct bufferevent * bev, string data)
{
    q.enqueue(bev, data);
    printf("q.size()=%d\n",q.size());
}


void * handler(void* arg)
{
    Queue* q = (Queue*)arg;
    while (1)
    {
        if (q->size())
        {
            QueueNode node = q->dequeue();
            printf("get queue node:%p, %s,handling...\n",
                    node.bev, node.data.c_str());

            google::protobuf::Message * message = decode(node.data.c_str());
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
        }
        else
        {
            printf("queue no data,sleeping...\n");
            sleep(3);
        }
    }
}


int main()
{
    
    pthread_t* th;
    if (pthread_create(th, NULL, handler, (void*)&q))
    {
        cout << "Create Thread failed" << endl;
        exit(-1);
    }

    readCallBackFun func = doEnqueue;

	InterfaceServer server = InterfaceServer(9999);
    
	if( server.init(&func) )
    {
        cout << "server init failed" << endl;
        exit(-1);
    }
   
    
    //before start we should start all other components
    //like: handler threads etc

	server.start();





   /*  
    char * query = "SET foo bara";
    server.queryRedis(query);
    
    server.queryRedis("GET foo");
*/

    cout << "about to exit" <<endl;
    sleep(3);
	
    return 0;
}


