/*
 * HttpInterFace.h
 *
 *  Created on: Dec 18, 2013
 *      Author: surya
 */

#ifndef HTTPINTERFACE_H_
#define HTTPINTERFACE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include"AutoLocker.h"
#include"URLRequest.h"
class CHttpInterface {
	private:
		std::string meC_Url;
		int mesi_reqTimeout;
		CCriticalSection meC_reqLock;	
		STSHttp meC_httpObj;
	public:
		CHttpInterface(std::string CL_Url="",int iL_reqTimeout=30);
		~CHttpInterface();
		void mcfn_initialise(std::string CL_Url,int iL_reqTimeout=30);
		bool mcfn_doHttpGetReq(std::string CL_request,std::string &CL_responce);
		bool mcfn_doHttpPostReq(std::string CL_request,std::string &CL_responce);
		void mcfn_setHeader(std::string headerStr){meC_httpObj.mcfn_setHeader((char*)headerStr.c_str());}
};
#endif /* HTTPINTERFACE_H_*/
