#include"UssdAs.h"
#include"Configuration.h"
#include"xmlInterface.hpp"
#include"DBOperations.h"
#define MODULE_NAME "AGGRIGATOR"
#define VERSION "1.0"
#define RELEASE_DATE "27-06-2015"

UssdAs CG_application;
CConfiguration *pCG_Config = new CConfiguration();
FrameWork::IConfiguration *pCG_Cfg = pCG_Config;
CEsmeServer  *pCG_xmlServer = NULL;
UssdAs::UssdAs( ) : FrameWork::IFrameWork(MODULE_NAME,VERSION,RELEASE_DATE)
{

}

void  UssdAs::mcfn_initApplication() 
{
	printf("class been created\n");
	CDBPoolManager *pCL_PoolManager = CDBPoolManager::mcfnS_GetInstance();
	pCL_PoolManager->mcfn_AddDB(pCG_Config->mcfn_getDbName(),pCG_Config->mcfn_getDbIp(),pCG_Config->mcfn_getDbUserName(),pCG_Config->mcfn_getDbPassword());
	DBG_VERBOSE((CG_EventLog), ("PoolMgr Size:%d", pCL_PoolManager->mcfn_GetNumberOfPools()));
	pCG_xmlServer= new xmlInterface(pCG_Config->mcfn_getServerPort());
}

void UssdAs::mcfn_clearApplication()
{
	delete pCG_xmlServer;
	delete pCG_Config;
}
