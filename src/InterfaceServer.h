#ifndef __INTERFACESERVER_H__
#define __INTERFACESERVER_H__

#include "../common/common.h"


#define SERVER_PORT  9999
#define SERVER_BACKLOG 500


class InterfaceServer
{
	public:
		InterfaceServer(int port);
		~InterfaceServer();

		int init(MsgDispatchFunc* rcb);

		void start();

    public:
        int nConnections;


    public:
        MsgDispatchFunc* rcb;

	private:
        //config
        int port;
 
        //event context
        struct event_base * evbase;
        struct evconnlistener * listener;
};



#endif

