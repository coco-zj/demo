#ifndef __INTERFACESERVER_H__
#define __INTERFACESERVER_H__

#include <hiredis.h>

struct InterfaceServerConfig
{
	int interfaceServerPort;

	char redisIP[20];
	int redisPort;
};




#define REDIS_PORT  6379
#define SERVER_PORT 9999
#define SERVER_BACKLOG 500




class InterfaceServer
{
	public:
		InterfaceServer(InterfaceServerConfig *cfg);
		~InterfaceServer();

		int init();

		void start();
        void queryRedis(char * query);

        struct event_base * getEventBase();

    public:
        int nConnections;


	private:
        //config
        InterfaceServerConfig config;
        //redis context
        redisContext * rContext;
        //event context
        struct event_base * evbase;
        struct evconnlistener * listener;

};



#endif

