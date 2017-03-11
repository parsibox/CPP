#include"WebService.h"

extern CEventLog CG_EventLog;
bool WebPush::getPage(HttpRequest* request, HttpResponse *response)
{
	std::string content="200 OK";
	std::string paramValue[MAX_PARAM];
	for(int i=0;i<MAX_PARAM;i++)
	{
		if (!request->getParameter(paramProper[i].paramName, paramValue[i]))
		{
			if(paramProper[i].isMandetory)
			{
				content = "ERR:param "+paramProper[i].paramName+" is missing";
				return fromString(content, response);
			}
                        if(!mefn_validateParam(i,paramValue[i]))
                        {
    		             content = "ERR:invalid param "+paramProper[i].paramName+" value:"+paramValue[i];
		       	     return fromString(content, response);
                        }
		}
                else
                {
		  DBG_VERBOSE((CG_EventLog), ("param:%s,value:%s",paramProper[i].paramName.c_str(),paramValue[i].c_str() ));
                }
	}
	return fromString(content, response);
}


bool WebPush::mefn_validateParam(int paramIndex,std::string paramValue)
{
	return true;
}
