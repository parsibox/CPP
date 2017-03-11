/*
 * Receiver.hpp
 *
 *  Created on: Apr 05, 2014
 *      Author: surya
 */

#ifndef RECEIVER_H_
#define RECEIVER_H_
#include"TcpServer.h"
class CReceiver:public CTcpServer
{
	private:
		bool meb_isThrowtteled;
		int mesi_ThrowttleSize;
		std::vector<CSTSSocket*> meC_connVec;
			
	public:
		CReceiver(int iL_port,int iL_trwQSize,std::string CL_ip="");
		~CReceiver();
		//virtual function from CTcpServer class
		bool mcfn_onAccept(CSTSSocket *);
		bool mcfn_onReceiveData(std::string,int,CSTSSocket*);
		bool mcfn_onDisconnect(CSTSSocket*);
		bool mcfn_sendThrottleMessage(int);
			
};
#endif

