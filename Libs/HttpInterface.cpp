/*
 * HttpInterface.cpp
 *
 *  Created on: Dec 18, 2013
 *      Author: surya
 */

#include"HttpInterface.h"
CHttpInterface::CHttpInterface(std::string CL_Url,int iL_reqTimeout){
	mcfn_initialise(CL_Url,iL_reqTimeout);
}

CHttpInterface::~CHttpInterface(){
}

void CHttpInterface::mcfn_initialise(std::string CL_Url,int iL_reqTimeout)
{
	CAutoLocker CL_reqLocket(meC_reqLock);
	meC_Url=CL_Url;
	mesi_reqTimeout=iL_reqTimeout;
	meC_httpObj.mcfn_SetURL((char*)CL_Url.c_str());	
}
bool CHttpInterface::mcfn_doHttpGetReq(std::string CL_request,std::string &CL_responce)
{
	CAutoLocker CL_reqLocket(meC_reqLock);
	CL_responce="";
	try{
		meC_httpObj.mcfn_SetURL((char*)CL_request.c_str());
		if(meC_httpObj.mcfn_ExecuteGETBuffer(mesi_reqTimeout)){
			if(meC_httpObj.mcfn_GetResponseBuffer()!=0x00)
			CL_responce.assign(meC_httpObj.mcfn_GetResponseBuffer());
			return true;
		}
	}
	catch(...){
	}
	return false;	
}

bool CHttpInterface::mcfn_doHttpPostReq(std::string CL_request,std::string &CL_responce)
{
	try{
		CAutoLocker CL_reqLocket(meC_reqLock);
		CL_responce="";
		meC_httpObj.mcfn_SetPostData((char*)CL_request.c_str());
		if(meC_httpObj.mcfn_ExecutePOSTBuffer(mesi_reqTimeout)){
			if(meC_httpObj.mcfn_GetResponseBuffer()!=0x00)
			CL_responce.assign(meC_httpObj.mcfn_GetResponseBuffer());
			return true;
		}
	}
	catch(...){
	}
	return false;
}

