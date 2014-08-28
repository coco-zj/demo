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
#include <string.h>

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

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }

    Query q;
    q.set_id(1);
    q.set_from("zj");
   
//    makeTransportData(q, result, len);
    string result = encode(q);
    
    int len = 0;
    copy(result.begin(), result.begin()+sizeof(int), reinterpret_cast<char*>(&len));
    len = ntohl(len);
//    printf("sizeread:%d, sizereal:%lu\n",len,result.size()-sizeof(int));
/*  `
    for(int i=0; i < result.size(); ++i)
        cout << (int)*(result.c_str()+i) << " ";
    cout << endl;
*/
    for( int i = 0; i < 10; ++i)
    {
        if( send(sockfd, result.c_str(), result.size(), 0) < 0 )
        {
            printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
            exit(0);
        }
        printf("send one msg\n");
    }
    char data[128];
    memset(data, '\0', sizeof(data));
    
//    recv(sockfd, data, sizeof(data)-1,0);

    //printf("reveive data is %s\n", data);
    close(sockfd);
    return 0;
}

    
