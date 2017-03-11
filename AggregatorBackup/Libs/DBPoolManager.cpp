/*
 * DBPoolManager.cpp
 *
 *  Created on: Jun 17, 2010
 *      Author: sunil
 */
#include "DBPoolManager.h"
//#include "Externs.h"
CDBPoolManager * CDBPoolManager::pmeC_Instance = NULL;

CDBPoolManager * CDBPoolManager::mcfnS_GetInstance(){
	if(pmeC_Instance == NULL){
		DBG_INFO((CG_EventLog),("Creating PoolManager object..."));
		pmeC_Instance = new CDBPoolManager();
	}
	return pmeC_Instance;
}

CDBPoolManager::CDBPoolManager(){
	mesi_CurrentActiveIdx = 0;
}

void CDBPoolManager::mcfn_AddDB(const char *pscL_DBName, const char* pscL_Ip, const char* pscL_User, const char* pscL_Pass){
	DBG_VERBOSE((CG_EventLog), ("CDBPoolManager::mcfn_AddDB >>"));
	meC_CriticalSection.Lock();
	CDBConnPool  *pCL_DBPool = new CDBConnPool(pscL_DBName, pscL_Ip, pscL_User, pscL_Pass);
	meC_ListOfDBPools.push_back(pCL_DBPool);
	meC_CriticalSection.Unlock();
	DBG_VERBOSE((CG_EventLog), ("CDBPoolManager::mcfn_AddDB <<"));
}

int CDBPoolManager::mcfn_GetNumberOfPools() const{
	return meC_ListOfDBPools.size();
}

std::string CDBPoolManager::mcfn_LogString(){
	std::string CL_Return;
	POOL_LIST::iterator pCL_Itr ;
	for(pCL_Itr=meC_ListOfDBPools.begin();pCL_Itr!=meC_ListOfDBPools.end();++pCL_Itr){
		CL_Return += "[DBDetails="+ (*pCL_Itr)->mcfn_LogString() + "]";
	}
	return CL_Return;
}

mysqlpp::Connection* CDBPoolManager::mcfn_Grab(int &siLo_Idx){

	meC_CriticalSection.Lock();

	mysqlpp::Connection* pCL_Conn=NULL;
	pCL_Conn = meC_ListOfDBPools[mesi_CurrentActiveIdx]->grab();
	if(pCL_Conn!=NULL)
	{
		bool bL_Return =pCL_Conn->ping();

		if(!bL_Return)
		{
			meC_ListOfDBPools[mesi_CurrentActiveIdx]->release(pCL_Conn);  
			pCL_Conn=NULL;
		}
	}
	if(pCL_Conn!=NULL){
		siLo_Idx = mesi_CurrentActiveIdx;
		DBG_VERBOSE((CG_EventLog),("mesi_CurrentActiveIdx:%d",mesi_CurrentActiveIdx));
		meC_CriticalSection.Unlock();
		return pCL_Conn;
	}else{
		DBG_ERROR((CG_EventLog),("mesi_CurrentActiveIdx is not active, hence looping"));
	}

	for(int i=0;i<mcfn_GetNumberOfPools();++i){
		pCL_Conn = meC_ListOfDBPools[i]->grab();
		if(pCL_Conn!=NULL)
		{
			bool bL_Return =pCL_Conn->ping();

			if(!bL_Return)
			{
				meC_ListOfDBPools[i]->release(pCL_Conn);  
				pCL_Conn=NULL;
			}
		}

		if(pCL_Conn!=NULL){
			mesi_CurrentActiveIdx = i;
			siLo_Idx = mesi_CurrentActiveIdx;
			break;
		}
	}
	DBG_VERBOSE((CG_EventLog), ("Current active pool idx=%d", mesi_CurrentActiveIdx));
	meC_CriticalSection.Unlock();
	return pCL_Conn;
}

mysqlpp::Connection * CDBPoolManager::mcfn_GrabByIdx(int siL_Idx){
	DBG_VERBOSE((CG_EventLog), ("Grab conn by pool idx=%d", siL_Idx));
	if(siL_Idx<0 || siL_Idx>mcfn_GetNumberOfPools()){
		DBG_ERROR((CG_EventLog),("Pool Index out of bound"));
		return NULL;
	}
	//return meC_ListOfDBPools[siL_Idx]->safe_grab();
	return meC_ListOfDBPools[siL_Idx]->grab();
}

void CDBPoolManager::mcfn_Release(const mysqlpp::Connection *pCL_Conn, int siL_PoolIdx){
	if(siL_PoolIdx<0 || siL_PoolIdx>mcfn_GetNumberOfPools()){
			DBG_ERROR((CG_EventLog),("mcfn_Release:Pool Index out of bound"));
			return;
	}
	meC_ListOfDBPools[siL_PoolIdx]->release(pCL_Conn);
}

std::list<int> CDBPoolManager::mcfn_GetAllIdxOtherThanThis(int siL_idx){
	std::list<int> CL_list;
	if(siL_idx<0|| siL_idx>=meC_ListOfDBPools.size()){
		DBG_ERROR((CG_EventLog), ("passed index is out of bound"))
	}else{
		for(int i=0;i<meC_ListOfDBPools.size();++i){
			if(i!=siL_idx) CL_list.push_back(i);
		}
	}
	return CL_list;
}
