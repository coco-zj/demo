/*
 * =====================================================================================
 *
 *       Filename:  client.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/25/2014 05:34:53 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include <google/protobuf/message.h>
#include "common/protobuf_codec.h"
#include "protos/query.pb.h"

using namespace std;


#define SERVER_PORT 9999
#define SERVER_IPADDR "127.0.0.1"


/*  
void makeTransportData(Query q, string& out, int& len)
{
    string protoData;
    q.SerializeToString(&protoData);
    
    const string& typeName = q.GetTypeName();
    len = typeName.length() + 1 + protoData.length();
    
    out.empty();
    out.append(reinterpret_cast<char*>(&len), sizeof(len));
    out.append(typeName.c_str());
    out.append("\n");
    out.append(protoData);

    return;
}
*/

int setNonBlock(int fd)
{
    int fl = fcntl(fd, F_GETFL, 0);
    return fcntl(fd, F_SETFL, fl | O_NONBLOCK);
}

int Connect(int sockfd, sockaddr* srvaddr, size_t slen, int timeout)
{
    if (timeout > 0)
    {
        setNonBlock(sockfd);
    }
    if (0 == connect(sockfd, srvaddr, slen))
    {
        printf("connect directly success\n");
        return 0;
    }
    if (errno == EISCONN)
    {
        printf("connect directly success\n");
        return 0;
    }
    if (errno != EINPROGRESS)
    {
        printf("unexpected connect error:%s\n", strerror(errno));
        return -1;
    }
    
    fd_set rset, wset;
    FD_ZERO(&rset);
    FD_ZERO(&wset);
    FD_SET(sockfd, &rset);
    FD_SET(sockfd, &wset);
    
    struct timeval tv;
    tv.tv_sec = timeout;
    tv.tv_usec = 0;


    int res = -1;
    int nfd = select(sockfd+1, &rset, &wset, NULL, &tv);
    switch (nfd)
    {
        case -1:
            printf("select error\n");
            break;
        case 0: //timeout
            printf("connect timeout\n");
            break;
        default:
            if (FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset))
            {
                connect(sockfd, srvaddr, slen);
                res = (errno == EISCONN ? 0 : -1);
                if (!res)
                    printf("connect success\n");
            }
            else
            {
                printf("can't read or write\n");
            }
            break;
    }
    return res;
}

int main()
{
   
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if( inet_pton(AF_INET, SERVER_IPADDR, &server_addr.sin_addr) <= 0)
    {
        printf("inet_pton error\n");
        exit(0);
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    for(int i=0; i < 5; ++i)
    {
        int res = Connect(sockfd, (struct sockaddr*)&server_addr, 
                sizeof(server_addr), 5);
        if (res)
            continue;
        else
            break;
    }
  
    srand(time(0));
/*  
    for (int i = 0; i < 10; ++i)
    {

        Query q;
        q.set_id(rand());
        q.set_from("zj");

        string result = encode(q);

        if( send(sockfd, result.c_str(), result.size(), 0) < 0 )
        {
            printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
            exit(0);
        }
        printf("send one msg\n");
    }
*/    
    fd_set rset, wset;
    FD_ZERO(&rset);
    FD_ZERO(&wset);
    while(1)
    {
        FD_SET(sockfd, &rset);
        FD_SET(sockfd, &wset);
        int nfd = select(sockfd+1, &rset, &wset, NULL, NULL);
        switch(nfd)
        {
            case -1:
                printf("select errori:%s\n",strerror(errno));
                break;
            case 0:
                printf("select timeout\n");
                break;
            default:
                {
                    if (FD_ISSET(sockfd, &rset))
                    {
                        char data[128];
                        memset(data, '\0', sizeof(data));
                        recv(sockfd, data, sizeof(data)-1, 0);
                        printf("receive data is:<%s>\n", data);
                    }
                    if (FD_ISSET(sockfd, &wset))
                    {
                        Query q;
                        q.set_id(rand());
                        q.set_from("zj");
                        string transport = encode(q);
                        send(sockfd, transport.c_str(), transport.size(), 0);
                        printf("send one msg\n");
                        sleep(1);
                    }
                }break;
        }
    }

    /*  
    char data[128];
    while(1)
    {
        memset(data, '\0', sizeof(data));

        int nread = recv(sockfd, data, sizeof(data)-1,0);
        if (0 == nread)
            break;
        if (errno == EWOULDBLOCK)
        {
            printf("sleeping.\n");
            sleep(1);
        }
        else
            printf("reveive data is %s\n", data);
    }
    */
    close(sockfd);
    
    return 0;
}

    
