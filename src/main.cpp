#include <stdlib.h>
#include <unistd.h>
#include <iostream>
using namespace std;

#include "InterfaceServer.h"

int main()
{
    /*  
    InterfaceServerConfig ifconfig;
    ifconfig.redisIP = "127.0.0.1";
    ifconfig.redisPort = 6379;
    ifconfig.interfaceServerPort = 9999;
*/

	InterfaceServer server = InterfaceServer((InterfaceServerConfig*)(void*)(0));

	if( server.init() )
    {
        cout << "server init failed" << endl;
        exit(-1);
    }
	
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


