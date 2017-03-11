/*
 * SMPP.h
 *
 *  Created on: Apr 05, 2014
 *      Author: surya
 */

#ifndef _CSMPP_H_
#define _CSMPP_H_
#include"EsmeConnection.hpp"
#include"DBOperations.h"
#include"UssdData.pb.h"
#include"Thread.h"

class CSmpp:public CThread{
	public:
		int mcsi_MsgPerSec;
		CReceiver *pmcC_Receiver;	
		CEsmeConnection mcC_SmppClient;
	public:
		CSmpp(int iL_modId);
		~CSmpp();
		bool mcfn_loadConfigurations(int );
		int mcfn_loadEsmeDetails(int );
		bool  mcfn_loadSmscDetails(int );
		
		bool mcfn_createSmppMessage(DeliverToEsme *);
		bool mcfn_run(int iL_threadId,void *vL_arg);
		std::string mcfn_getThreadName(void*);
		bool  mcfn_processReceivedMsgs(void *);
		bool mcfn_sendSmppMsgsToSmsc(void *);
};
#endif


