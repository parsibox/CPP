#ifndef _STSSOCKET_H
#define _STSSOCKET_H

#ifdef __LINUX__

#include <error.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>


#define  SOCKET 	int
#define  SOCKET_ERROR 	-1
#define  INVALID_SOCKET (SOCKET)(~0)

#else
#include <winsock.h>
#include "stdio.h"
#endif

#define SD_BOTH         0x02


class CSTSSocket
{

private:
	SOCKET meS_Socket;
	int mesl_ServerPort;
	sockaddr_in meS_SockAddr;
	int mesl_Error;

public:
	CSTSSocket();
	~CSTSSocket();

	SOCKET mcfn_GetSocketDescriptor();
	bool mcfn_create(int);
	static long mcfn_getLastError();
	long mcfn_getLastErrorCode();
	bool mcfn_bind();
	bool mcfn_listen(int i);
	bool mcfn_accept(CSTSSocket &);
	bool mcfn_receive(char *,int);
	int  mcfn_recvAvail(char *,int);
	bool mcfn_send(const char *,int);
	bool mcfn_connect(char *);
	bool mcfn_recvByte(char *);
	bool mcfn_close();
	
	static bool mcfn_initialize();
public:
	int mbfn_getIpAddress(char *, size_t );
};


inline
SOCKET CSTSSocket::mcfn_GetSocketDescriptor()
{
	return meS_Socket;
}


       	
inline
bool CSTSSocket::mcfn_close()
{
	shutdown(meS_Socket,SD_BOTH);

#ifdef __LINUX__
	if(close(meS_Socket) == 0)
#else
	if(closesocket(meS_Socket) == 0)
#endif
		return true;
	return false;
}

inline
long CSTSSocket::mcfn_getLastError()
{
#ifdef __LINUX__
	return -1;

#else
	return WSAGetLastError();
#endif
}

inline
long CSTSSocket::mcfn_getLastErrorCode()
{
#ifdef __LINUX__
	return mesl_Error;

#else
	return WSAGetLastError();
#endif
}

inline
bool CSTSSocket::mcfn_bind()
{

	int option_value = 1;  
        if (setsockopt(meS_Socket,SOL_SOCKET, SO_REUSEADDR, (char *)&option_value,
           sizeof(option_value)) < 0)
        {
                //error("setsockopt");
                return false;
        }

	if(bind(meS_Socket, (struct sockaddr *)&meS_SockAddr, sizeof(meS_SockAddr)) == SOCKET_ERROR )
	{
	    	mesl_Error = errno;
		return false;
	}
	return true;
/*	if(bind(meS_Socket, (struct sockaddr *)&meS_SockAddr, sizeof(meS_SockAddr)) == SOCKET_ERROR )
	{
	    	mesl_Error = errno;
		return false;
	}
	return true;))*/
}

inline
int CSTSSocket::mcfn_recvAvail(char *pscL_Buffer,int slL_Len)
{
	return recv(meS_Socket,pscL_Buffer,slL_Len, 0);
}

inline
bool CSTSSocket::mcfn_listen(int i)
{
	if(listen(meS_Socket,i) == SOCKET_ERROR)
	{
	    mesl_Error = errno;
	    return false;
	}
	return true;
}

inline
bool CSTSSocket::mcfn_recvByte(char *pscL_Buffer)
{
	if(recv(meS_Socket,pscL_Buffer,1, 0) > 0) 
		return true;
	return false;
}

inline
bool CSTSSocket::mcfn_connect(char *pscL_IP)
{
	meS_SockAddr.sin_addr.s_addr = inet_addr(pscL_IP);
	if(connect(meS_Socket, (struct sockaddr *)&meS_SockAddr, sizeof(meS_SockAddr)) == SOCKET_ERROR)
	{
	    	mesl_Error = errno;
		return false;
	}
	return true;
}

inline
bool CSTSSocket::mcfn_send(const char *psl_Buffer,int slL_Len)
{
	int l_returnSend;
#ifdef __LINUX__
	l_returnSend = send(meS_Socket,psl_Buffer,slL_Len,MSG_NOSIGNAL);
#else
	l_returnSend = send(meS_Socket,psl_Buffer,slL_Len,0);
#endif
	if(l_returnSend == SOCKET_ERROR)
	{
	    mesl_Error = errno;
		return false;
	}
	return true;
}

#endif
