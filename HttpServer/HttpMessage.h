#ifndef __HTTPMESSAGE__H
#define __HTTPMESSAGE__H
#include<map>
#include<sstream>

#include"Util.h"

const char TERMINATOR[3]={0x0d,0x0a,0x00};

class HttpRequest
{
	public:
		std::string method;
		std::string reqUrl;
		std::string httpVersion;
                std::string httpBody;
                std::string service;
		std::map<std::string,std::string> hdrParam ;
		HttpRequest()
		{
			method.clear();
			reqUrl.clear();
			httpVersion.clear();
			hdrParam.clear();
                        httpBody.clear();
                        service.clear();
		}
		~HttpRequest(){}
                std::string getParamValue(std::string param)
                {
                    if(hdrParam.find(param)!= hdrParam.end())
                       return hdrParam[param];
                    return "";
                }
                std::string shortDebugString()
                {
                  std::stringstream strStream("");
                  strStream<<"method:"<<method<<",reqUrl:"<<reqUrl<<",httpVersion:"<<httpVersion<<",httpBody:"<<httpBody<<",hdrParamSize:"<<hdrParam.size();
                  return strStream.str();
		}
		bool decodeHttpHeader(std::string hdr);
};

class HttpResponse
{
	public:
                int errorCode;
                std::string version;
                std::string method;
		std::string respBody;
		std::map<std::string,std::string> hdrParam ;
                static std::map <int,std::string> errorIdStrMap; 
		HttpResponse()
		{ 
			respBody.clear();
			hdrParam.clear();
                        version.clear();
                        method.clear();
			errorCode = 200;
			errorIdStrMap[200] = "OK";
			errorIdStrMap[404] = "Not Found";
		}
		~HttpResponse(){}
		bool setParamWithValue(std::string param,std::string value)
		{ 
                           hdrParam[param]=value;
		}
               std::string encodeHttpResponse();
};
#endif
