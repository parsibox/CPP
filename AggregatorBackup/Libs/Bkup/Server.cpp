#include "Server.h"
#include "STSOSUtil.h"
static FNVOID fnG_WrapperServerThread(void *pvL_This){
	if(!pvL_This){
		 DBG_ERROR((CG_EventLog),(" #####  Param required,Not starting the process  #####"));
	}else{
		((CServer*)pvL_This)->mcfn_ServerThread();
	}
	RETURN;
}

static FNVOID fnG_WrapperClientHandler(void *pvL_Param){
		if(!pvL_Param){
			DBG_ERROR((CG_EventLog),(" #####  Param required,Not starting the process  #####"));
		}else{
			SClientHandlerParam CL_Param = *((SClientHandlerParam*)pvL_Param);	
			CL_Param.pmcC_This->mcfn_ClientHandler(CL_Param.pmcC_ConnSocket);
		}
		RETURN;
}

static FNVOID fnG_WrapperInternalProcessor(void *pvL_This){
	if(!pvL_This){
		DBG_ERROR((CG_EventLog),(" #####  Param required,Not starting the process  #####"));
	}else{
		((CServer*)pvL_This)->mcfn_InternalProcessor();
	}
	RETURN;
}

CServer::CServer(u16 u16L_ServerPort, u8 u8L_NumLenBytes, bool bL_StartServerProcess){
	meu16_ServerPort 	= u16L_ServerPort;
	meu8_NumOfLenBytes  = u8L_NumLenBytes;
	if(bL_StartServerProcess){
		mcfn_StartServer();
	}
}

bool CServer::mcfn_StartServer(){
	pthread_t L_Ret= CSTSOSUtil::mcfn_beginAndDetachThread(fnG_WrapperServerThread, this);
	if(L_Ret == -1){
		DBG_ERROR((CG_EventLog),("Unable to create server thread..."));
		return false;
	}
	L_Ret= CSTSOSUtil::mcfn_beginAndDetachThread(fnG_WrapperInternalProcessor, this);
	if(L_Ret == -1){
		DBG_ERROR((CG_EventLog),("Unable to create server thread..."));
		return false;
	}
	return true;
}

void CServer::mcfn_ServerThread(){
	DBG_INFO((CG_EventLog),("Server thread is started ..."));
	while(true){
		DBG_INFO((CG_EventLog),("Creating server socket"));
		while(!mcfn_create(meu16_ServerPort)){
			DBG_ERROR((CG_EventLog),("Unable to create server socket:%d",meu16_ServerPort ));
			CSTSOSUtil::mcfn_sleep(1);
		}
		while(!mcfn_bind()){
			DBG_ERROR((CG_EventLog),("Bind failed port=%d",meu16_ServerPort));
			CSTSOSUtil::mcfn_sleep(1);
		}
		mcfn_listen(5);
		DBG_INFO((CG_EventLog),("Accepting ..."));
		while(true){
			CSTSSocket *pCL_Socket = new CSTSSocket();
			if(!mcfn_accept(*pCL_Socket)){
				DBG_ERROR((CG_EventLog),("Accept failed, re-creating socket..."));
				delete pCL_Socket;
				mcfn_close();
				break;
			}else{
				DBG_INFO((CG_EventLog),("Client connected ..."));
				SClientHandlerParam SL_Param(this, pCL_Socket);
				CSTSOSUtil::mcfn_beginAndDetachThread(fnG_WrapperClientHandler, &SL_Param);
			}
		}
	}
	DBG_ERROR((CG_EventLog),("Server thread is terminated."));
}

void CServer::mcfn_ClientHandler(CSTSSocket *pCL_Conn){
	DBG_INFO((CG_EventLog),("[%u]Starting client handler...", pCL_Conn));
	while(true){
		u32 u32L_Len = 0x00;
		if(!pCL_Conn->mcfn_receive((char*)&u32L_Len, meu8_NumOfLenBytes)){
			DBG_ERROR((CG_EventLog),("[%u]Client disconnected ...",pCL_Conn));
			pCL_Conn->mcfn_close();
			delete pCL_Conn;
			return ;
		}
		DBG_VERBOSE((CG_EventLog),("[%u]Len = %d",pCL_Conn, u32L_Len));	
		u8* pu8L_Ptr = new u8[u32L_Len];
		if(!pu8L_Ptr){
			DBG_CRITICAL((CG_EventLog),("[%u]Memory overflow", pCL_Conn));
			pCL_Conn->mcfn_close();
			delete pCL_Conn;
			return;
		}	
		if(!pCL_Conn->mcfn_receive((char*)pu8L_Ptr, u32L_Len)){
			DBG_ERROR((CG_EventLog),("[%u]Client disconnected ...",pCL_Conn));
			pCL_Conn->mcfn_close();
			delete [] pu8L_Ptr;
			delete pCL_Conn;
			return ;
		}else{
			SByteStream SL_ByteStream(pu8L_Ptr,u32L_Len);
			mdC_ProcessQ.mcfb_insertIntoQue(SL_ByteStream);
		}
	}
	DBG_ERROR((CG_EventLog),("[%u]Client handler terminated.",pCL_Conn));
}

void CServer::mcfn_InternalProcessor(){
	DBG_INFO((CG_EventLog),("GenServer:Internal processor thread started"));
	while(true){
		SByteStream SL_ByteStream(NULL, 0x00);
		if(!mdC_ProcessQ.mcfb_getNextMessage(SL_ByteStream)){
			CSTSOSUtil::mcfn_sleep(1);
			continue;
		}else{
			DBG_INFO((CG_EventLog),("%u:%d",SL_ByteStream.pmcu8_Ptr, SL_ByteStream.mcu32_Len));
			if(mcfn_OnRx(SL_ByteStream)){
				DBG_VERBOSE((CG_EventLog),("Received data processed successfully"));
			}else{
				DBG_ERROR((CG_EventLog),("Failed to process data"));
				//Some thing bad has happened in OnRx function release the ByteSream to be on 
				//safe side :)
				SL_ByteStream.mcfn_Release();
				continue;
			}	
			//TODO:whether to delete the memory here or leave it to subclass to handle 
			//memory release for SByteStream
			//SL_ByteStream.mcfn_Release();
		}
	}
	DBG_ERROR((CG_EventLog),("GenServer:Internal processor thread terminated"));
}
