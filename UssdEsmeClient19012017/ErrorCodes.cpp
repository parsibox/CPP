#include"ErrorCodes.hpp"
#include"DBOperations.h"
CErrorCodes *CErrorCodes::pmeC_Instance = NULL;

CErrorCodes * CErrorCodes::mcfnS_GetInstance(){
        if(pmeC_Instance == NULL){
                pmeC_Instance = new CErrorCodes();
        	pmeC_Instance->mcfn_loadErrorDetails();
	}
        return pmeC_Instance;
}

std::string CErrorCodes::mcfn_getErrorString(int iL_errorCode){
	ErrorDetails CL_errDet;
	CAutoLocker CL_auto(meC_errLock);
	if(meC_errorDetailsMap.mcfb_findElement(iL_errorCode,CL_errDet)){
		return CL_errDet.mcC_errDescription;
	}	
	return "Error Not Configured";
}
int CErrorCodes::mcfn_getErrorFlags(int iL_errorCode){
	CAutoLocker CL_auto(meC_errLock);
	ErrorDetails CL_errDet;
        if(meC_errorDetailsMap.mcfb_findElement(iL_errorCode,CL_errDet)){
                return CL_errDet.mcsi_isRetry;
	}
	return 0;
}
void CErrorCodes::mcfn_loadErrorDetails(){
	CAutoLocker CL_auto(meC_errLock);
	meC_errorDetailsMap.mcf_clear();
	mysqlpp::StoreQueryResult CL_ResultSet;
	CDBOperations CL_dbObj;
	std::stringstream CL_QueryStream;
	CL_QueryStream.str("");
	CL_QueryStream<<"SELECT * from errorcode_details";
	if(CL_dbObj.mcfn_ExecuteQuery(CL_QueryStream.str(),CL_ResultSet)==0){
		for(int i=0;i<CL_ResultSet.num_rows();i++){
			ErrorDetails CL_errDet;
			CL_errDet.mcC_errDescription.assign(CL_ResultSet[i]["error_description"].c_str());
			CL_errDet.mcsi_isRetry=atoi(CL_ResultSet[i]["is_retry"].c_str());
			meC_errorDetailsMap.mcfb_insert(atoi(CL_ResultSet[i]["error_code"].c_str()),CL_errDet);
		}
	}
}
