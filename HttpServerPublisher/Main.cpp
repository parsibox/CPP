#include <signal.h> 
#include"signalHandler.h"
#include"EventLog.h"
#include"DBPicker.h"
#include"Configuration.h"
#include"WebService.h"
#define REL_VER "V1.0"
#define RELEASE_DATE "14-Apr-2014"

CEventLog CG_EventLog;
CConfiguration CG_Cfg;
CDBPicker *pCG_publisherObj=NULL;
WebServer *httpWebServer = NULL;
static void fnG_PrintCompanyBanner(char* pscL_ModuleName,char* pscL_Version)
{
	printf("******************************************************************\n");
	printf("SmppClient- (All rights reserved)\n");
	printf(" %s--Version: %s\n", pscL_ModuleName,REL_VER);
	printf("Release date :%s                                        \n",RELEASE_DATE);
	printf("******************************************************************\n");
}

void exitFunction( int dummy )
{
   if (httpWebServer != NULL) httpWebServer->stopService();
}

bool isValidPort(int iL_port)
{

	if(iL_port>0 && iL_port< 32365) return true;
	return false;
}


int main(int argc, char* argv[]) {
	if((argc == 2) && (strcasecmp(argv[1],"-v")==0))
	{
		fnG_PrintCompanyBanner("Publisher", REL_VER);
		return 0;
	}

	signal( SIGTERM, exitFunction );
	signal( SIGINT, exitFunction );


	char pscL_Temp[40]={0x00};

	sprintf(pscL_Temp, "HTTPPUBLISHER%d",CG_Cfg.mcfn_getModuleId());

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

	sleep(1);
	//NVJ_LOG->addLogOutput(new LogStdOutput);
	char pCL_logFileName[1024]={0x00} ;
	sprintf(pCL_logFileName,"%s/webserver.log",CG_Cfg.mcfn_getLogPath());
	NVJ_LOG->addLogOutput(new LogFile(pCL_logFileName));

	WebPush webpush ;
	DynamicRepository myRepo;
	myRepo.add("/webpush",&webpush); // unusual html extension for a dynamic page !

	if(isValidPort(CG_Cfg.mcfn_getHttpPort()))
	{
		httpWebServer = new WebServer;
		httpWebServer->addLoginPass("icmuser","icmuser");
		httpWebServer->listenTo(CG_Cfg.mcfn_getHttpPort());
		httpWebServer->setThreadsPoolSize(CG_Cfg.mcfn_getNumOfWorkerThreads());
		if(CG_Cfg.mcfn_getSslFileName()[0]!=0x00)
		{
			httpWebServer->setUseSSL(true, CG_Cfg.mcfn_getSslFileName());
		}
		httpWebServer->addRepository(&myRepo);
		httpWebServer->startService();
		httpWebServer->wait();
	}
	else
	{
		DBG_CRITICAL((CG_EventLog), ("Http Port configured is invalid:%d",CG_Cfg.mcfn_getHttpPort()));
	}
	LogRecorder::freeInstance();
	return 0;
}
