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
#include "ThreadPool.h"
#include "RedisOperator.h"
//#include "ThreadHandler.h"
#include "InterfaceServer.h"


using namespace std;

/*  
void doEnqueue(struct bufferevent * bev, string data)
{
    q.enqueue(bev, data);
}
*/
//this lock is used to lock consumers
//
void* handle(void* arg)
{
    printf("running thread..\n");
    Queue* q =(Queue*)arg;
    q->lockConsumer();
    QueueNode node = q->dequeue();
    q->unlockConsumer();

    RedisOperator redis("127.0.0.1", 6379, "");

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
/*  
                char output[100];
                memset(output, '\0', sizeof(output));
                snprintf(output, sizeof(output)-1, 
                    "hi %s,requese id %d accepte.\nfrom redis:ip:%s,port:%s\n",
                        q->from().c_str(), q->id(), ip.c_str(), port.c_str());
                bufferevent_write(node.bev, (void*)output, strlen(output));
*/
            }
            delete message;
    return NULL;
}

int main()
{

    ThreadFunc pThreadFunc = handle;
    ThreadPool pool;
    pool.init(1,1);
    pool.start(&pThreadFunc);

    Queue q;
    q.registerConsumer(&pool, true);

	InterfaceServer server = InterfaceServer(9999);
    server.registMsgContainer(&q);



    //start loop 
    server.start();


    cout << "about to exit" <<endl;
    sleep(3);

    return 0;
}


