/*
 * =====================================================================================
 *
 *       Filename:  slave.cpp
 *
 *    Description:  jj
 *
 *        Version:  1.0
 *        Created:  09/01/2014 07:54:36 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#include <hiredis/hiredis.h>

using namespace std;

#define REDISIP     "127.0.0.1"
#define REDISPORT   6379

int registRedis(int port)
{
    redisContext * redis = NULL;
    redisReply * reply = NULL;
    int result = -1;
    do
    {
        redis = redisConnect(REDISIP, REDISPORT);
        if (!redis)
        {
            printf("init redis connection failed\n");
            break;
        }
        reply = static_cast<redisReply*>(redisCommand(redis, "select 15"));
        if (!reply)
        {
            printf("select database failed\n");
            break;
        }
        reply = static_cast<redisReply*>
            (redisCommand(redis, "LPUSH worker_list %d", port));
        if (!reply)
        {
            printf("execute redisCommand failed:%d(%s)\n", reply->type, reply->str);
            break;
        }
        printf("reply result:%s", reply->str);
    }while(0);
    
    result = 0;
    if (reply)
        freeReplyObject(reply);
    if (redis)
        redisFree(redis);

    return result;
}




int main(int argc, char *argv[])
{
    if (2 != argc)
    {
        printf("Usage: %s <port>\n", argv[0]);
        return -1;
    }
    int port = atoi(argv[1]);

    sockaddr_in srvaddr;
    memset(&srvaddr, '\0', sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srvaddr.sin_port = htons(port);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    if (-1 == bind(sockfd, (struct sockaddr*)&srvaddr, sizeof(srvaddr)))
    {
        printf("binding failed\n");
        close(sockfd);
        return -1;
    }
    //Go register yourself
    if (-1 == registRedis(port))
    {
        printf("register redis failed,existing\n");
        return -1;
    }
    printf("started at port %d, waiting for client\n", port);
    
    sockaddr_in cliaddr;
    memset(&cliaddr, '\0', sizeof(cliaddr));
    socklen_t slen = sizeof(cliaddr);

    char buffer[256];

    while (1)
    {
        memset(buffer, '\0', sizeof(buffer));
        int nread = recvfrom(sockfd, buffer, sizeof(buffer)-1, 0,
                (sockaddr*)&cliaddr, &slen);

        printf("recv data:%s\n", buffer);

        usleep(300);
        memset(buffer, '\0', sizeof(buffer));
        snprintf(buffer, sizeof(buffer)-1, "this reply is from slave:%d\n",port);
        sendto(sockfd, buffer, strlen(buffer), 0, (sockaddr*)&cliaddr, slen);
        usleep(100);
    }
    
}

