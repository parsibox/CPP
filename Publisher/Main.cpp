#include"signalHandler.h"
#include"EventLog.h"
#include"DBPicker.h"
#include"Configuration.h"
#define REL_VER "V1.0"
#define RELEASE_DATE "14-Apr-2014"

CEventLog CG_EventLog;
CConfiguration CG_Cfg;
CDBPicker *pCG_publisherObj=NULL;
static void fnG_PrintCompanyBanner(char* pscL_ModuleName,char* pscL_Version)
{
	printf("******************************************************************\n");
	printf("SmppClient- (All rights reserved)\n");
	printf(" %s--Version: %s\n", pscL_ModuleName,REL_VER);
	printf("Release date :%s                                        \n",RELEASE_DATE);
	printf("******************************************************************\n");
}

int main(int argc, char* argv[]) {
	if((argc == 2) && (strcasecmp(argv[1],"-v")==0))
	{
		fnG_PrintCompanyBanner("Publisher", REL_VER);
		return 0;
	}
	char pscL_Temp[40]={0x00};

	sprintf(pscL_Temp, "PUBLISHER%d",CG_Cfg.mcfn_getModuleId());

	CG_EventLog.mcfn_setAPPTag(pscL_Temp);
	CG_EventLog.mcfn_setFilePath((char*)CG_Cfg.mcfn_getLogPath());
	CG_EventLog.mcfn_setLogLevel(CG_Cfg.mcfn_getLogLevel());
	CG_EventLog.mcfn_setNewFileGenerationPeriod(CG_Cfg.mcfn_getLogChangeInterval());
	try {
		CG_EventLog.mcfn_openNewFile();
	} catch (STSGenException e) {
		printf("Unable to open file in path:%s",e.reasonString());
		exit(1);
	}

	//CSignalHandler CL_SigHandler;
	DBG_INFO((CG_EventLog),("Version:%s",REL_VER));
	DBG_INFO((CG_EventLog),("Release Date:%s",RELEASE_DATE)); 
	//ulG_ProcessId = getpid();
	CDBPoolManager *pCL_PoolManager = CDBPoolManager::mcfnS_GetInstance();
	DBG_VERBOSE((CG_EventLog), ("PoolMgr Size:%d", pCL_PoolManager->mcfn_GetNumberOfPools()));
	pCL_PoolManager->mcfn_AddDB(CG_Cfg.mcfn_getDbName(),CG_Cfg.mcfn_getDbIp(),CG_Cfg.mcfn_getDbUserName(),CG_Cfg.mcfn_getDbPassword());
	CPublisher::mcfnS_GetInstance();
	sleep(1);
	pCG_publisherObj=new CDBPicker(CG_Cfg.mcfn_getModuleId(),CG_Cfg.mcfn_getNumOfWorkerThreads());	
	while(1){
		try{
			sleep(10);
		}
		catch(CSignalException e){
			DBG_ERROR((CG_EventLog),("Exception:%s",e.what()));
		}
		catch(...){
			DBG_ERROR((CG_EventLog),("Some unknown Exception came in main"));
		}
	}
	return 0;
}
