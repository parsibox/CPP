/*
 * Publisher.h
 *
 *  Created on: Apr 14, 2014
 *      Author: surya
 */


#ifndef __PUBLISHER_H_
#define __PUBLISHER_H_
#include"TcpClient.h"
#include"STSMsgQue.h"
#include"Thread.h"
#include"DeliverToEsme.pb.h"
#include"AutoLocker.h"


class CClientConnect:public CTcpClient{
	int mesi_esmeId;
	public:
	CClientConnect():CTcpClient("EsmeClinet"){
	}
	void mcfn_setEsmeId(int iL_esmeId){mesi_esmeId=iL_esmeId;}
	//virtual functions
	int mcfn_getEsmeId(){return mesi_esmeId;}
	bool mcfn_receiveData();
	bool mcfn_onConnect();

};

class CPublisher:public CThread{
	private:
		int mesi_activeIdx;
		int mesi_totalConn;
		CCriticalSection meC_connLock;
		static CPublisher *pmeC_Instance;
		std::vector<long> meC_ClientList;
		CMsgQue<DeliverToEsme *> meC_Que;
	private:
		CPublisher(){
			mesi_activeIdx=0;
			mesi_totalConn=0;
			meC_ClientList.clear();
			meC_Que.mcf_clearQue();
			mcfn_start();
		}

		~CPublisher(){};
	public:
		static CPublisher *mcfnS_GetInstance();
		bool mcfn_addToQue(DeliverToEsme* pCL_Msg,bool bL_needCheck=true){
			if(!bL_needCheck||(mcfn_isConnAvailable()&&meC_Que.mcfb_getCount()<10000)){
				meC_Que.mcfb_insertIntoQue(pCL_Msg);
				return true;
			}
			return false;
		}
		bool mcfn_addConnection(CClientConnect *);
		bool mcfn_deleteConnection(CClientConnect *);
		CClientConnect * mcfn_getConnection();
		bool mcfn_isConnAvailable();
		//virtual function
		bool  mcfn_run(int,void*);
		std::string mcfn_getThreadName(void *){
			return "PublisherThread";
		}

};
#endif
