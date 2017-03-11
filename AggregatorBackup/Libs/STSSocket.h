/**
 * SixDEE Telecom Solutions Pvt. Ltd.
 * Copyright 2006
 * All Rights Reserved.
 */

#ifndef _STSSOCKET_H
#define _STSSOCKET_H

#ifdef __LINUX__

#include <error.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include "memory.h"
#include "STSOSUtil.h"
#include <stdio.h>
#include "CriticalSection.h"
#include <stropts.h>
#include <poll.h>
#include <sys/types.h>
#include <fcntl.h>

#define  SOCKET 	int
#define  SOCERRNO	errno
#define  SOCKET_ERROR 	-1
#define  INVALID_SOCKET (SOCKET)(~0)
#ifndef __LINUX__
#define MSG_NOSIGNAL    0
#endif

#else
#include <winsock.h>
	#define EWOULDBLOCK             WSAEWOULDBLOCK
	#define EINPROGRESS             WSAEINPROGRESS
	#define EALREADY                WSAEALREADY
	#define ENOTSOCK                WSAENOTSOCK
	#define EDESTADDRREQ            WSAEDESTADDRREQ
	#define EMSGSIZE                WSAEMSGSIZE
	#define EPROTOTYPE              WSAEPROTOTYPE
	#define ENOPROTOOPT             WSAENOPROTOOPT
	#define EPROTONOSUPPORT         WSAEPROTONOSUPPORT
	#define ESOCKTNOSUPPORT         WSAESOCKTNOSUPPORT
	#define EOPNOTSUPP              WSAEOPNOTSUPP
	#define EPFNOSUPPORT            WSAEPFNOSUPPORT
	#define EAFNOSUPPORT            WSAEAFNOSUPPORT
	#define EADDRINUSE              WSAEADDRINUSE
	#define EADDRNOTAVAIL           WSAEADDRNOTAVAIL
	#define ENETDOWN                WSAENETDOWN
	#define ENETUNREACH             WSAENETUNREACH
	#define ENETRESET               WSAENETRESET
	#define ECONNABORTED            WSAECONNABORTED
	#define ECONNRESET              WSAECONNRESET
	#define ENOBUFS                 WSAENOBUFS
	#define EISCONN                 WSAEISCONN
	#define ENOTCONN                WSAENOTCONN
	#define ESHUTDOWN               WSAESHUTDOWN
	#define ETOOMANYREFS            WSAETOOMANYREFS
	#define ETIMEDOUT               WSAETIMEDOUT
	#define ECONNREFUSED            WSAECONNREFUSED
	#define ELOOP                   WSAELOOP
	#define EHOSTDOWN               WSAEHOSTDOWN
	#define EHOSTUNREACH            WSAEHOSTUNREACH
	#define EPROCLIM                WSAEPROCLIM
	#define EUSERS                  WSAEUSERS
	#define EDQUOT                  WSAEDQUOT
	#define ESTALE                  WSAESTALE
	#define EREMOTE                 WSAEREMOTE
	typedef int socklen_t;
	#define SOCERRNO				h_errno
	#define SD_RECV			0
	#define SD_SEND			1
	#define SD_BOTH			2

#endif


//shutdown socket direction
#define SD_RECV				SHUT_RD
#define SD_SEND				SHUT_WR
#define SD_BOTH				SHUT_RDWR

#define STS_SUCCESS			0
#define STS_FAILURE			-1
#define STS_ERR_INV_IPADDR 20


class CSTSSocket
{

private:
	SOCKET meS_Socket;
	bool meb_WRClosed;
	bool meb_RDClosed;
	int mesl_ServerPort;
	sockaddr_in meS_SockAddr;
	int mesl_Error;
	char pmesc_ErrStrg[1024];

public:
	CSTSSocket();
	~CSTSSocket();
	void mcfn_init();
	SOCKET mcfn_GetSocketDescriptor();
	bool mcfn_setSocketDescriptor(SOCKET);
	bool mcfn_create(int, bool bL_IsKeepAlive=true);
	bool mcfn_create(int ,char *, bool bL_IsKeepAlive=true);
	long mcfn_getLastError();
	char* mcfn_getErrStrg();
	long mcfn_getLastErrorCode();
	bool mcfn_bind(bool = true,bool = true,bool =true);
	bool mcfn_listen(int i);
	bool mcfn_accept(CSTSSocket &);
	bool mcfn_receive(char *,int,bool bL_block=true);
	int  mcfn_recvAvail(char *,int);
	bool mcfn_send(const char *,int);
	bool mcfn_send(const char *,int ,int );
	bool mcfn_connect(char *);
#ifdef __LINUX__
	bool mcfn_connect(char *, int siL_TimeOut);
#endif
	bool mcfn_recvByte(char *);
	bool mcfn_close();
	bool mcfn_closeByDirection(int = SD_BOTH);
	int mcfn_isDataAvailable(int siL_TimeOut);
	int mbfn_getIpAddress(char *,size_t );
	int mbfn_getIPAndPort(char *,size_t );
	int mbfn_getPort();
	static bool mcfn_initialize();
	const char* what();

	CCriticalSection meC_LockObj;
};


inline
SOCKET CSTSSocket::mcfn_GetSocketDescriptor()
{
	return meS_Socket;
}
inline
bool CSTSSocket::mcfn_setSocketDescriptor(SOCKET SL_Socket)
{
	meS_Socket = SL_Socket;
	return true;
}
inline
bool CSTSSocket::mcfn_closeByDirection(int siL_CloseType)
{
	if(meS_Socket == -1)
		return false;
	
	if(siL_CloseType == SHUT_WR)
	{
		if(!meb_WRClosed)
		{
			shutdown(meS_Socket,siL_CloseType);
			meb_WRClosed = true;
		}
	}
	else if(siL_CloseType == SHUT_RD)
	{
		if(!meb_RDClosed)
		{
			shutdown(meS_Socket,siL_CloseType);
			meb_RDClosed = true;
		}
	}
	else if(siL_CloseType == SHUT_RDWR)
	{
		if(!meb_WRClosed)
		{
			shutdown(meS_Socket,SHUT_WR);
			meb_WRClosed = true;
		}
		if(!meb_RDClosed)
		{
			shutdown(meS_Socket,SHUT_RD);
			meb_RDClosed = true;
		}
	}
	else
		return false;
	

	return true;
}

inline
bool CSTSSocket::mcfn_close()
{
	if(meS_Socket == -1)
		return false;

	shutdown(meS_Socket,SHUT_RDWR);

#ifdef __LINUX__
	if(close(meS_Socket) == 0)
#else
	if(closesocket(meS_Socket) == 0)
#endif
	meS_Socket = -1;

	return true;
}

inline
long CSTSSocket::mcfn_getLastError()
{
#ifdef __LINUX__
	return mesl_Error;

#else
	return WSAGetLastError();
#endif
}
inline
char* CSTSSocket::mcfn_getErrStrg()
{
	memset(pmesc_ErrStrg,0x00,sizeof(pmesc_ErrStrg));
	sprintf(pmesc_ErrStrg,"[%d]-%s-%s",mesl_Error,strerror(errno),what());
	return pmesc_ErrStrg;
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
bool CSTSSocket::mcfn_bind(bool bL_ReuseAddr,bool bL_KeppAlive,bool bL_NoDelay)
{
	int option_value = 1;
	if(bL_ReuseAddr)
	{/*Enable address resuse */
		if(setsockopt(meS_Socket,SOL_SOCKET,SO_REUSEADDR,(char *)&option_value,sizeof(option_value)) < 0)
		{
			mesl_Error = SOCERRNO;
			return false;
		}
	}
	if(bL_KeppAlive)
	{/* Enable TCP keep alive */
		if(setsockopt(meS_Socket,SOL_SOCKET,SO_KEEPALIVE,(char *)&option_value,sizeof(option_value)) < 0)
		{
			mesl_Error = SOCERRNO;
			return false;
		}
	}
	if(bL_NoDelay)
	{/* Disable the Nagle (TCP No Delay) algorithm */
		if(setsockopt(meS_Socket,IPPROTO_TCP,TCP_NODELAY,(char *)&option_value,sizeof(option_value)) < 0)
		{
			mesl_Error = SOCERRNO;
			return false;
		}
	}
	if(bind(meS_Socket,(struct sockaddr *)&meS_SockAddr, sizeof(meS_SockAddr)) != 0 )
	{
		mesl_Error = SOCERRNO;
		return false;
	}
	return true;
}

inline
int CSTSSocket::mcfn_recvAvail(char *pscL_Buffer,int slL_Len)
{
	return recv(meS_Socket,pscL_Buffer,slL_Len, 0);
}

inline
bool CSTSSocket::mcfn_listen(int i)
{
	if(listen(meS_Socket,i) != 0)
	{
	    mesl_Error = SOCERRNO;
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
	//fcntl(meS_Socket,F_SETFD,O_NONBLOCK);
	if(connect(meS_Socket, (struct sockaddr *)&meS_SockAddr, sizeof(meS_SockAddr)) != 0)
	{
	    	mesl_Error = SOCERRNO;
		return false;
	}
	return true;
}

inline
bool CSTSSocket::mcfn_send(const char *psl_Buffer,int slL_Len)
{
	int l_returnSend;
	
	meC_LockObj.Lock();
	if(meS_Socket == -1)
	{
		mesl_Error = ENOTCONN;
		meC_LockObj.Unlock();
		return false;
	}
#ifdef __LINUX__
        l_returnSend = send(meS_Socket,psl_Buffer,slL_Len,MSG_NOSIGNAL);
#else
        l_returnSend = send(meS_Socket,psl_Buffer,slL_Len,0);
#endif
	meC_LockObj.Unlock();

        if(l_returnSend == 0)
        {
            mesl_Error = ECONNRESET;
            return false;
        }
	else if (l_returnSend < 0)
	{
            mesl_Error = SOCERRNO;
            return false;
	}
	else
		return true;
}
inline
bool CSTSSocket::mcfn_send(const char *psl_Buffer,int slL_Len,int siL_Flag)
{
	int l_returnSend;

	meC_LockObj.Lock();

	if(meS_Socket == -1)
	{
		mesl_Error = ENOTCONN;
		meC_LockObj.Unlock();
		return false;
	}
#ifdef __LINUX__
        l_returnSend = send(meS_Socket,psl_Buffer,slL_Len,MSG_NOSIGNAL|siL_Flag);
#else
        l_returnSend = send(meS_Socket,psl_Buffer,slL_Len,0);
#endif
	meC_LockObj.Unlock();

        if(l_returnSend == 0)
        {
            mesl_Error = ECONNRESET;
            return false;
        }
	else if (l_returnSend < 0)
	{
            mesl_Error = SOCERRNO;
            return false;
	}
	else if(l_returnSend < slL_Len)
	{
		mesl_Error = ENOBUFS;
		mcfn_closeByDirection(SHUT_WR);
		return false;
	}
	else
		return true;
}

#endif
