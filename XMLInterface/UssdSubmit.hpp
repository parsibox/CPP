#ifndef _USSD_SUBMIT_H_
#define _USSD_SUBMIT_H_
#include"UssdHeader.hpp"

enum XmlMsgTypes
{
	NONE = 0,
	PSSR = 1,
	USSR = 2,
	USSN = 3,
	USSRACK = 4,
	PSSRACK = 6,
	USSNACK = 7
};
class CUssdMsg{
	public:
		std::string mcC_requestId;
		std::string mcC_msisdn;
		std::string mcC_starCode;
		std::string mcC_imsi;
		std::string mcC_clientId;
		std::string mcC_phase;
		std::string mcC_dcs;
		std::string mcC_userData;
		std::string mcC_msgType;
		std::string mcC_endOfSession;
		bool mcb_isRequest;
		CUssdMsg(){
			mcfn_init();
		}
		void mcfn_init(){
			mcC_requestId.clear();
			mcC_msisdn.clear();
			mcC_starCode.clear();
			mcC_imsi.clear();
			mcC_clientId.clear();
			mcC_phase.clear();
			mcC_dcs.clear();
			mcC_userData.clear();
			mcC_msgType.clear();
			mcC_endOfSession.clear();
			mcb_isRequest = true;
		}
		std::string  mcfn_encode(){
			std::stringstream CL_xmlMsg;
			std::string CL_parent = mcb_isRequest ? "USSDRequest": "USSDResponse" ;
			CL_xmlMsg<<"<" << CL_parent <<">";
			CL_xmlMsg<<Xml::mcfn_setXmlValue(mcC_requestId,"requestId");
			CL_xmlMsg<<Xml::mcfn_setXmlValue(mcC_msisdn,"msisdn");
			CL_xmlMsg<<Xml::mcfn_setXmlValue(mcC_starCode,"starCode");
			CL_xmlMsg<<Xml::mcfn_setXmlValue(mcC_imsi,"imsi");
			CL_xmlMsg<<Xml::mcfn_setXmlValue(mcC_clientId,"clientId");
			CL_xmlMsg<<Xml::mcfn_setXmlValue(mcC_phase,"phase");
			CL_xmlMsg<<Xml::mcfn_setXmlValue(mcC_dcs,"dcs");
			CL_xmlMsg<<Xml::mcfn_setXmlValue(mcC_userData,"userdata");
			CL_xmlMsg<<Xml::mcfn_setXmlValue(mcC_msgType,"msgtype");
			CL_xmlMsg<<Xml::mcfn_setXmlValue(mcC_endOfSession,"EndofSession");
			CL_xmlMsg<<"</" <<CL_parent <<">";
			return CL_xmlMsg.str();

		}
		bool  mcfn_decode(std::string CL_xmlMsg){
			XMLNode xMainNode = XMLNode::parseString(CL_xmlMsg.c_str(),"USSDRequest");
			if(xMainNode.isEmpty()) 
			{ 
				xMainNode = XMLNode::parseString(CL_xmlMsg.c_str(),"USSDResponse");
				if(xMainNode.isEmpty()) 
				return false;
				mcb_isRequest = false;
			}
			mcC_requestId = Xml::mcfn_getXmlTagValue(xMainNode,"requestId");	
			mcC_msisdn = Xml::mcfn_getXmlTagValue(xMainNode,"msisdn");	
			mcC_starCode = Xml::mcfn_getXmlTagValue(xMainNode,"starCode");	
			mcC_imsi = Xml::mcfn_getXmlTagValue(xMainNode,"imsi");	
			mcC_clientId = Xml::mcfn_getXmlTagValue(xMainNode,"clientId");	
			mcC_phase = Xml::mcfn_getXmlTagValue(xMainNode,"phase");	
			mcC_dcs = Xml::mcfn_getXmlTagValue(xMainNode,"dcs");	
			mcC_userData = Xml::mcfn_getXmlTagValue(xMainNode,"userdata");	
			mcC_msgType =  Xml::mcfn_getXmlTagValue(xMainNode,"msgtype");	
			mcC_endOfSession = Xml::mcfn_getXmlTagValue(xMainNode,"EndofSession");	
			return true;

		}
		 std::string mcfn_ShortDebugString(){
                        std::stringstream CL_strStr;
                        CL_strStr.str("");
                        CL_strStr<<"requestId:"<<mcC_requestId<<",msisdn:"<<mcC_msisdn<<",starCode:"<<mcC_starCode<<",imsi:"<<mcC_imsi<<",clientId:"<<mcC_clientId<<",phase:"<<mcC_phase<<",dcs:"<<mcC_dcs<<",userdata:"<<mcC_userData<<",msgtype:"<<mcC_msgType<<",EndofSession:"<<mcC_endOfSession;
                        return CL_strStr.str();
                }




};
#endif

