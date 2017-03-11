#include"HttpMessage.h"
std::map <int,std::string> HttpResponse::errorIdStrMap;

//TODO::need to insert all error code with description here

bool HttpRequest::decodeHttpHeader(std::string hdr)
{
        if(hdr.length() == 0 ) return false;

	std::vector<std::string> hdrLines;
	std::string newLine="\n";
	Util::SplitFields(hdr,hdrLines,newLine);

	std::string startLine = hdrLines[0];
	if(startLine.length()) 
	{
		std::vector<std::string> startLineToken;
		std::string space=" ";
		if(Util::SplitFields(startLine,startLineToken,space) &&
				startLineToken.size() !=3)
		{
			return false;
		} 
		method = startLineToken[0];
		reqUrl = startLineToken[1];
		httpVersion = startLineToken[2];
		int index = 0;
		if((index=reqUrl.find_first_of("?",0)) != std::string::npos)
		{
			service = reqUrl.substr(0,index);
		}
		else
		{
			service = reqUrl;
		}
	}
	for(int i=1;i<hdrLines.size();++i)
	{
		int index = 0;
		if((index=hdrLines[i].find_first_of(":",0)) != std::string::npos)
		{
			hdrParam[Util::trim(hdrLines[i].substr(0,index))]= hdrLines[i].substr(index,hdrLines[i].length());
		}
	}
	printf("after decode:%s\n",shortDebugString().c_str());
	return true;
}

std::string HttpResponse::encodeHttpResponse()
{
	std::stringstream httpRes("");
	httpRes<<version<<" "<<errorCode<<" "<<errorIdStrMap[errorCode]<<TERMINATOR;
	if(errorCode == 200)
	{
		httpRes<<" Server : surya http Server 1.0"<<TERMINATOR;
		std::map<std::string,std::string>::iterator itr = hdrParam.begin();
		while(itr != hdrParam.end())
		{
			httpRes<<itr->first<<" : "<<itr->second<<TERMINATOR;
			itr++;
		}
		if(respBody.length() !=0 && method.compare("HEAD") != 0)
		{
			httpRes<<"Content-Length : "<<respBody.length()<<TERMINATOR;
		}
	}
	httpRes<<TERMINATOR;
	if(method.compare("HEAD") != 0)
	{ 
		httpRes<<respBody;
	}
	return httpRes.str();
}
