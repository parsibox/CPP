/*
 * DBOperations.cpp
 *
 *  Created on: Aug 28, 2013
 *      Author: surya
 */

#include "DBOperations.h"
#include <sstream>
#include "STSOSUtil.h"
std::string CDBOperations::mcfn_ChangeIgnoreLetters(char * pscL_InputTemp)
{
	char pscL_Output[2000],pscL_Input[2000];
	memset(pscL_Output,0x00,2000);
	memset(pscL_Input, 0x00,2000);
	int slL_InputLen;
	int slL_i,slL_k=0;
	char pscL_ch[]="\\";
	strcpy(pscL_Input,pscL_InputTemp);
	slL_InputLen=strlen(pscL_Input);
	for(slL_i=0;slL_i<slL_InputLen;++slL_i)
	{
		if(pscL_Input[slL_i]=='\''||pscL_Input[slL_i]=='\"'||pscL_Input[slL_i]=='\\'){
			pscL_Output[slL_k++]=pscL_ch[0];
			pscL_Output[slL_k++]=pscL_Input[slL_i];
		}
		else{
			pscL_Output[slL_k++]=pscL_Input[slL_i];
		}
	}
	pscL_Output[slL_k++]='\0';
	strcpy(pscL_Input,pscL_Output);
	return pscL_Output;
}


int CDBOperations::mcfn_ExecuteQuery(std::string psL_Query)
{
	mysqlpp::Connection *pCL_DBConn = NULL;
	int siL_Idx = -1;
	CDBPoolManager *pCL_PoolMgr =NULL;
	pCL_PoolMgr = CDBPoolManager::mcfnS_GetInstance();
	if ((pCL_DBConn = pCL_PoolMgr->mcfn_Grab(siL_Idx)) == NULL) {
		DBG_ERROR((CG_EventLog), ("No connection found whlie mcfn_GetTaskList"));
		pCL_PoolMgr->mcfn_Release(pCL_DBConn, siL_Idx);
		return -1;
	}
	DBG_VERBOSE((CG_EventLog),("DBConn:%u,Idx:%d", pCL_DBConn, siL_Idx));
	mysqlpp::Query CL_Query = pCL_DBConn->query();
	//CL_Query<<mcfn_ChangeIgnoreLetters((char*)psL_Query.c_str());
	CL_Query<<psL_Query;
	DBG_VERBOSE((CG_EventLog),("Executing Query:%s", CL_Query.str().c_str()));
	if (CL_Query.execute()) {
		DBG_VERBOSE((CG_EventLog),("Executed SuccessFully"));
		pCL_PoolMgr->mcfn_Release(pCL_DBConn, siL_Idx);
		return SUCCESS;
	}
	else{
		DBG_ERROR((CG_EventLog), ("Error in executing query for tasklist[%d]:%s",pCL_DBConn->errnum(),pCL_DBConn->error()));
		pCL_PoolMgr->mcfn_Release(pCL_DBConn, siL_Idx);
		return pCL_DBConn->errnum();
	}
}

int CDBOperations::mcfn_ExecuteQuery(std::string psL_Query,mysqlpp::StoreQueryResult &CL_Result)
{
	mysqlpp::Connection *pCL_DBConn = NULL;
	int siL_Idx = -1;
	CDBPoolManager *pCL_PoolMgr =NULL;
	pCL_PoolMgr = CDBPoolManager::mcfnS_GetInstance();
	if ((pCL_DBConn = pCL_PoolMgr->mcfn_Grab(siL_Idx)) == NULL) {
		DBG_ERROR((CG_EventLog), ("No connection found whlie mcfn_GetTaskList"));
		pCL_PoolMgr->mcfn_Release(pCL_DBConn, siL_Idx);
		return -1;
	}
	DBG_VERBOSE((CG_EventLog),("DBConn:%u,Idx:%d", pCL_DBConn, siL_Idx));
	mysqlpp::Query CL_Query = pCL_DBConn->query();
	//CL_Query<<mcfn_ChangeIgnoreLetters((char*)psL_Query.c_str());
	CL_Query<<psL_Query;
	DBG_VERBOSE((CG_EventLog),("Executing Query:%s", CL_Query.str().c_str()));
	CL_Result= CL_Query.store();
	if (CL_Result) {
		DBG_VERBOSE((CG_EventLog),("Executed SuccessFully"));
		pCL_PoolMgr->mcfn_Release(pCL_DBConn, siL_Idx);
		return SUCCESS;
	}
	else{
		DBG_ERROR((CG_EventLog), ("Error in executing query for tasklist[%d]:%s",pCL_DBConn->errnum(),pCL_DBConn->error()));
		pCL_PoolMgr->mcfn_Release(pCL_DBConn, siL_Idx);
		return pCL_DBConn->errnum();
	}
}

/*
bool CDBOperations::mcfn_updateCdrStatus(std::string CL_smiCsv,int iL_lastDigit){
	_6d_TraceFun;
	if(CL_smiCsv.empty())
		return true;
	std::stringstream CL_QueryStream;
	CL_QueryStream.str("");
	CL_QueryStream<<"UPDATE MSGMON_0"<<iL_lastDigit<<" SET CDR_STATUS=1 WHERE SMI in ("<<CL_smiCsv<<")";
	CSTSOSUtil::mcfn_sleep(6);
	if(mcfn_ExecuteQuery(CL_QueryStream.str())==0){
		return true;
	}
	return false;
}
bool CDBOperations::mcfn_updateMsgStatus(std::string CL_smiCsv,int iL_lastDigit,int iL_MessageStatus){
	_6d_TraceFun;
	if(CL_smiCsv.empty())
		return true;
	std::stringstream CL_QueryStream;
	CL_QueryStream.str("");
	CL_QueryStream<<"UPDATE MSGMON_0"<<iL_lastDigit<<" SET SUBMIT_STATUS=1 ,RETRY_COUNT=RETRY_COUNT+1,FINAL_MSG_STATUS="<<iL_MessageStatus<<" WHERE SMI in ("<<CL_smiCsv<<")";
	CSTSOSUtil::mcfn_sleep(6);
	if(mcfn_ExecuteQuery(CL_QueryStream.str())==0){
		return true;
	}
	return false;
}

bool CDBOperations::mcfn_updateSchduledTime(std::string CL_smiCsv,std::string CL_tableName,std::string CL_scheduleTime){
	_6d_TraceFun;
	if(CL_smiCsv.empty())
		return true;

	std::stringstream CL_QueryStream;
	CL_QueryStream.str("");
	CL_QueryStream<<"UPDATE "<<CL_tableName<<" SET SCHEDULE_TIME='"<<CL_scheduleTime<<"' WHERE SMI in ("<<CL_smiCsv<<")";
	if(mcfn_ExecuteQuery(CL_QueryStream.str())==0){
		return true;
	}
	return false;
}
*/
