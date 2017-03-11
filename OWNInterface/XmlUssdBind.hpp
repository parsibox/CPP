#ifndef _OWN_USSD_BIND_H
#define _OWN_USSD_BIND_H
#include "Xml.h"
class CXmlUssdBind{
	public:
		std::string mcC_requestId;
		std::string mcC_userName;
		std::string mcC_password;
		std::string mcC_applicationId;
	public:
		CXmlUssdBind(){
			init();
		}
		void init()
		{
			mcC_requestId.clear();
			mcC_userName.clear();
			mcC_password.clear();
			mcC_applicationId.clear();

		}
		bool  mcfn_decode(std::string CL_xmlMsg){
			XMLNode xMainNode = XMLNode::parseString(CL_xmlMsg.c_str(),"AUTHRequest");
			if(xMainNode.isEmpty()) return false;
			mcC_requestId = Xml::mcfn_getXmlTagValue(xMainNode,"requestId");	
			mcC_userName = Xml::mcfn_getXmlTagValue(xMainNode,"userName");	
			mcC_password = Xml::mcfn_getXmlTagValue(xMainNode,"passWord");	
			mcC_applicationId = Xml::mcfn_getXmlTagValue(xMainNode,"applicationId");	
			return true;
		}
		std::string mcfn_encode(){
			std::stringstream CL_xmlMsg;
			CL_xmlMsg<<"<AUTHRequest>";
			CL_xmlMsg<<Xml::mcfn_setXmlValue(mcC_requestId,"requestId");
			CL_xmlMsg<<Xml::mcfn_setXmlValue(mcC_userName,"userName");
			CL_xmlMsg<<Xml::mcfn_setXmlValue(mcC_password,"passWord");
			CL_xmlMsg<<Xml::mcfn_setXmlValue(mcC_applicationId,"applicationId");
			CL_xmlMsg<<"</AUTHRequest>";
			return CL_xmlMsg.str();
		}
		std::string mcfn_ShortDebugString(){
			std::stringstream CL_strStr;
			CL_strStr.str("");
			CL_strStr<<"AccountName:"<<mcC_userName<<",password:"<<mcC_password<<",applicationId:"<<mcC_applicationId<<",requestId:"<<mcC_requestId;
			return CL_strStr.str();
		}

};

class CXmlUssdBindResp{
	public:
		std::string mcC_requestId;
		std::string mcC_authMsg;
		std::string mcC_errorCode;	
	public:
		CXmlUssdBindResp(){
			mcC_requestId.clear();
			mcC_authMsg.clear();
			mcC_errorCode.clear();

		}
		std::string  mcfn_encode(){
			std::stringstream CL_xmlMsg;
			std::string CL_parentTag= mcC_errorCode.empty()?"AUTHResponse":"AUTHError";
			CL_xmlMsg<<"<" <<CL_parentTag <<">";
			CL_xmlMsg<<Xml::mcfn_setXmlValue(mcC_requestId,"requestId");
			CL_xmlMsg<<Xml::mcfn_setXmlValue(mcC_authMsg,"authMsg");
			CL_xmlMsg<<Xml::mcfn_setXmlValue(mcC_errorCode,"errorCode",false);
			CL_xmlMsg<<"</" <<CL_parentTag <<">";
			return CL_xmlMsg.str();
		}	
		bool  mcfn_decode(std::string CL_xmlMsg){
			XMLNode xMainNode = XMLNode::parseString(CL_xmlMsg.c_str(),"AUTHResponse");
			if(xMainNode.isEmpty()){
				xMainNode = XMLNode::parseString(CL_xmlMsg.c_str(),"AUTHError");
				if(xMainNode.isEmpty()) return false;
			}
			mcC_requestId = Xml::mcfn_getXmlTagValue(xMainNode,"requestId");	
			mcC_authMsg = Xml::mcfn_getXmlTagValue(xMainNode,"authMsg");	
			mcC_errorCode = Xml::mcfn_getXmlTagValue(xMainNode,"errorCode");	
			return true;
		}
		std::string mcfn_ShortDebugString(){
			std::stringstream CL_strStr;
			CL_strStr.str("");
			CL_strStr<<"requestId:"<<mcC_requestId<<",authMsg:"<<mcC_authMsg<<",errorCode:"<<mcC_errorCode;
			return CL_strStr.str();
		}

};


#endif
