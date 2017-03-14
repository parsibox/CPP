
#ifndef _USSD_HEADER_H
#define _USSD_HEADER_H
#include<string>
#include <sstream>
#include<smpp.hpp>
 #include <arpa/inet.h>
typedef unsigned char u8;

class CHeader{
	public:	
		unsigned mcsi_cmdLength;
		unsigned mcsi_cmdId;
		unsigned mcsi_cmdStatus;
		unsigned mcsi_sessionId;
		CHeader(int iL_cmdLen=0,int iL_cmdId=0x00,int iL_cmdStatus=0x00,int iL_sessionId=0xFFFFFFFF){
			mcsi_cmdLength=iL_cmdLen;
			mcsi_cmdId=iL_cmdId;
			mcsi_cmdStatus=iL_cmdStatus;
			mcsi_sessionId=iL_sessionId;
		}
		CHeader(const CHeader &CL_copy){
			mcsi_cmdLength=CL_copy.mcsi_cmdLength;
			mcsi_cmdId=CL_copy.mcsi_cmdId;
			mcsi_cmdStatus=CL_copy.mcsi_cmdStatus;
			mcsi_sessionId=CL_copy.mcsi_sessionId;
		}
		~CHeader(){};
	        void mcfn_setHeader(u8* u8L_header){
			mcsi_cmdLength = Smpp::get_command_length((Smpp::Uint8*)u8L_header); 
			mcsi_cmdId = Smpp::get_command_id((Smpp::Uint8*)u8L_header); 
			mcsi_cmdStatus = Smpp::get_command_status((Smpp::Uint8*)u8L_header); 
			mcsi_sessionId = Smpp::get_sequence_number((Smpp::Uint8*)u8L_header); 

		}
		u8* mcfn_getHeader(u8* u8L_header){
			memcpy(&u8L_header[0],&mcsi_cmdLength,4);
			memcpy(&u8L_header[4],&mcsi_cmdId,4);
			memcpy(&u8L_header[8],&mcsi_cmdStatus,4);
			memcpy(&u8L_header[12],&mcsi_sessionId,4);
			return u8L_header;
		}
		std::string mcfn_ShortDebugString(){
			std::stringstream CL_bufStr;
			CL_bufStr.str("");
			CL_bufStr<<"cmdLength:"<<mcsi_cmdLength<<",cmdId:0x"<<std::hex<<mcsi_cmdId<<",cmdStatus:"<<mcsi_cmdStatus<<",mcsi_sessionId"<<mcsi_sessionId;
			return  CL_bufStr.str();
		}
		std::string mcfn_getHeaderPrintable(){
			return mcfn_ShortDebugString();
		}	
};
#endif

