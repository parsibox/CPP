#ifndef _OWN_USSD_HEADER_H
#define _OWN_USSD_HEADER_H
#include<string>
#include <sstream>
#define OWN_HEADER_LENGTH 28
typedef  unsigned char u8;
enum Operations
{
    XML_LOGIN = 0x0001,
    XML_LOGIN_RESP = 0x0011,
    XML_LOGOUT = 0x0002,
    XML_LOGOUT_RESP = 0x0012,
    XML_START = 0x0020,
    XML_CONTINUE = 0x0021,
    XML_END = 0x0022,
    XML_ABORT = 0x0030,
    XML_ENQ_LINK = 0x0040,
    XML_ENQ_LINK_RESP = 0x0041
};

class CXmlHeader
{
	public:	
		long long mcsi_sessionId[2];
		int mcsi_cmdLength;
		int mcsi_operationType; 
		int mcsi_sessionStatus;
		CXmlHeader(int iL_cmdLen,long iL_SessionId[2],int iL_opType,int iL_sessType){
			mcsi_cmdLength=iL_cmdLen ;
			mcsi_sessionId[0]=iL_SessionId[0];
			mcsi_sessionId[1]=iL_SessionId[1];
			mcsi_operationType= iL_opType;
			mcsi_sessionStatus=iL_sessType;
		}
		CXmlHeader(int iL_cmdLen,long iL_seqId)
		{
			mcsi_cmdLength=iL_cmdLen;
                        mcsi_sessionId[0]=0;
			mcsi_sessionId[1]=iL_seqId;
                         mcsi_sessionStatus = 0;
		}
		CXmlHeader(const CXmlHeader &CL_copy){
			mcsi_cmdLength=CL_copy.mcsi_cmdLength;
			mcsi_sessionId[0]=CL_copy.mcsi_sessionId[0];
			mcsi_sessionId[1]=CL_copy.mcsi_sessionId[1];
                        mcsi_operationType=CL_copy.mcsi_operationType;
                        mcsi_sessionStatus=CL_copy.mcsi_sessionStatus;
		}
		CXmlHeader(){}
		~CXmlHeader(){}
		void mcfn_setHeader(u8* u8L_header){
			memcpy(&mcsi_sessionId[0],&u8L_header[0],8);
			memcpy(&mcsi_sessionId[1],&u8L_header[8],8);
			memcpy(&mcsi_cmdLength,&u8L_header[16],4);
			memcpy(&mcsi_operationType,&u8L_header[20],4);
			memcpy(&mcsi_sessionStatus,&u8L_header[24],4);
		}
		u8* mcfn_getHeader(u8* u8L_header){
			memcpy(&u8L_header[0],&mcsi_sessionId[0],8);
			memcpy(&u8L_header[8],&mcsi_sessionId[1],8);
			memcpy(&u8L_header[16],&mcsi_cmdLength,4);
			memcpy(&u8L_header[20],&mcsi_operationType,4);
			memcpy(&u8L_header[24],&mcsi_sessionStatus,4);
			return u8L_header;
		}
		std::string mcfn_ShortDebugString(){
			std::stringstream CL_bufStr;
			CL_bufStr.str("");
			CL_bufStr<<"cmdLength:"<<mcsi_cmdLength<<",SessionId:"<<mcsi_sessionId[0]<<mcsi_sessionId[1]<<", OperationType:"<<mcsi_operationType<<", Session Staus:"<<mcsi_sessionStatus ;
			return  CL_bufStr.str();
		}
		std::string mcfn_getHeaderPrintable(){
			return mcfn_ShortDebugString();
		}	
};
#endif

