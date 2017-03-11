#include"TcpServer.h"

struct SParam{
	TcpServer* pmcC_Obj;
	Socket * pmcC_Sock;
	SParam(TcpServer* pCL_Obj, Socket* pCL_Sock){
		pmcC_Obj  = pCL_Obj;
		pmcC_Sock = pCL_Sock;
	}
};

static void* fnG_Wrapper_ServerClientHandlerThread(void *pvL_Param){
	try{
		SParam SL_Param = *(static_cast<SParam*>(pvL_Param));
		SL_Param.pmcC_Obj->StartClientThread(SL_Param.pmcC_Sock);
	}catch(...){
		printf( ("Bad cast while starting StartClientThread"));
	}
	return NULL;
}
static void* fnG_Wrapper_ThStartServerThread(void *pvL_This) {
	try {

		(static_cast<TcpServer*> (pvL_This))->StartServerThread();
	}catch(...) {
		printf(("Unable to start connection Thread,Reason:Bad Cast"));
	}
	return NULL; 
}              
/*
bool TcpServer::receiveFromTcp(Socket *pCL_Socket){
	int ulL_RecvLen = 0x00;
	if(!pCL_Socket->receive((char*)&ulL_RecvLen, sizeof(ulL_RecvLen)))
	{
		printf("%s Socket Disconnected : %d\n",pCL_Socket->what(),getLastErrorCode());
		return false;
	}
	if(ulL_RecvLen>10000){
		printf("Packet length is coming more that 10000 disconnecting the %s socket\n",pCL_Socket->what());
		return false;	
	}
	char *pscL_Bytes = new char[ulL_RecvLen];
	if(!pCL_Socket->receive(pscL_Bytes,ulL_RecvLen)){
		printf("%s Socket Disconnected : %d\n",pCL_Socket->what(),getLastErrorCode());
		delete []pscL_Bytes;	
		return false;	
	}
	std::string CL_receBytes="";
	CL_receBytes.assign(pscL_Bytes,ulL_RecvLen);
	delete []pscL_Bytes;
	//TODO::return onReceiveData(CL_receBytes,ulL_RecvLen,pCL_Socket);
}
*/
TcpServer::TcpServer(int iL_port,std::string CL_ip)
{
	if(setServerPort(iL_port,CL_ip))
	{
		Thread::beginAndDetachThread(serverThreadHandle,fnG_Wrapper_ThStartServerThread,this);
	}
	else
	{
		printf("port number is out of range :[%s]%d\n",CL_ip.c_str(),iL_port);
		exit(0);
	}
}

TcpServer::~TcpServer()
{
	if(serverThreadHandle){
		int result = pthread_cancel(serverThreadHandle);
		printf("Exited the thread with status:%d\n",result);
	}
}

void TcpServer::StartServerThread(){
	printf("StartServerThread started...\n");
	while(true){
		try{
			while(1)
			{	
				mcfn_close();
				if(tcpIp.empty()){
					if(!mcfn_create(tcpPort)){
						printf("Error in creating socket :%d\n",tcpPort);
						sleep(2);
						continue;
					}
				}
				else{
					if(!mcfn_create(tcpPort,(char*)tcpIp.c_str())){
						printf("Error in creating socket %s:%d\n",tcpIp.c_str(),tcpPort);
						sleep(2);
						continue;
					}   
				}
				if(!mcfn_bind()){
					printf("Error in binding on port %s:%d\n",tcpIp.c_str(),tcpPort);
					sleep(2);
					continue;
				}
				break;
			}
			mcfn_listen(5);
			while(true){
				Socket *pCL_Conn = new Socket;
				if(!mcfn_accept(*pCL_Conn)){
					printf("Accept failed \n");
					mcfn_close();
					delete pCL_Conn;
					break;
				}else{
					SParam SL_Param(this, pCL_Conn);
					Thread::beginAndDetachThread(fnG_Wrapper_ServerClientHandlerThread, &SL_Param);
				}
			}
		}
		catch(...){
			printf("Some unknown Exception came in StartServer Thread\n");
		}

	}
	printf(("StartServerThread down"));
}

void TcpServer::StartClientThread(Socket *pCL_Socket){
	bool is_needToClose=false;
	printf("new Clinet connected\n");
	while(1){
		try{
			if(receiveFromTcp(pCL_Socket)==false){
				if(pCL_Socket!=NULL){
					pCL_Socket->mcfn_close();
					delete pCL_Socket;
				}
				break;
			}
		}
		catch(...){
			printf("Some unknown Exception came in Clinet Thread\n");
		}

	}	
	printf("Clinet  Disconnected\n");
}

