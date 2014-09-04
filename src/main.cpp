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
#include "InterfaceServer.h"

using namespace std;

string getDataFromSlave(string ip, int port, int data)
{
    sockaddr_in slavaddr;
    memset(&slavaddr, '\0', sizeof(slavaddr));
    slavaddr.sin_family = AF_INET;
    slavaddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &slavaddr.sin_addr);
    
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    char buffer [256];
    memset(buffer, '\0', sizeof(buffer));
    snprintf(buffer, sizeof(buffer)-1, "%d", data);
    sendto(sockfd, buffer, strlen(buffer), 0,
            (sockaddr*)&slavaddr, sizeof(slavaddr));
    
    socklen_t slen;
    memset(buffer, '\0', sizeof(buffer));
    recvfrom(sockfd, buffer, sizeof(buffer)-1, 0, (sockaddr*)&slavaddr, &slen); 

    close(sockfd);
    printf("buffer now is: %s\n", buffer);
    return string(buffer);

}

int pickASlave(RedisOperator* redis)
{
    redis->selectDatabase("15");
    int total = redis->queryInt("LLEN worker_list");
    printf("total worker is :%d\n", total);
    if (total < 1)
    {
        printf("currently no slaves are ready,this request ommitted\n");
        return -1;
    }
    
    srand(time(0));
    int idx = rand() % total;

    char getPortCmd[64];
    memset(getPortCmd, '\0', sizeof(getPortCmd));
    snprintf(getPortCmd, sizeof(getPortCmd)-1, "LINDEX worker_list %d", idx);
    string pstring = redis->queryString(getPortCmd);
    return atoi(pstring.c_str());

}


void* handle(void* db, void* arg)
{
    Queue* queue =(Queue*)arg;
    RedisOperator* redis = (RedisOperator*)db;
    QueueNode node = queue->dequeue();

    int port = pickASlave(redis);
    if (port < 0)
        return NULL;

    google::protobuf::Message * message = decode(node.data);
    if (!message)
    {
        printf("decode failed\n");
        return NULL;
    }
    //do 
    Query * q = dynamic_cast<Query*>(message);
    printf("Query received(id:%d,from:%s)\n", q->id(), q->from().c_str());

    //go to slave to get data
    string data = getDataFromSlave("127.0.0.1", port, q->id());

    printf("data from slave:%s\n", data.c_str());

  
    char output[100];
    memset(output, '\0', sizeof(output));
    snprintf(output, sizeof(output)-1, 
            "hi %s,request id %d accepted.\nfrom slave(port:%d):%s\n",
            q->from().c_str(), q->id(), port, data.c_str());
    bufferevent_write(node.bev, (void*)output, strlen(output));

    
    delete message;
    return NULL;
}

int main()
{

    ThreadFunc pThreadFunc = handle;
    ThreadPool pool;
    pool.init(1,3);
    pool.start(&pThreadFunc);
    sleep(1);    
    
    Queue q;
    q.registerConsumer(&pool, true);
    
    InterfaceServer server = InterfaceServer(9999);
    server.registMsgContainer(&q);

    server.init();
   
    //start loop 
    server.start();


    cout << "about to exit" <<endl;
    sleep(10);

    return 0;
}


