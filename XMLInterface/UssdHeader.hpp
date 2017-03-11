#ifndef _USSD_HEADER_H
#define _USSD_HEADER_H
#include<string>
#include <sstream>

#define HEADER_LENGTH 4

typedef unsigned char u8;
class CHeader
{
	public:	
		short int mcsi_cmdSessionId;
		short int mcsi_cmdLength;
		CHeader(int iL_cmdLen,int iL_SessionId){
			mcsi_cmdLength=iL_cmdLen+4;
			mcsi_cmdSessionId=iL_SessionId;
		}
		CHeader(const CHeader &CL_copy){
			mcsi_cmdLength=CL_copy.mcsi_cmdLength;
			mcsi_cmdSessionId=CL_copy.mcsi_cmdSessionId;
		}
		CHeader(){}
		~CHeader(){}
		void mcfn_setHeader(u8* u8L_header){
			memcpy(&mcsi_cmdSessionId,&u8L_header[2],2);
			memcpy(&mcsi_cmdLength,&u8L_header[0],2);
		}
		u8* mcfn_getHeader(u8* u8L_header){
			memcpy(&u8L_header[2],&mcsi_cmdSessionId,2);
			memcpy(&u8L_header[0],&mcsi_cmdLength,2);
			return u8L_header;
		}
		std::string mcfn_ShortDebugString(){
			std::stringstream CL_bufStr;
			CL_bufStr.str("");
			CL_bufStr<<"cmdLength:"<<mcsi_cmdLength<<",SessionId:"<<mcsi_cmdSessionId ;
			return  CL_bufStr.str();
		}
		std::string mcfn_getHeaderPrintable(){
			return mcfn_ShortDebugString();
		}	
};
#endif

