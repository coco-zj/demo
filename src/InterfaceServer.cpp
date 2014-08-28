#include <iostream>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>


#include "../common/protobuf_codec.h"
#include "../protos/query.pb.h"

#include "InterfaceServer.h"

using namespace std;


InterfaceServer::InterfaceServer(int port)
{
    this->nConnections = 0;
    this->port = (port < 65535 && port > 1024) ? port : SERVER_PORT;

    this->evbase = NULL;
    this->listener = NULL;
}


InterfaceServer::~InterfaceServer()
{
    if (this->listener)
    {
        evconnlistener_free(this->listener);
        this->listener = NULL;
    }
    if (this->evbase)
    {
        event_base_free(this->evbase);
        this->evbase = NULL;
    }

}



void message_handle(google::protobuf::Message * message)
{
    
}

//typedef void(* bufferevent_data_cb)(struct bufferevent *bev, void *ctx)
void bufferevent_on_read(struct bufferevent *bev, void * arg)
{
    printf("read callback called\n");

    InterfaceServer * server  = (InterfaceServer*) arg;
    struct evbuffer * evInputBuffer = bufferevent_get_input(bev);

    size_t buffer_len = 0;

    do
    {
    buffer_len = evbuffer_get_length(evInputBuffer);
    printf("buffer_len is %lu\n", buffer_len);
    if (buffer_len < 4)
        break;

    int record_len;
    evbuffer_copyout(evInputBuffer, &record_len, kHeaderLen);
    record_len = ntohl(record_len);
    printf("record_len is %d\n", record_len);

    if (buffer_len < record_len + kHeaderLen)
        break;
    
    char * record = (char*) malloc(record_len);
    if (!record)
    {
        printf("malloc %d Bytes failed\n",record_len);
        return ;
    }
    evbuffer_drain(evInputBuffer, kHeaderLen);
    evbuffer_remove(evInputBuffer, record, record_len);

    string msg(record,record_len);
    delete record;

    if (server->rcb)
    {
        printf("start to call readCallBackFun\n");
        readCallBackFun * func = server->rcb;
        (*func)(bev, msg);
        printf("finish to call readCallBackFun\n");
    }
    buffer_len = evbuffer_get_length(evInputBuffer);
    printf("buffer_len left :%lu\n",buffer_len);
    }while(buffer_len);
/*  
    google::protobuf::Message * message = decode(msg);
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
        bufferevent_write(bev, (void*)output, strlen(output));
    }
    delete message;

/*      
    char * type = NULL;
    char * record = NULL;
    record = (char*)malloc(record_len);
    if (record == NULL)
        return;
    evbuffer_drain(evInputBuffer, 4);
    evbuffer_remove(evInputBuffer, record, record_len);

    //Now record can be make to proto class
    char * p = strstr(record, "\n");
    if (!p)
    {
        printf("broken string\n");
        return;
    }
    *p = '\0';
    type = record;
    string typeName(type);
    google::protobuf::Message * message = createMessage(typeName);
    if (!message)
    {
        printf("data to decode failed\n");
        return;
    }
    char * protoData = p+1;
    int protoDataLen = record_len - strlen(type) - 1;
    if (!message->ParseFromArray(protoData, protoDataLen))
    {
        delete message;
    }
    delete record;
    Query* q = (Query*) message;

    printf("Query received(id:%d,from:%s)\n", q->id(), q->from().c_str());
    
    //TODO query redis
    char output[128];
    memset(output, '\0', sizeof(output));
    snprintf(output,sizeof(output)-1, "hi %s,requese id %d accepted\n",
            q->from().c_str(), q->id());
    
    bufferevent_write(bev, (void*)output, strlen(output));
    
    delete message;
    */

}

/*  
void bufferevent_on_write(struct bufferevent * bev, void * arg)
{
    InterfaceServer * server  = (InterfaceServer*) arg;

}
*/
//typedef void(* bufferevent_event_cb)(struct bufferevent *bev, short what,
//             void *ctx)
void bufferevent_on_event(struct bufferevent * bev, short whatEvent, void * arg)
{
    InterfaceServer * server  = (InterfaceServer*) arg;
    if (whatEvent | BEV_EVENT_EOF)
    {
        printf("connection closed\n");
    }
    else
    {
        printf("Error occured while %s\n",
                whatEvent | BEV_EVENT_READING ? "reading" : "writing");
        printf("Error is %d\n", whatEvent);
        printf("Error type : eof:%d,err:%d,timeout:%d,connected:%d\n",
                BEV_EVENT_EOF, BEV_EVENT_ERROR, BEV_EVENT_TIMEOUT,
                BEV_EVENT_CONNECTED);
    }
    bufferevent_free(bev);
    server->nConnections--;
}

void evconnlistener_error_cb(struct evconnlistener *listener, void *ctx)
{
      struct event_base *base = evconnlistener_get_base(listener);
      int err = EVUTIL_SOCKET_ERROR();
      fprintf(stderr, "Got an error %d (%s) on the listener. "
              "Shutting down.\n", err, evutil_socket_error_to_string(err));
      event_base_loopexit(base, NULL);

}
//typedef void (*evconnlistener_cb)(struct evconnlistener *listener,
//            evutil_socket_t sock, struct sockaddr *addr, int len, void *ptr);
void evconnlistener_on_accept(evconnlistener* listener, evutil_socket_t sock,
        sockaddr* addr, int len, void* arg)
{
    sockaddr_in* cli_addr = (sockaddr_in*)addr;
    InterfaceServer * server = (InterfaceServer*)arg;

    char ip[20];
    memset(ip, '\0', sizeof(ip));
    inet_ntop(AF_INET, &cli_addr, ip, len);

    printf("Client:%s,%d has connect\n", ip, cli_addr->sin_port);
   
    struct event_base *base = evconnlistener_get_base(listener);
    struct bufferevent * buffevent = bufferevent_socket_new(base,sock, 
            BEV_OPT_CLOSE_ON_FREE);
    if (!buffevent)
    {
        close(sock);
        printf("bufferevent create failed\n");
        return ;
    }
    
    bufferevent_setcb(buffevent, bufferevent_on_read, NULL,
            bufferevent_on_event, (void*)server);

    server->nConnections++;
    printf("server nConnections:%d\n",server->nConnections);
    bufferevent_enable(buffevent, EV_READ|EV_WRITE);
}

int InterfaceServer::init(readCallBackFun * rcb)
{
    this->rcb = rcb;

    this->evbase = event_base_new();

    struct sockaddr_in my_addr;
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    
    socklen_t socklen = sizeof(my_addr);

    this->listener = evconnlistener_new_bind(this->evbase, evconnlistener_on_accept,
            (void*)this, LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE, SERVER_BACKLOG,
            (sockaddr*)&my_addr, (int)socklen);

    evconnlistener_set_error_cb(listener, evconnlistener_error_cb);

    //TODO
	//check to see if we've init successfully
	if (!this->evbase || !this->listener)
	{
		return -1;
	}
	return 0;
}


void InterfaceServer::start()
{
	cout << "server started" << endl;

    event_base_dispatch(this->evbase);
    cout << "server ended" << endl;
}




/*  
void InterfaceServer::queryRedis(char * query)
{
    redisReply * reply = (redisReply*)redisCommand(this->rContext, query);
    switch( reply->type )
    {
    case REDIS_REPLY_INTEGER:
        cout << reply->integer;
        break;
    case REDIS_REPLY_STRING:
        cout << reply->str;
        break;
    case REDIS_REPLY_STATUS:
        cout << reply->str;
        break;
    case REDIS_REPLY_ERROR:
        cout << reply->str;
        break;
    default:
        cout << "got error: " << reply->type << endl;
    }


    freeReplyObject(reply);
}


struct event_base * InterfaceServer::getEventBase()
{
    return this->evbase;
}
*/



