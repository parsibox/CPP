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
#include"Receiver.hpp"
#include"DeliverToEsme.pb.h"
class CSmpp{
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
		
		static void mcfnS_processReceivedMsgs(void *);
		static void mcfnS_sendSmppMsgsToSmsc(void *);
};
#endif


