//
#include <stdio.h>
#include <string.h>
#include "hiredis/hiredis.h"

int main()
{
	redisContext *c = redisConnect("127.0.0.1",6379);
	if (c->err)
	{
		printf("connect error.%s\n", c->errstr);
		return -1;
	}
	else
	{
		printf("connected\n");
	}
	char * value = "IT's a test";
	redisReply * reply = redisCommand(c,"set key %s", value);
	freeReplyObject(reply);
	
	redisReply * rep2 = redisCommand(c, "get key");
	printf("key:1 value:%s\n",rep2->str);
	freeReplyObject(rep2);
	
	redisFree(c);

	return 0;

}
