#include"SMPP.hpp"
#include"Externs.h"
#include"signalHandler.h"

#define REL_VER "V1.0"
#define RELEASE_DATE "05-Apr-2014"

CEventLog CG_EventLog;
CMsgQue<DeliverToEsme*> CG_ReceiverQ;
CMsgMap<int,MsgTypes*> CG_seqMap;
CSmpp *pCG_Smpp=NULL;
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
		fnG_PrintCompanyBanner("SmppClient", REL_VER);
		return 0;
	}
	CSignalHandler CL_SigHandler;
	char pscL_Temp[40]={0x00};
	
	sprintf(pscL_Temp, "SMPPCLINET%d",10);
	CG_EventLog.mcfn_setAPPTag(pscL_Temp);
	CG_EventLog.mcfn_setFilePath((char*)"./Logs");
	CG_EventLog.mcfn_setLogLevel(63);
	CG_EventLog.mcfn_setNewFileGenerationPeriod(3600);
	try {
		CG_EventLog.mcfn_openNewFile();
	} catch (...) {
		printf("Unable to open file in path:./Logs");
		exit(1);
	}

	DBG_INFO((CG_EventLog),("Version:%s",REL_VER));
	DBG_INFO((CG_EventLog),("Release Date:%s",RELEASE_DATE)); 
	//ulG_ProcessId = getpid();
	CDBPoolManager *pCL_PoolManager = CDBPoolManager::mcfnS_GetInstance();
	DBG_VERBOSE((CG_EventLog), ("PoolMgr Size:%d", pCL_PoolManager->mcfn_GetNumberOfPools()));
	pCL_PoolManager->mcfn_AddDB("CMS","127.0.0.1","cmsuser","cmsuser");
	pCG_Smpp= new CSmpp(10);
	while(1){
		try{
			sleep(1);
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
