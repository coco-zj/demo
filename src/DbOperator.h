/*
 * =====================================================================================
 *
 *       Filename:  DbOperator.h
 *
 *    Description:  abstract class of all db operators
 *
 *        Version:  1.0
 *        Created:  08/26/2014 05:12:15 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  zj,
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef __DBOPERATOR_H__
#define __DBOPERATOR_H__

#include <string>

using namespace std;


class DbOperator 
{
    protected:
        string host;
        int port;
        string password;

    public:
        DbOperator();
        DbOperator(string h, int p, string pass);
        virtual ~DbOperator(){};

        virtual void query(string queryString){};


        inline const string getHost()
        {
            return this->host;
        }
        inline int getPort()
        {
            return this->port;
        }
        inline const string getPassword()
        {
            return this->password;
        }
};



#endif


