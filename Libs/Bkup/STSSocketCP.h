#ifndef _STSSOCKET_H
#define _STSSOCKET_H

#ifdef __LINUX__

#include <error.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define  SOCKET 	int
#define  SOCKET_ERROR 	-1
#define  INVALID_SOCKET (SOCKET)(~0)

#else
#include <winsock.h>

#endif

#define SD_BOTH         0x02


class CSTSSocket
{

private:
	SOCKET meS_Socket;
	int mesl_ServerPort;
	sockaddr_in meS_SockAddr;

public:
	CSTSSocket();
	~CSTSSocket();
	bool mcfn_create(int);
	static long mcfn_getLastError();
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
};

inline
bool CSTSSocket::mcfn_close()
{
//#ifdef __LINUX__
//	if(close(meS_Socket) == 0)
//		return true;
//#else

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
bool CSTSSocket::mcfn_bind()
{
	if(bind(meS_Socket, (struct sockaddr *)&meS_SockAddr, sizeof(meS_SockAddr)) == SOCKET_ERROR )
		return false;
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
	if(listen(meS_Socket,i) == SOCKET_ERROR)
		return false;
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
		return false;
	return true;
}

inline
bool CSTSSocket::mcfn_send(const char *psl_Buffer,int slL_Len)
{
	if(send(meS_Socket,psl_Buffer,slL_Len,0) == SOCKET_ERROR )
		return false;
	return true;
}

#endif
