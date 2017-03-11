#include"Task.h"


CTask::~CTask(){

}
bool CTask::mcfn_loadSubTask(){
	mysqlpp::StoreQueryResult CL_ResultSet;
	CDBOperations CL_dbObj;
	std::stringstream CL_QueryStream;
	CL_QueryStream.str("");
	CL_QueryStream<<"select DISTINCT(TABLE_NAME) from schedule_table_mapping where SCHEDULE_ID="<<mesi_taskId<<" AND STATUS='P'";
	if(CL_dbObj.mcfn_ExecuteQuery(CL_QueryStream.str(),CL_ResultSet)==0){
		meC_subTasks.clear();
		if(CL_ResultSet.num_rows()>0){
			for(int i=0;i<CL_ResultSet.num_rows();i++){
				CSubTask *pCL_temp=new CSubTask(CL_ResultSet[i]["table_name"].c_str());
				//DBG_DEBUG((CG_EventLog), ("TaskId:%d,TableName:%s",mesi_taskId,pCL_temp->mcfn_getTableName().c_str()));	
				bool isExist=false;
				for(int i=0;i<meC_subTasks.size();i++){
					if(meC_subTasks[i]->mcfn_getTableName().compare(pCL_temp->mcfn_getTableName())==0){
						isExist=true;
						delete pCL_temp;
						break;
					}
				}
				if(!isExist){
				mcfn_updateTaskTableMap(mesi_taskId,'E',pCL_temp->mcfn_getTableName());
				meC_subTasks.push_back(pCL_temp);
				}
			}
			return true;
		}
	}	
	return false;
}

bool CTask::mcfn_isPause(){
	 mysqlpp::StoreQueryResult CL_ResultSet;
        CDBOperations CL_dbObj;
        std::stringstream CL_QueryStream;
        CL_QueryStream.str("");
	CL_QueryStream<<"select IS_PAUSE from schedule_profile where SCHEDULE_ID="<<mesi_taskId;
	if(CL_dbObj.mcfn_ExecuteQuery(CL_QueryStream.str(),CL_ResultSet)==0){
		 if(CL_ResultSet.num_rows()>0){
			if(atoi(CL_ResultSet[0]["is_pause"].c_str())==1)
				return true;
		 }
	}
	return false;
}

bool CTask::mcfn_checkAssignment(){
	//TODO ::need to assign sub task's in this function
	bool bL_return=false;
	if(meE_globalStatus==STATUS_NONE){
		meE_globalStatus=STATUS_ASSIGNED;
		for(int i=0;i<meC_subTasks.size();i++){
			if(meC_subTasks[i]->mcfn_getStatus()==STATUS_NONE){
				meC_subTasks[i]->mcfn_setStatus(STATUS_ASSIGNED);
				bL_return=true;
			}
		}
	}
	return bL_return;
}

bool CTask::mcfn_getAssignedSubTask(CSubTask *&pCL_subTask){
	for(int i=0;i<meC_subTasks.size();i++){
		if(meC_subTasks[i]->mcfn_getStatus()==STATUS_ASSIGNED){
			meC_subTasks[i]->mcfn_setStatus(STATUS_EXICUTING);
			pCL_subTask=meC_subTasks[i];
			return true;
		}
	}
	pCL_subTask=NULL;
	return false;
}

bool CTask::mcfn_getAllSubTasksCompleted(){
	for(int i=0;i<meC_subTasks.size();i++){
		if(meC_subTasks[i]->mcfn_getStatus()!=STATUS_COMPLETED)
                        return false;
	}
	return true;;
}
bool CTask::mcfn_updateTaskTableMap(int iL_taskId,char cL_status,std::string CL_tableName){
        mysqlpp::StoreQueryResult CL_ResultSet;
        CDBOperations CL_dbObj;
        std::stringstream CL_QueryStream;
        CL_QueryStream.str("");
        CL_QueryStream<<"UPDATE schedule_table_mapping set STATUS='"<<cL_status<<"' WHERE SCHEDULE_ID="<<iL_taskId<<" AND TABLE_NAME='"<<CL_tableName<<"'";
        if(CL_dbObj.mcfn_ExecuteQuery(CL_QueryStream.str())!=0)
                return false;
        return true;

}

