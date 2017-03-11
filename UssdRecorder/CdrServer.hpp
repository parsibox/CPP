/*
 * CdrServer.hpp
 *
 *  Created on: Apr 05, 2014
 *      Author: surya
 */

#ifndef CDRSERVER_H_
#define CDRSERVER_H_
#include"TcpServer.h"
#include"CdrMsg.pb.h"
#include"STSMsgQue.h"
class CCdrServer:public CTcpServer
{
	private:
			
	public:
		CCdrServer(int iL_port,std::string CL_ip="");
		~CCdrServer();
		//virtual function from CTcpServer class
		bool mcfn_onAccept(CSTSSocket *);
		bool mcfn_onReceiveData(std::string,int,CSTSSocket*);
		bool mcfn_onDisconnect(CSTSSocket*);
			
};
#endif

