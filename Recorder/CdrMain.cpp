#include"CdrServer.hpp"
#include"signalHandler.h"
#include"EventLog.h"
#include"WriteFile.h"
#include"sstream"
#define REL_VER "V1.0"
#define RELEASE_DATE "05-Apr-2014"

CWriteFile CG_SubmitWriter;
CWriteFile CG_DeliverWriter;
std::string CG_columnSep="|";

CEventLog CG_EventLog;
CMsgQue<CdrMsg*> CG_ReceiverQ;

CCdrServer *pCG_CdrServer=NULL;
static void fnG_PrintCompanyBanner(char* pscL_ModuleName,char* pscL_Version)
{
        printf("******************************************************************\n");
        printf("SmppClient- (All rights reserved)\n");
        printf(" %s--Version: %s\n", pscL_ModuleName,REL_VER);
        printf("Release date :%s                                        \n",RELEASE_DATE);
        printf("******************************************************************\n");
}

void* fnG_processCdrMsgsThread(void *p){
	
	DBG_INFO((CG_EventLog),("Started fnG_processCdrMsgsThread "));
	while(1){
		try{	CdrMsg *pCL_Msg=NULL;
			if(!CG_ReceiverQ.mcfb_getNextMessage(pCL_Msg,true)){
				sleep(1);
				continue;
			}
			if(pCL_Msg->msg_type()==CdrMsg::SUBMIT){
				SubmitMsg *pCL_SubMsg=pCL_Msg->mutable_submit_msg();
				//TODO::here need to genetate submit cdr
				std::stringstream CL_Msg;
				CL_Msg.str("");
				CL_Msg<<pCL_SubMsg->task_id()<<CG_columnSep<<pCL_SubMsg->msg_id()<<CG_columnSep<<pCL_SubMsg->oa()<<CG_columnSep<<pCL_SubMsg->da()
				      <<CG_columnSep<<pCL_SubMsg->msg()<<CG_columnSep<<pCL_SubMsg->interface_id()<<CG_columnSep<<pCL_SubMsg->reg_del()<<CG_columnSep
				      <<pCL_SubMsg->circle_id()<<CG_columnSep<<pCL_SubMsg->channel_id()<<CG_columnSep<<pCL_SubMsg->expiry_time()<<CG_columnSep
				      <<pCL_SubMsg->pid()<<CG_columnSep<<pCL_SubMsg->dcs()<<CG_columnSep<<pCL_SubMsg->lang_id()<<CG_columnSep<<pCL_SubMsg->status()
				      <<CG_columnSep<<pCL_SubMsg->submit_time()<<CG_columnSep<<pCL_SubMsg->submit_msgid();
				if(!CG_SubmitWriter.mcfn_AddStringToFile(CL_Msg.str(),pCL_SubMsg->submit_time())){
					CG_ReceiverQ.mcfb_insertIntoQue(pCL_Msg);
					continue;
				}
				DBG_VERBOSE((CG_EventLog),("Generated Submit CDR Sucessfully:%s",pCL_Msg->ShortDebugString().c_str()));
			}
			else{
				//TODO ::here need to genearate deliver CDR
				std::stringstream CL_Msg;
				CL_Msg.str("");
				DeliverMsg *pCL_DlrMsg=pCL_Msg->mutable_deliver_msg();
				CL_Msg<<pCL_DlrMsg->submit_msgid()<<CG_columnSep<<pCL_DlrMsg->submit_time()<<CG_columnSep<<pCL_DlrMsg->status()<<CG_columnSep<<pCL_DlrMsg->delivery_time()<<CG_columnSep<<pCL_DlrMsg->error_code();
				if(!CG_DeliverWriter.mcfn_AddStringToFile(CL_Msg.str(),pCL_DlrMsg->submit_time())){
					CG_ReceiverQ.mcfb_insertIntoQue(pCL_Msg);
					continue;
				}
				DBG_VERBOSE((CG_EventLog),("Generated Deliver CDR Sucessfully:%s",pCL_Msg->ShortDebugString().c_str()));
			}
			delete pCL_Msg;
		}
		catch(CSignalException e){
                        DBG_ERROR((CG_EventLog),("Exception:%s",e.what()));
                }
                catch(...){
                        DBG_ERROR((CG_EventLog),("Some unknown Exception came in main"));
                }
	}
	DBG_INFO((CG_EventLog),("Exiting From fnG_processCdrMsgsThread"));
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
	pCG_CdrServer= new CCdrServer(9898);
	//TODO::need to init Write  file objects
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
