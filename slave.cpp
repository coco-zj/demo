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
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <thread>

using namespace std;



void handle(int sockfd)
{
    char buffer[256];
    memset(buffer, '\0', sizeof(buffer));
    int nread = recvfrom(sockfd, buffer, sizeof(buffer)-1, 0, NULL, NULL);
        
    printf("recv data:%s\n", buffer);

    usleep(300);
    memset(buffer, '\0', sizeof(buffer));
    snprintf(buffer, sizeof(buffer)-1, "this reply is from slave:\n" );
    sendto(sockfd, buffer, strlen(buffer), 0, NULL, 0);
    close(sockfd);
}



int main(int argc, char *argv[])
{
    if (2 != argc)
    {
        printf("Usage: %s <filepath>", argv[0]);
        return -1;
    }

    int listenfd;
    sockaddr_un srvaddr;
    memset(&srvaddr, '\0', sizeof(srvaddr));
    srvaddr.sun_family = AF_LOCAL;
    strcpy(srvaddr.sun_path, argv[1]);


    listenfd = socket(PF_LOCAL, SOCK_STREAM, 0);
    bind(listenfd, (struct sockaddr*)&srvaddr, sizeof(srvaddr));

    listen(listenfd, 30);
    
    char buffer[256];

    while (1)
    {
        int sockfd = accept(listenfd, NULL, NULL);
        if (-1 == sockfd)
        {
            printf("accept error:%s\n", strerror(errno));
        }
        thread th(handle, sockfd);
        th.detach();
    }






}

