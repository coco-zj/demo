/*
 * =====================================================================================
 *
 *       Filename:  TestRedisOperator.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/27/2014 10:03:42 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <iostream>
#include <string>

#include "src/DbOperator.h"
#include "src/RedisOperator.h"

using namespace std;


int main()
{
    RedisOperator *p = new RedisOperator("125.88.168.100",6379,"");
    p->selectDatabase("15");

    string res = p->queryString("hget TOKEN_HASH 109");
    cout << res << endl;

}



