#include "Socket.h"
#include <sys/types.h>
#include <linux/unistd.h>


Socket::Socket()
{
	mesl_Error = 0x00;
	meS_Socket = -1;
	mesl_ServerPort = 0;
	meb_WRClosed = false;
	meb_RDClosed = false;
	memset(pmesc_ErrStrg,0x00,sizeof(pmesc_ErrStrg));
        memset(&meS_SockAddr,0x00,sizeof(meS_SockAddr));
}

void Socket::mcfn_init()
{
	mesl_Error = 0x00;
        meS_Socket = -1;
        mesl_ServerPort = 0;
        meb_WRClosed = false;
        meb_RDClosed = false;
	memset(pmesc_ErrStrg,0x00,sizeof(pmesc_ErrStrg));
	memset(&meS_SockAddr,0x00,sizeof(meS_SockAddr));
}

Socket::~Socket()
{
	mcfn_close();
}

bool Socket::mcfn_initialize()
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

const char* Socket::what()
{ 
#define RETS(s,m) case s: return #s": "m;
#ifdef _WIN32
#define SRETS(s,m) case s+WSABASEERR: return #s": "m;
#else
#define SRETS(s,m) RETS(s,m)
#endif

	switch (mesl_Error) {
		RETS(EWOULDBLOCK,"Resource temporarily unavailable")
			RETS(EINPROGRESS,"Operation now in progress")
			RETS(EALREADY,"Operation already in progress")
			RETS(ENOTSOCK,"Socket operation on nonsocket")
			RETS(EDESTADDRREQ,"Destination address required")
			RETS(EMSGSIZE,"Message too long")
			RETS(EPROTOTYPE,"Protocol wrong type for socket")
			RETS(ENOPROTOOPT,"Bad protocol option")
			RETS(EPROTONOSUPPORT,"Protocol not supported")
			RETS(ESOCKTNOSUPPORT,"Socket type not supported")
			RETS(EOPNOTSUPP,"Operation not supported")
			RETS(EPFNOSUPPORT,"Protocol family not supported")
			RETS(EAFNOSUPPORT,"Address family not supported by protocol family")
			RETS(EADDRINUSE,"Address already in use")
			RETS(EADDRNOTAVAIL,"Cannot assign requested address")
			RETS(ENETDOWN,"Network is down")
			RETS(ENETUNREACH,"Network is unreachable")
			RETS(ENETRESET,"Network dropped connection on reset")
			RETS(ECONNABORTED,"Software caused connection abort")
			RETS(ECONNRESET,"Connection reset by peer")
			RETS(ENOBUFS,"No buffer space available")
			RETS(EISCONN,"Socket is already connected")
			RETS(ENOTCONN,"Socket is not connected")
			RETS(ESHUTDOWN,"Cannot send after socket shutdown")
			RETS(ETIMEDOUT,"Connection timed out")
			RETS(ECONNREFUSED,"Connection refused")
			RETS(EHOSTDOWN,"Host is down")
			RETS(EHOSTUNREACH,"No route to host")
			SRETS(EINTR,"Operation interrupted")
			SRETS(EBADF,"Bad file number")
			SRETS(EACCES,"Permission denied")
			SRETS(EFAULT,"Bad address")
			SRETS(EINVAL,"Invalid argument")
			SRETS(EMFILE,"Too many files opened")
	}
#undef RETS
#undef SRETS
	return "Unknown Error";
}

bool Socket::mcfn_create(int slL_Port, bool bL_IsKeepAlive)
{
	mesl_ServerPort = slL_Port;
	meS_SockAddr.sin_family = AF_INET;
	meS_SockAddr.sin_port= htons(mesl_ServerPort);
	meS_SockAddr.sin_addr.s_addr= INADDR_ANY;
	if((meS_Socket = socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
		mesl_Error = SOCERRNO;
		return false;
	}
	int option_value = 1;
	if(bL_IsKeepAlive){
		if(setsockopt(meS_Socket,SOL_SOCKET,SO_KEEPALIVE,(char *)&option_value,sizeof(option_value)) < 0)
		{
			mesl_Error = SOCERRNO;
			return false;
		}
	}
	return true;
}

bool Socket::mcfn_create(int slL_Port,char * pscL_IP, bool bL_IsKeepAlive)
{
	mesl_ServerPort = slL_Port;
	meS_SockAddr.sin_family = AF_INET;
	meS_SockAddr.sin_port= htons(mesl_ServerPort);
	meS_SockAddr.sin_addr.s_addr= inet_addr(pscL_IP);
	if((meS_Socket = socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
		mesl_Error = SOCERRNO;
		return false;
	}
	int option_value = 1;
	if(bL_IsKeepAlive){
		if(setsockopt(meS_Socket,SOL_SOCKET,SO_KEEPALIVE,(char *)&option_value,sizeof(option_value)) < 0)
		{
			mesl_Error = SOCERRNO;
			return false;
		}
	}
	return true;
}

bool Socket::mcfn_accept(Socket &CL_STSSocket)
{
#ifdef __LINUX__
	int slL_AddrLen = sizeof(CL_STSSocket.meS_SockAddr);
	if((CL_STSSocket.meS_Socket = accept(meS_Socket, (struct sockaddr *)&CL_STSSocket.meS_SockAddr,	(socklen_t*)&slL_AddrLen)) < 0)
	{
		mesl_Error = SOCERRNO;
		return false;
	}
#else
	int slL_AddrLen = sizeof(sockaddr_in);
	if((CL_STSSocket.meS_Socket = accept(meS_Socket, (struct sockaddr *)&CL_STSSocket.meS_SockAddr,	&slL_AddrLen)) == INVALID_SOCKET)
	{
		mesl_Error = SOCERRNO;
		return false;
	}
#endif
	return true;
}


bool Socket::mcfn_receive(char *psl_Buffer,int slL_TotLen,bool bL_block)
{
	int slL_NoByte = 0;
	int slL_Len = 0;

	int siL_ReturnValue = 0x00;
	/* ussing poll */
	struct pollfd fds[2];
	int timeout_msecs = 50;
	/* using poll end*/

	while(1)
	{
#ifdef __LINUX__
		if(meS_Socket == -1)
		{
			mesl_Error = ECONNRESET;
			return false;
		}

		/* using poll */
		fds[0].fd = meS_Socket;	
		fds[0].events = POLLIN|POLLRDNORM|POLLRDBAND|POLLPRI|POLLERR|POLLHUP|POLLNVAL;
		fds[0].revents = POLLIN|POLLRDNORM|POLLRDBAND|POLLPRI|POLLERR|POLLHUP|POLLNVAL;
		siL_ReturnValue = poll(fds,1,timeout_msecs);

		/* using poll end */
		if(meb_WRClosed || (meS_Socket == -1))
		{
			mesl_Error = ECONNRESET;
			return false;
		}
		if((siL_ReturnValue== -1)&& ((SOCERRNO == EINTR)||(SOCERRNO == EAGAIN)))//signal interupt 
				continue;	
		if((siL_ReturnValue== -1)&& ((SOCERRNO == EBADF)||(SOCERRNO == EINVAL)))//bad socket descriptor
		{
			mesl_Error = ECONNRESET;
			return false;
			
		}
		if(siL_ReturnValue== 0)//timeout
			continue;	
		if((fds[0].revents & POLLERR)||(fds[0].revents & POLLHUP)||(fds[0].revents & POLLNVAL))
		{
			mesl_Error = ECONNRESET;
                        return false;	
		}
		if((slL_NoByte=recv(meS_Socket,psl_Buffer,slL_TotLen - slL_Len, MSG_NOSIGNAL)) > 0) 
#else
			if((slL_NoByte=recv(meS_Socket,psl_Buffer,slL_TotLen - slL_Len, 0)) > 0) 
#endif
			{
				slL_Len += slL_NoByte;
				if(slL_Len != slL_TotLen)
					psl_Buffer += slL_NoByte;
				else
					return true;
			}
			else if(slL_NoByte == 0)
			{
				mesl_Error = ECONNRESET;
				return false;
			}
			else
			{
				mesl_Error = SOCERRNO;
				if (mesl_Error == EWOULDBLOCK) 
				{
					if(bL_block)
					{
						usleep(1000);
						continue;
					}
					else
						return false;
				}
				else
					return false;
			}
	}
}


int Socket::mbfn_getIpAddress(char *buf, size_t buflen)
{
	char *ipAddress = inet_ntoa( meS_SockAddr.sin_addr );
	mesl_Error = STS_SUCCESS;
	if ( ipAddress )
	{
		size_t len = strlen( ipAddress );
		if ( buflen > len )
			strcpy( buf, ipAddress );
		return STS_SUCCESS;
	}
	else
		mesl_Error = STS_ERR_INV_IPADDR;

	return STS_FAILURE;
}
int Socket::mbfn_getIPAndPort(char *buf, size_t buflen)
{
	char *ipAddress = inet_ntoa( meS_SockAddr.sin_addr );
	int siL_Port = ntohs( meS_SockAddr.sin_port );
	mesl_Error = STS_SUCCESS;
	if ( ipAddress )
	{
		size_t len = strlen(ipAddress) + 4;
		if ( buflen > len )
			sprintf(buf,"%s-%d",ipAddress,siL_Port);
		return STS_SUCCESS;
	}
	else
		mesl_Error = STS_ERR_INV_IPADDR;

	return STS_FAILURE;
	
}
int Socket::mbfn_getPort()
{
	int siL_Port = ntohs( meS_SockAddr.sin_port );
	return siL_Port;
}

#ifdef __LINUX__
bool Socket::mcfn_connect(char *pscL_IP, int siL_Timeout){
        bool bL_Return = true;
        meS_SockAddr.sin_addr.s_addr = inet_addr(pscL_IP);
        int             siL_Flags, siL_Ret=0, siL_Error=0;
        socklen_t       len;
        fd_set          rset, wset;
        struct timeval  tval;

        siL_Flags = fcntl(meS_Socket, F_GETFL, 0);
        fcntl(meS_Socket, F_SETFL, siL_Flags | O_NONBLOCK);

        try{
                siL_Error = 0;
                puts("lib:connecting");
                if ( (siL_Ret = connect(meS_Socket, (struct sockaddr *)&meS_SockAddr, sizeof(meS_SockAddr))) < 0){
                        if (errno != EINPROGRESS){
                          throw 1;
                        }
                }

                if (siL_Ret ==0 ){
                        //Connected immediately reset the socket fd flags and return true;
                        puts("lib:immediately connected");
                        throw 0;
                }

                FD_ZERO(&rset);
                FD_SET(meS_Socket, &rset);
                wset = rset;
                tval.tv_sec = siL_Timeout;
                tval.tv_usec = 0;
                puts("lib:selecting");
                if ( (siL_Ret = select(meS_Socket+1, &rset, &wset, NULL, siL_Timeout ? &tval : NULL)) <= 0) {
                        puts("Select: timeout");
                        errno = ETIMEDOUT;
                        throw 1;
                }
                puts("lib:checking is fdset");
                if (/*FD_ISSET(meS_Socket, &rset) || */FD_ISSET(meS_Socket, &wset)) {
                        puts("lib:FD_ISSET true");
                        len = sizeof(siL_Error);
                        if (getsockopt(meS_Socket, SOL_SOCKET, SO_ERROR, &siL_Error, &len) < 0){
                                throw 1;
                        }
                        if(siL_Error){
                                errno = siL_Error;
                                throw 1;
                        }
                }else{
                        //select returned +ive value but FD_ISSET is false : very rare condition need to return false;
                        puts("lib:FD_ISSET false but select was success");
                        throw 1;
                }
        }catch(int e){
                if(e==0){
                        puts("lib: Connect success");
                        bL_Return = true;
                }else{
                        puts("lib: Connect failed");
                        bL_Return = false;
                }

        }
        //restore the fd flags back
        fcntl(meS_Socket, F_SETFL, siL_Flags);
        if (!bL_Return) {
                mesl_Error = errno;
        }
        return bL_Return;
}
#endif
