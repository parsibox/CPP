#include "STSSocket.h"


CSTSSocket::CSTSSocket()
{
}

CSTSSocket::~CSTSSocket()
{
	mcfn_close();
}

bool CSTSSocket::mcfn_initialize()
{
#ifdef __LINUX__
	return true;
#else
	WORD wVersionRequested;
	WSADATA wsadata;
	wVersionRequested = MAKEWORD (2,2);
	if(WSAStartup(2,&wsadata) == 0)
		return true;
	return false;
#endif
}

bool CSTSSocket::mcfn_create(int slL_Port)
{
	mesl_ServerPort = slL_Port;
	meS_SockAddr.sin_family = AF_INET;
	meS_SockAddr.sin_port= htons(mesl_ServerPort);
	meS_SockAddr.sin_addr.s_addr= INADDR_ANY;
	if((meS_Socket = socket(AF_INET,SOCK_STREAM,0)) == INVALID_SOCKET )
		return false;
	return true;
}



bool CSTSSocket::mcfn_accept(CSTSSocket &CL_STSSocket)
{
#ifdef __LINUX__
	int slL_AddrLen = sizeof(CL_STSSocket.meS_SockAddr);
	if((CL_STSSocket.meS_Socket = accept(meS_Socket, (struct sockaddr *)&CL_STSSocket.meS_SockAddr,	(socklen_t*)&slL_AddrLen)) == INVALID_SOCKET)
	{
		return false;
	}
#else
	int slL_AddrLen = sizeof(sockaddr_in);
	if((CL_STSSocket.meS_Socket = accept(meS_Socket, (struct sockaddr *)&CL_STSSocket.meS_SockAddr,	&slL_AddrLen)) == INVALID_SOCKET)
	{
		return false;
	}
#endif
	return true;
}


bool CSTSSocket::mcfn_receive(char *psl_Buffer,int slL_TotLen)
{
	int slL_NoByte = 0;
	int slL_Len = 0;
	while(1)
	{
		if((slL_NoByte=recv(meS_Socket,psl_Buffer,slL_TotLen - slL_Len, 0)) > 0) 
		{
			slL_Len += slL_NoByte;
			if(slL_Len != slL_TotLen)
				psl_Buffer += slL_NoByte;
			else
				return true;
		}
		else
		{
			
#ifdef __LINUX__

#else
			long y = WSAGetLastError();
#endif
			return false;
		}
	}
}

