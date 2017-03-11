/*
 * DBPicker.cpp
 *
 *  Created on: Apr 14, 2014
 *      Author: surya
 */

#include"DBPicker.h"
#include"AutoLocker.h"
#include"Configuration.h"
extern CConfiguration CG_Cfg;


CDBPicker::CDBPicker(int iL_modId,int iL_numOfWorkerThreads){
	mesi_workerThreadCounter=0;
	while(!mcfn_loadEsmeDetails(iL_modId));
	sleep(1);
	int iL_threadId[3]={1,2,3};
	for(int i=0;i<iL_numOfWorkerThreads;i++){
		mcfn_start(&iL_threadId[2]);
	}
	mcfn_start(&iL_threadId[0]);
	mcfn_start(&iL_threadId[1]);

}
CDBPicker::~CDBPicker(){
}

bool  CDBPicker::mcfn_loadEsmeDetails(int iL_modId){
	mysqlpp::StoreQueryResult CL_ResultSet;
	CDBOperations CL_dbObj;
	std::stringstream CL_QueryStream;
	CL_QueryStream.str("");
	CL_QueryStream<<"select * from esme_details where SMSC_ID in (select SMSC_ID from publisher_smsc_map where MODULE_ID="<<iL_modId<<")";
	if(CL_dbObj.mcfn_ExecuteQuery(CL_QueryStream.str(),CL_ResultSet)==0){
		if(CL_ResultSet.num_rows()<=0){
			DBG_ERROR((CG_EventLog),("No Esme configuration present for ModuleId:%d",iL_modId));
			exit(1);
		}
		DBG_INFO((CG_EventLog),("Number of  Esme configuration present for ModuleId:%d[%d]",iL_modId,CL_ResultSet.num_rows()));
		pmeC_Connections =new CClientConnect[CL_ResultSet.num_rows()];
		for(int i=0;i<CL_ResultSet.num_rows();i++){
			pmeC_Connections[i].mcfn_setEsmeId(atoi(CL_ResultSet[i]["ESME_ID"].c_str()));
			pmeC_Connections[i].mcfn_addTcpDetails(CL_ResultSet[i]["ESME_IP"].c_str(),atoi(CL_ResultSet[i]["ESME_PORT"].c_str()),CL_ResultSet[i]["ESME_SECIP"].c_str(),atoi(CL_ResultSet[i]["ESME_SECPORT"].c_str()));
		}
		return true;
	}
	return false;
}
//virtual function
bool CDBPicker::mcfn_run(int iL_threadId,void *vL_arg){
	switch(*(int*)vL_arg){
		case 1:
			return mcfn_taskSelectionThread();
		case 2:
			return mcfn_taskAssignmentThread();
		default:
			return mcfn_workerThread(++mesi_workerThreadCounter);
	}

}
std::string CDBPicker::mcfn_getThreadName(void* vL_arg){
	switch(*(int*)vL_arg){
                case 1:
                        return "TaskSelectionThread";
                case 2:
                        return "TaskAssignmentThread";
                default:
                        return "WorkerThread";
        }

}


bool CDBPicker::mcfn_taskSelectionThread(){
	while(1){
			mysqlpp::StoreQueryResult CL_ResultSet;
			CDBOperations CL_dbObj;
			std::stringstream CL_QueryStream;
			CL_QueryStream.str("");
			CL_QueryStream<<"select *,UNIX_TIMESTAMP(END_TIME) as EXPIRY_TIME from schedule_profile where STATUS in (2,5) AND CHANNEL_ID in ("<<CG_Cfg.mcfn_getChannelId()<<") and SCHEDULE_TIME<=CURRENT_TIMESTAMP and SCHEDULE_ID in (select SCHEDULE_ID from schedule_table_mapping where STATUS='P')";
			if(CL_dbObj.mcfn_ExecuteQuery(CL_QueryStream.str(),CL_ResultSet)==0){
				for(int i=0;i<CL_ResultSet.num_rows();i++){
					CTask *pCL_task=new CTask(atoi(CL_ResultSet[i]["schedule_id"].c_str()),atoi(CL_ResultSet[i]["msg_type"].c_str()),atoi(CL_ResultSet[i]["EXPIRY_TIME"].c_str()),atoi(CL_ResultSet[i]["lang_id"].c_str()),atoi(CL_ResultSet[i]["dlr_reg"].c_str()),atoi(CL_ResultSet[i]["interface_id"].c_str()),CL_ResultSet[i]["msg_text"].c_str(),CL_ResultSet[i]["schedule_name"].c_str(),CL_ResultSet[i]["menu_service_code"].c_str(),atoi(CL_ResultSet[i]["is_pause"].c_str()),atoi(CL_ResultSet[i]["channel_id"].c_str()),atoi(CL_ResultSet[i]["dcs"].c_str()),CL_ResultSet[i]["oa"].c_str(),CL_ResultSet[i]["created_by"].c_str());
					if(!(pCL_task->mcfn_loadSubTask()&&mefn_addTaskToList(pCL_task))){
						DBG_ERROR((CG_EventLog),("Loading SubTask details faild .TaskId:%d",atoi(CL_ResultSet[i]["SCHEDULE_ID"].c_str())));
						delete pCL_task;
					}
				}
			}
		DBG_INFO((CG_EventLog), ("Task Selection Thread Sleeping:%d",CG_Cfg.mcfn_getTaskPoolInterval()));
		sleep(CG_Cfg.mcfn_getTaskPoolInterval());
	}
	return true;
}
bool CDBPicker::mcfn_taskAssignmentThread(){
	while(1){
		{
			CAutoLocker CL_auto(meC_taskListLock);
			std::list<CTask*> ::iterator pCL_Itr;
			for(pCL_Itr=meC_TaskList.begin();pCL_Itr!=meC_TaskList.end();++pCL_Itr){
				CTask *pCL_Task=(*pCL_Itr);
				DBG_INFO((CG_EventLog), ("Checking Task for  Assignment:%d",pCL_Task->mcfn_TaskId()));
				if(pCL_Task->mcfn_isExpired()||pCL_Task->mcfn_getTaskStatus()==STATUS_COMPLETED){
					//TODO::need to update the status to exired here
					int iL_statusCode=125;
					if(pCL_Task->mcfn_isExpired()){
						iL_statusCode=125;
						DBG_CRITICAL((CG_EventLog), ("Task Expired:%d",pCL_Task->mcfn_TaskId()));
					}
					else{
						iL_statusCode=7;
						DBG_CRITICAL((CG_EventLog), ("Task Completed:%d",pCL_Task->mcfn_TaskId()));

					}
					if(mefn_updateTaskStaus(pCL_Task->mcfn_TaskId(),iL_statusCode)){
						delete pCL_Task;
						pCL_Itr = meC_TaskList.erase(pCL_Itr);
					}
					continue;
				}
				if(pCL_Task->mcfn_checkAssignment()){
					DBG_INFO((CG_EventLog), ("Task Assigned:%d",pCL_Task->mcfn_TaskId()));
					meC_workerQue.mcfb_insertIntoQue(pCL_Task);
				}

			}	

		}
		sleep(CG_Cfg.mcfn_getAssignmentInterval());
	}
	return true;
}
bool CDBPicker::mcfn_workerThread(int iL_threadId){
	while(1){
		CTask *pCL_task=NULL;
		if(meC_workerQue.mcfb_getNextMessage(pCL_task,false)==false){
			DBG_DEBUG((CG_EventLog), ("No Assignment task going for sleep for 2 sec"));
			sleep(2);
			continue;
		}
		CSubTask *pCL_SubTask=NULL;
		while(pCL_task->mcfn_getAssignedSubTask(pCL_SubTask)){
			mysqlpp::StoreQueryResult CL_ResultSet;
			CDBOperations CL_dbObj;
			std::stringstream CL_QueryStream,CL_msgIdCsv;
			CL_QueryStream.str("");
			int iL_counter=0,iL_msgId;
			CL_QueryStream<<"SELECT * FROM "<<pCL_SubTask->mcfn_getTableName()<<" WHERE SCHEDULE_ID="<<pCL_task->mcfn_TaskId()<<" and STATUS=0 limit "<<CG_Cfg.mcfn_BunchSize();
			if(CL_dbObj.mcfn_ExecuteQuery(CL_QueryStream.str(),CL_ResultSet)==0){
				 DBG_INFO((CG_EventLog), ("Number of Messages Selected:%d,TaskId:%d",CL_ResultSet.num_rows(),pCL_task->mcfn_TaskId()));
				if(CL_ResultSet.num_rows()>0){
					for(int i=0;i<CL_ResultSet.num_rows();i++){
						DeliverToEsme *pCL_msg=new DeliverToEsme();
						iL_msgId=atoi(CL_ResultSet[i]["msg_id"].c_str());
						pCL_task->mcfn_setValues(pCL_msg);
						pCL_msg->set_msg_id(iL_msgId);
						pCL_msg->set_da(CL_ResultSet[i]["da"].c_str());
						if(!CPublisher::mcfnS_GetInstance()->mcfn_addToQue(pCL_msg)){
							DBG_DEBUG((CG_EventLog), ("No Connection Avaiable to send the messages"));
							delete pCL_msg;
							usleep(10);
							continue;
						}
						if(iL_counter==0){
							CL_msgIdCsv<<iL_msgId;
						}
						else{
							CL_msgIdCsv<<","<<iL_msgId;
						}
						++iL_counter;
					}
					while(!mefn_updateTaskStaus(pCL_task->mcfn_TaskId(),1,iL_counter,pCL_SubTask->mcfn_getTableName(),CL_msgIdCsv.str())){
						sleep(10);
					}
				}
				else{
					pCL_SubTask->mcfn_setStatus(STATUS_COMPLETED);
					while(!pCL_task->mcfn_updateTaskTableMap(pCL_task->mcfn_TaskId(),'C',pCL_SubTask->mcfn_getTableName()))
						sleep(10);
				}
			}
			if(pCL_SubTask->mcfn_getStatus()!=STATUS_COMPLETED)
				pCL_SubTask->mcfn_setStatus(STATUS_NONE);

		}
		if(pCL_task->mcfn_getAllSubTasksCompleted()){
			pCL_task->mcfn_setTaskStatus(STATUS_COMPLETED);
		}
		else{
			pCL_task->mcfn_setTaskStatus(STATUS_NONE);
		}
	}
	return true;
}
bool CDBPicker::mefn_addTaskToList(CTask* pCL_Task){
	CAutoLocker CL_auto(meC_taskListLock);
	 std::list <CTask*>::iterator pCL_Itr;
	bool bL_IsExists=false;
	for(pCL_Itr=meC_TaskList.begin();pCL_Itr!=meC_TaskList.end();++pCL_Itr){
		if((*pCL_Itr)->mcfn_TaskId()==pCL_Task->mcfn_TaskId()){
			DBG_INFO((CG_EventLog), ("Task already present in the list hence not adding .Going to update the Task "));
			(*pCL_Itr)->mcfn_Update(pCL_Task);
			bL_IsExists = true;
			return false;
		}
	}
	if(bL_IsExists==false){
		mefn_updateTaskStaus(pCL_Task->mcfn_TaskId(),5);
		DBG_INFO((CG_EventLog), ("Task added=%d", pCL_Task->mcfn_TaskId()));
		meC_TaskList.push_back(pCL_Task);
		return true;
	}
}

bool CDBPicker::mefn_updateTaskStaus(int iL_taskId,int iL_status){
	   mysqlpp::StoreQueryResult CL_ResultSet;
                CDBOperations CL_dbObj;
                std::stringstream CL_QueryStream;
                CL_QueryStream.str("");
		CL_QueryStream<<"UPDATE schedule_profile set STATUS="<<iL_status<<" WHERE SCHEDULE_ID="<<iL_taskId;	
		 if(CL_dbObj.mcfn_ExecuteQuery(CL_QueryStream.str())!=0)
			 return false;
		 return true;
}

bool CDBPicker::mefn_updateTaskStaus(int iL_taskId,int iL_status,int iL_count,std::string CL_tableName,std::string CL_msgCsv){
	if(CL_msgCsv.empty())
	return true;
	mysqlpp::StoreQueryResult CL_ResultSet;
	CDBOperations CL_dbObj;
	std::stringstream CL_QueryStream;
	CL_QueryStream.str("");
	CL_QueryStream<<"UPDATE "<<CL_tableName<<" set STATUS="<<iL_status<<" WHERE SCHEDULE_ID="<<iL_taskId<<" AND MSG_ID in ("<<CL_msgCsv<<")";	
	if(CL_dbObj.mcfn_ExecuteQuery(CL_QueryStream.str())!=0)
		return false;
	CL_QueryStream.str("");
        CL_QueryStream<<"UPDATE schedule_profile set SUBMIT_COUNT=SUBMIT_COUNT+"<<iL_count<<" WHERE SCHEDULE_ID="<<iL_taskId;
	if(CL_dbObj.mcfn_ExecuteQuery(CL_QueryStream.str())!=0)
		return false;

	return true;
}

