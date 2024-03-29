#include"SMPP.hpp"
#include"Externs.h"
#include"signalHandler.h"
#include"Configuration.h"
#define REL_VER "V1.0"
#define RELEASE_DATE "05-Apr-2014"

CEventLog CG_EventLog;
CConfiguration CG_Cfg;
CMsgQue<DeliverToEsme*> CG_ReceiverQ;
CMsgMap<int,MsgTypes*> CG_seqMap;
CMsgMap<std::string,CMsgMap<std::string,NodeTree*>*> *pCG_menuMap=NULL;
CCdrClient *pCG_CdrClient =NULL;
CSmpp *pCG_Smpp=NULL;
static void fnG_PrintCompanyBanner(char* pscL_ModuleName,char* pscL_Version)
{
        printf("******************************************************************\n");
        printf("SmppClient- (All rights reserved)\n");
        printf(" %s--Version: %s\n", pscL_ModuleName,REL_VER);
        printf("Release date :%s                                        \n",RELEASE_DATE);
        printf("******************************************************************\n");
}

class CRemoveOldData:public ITimer{
	public:
		CMsgMap<std::string,CMsgMap<std::string,NodeTree*>*> *pmcC_menuConf;
	public:
		CRemoveOldData(){
			pmcC_menuConf=NULL;
		}
		~CRemoveOldData(){
		}
		void mcfn_onTimerExpiry(void *p)
		{
			pmcC_menuConf->mcf_startGet();
			NodeTree* pCL_tempMenuTree=NULL;
			std::string CL_serviceCode="",CL_userName="";
			CMsgMap<std::string,NodeTree*> *pCL_userMenus=NULL;
			while(pmcC_menuConf->mcfb_getNextElement(CL_userName,pCL_userMenus))
			{
				pCL_userMenus->mcf_startGet();
				DBG_INFO((CG_EventLog),("deleting the menus for user[%d]:%s",pCL_userMenus->mcfi_getCount(),CL_userName.c_str()));	
				while(pCL_userMenus->mcfi_getCount() >0  && pCL_userMenus->mcfb_getNextElement(CL_serviceCode,pCL_tempMenuTree))
				{
					DBG_INFO((CG_EventLog),("deleting the menus for user:%s,serviceCode:%s",CL_userName.c_str(),CL_serviceCode.c_str()));	
					pCL_userMenus->mcfb_removeElement(CL_serviceCode);
					delete pCL_tempMenuTree;
				}
				pmcC_menuConf->mcfb_removeElement(CL_userName);
				delete pCL_userMenus;
			}
			delete pmcC_menuConf;
			//delete this;
		}

} ;

class CReloadHandler:public CTcpServer
{
	public:
		CReloadHandler(int iL_port):CTcpServer(iL_port,"")
		{	
			mcfn_setAppName("RELOAD");
		}
		bool mcfn_receiveFromTcp(CSTSSocket *pCL_Socket){};
		bool mcfn_onAccept(CSTSSocket *)
		{
			mcfnS_reloadConfigurations();
			return false;
		}
		static void mcfnS_reloadConfigurations()
		{
			DBG_INFO((CG_EventLog),("Reload Started"));
			CG_Cfg.mcfn_loadConfigurations();
			CG_EventLog.mcfn_setLogLevel(CG_Cfg.mcfn_getLogLevel());
			CRemoveOldData *pCL_removeData =new CRemoveOldData();
			pCL_removeData->pmcC_menuConf = pCG_menuMap;
			pCG_menuMap=new CMsgMap<std::string,CMsgMap<std::string,NodeTree*>*> ();	
			CTimerManeger::mcfnS_GetInstance()->mcfn_registerTimer((ITimer*)pCL_removeData,60,NULL);//deleteing after 10 min
			pCG_CdrClient->mcC_retryPlanner.mcfn_loadConfiguration();
			DBG_INFO((CG_EventLog),("Reload Completed"));
		}

};




int main(int argc, char* argv[]) {
	if((argc == 2) && (strcasecmp(argv[1],"-v")==0))
	{
		fnG_PrintCompanyBanner("SmppClient", REL_VER);
		return 0;
	}
	//CSignalHandler CL_SigHandler;
	char pscL_Temp[40]={0x00};
	
	sprintf(pscL_Temp, "SMPPCLINET%d",CG_Cfg.mcfn_getModuleId());
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

	DBG_INFO((CG_EventLog),("Version:%s",REL_VER));
	DBG_INFO((CG_EventLog),("Release Date:%s",RELEASE_DATE)); 
	//ulG_ProcessId = getpid();
	CDBPoolManager *pCL_PoolManager = CDBPoolManager::mcfnS_GetInstance();
	pCL_PoolManager->mcfn_AddDB(CG_Cfg.mcfn_getDbName(),CG_Cfg.mcfn_getDbIp(),CG_Cfg.mcfn_getDbUserName(),CG_Cfg.mcfn_getDbPassword());
	DBG_VERBOSE((CG_EventLog), ("PoolMgr Size:%d", pCL_PoolManager->mcfn_GetNumberOfPools()));
	pCG_menuMap=new CMsgMap<std::string,CMsgMap<std::string,NodeTree*>*> ();
	pCG_Smpp= new CSmpp(CG_Cfg.mcfn_getModuleId());
	pCG_CdrClient = new CCdrClient();
	pCG_CdrClient->mcfn_addTcpDetails(CG_Cfg.mcfn_getCdrIp(),CG_Cfg.mcfn_getCdrPort(),CG_Cfg.mcfn_getCdrSecIp(),CG_Cfg.mcfn_getCdrSecPort());
	CReloadHandler CL_reload(CG_Cfg.mcfn_getReloadPort());
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
