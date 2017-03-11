/**
 * SixDEE Telecom Solutions Pvt. Ltd.
 * Copyright 2007
 * All Rights Reserved.
 */

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  File name					:: URLRequest.h
//  Purpose				      	:: Interface for class URLRequest
//
//  Name	                        Purpose
//  -------                         -----------
//	STSHttp							Provides methods for http GET and POST request.
//
//   Development History					::
// 
//  Date					Author                      Description
//  -----------				-----------------------		--------------------------------------------------------------     
//	10/04/2007 12:00:00		Amiya Ranjan				Created.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __URLREQUEST_H
#define __URLREQUEST_H

#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#ifdef __LINUX__
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/types.h>
#else
#include <curl.h>
#endif


struct URLParam
{
	char mesc_ParamName[255];				  //Parameter name in query string
	char mesc_ParamValue[255];				  //Parameter value in query string
};

struct URLResponseBuffer 
{ 											  
  char *pmesc_Buff; 						  //Buffer for URL reponse data
  size_t mesl_Size; 						  //Length of response data
}; 

class STSHttp
{
private:
	CURL *mepv_Handle;  					   //Handle for CURL
	char mesc_URL[255];						   //URL
	URLParam meS_Param[25]; 				   //Parameters for GET Request
	char mesc_postdata[2500];				   //Data for POST Request
	int mesl_NumParam;						   //Number of Parameters for GET request
	char mesc_OutPutFileName[255];			   //Name of outfile for URL response.
	char mesc_Error[CURL_ERROR_SIZE];		   //Error
	URLResponseBuffer meS_resp;				   //Structure for URL Reponse
	struct curl_slist *pmeS_headerList;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  Sets output File Name
///  @param scL_fname File name of output file	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void mefn_SetOutputFileName(char* scL_fname);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


public:
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// A Constructor 
/// @see  STSHttp
///	For initializing the message list
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	STSHttp();
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// A Destructor 
/// @see  STSHttp
///	For initializing the message list
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~STSHttp();
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  Completes Initialization, 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void mcfn_initialise();
///  Sets URL to be invoked.
///  @param scL_url Complete URL to be invoked	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void mcfn_SetURL(char* scL_url);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  Sets data for HTTP POST request.
///  @param scL_pdata Data for HTTP POST request.	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void mcfn_SetPostData(char* scL_pdata);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  Adds parameter to query string for HTTP GET request.
///  @param scL_Pname Name of parameter 
///	 @param scL_Pval  Value of parameter 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void mcfn_AddParam(char* scL_Pname, char* scL_Pval);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  Executes HTTP GET request.Response is written in a file.
///	 @param scL_fname Output File Name
///  @return true/false  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool mcfn_ExecuteGETFile(char* scL_fname,unsigned int u32L_Time=0);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  Executes HTTP POST request. Response is written in a file.
///	 @param scL_fname Output File Name
///  @return true/false  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool mcfn_ExecutePOSTFile(char* scL_fname,unsigned int u32L_Time=0);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  Executes HTTP GET request.Response written to a buffer. 
///  @return true/false  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool mcfn_ExecuteGETBuffer(unsigned int u32L_Time=0);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  Executes HTTP POST request. Response copied to buffer. 
///  @return true/false  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool mcfn_ExecutePOSTBuffer(unsigned int u32L_Time=0);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  Returns pointer to buffer containing URL reponse for both POST and GET.  
///  @return true/false  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	char* mcfn_GetResponseBuffer();

        bool mcfn_setHeader(char* header);
	const char* mcfn_GetErrorStr() const;	
};

#endif





	


