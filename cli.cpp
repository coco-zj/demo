/*
 * =====================================================================================
 *
 *       Filename:  cli.cpp
 *
 *    Description:  j
 *
 *        Version:  1.0
 *        Created:  09/02/2014 11:20:35 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    if (3 != argc)
    {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        return -1;
    }
    sockaddr_in srvaddr;
    memset(&srvaddr, '\0', sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &srvaddr.sin_addr);
    
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    char buffer [256];
    memset(buffer, '\0', sizeof(buffer));
    snprintf(buffer, sizeof(buffer)-1, "msg from client:haha");
    sendto(sockfd, buffer, strlen(buffer), 0, (sockaddr*)&srvaddr, sizeof(srvaddr));
    
    printf("aaa\n");

    socklen_t slen;
    memset(buffer, '\0', sizeof(buffer));
    recvfrom(sockfd, buffer, sizeof(buffer)-1, 0, (sockaddr*)&srvaddr, &slen); 
    printf("buffer now is: %s\n", buffer);

    close(sockfd);
    return -1;
}

