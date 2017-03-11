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
	while(!mcfn_loadEsmeDetails(iL_modId));
	sleep(1);

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
bool  CDBPicker::mcfn_setTaskByServiceName(std::string CL_serviceName,DeliverToEsme *pCL_msg){
	mysqlpp::StoreQueryResult CL_ResultSet;
	CDBOperations CL_dbObj;
	std::stringstream CL_QueryStream;
	CL_QueryStream.str("");
      CL_QueryStream<<"SELECT * from mo_menu_map WHERE mo_srv_code='"<<CL_serviceName<<"' and status = 'A' limit 1";
//	CL_QueryStream<<"select * from http_service_details  where service_id="<<CL_serviceName << "and status='A' limit 1";
	if(CL_dbObj.mcfn_ExecuteQuery(CL_QueryStream.str(),CL_ResultSet)==0){
		if(CL_ResultSet.num_rows()!=1){
			DBG_ERROR((CG_EventLog),("Service is not configured in http_service_details tables :%s:%d",CL_serviceName.c_str(),CL_ResultSet.num_rows()));
			return false;
		}
		DBG_INFO((CG_EventLog),("http_service_details tables has services :%s[%d]",CL_serviceName.c_str(),CL_ResultSet.num_rows()));
		pCL_msg->set_task_id(atoi(CL_ResultSet[0]["task_id"].c_str()));
		pCL_msg->set_username(CL_ResultSet[0]["username"].c_str());
                pCL_msg->set_srv_code(CL_ResultSet[0]["menu_srv_code"].c_str());

		return true;
	}
	return false;
}
