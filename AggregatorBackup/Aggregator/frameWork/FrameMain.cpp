#include"IConfigParams.hpp"
#include"IFrameWork.hpp"
#include"EventLog.h"
#include"FrameWorkExterns.hpp"
#define REL_VER "V1.0"
#define RELEASE_DATE "05-Apr-2014"

using namespace FrameWork;
CEventLog CG_EventLog;
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
		pCG_FrameWorkObj->mcfn_printBanner( );
		return 0;
	}
	char pscL_Temp[100]={0x00};

	sprintf(pscL_Temp, "%s%d",pCG_FrameWorkObj->mcfn_getModuleName().c_str(),pCG_FrameWorkCfgObj->mcfn_getModuleId());
	CG_EventLog.mcfn_setAPPTag(pscL_Temp);
	CG_EventLog.mcfn_setFilePath((char*)pCG_FrameWorkCfgObj->mcfn_getLogPath());
	CG_EventLog.mcfn_setLogLevel(pCG_FrameWorkCfgObj->mcfn_getLogLevel());
	CG_EventLog.mcfn_setNewFileGenerationPeriod(pCG_FrameWorkCfgObj->mcfn_getLogChangeInterval());
	try 
	{
		CG_EventLog.mcfn_openNewFile();
	} 
	catch (STSGenException e) 
	{
		printf("Unable to open file in path:%s",e.reasonString());
		exit(1);
	}
	pCG_FrameWorkObj->mcfn_printBanner( true );
	pCG_FrameWorkObj->mcfn_initApplication();

	pCG_FrameWorkObj->mcfn_handleApplication();

	return 0;
}
