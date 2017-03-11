#ifndef __IPCTYPES__
#define __IPCTYPES__

#include<string>

class CHeader
{
	public:
		short src;
		short dst;
		short status;
		short len;
	public:
		CHeader()
		{
			clear();
		}
		void clear(){
			src=0;
			dst=0;
			status=0;
			len=0;
		}
};


class IPCMsg
{
	public:
		CHeader hdr;
		std::string data;	
	public:
		IPCMsg(){
			clear();
		}
		void clear()
		{
			hdr.clear();
			data.clear();
		}	
};

const int HEADER_LENGTH      = sizeof(CHeader);
const int IPC_SUCCESS	     = 0;	
const int IPC_ALREADY_BINDED = 1;
#endif
