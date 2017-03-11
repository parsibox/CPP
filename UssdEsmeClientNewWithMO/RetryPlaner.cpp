#include"RetryPlaner.hpp"
#include <File_Interpretor.h>
#include"DBOperations.h"
#include"STSMsgQue.h"
extern CMsgQue<DeliverToEsme*> CG_ReceiverQ;

CRetryProfile::CRetryProfile()
{
	mcfn_loadPlansConfiguration();
}

CRetryProfile::~CRetryProfile()
{
}

bool CRetryProfile::mcfn_loadPlansConfiguration()
{
	DBG_INFO((CG_EventLog),("Loading retry plan from configuation"));
	mysqlpp::StoreQueryResult CL_ResultSet;
	CDBOperations CL_dbObj;
	std::stringstream CL_QueryStream;
	CL_QueryStream.str("");
	CL_QueryStream<<"SELECT * from menu_retry_plan WHERE status='A'";
	meC_planProfile.mcf_clear();
	if(CL_dbObj.mcfn_ExecuteQuery(CL_QueryStream.str(),CL_ResultSet)==0){
		if(CL_ResultSet.num_rows()>0){
			DBG_INFO((CG_EventLog),("retry plans are configured :%d",CL_ResultSet.num_rows()));
			for(int i=0;i<CL_ResultSet.num_rows();i++){
				std::vector<int> retryInterval;
				if(!mcfn_getRetryInterval(CL_ResultSet[i]["interval_ration"].c_str(),atoi(CL_ResultSet[i]["interval"].c_str()),retryInterval))
				{
					DBG_CRITICAL((CG_EventLog),("Invalid retry plan configured please check:%d[%s]",
								atoi(CL_ResultSet[i]["plan_id"].c_str()),
								CL_ResultSet[i]["interval_ration"].c_str()));

					continue;
				}
				meC_planProfile.mcfb_insert(atoi(CL_ResultSet[i]["plan_id"].c_str()),retryInterval);	
			}
			return true;
		}
		else{
			DBG_CRITICAL((CG_EventLog),("No retry plan is configured "));
		}
	}
	return false;
}

bool CRetryProfile::mcfn_getRetryInterval(std::string CL_intervalRation,int iL_interval,std::vector<int>& CL_rationVec)
{
	std::vector<std::string> CL_FieldsByColon;
	CFileInterpretor::mcfn_GetFields(CL_intervalRation, CL_FieldsByColon, ":");
	DBG_INFO((CG_EventLog),("plan interval ration:%s,interval  :%d",CL_intervalRation.c_str(),iL_interval));
	for(int i=0;i<CL_FieldsByColon.size();i++)
	{
		CL_rationVec.push_back(iL_interval * atoi(CL_FieldsByColon[i].c_str()));
	}
	return CL_rationVec.size() > 0 ? true :false;	
}

bool CRetryProfile::mcfn_getPlanTimer(int iL_planId,int iL_retryCount,int &iL_nextScheduleTime)
{
	std::vector<int> CL_retryInterval;
	if( meC_planProfile.mcfb_findElement( iL_planId ,CL_retryInterval ) )
	{
		if( CL_retryInterval.size() <= iL_retryCount )
		{
			DBG_CRITICAL((CG_EventLog),("Plan retry count exceeded ,plan count:%d,retry count:%d",CL_retryInterval.size(),iL_retryCount));
			return false;
		}
		iL_nextScheduleTime = CL_retryInterval[iL_retryCount];
		DBG_DEBUG((CG_EventLog),("[%d][%d]next timer to schedule :%d,Size:%d,RetryCount:%d", iL_planId,iL_retryCount,iL_nextScheduleTime,CL_retryInterval.size(),iL_retryCount));
		return true;
	}
	DBG_CRITICAL((CG_EventLog),("Plan not Configured but profile selected this plan. please check configurations"));
	return false;
}

CRetryPlanner::CRetryPlanner()
{
	mcfn_start();
	pmeC_retryProfile = new CRetryProfile();
	mcfn_loadPlannerConfigurations();
}
CRetryPlanner::~CRetryPlanner()
{
	delete pmeC_retryProfile;
}

bool CRetryPlanner::mcfn_loadConfiguration()
{
	CAutoLocker CL_auto(meC_retryLocker);
	pmeC_retryProfile->mcfn_loadPlansConfiguration();
	mcfn_loadPlannerConfigurations();
	return true;
}


bool CRetryPlanner::mcfn_loadPlannerConfigurations()
{
	DBG_INFO((CG_EventLog),("Loading planner configurations"));
	mysqlpp::StoreQueryResult CL_ResultSet;
	CDBOperations CL_dbObj;
	std::stringstream CL_QueryStream;
	CL_QueryStream.str("");
	CL_QueryStream<<"SELECT * from plan_profile WHERE  STATUS='A'";
	meC_retryPlanner.clear();
	if(CL_dbObj.mcfn_ExecuteQuery(CL_QueryStream.str(),CL_ResultSet)==0){
		if(CL_ResultSet.num_rows()>0){
			DBG_INFO((CG_EventLog),("plan profiles are configured :%d",CL_ResultSet.num_rows()));
			for(int i=0;i<CL_ResultSet.num_rows();i++){
				planData CL_planData;
				CL_planData.mcsi_setCount = 0;
				CL_planData.mcC_username.assign(CL_ResultSet[i]["user_name"].c_str());
				if( CL_planData.mcC_username.compare("-1")!=0 ) CL_planData.mcsi_setCount++;
				CL_planData.mcsi_errorCode = atoi(CL_ResultSet[i]["error_code"].c_str());
				if(CL_planData.mcsi_errorCode != -1 ) CL_planData.mcsi_setCount++;
				CL_planData.mcsi_msgType = atoi(CL_ResultSet[i]["msg_type"].c_str());
				if(CL_planData.mcsi_msgType != -1 ) CL_planData.mcsi_setCount++;
				CL_planData.mcsi_retryValue = atoi(CL_ResultSet[i]["retry_count"].c_str());
				if(CL_planData.mcsi_retryValue != -1 ) CL_planData.mcsi_setCount++;
				CL_planData.mcsi_planId = atoi(CL_ResultSet[i]["plan_id"].c_str());
				meC_retryPlanner.push_back ( CL_planData );	

			}
			return true;
		}
		else{
			DBG_CRITICAL((CG_EventLog),("No plan profile configuration "));
		}
	}
	return false;

}

bool CRetryPlanner::mcfn_getCheckForRetry(DeliverToEsme *pcL_data)
{
	CAutoLocker CL_auto(meC_retryLocker);
	int iL_planId=0;
	DBG_INFO((CG_EventLog),("retry check  :%s",pcL_data->ShortDebugString().c_str()));
	if(true == mcfn_gerPlanId(pcL_data,iL_planId))
	{
		int iL_nextScheduleTime = 0;
		if(pmeC_retryProfile->mcfn_getPlanTimer(iL_planId,pcL_data->retry_count(),iL_nextScheduleTime))
		{
			pcL_data->set_retry_count( pcL_data->retry_count()+ 1 ); 
			CRetryMsgTypes *pCL_retryMsg = new CRetryMsgTypes( iL_nextScheduleTime );
			pCL_retryMsg->pmcC_EsmeMsg = pcL_data;
			meC_retryMsgQue.mcfb_insertIntoQue( pCL_retryMsg );
		return true;
		}
	}
	return false;
}

bool CRetryPlanner::mcfn_run(int iL_threadId,void *pvL_obj)
{
	while(1)
	{
		CRetryMsgTypes *pCL_retryMsg = NULL;

		if( false ==  meC_retryMsgQue.mcfb_getNextMessage(pCL_retryMsg,true) )
		{
			DBG_DEBUG((CG_EventLog), ("No data Found in retry message queue going for sleep for 2 sec"));
			sleep(2);
			continue;
		}
		DBG_VERBOSE((CG_EventLog),(" Message for retry configured [%d]:%s",pCL_retryMsg->mesi_nextTimeSchedule,pCL_retryMsg->pmcC_EsmeMsg->ShortDebugString().c_str()));
		CTimerManeger::mcfnS_GetInstance()->mcfn_registerTimer((ITimer*)pCL_retryMsg,pCL_retryMsg->mesi_nextTimeSchedule,NULL);	
	}
	return false;
}

bool CRetryPlanner::mcfn_gerPlanId(DeliverToEsme *pcL_data,int &iL_planId)
{
	int  iL_matchCount=0 ;
	bool bL_return = false;
	for(int i=0;i<meC_retryPlanner.size();i++)
	{	
		int iL_presentCount=0;
		if(pcL_data->username().compare(meC_retryPlanner[i].mcC_username) == 0 ) iL_presentCount++;
		if(meC_retryPlanner[i].mcsi_errorCode == pcL_data->status()) iL_presentCount++;
		if(meC_retryPlanner[i].mcsi_msgType == pcL_data->msg_type()) iL_presentCount++;
		if(meC_retryPlanner[i].mcsi_retryValue == pcL_data->retry_count()) iL_presentCount++;

		DBG_INFO((CG_EventLog),("meC_retryPlanner[i].mcsi_setCount :%d, iL_presentCount :%d,iL_presentCount:%d,iL_matchCount:%d",meC_retryPlanner[i].mcsi_setCount,iL_presentCount,iL_presentCount,iL_matchCount));
		if(meC_retryPlanner[i].mcsi_setCount == iL_presentCount && (iL_presentCount > iL_matchCount || 0 == iL_matchCount) )
		{
			iL_matchCount =  iL_presentCount;
			iL_planId = meC_retryPlanner[i].mcsi_planId ;
			bL_return = true;
		}
	}
	return bL_return;
}

void CRetryMsgTypes::mcfn_onTimerExpiry(void *p)
{
	DBG_VERBOSE((CG_EventLog),("Retry time over tring to resend msg:%s",pmcC_EsmeMsg->ShortDebugString().c_str()))
	CG_ReceiverQ.mcfb_insertIntoQue(pmcC_EsmeMsg);
	//delete this;
}

