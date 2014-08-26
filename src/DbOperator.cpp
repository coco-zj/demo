/*
 * =====================================================================================
 *
 *       Filename:  DbOperator.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/26/2014 08:40:13 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <string>

#include "DbOperator.h"


DbOperator::DbOperator()
{
    host = string("127.0.0.1");
    port = -1;
    password = string("nopassword");
}

DbOperator::DbOperator(string h, int p, string pass)
{
    host = h;
    port = p;
    password = pass;
}
