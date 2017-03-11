/*
 * DBPoolManager.cpp
 *
 *  Created on: Jun 17, 2010
 *      Author: sunil
 */
#include "Externs.h"
#include "DBPoolManager.h"
//CDBPoolManager * CDBPoolManager::pmeC_Instance = NULL;

/*CDBPoolManager * CDBPoolManager::mcfnS_GetInstance(){
	if(pmeC_Instance == NULL){
		DBG_INFO((CG_EventLog),("Creating PoolManager object..."));
		pmeC_Instance = new CDBPoolManager();
	}
	return pmeC_Instance;
}*/

CDBPoolManager::CDBPoolManager(){
	mesi_CurrentActiveIdx = 0;
}

void CDBPoolManager::mcfn_AddDB(const char *pscL_DBName, const char* pscL_Ip, const char* pscL_User, const char* pscL_Pass){
	DBG_VERBOSE((CG_EventLog), ("CDBPoolManager::mcfn_AddDB >>"));
	DBG_INFO((CG_EventLog),("%s,%s,%s,%s",pscL_DBName,pscL_Ip,pscL_User,pscL_Pass));
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
	//////////////////////Date: 06-09-2010
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
		DBG_INFO((CG_EventLog),("mesi_CurrentActiveIdx[%d] is not active, hence looping", mesi_CurrentActiveIdx));
	}
       

/////////////////////////////

	for(int i=0;i<mcfn_GetNumberOfPools();++i){
		meC_ListOfDBPools[i]->shrink();
		pCL_Conn = meC_ListOfDBPools[i]->grab();
		DBG_VERBOSE((CG_EventLog), ("Looping idx=%d, pCL_Conn=%u",i, pCL_Conn));
		if(pCL_Conn!=NULL)
		{
			bool bL_Return =pCL_Conn->ping();
			DBG_VERBOSE((CG_EventLog), ("Looping idx=%d, pCL_Conn=%u, bool=%d",i, pCL_Conn, bL_Return));

			if(!bL_Return)
			{
				meC_ListOfDBPools[i]->release(pCL_Conn);  
				char cL_TrapMsg[200];
				memset(cL_TrapMsg,0x00,sizeof(cL_TrapMsg));
				sprintf(cL_TrapMsg,"DB  %s is Down",meC_ListOfDBPools[mesi_CurrentActiveIdx]->mcfn_LogString().c_str());

				pCL_Conn=NULL;
			}
		}

		if(pCL_Conn!=NULL){
			mesi_CurrentActiveIdx = i;
			siLo_Idx = mesi_CurrentActiveIdx;
			char cL_TrapMsg[200];
                        memset(cL_TrapMsg,0x00,sizeof(cL_TrapMsg));
                        sprintf(cL_TrapMsg,"DB  %s is UP",meC_ListOfDBPools[mesi_CurrentActiveIdx]->mcfn_LogString().c_str());

			break;
		}
	}

	DBG_VERBOSE((CG_EventLog), ("Current active pool idx=%d, %s[[pCL_Conn=%u]", 
			mesi_CurrentActiveIdx, meC_ListOfDBPools[mesi_CurrentActiveIdx]->mcfn_LogString().c_str(), pCL_Conn));
	meC_CriticalSection.Unlock();
	return pCL_Conn;
}

void CDBPoolManager::mcfn_removeConn(mysqlpp::Connection *pCL_Conn, int siL_PoolIdx){
	DBG_INFO((CG_EventLog), ("Removing the connection:%u,from pool idx=%d", pCL_Conn, siL_PoolIdx));
	if(siL_PoolIdx<0 || siL_PoolIdx>mcfn_GetNumberOfPools()){
                DBG_ERROR((CG_EventLog),("Pool Index out of bound- while removing connection"));
                return;
        }
	meC_ListOfDBPools[siL_PoolIdx]->remove(pCL_Conn);
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

std::list<int> CDBPoolManager::mcfn_GetAllIdxFrmListOtherThanThis(std::list<int>& L_List, int siL_idx){
	std::list<int> CL_List;
	if(siL_idx<0|| siL_idx>=meC_ListOfDBPools.size()){
                DBG_ERROR((CG_EventLog), ("passed index is out of bound"))
        }else{
		for(int i=0;i<meC_ListOfDBPools.size();++i){
                        if(i!=siL_idx) {
				std::list<int>::iterator itr;
				bool bL_Found = false;
				for(itr=L_List.begin();itr!=L_List.end();++itr){
					if((*itr)==i){
						bL_Found = true;
						break;
					}
				}
				if(bL_Found){
					CL_List.push_back(i);
				}
			}
                }
	}
	return CL_List;
}

mysqlpp::Connection * CDBPoolManager::mcfn_GrabFrmListOfIdx(std::list<int> L_ListOfAvailableDBIdx, int& siL_Idx){
	std::list<int>::iterator itr;
	mysqlpp::Connection *pCL_Conn = NULL;
	meC_CriticalSection.Lock();
	for(itr=L_ListOfAvailableDBIdx.begin();itr!=L_ListOfAvailableDBIdx.end();++itr){
		pCL_Conn = mcfn_GrabByIdx((*itr));
		if(pCL_Conn==NULL){
			continue;
		}else {
			
			bool bL_Return =pCL_Conn->ping();
			if(!bL_Return)
			{
				meC_ListOfDBPools[(*itr)]->release(pCL_Conn);
				meC_ListOfDBPools[(*itr)]->shrink();
				pCL_Conn = NULL;
			}else{
				siL_Idx = (*itr);
				break;
			}
		}
	}
	meC_CriticalSection.Unlock();
	return pCL_Conn;
}
