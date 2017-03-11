#ifndef __XML_INTERFACE__
#define __XML_INTERFACE__
#include<EsmeServer.hpp>

class xmlInterface: public CEsmeServer
{
	private :
		int mefn_getCommandLen(int iL_cmd);
		int  mefn_encodeOperationType(UssdData::operation_type eL_opType);
		int mefn_decodeOperationType(int iL_operation);
		std::string  mefn_encodeMessageType(UssdData::Msg_Type eL_msgType);
		int mefn_decodeMessageType(std::string opType);
		std::string mefn_convertString(int);
	public:
                xmlInterface(int iL_port);
                ~xmlInterface();
		//pure virtual function 
		bool mcfn_encode(UssdData&,std::string &);
		bool mcfn_decode(std::string &,UssdData&);
                int mcfn_getDataLen(u8* );
};
#endif
