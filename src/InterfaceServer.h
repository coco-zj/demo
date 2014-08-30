#ifndef __INTERFACESERVER_H__
#define __INTERFACESERVER_H__

#include "../common/common.h"
#include "../common/MsgContainer.h"

#define SERVER_PORT  9999
#define SERVER_BACKLOG 500


class InterfaceServer
{
	public:
		InterfaceServer(int port);
		~InterfaceServer();

        void registMsgContainer(MsgContainer * c);
        MsgContainer* getMsgContainer();

		void start();
    
    public:
        int nConnections;

    private:
        MsgContainer * container;
	private:
        //config
        int port;
 
        //event context
        struct event_base * evbase;
        struct evconnlistener * listener;
};



#endif

