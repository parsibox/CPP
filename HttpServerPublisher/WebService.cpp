#include"WebService.h"
#include"Publisher.h"
#include"DBPicker.h"

extern CDBPicker *pCG_publisherObj;
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
        if(!mefn_insertDataValuesToQue(paramValue,content))
	{
		content += "internal server error not able to process request ";
		return fromString(content, response);
	}    
	return fromString(content, response);
}


bool WebPush::mefn_validateParam(int paramIndex,std::string paramValue)
{
	return true;
}
bool WebPush::mefn_insertDataValuesToQue(std::string* params,std::string& respStr)
{

	DeliverToEsme *pCL_msg=new DeliverToEsme();
	pCL_msg->set_task_id(1);
	pCL_msg->set_oa("HTTP");
	pCL_msg->set_interface_id(4);
	pCL_msg->set_reg_del(1);
	pCL_msg->set_channel_id(2);
	pCL_msg->set_expiry_time(time(NULL)+102400);
	pCL_msg->set_pid(0);
	pCL_msg->set_dcs(0x0f);//as per the message need to set this value
	pCL_msg->set_lang_id(0);
	pCL_msg->set_task_name("HttpPush");
	pCL_msg->set_username("default");
        pCL_msg->set_srv_code(params[SERVICE_ID]);
	if(params[MESSAGE_TYPE]=="USSR")
	{
		pCL_msg->set_msg_type(2);
                if(!pCG_publisherObj->mcfn_setTaskByServiceName(params[SERVICE_ID],pCL_msg))
                { 
                     delete pCL_msg;
                     respStr=params[SERVICE_ID]+" is invalid";
                }
	}
	else
	{
		pCL_msg->set_msg_type(1);
	}
	pCL_msg->set_msg_id(atoi(params[UNIQUE_ID].c_str()));
	pCL_msg->set_da(params[MSISDN].c_str());
	pCL_msg->set_msg(params[MESSAGE]);
	pCL_msg->set_service_url(params[RESPONCE_URL]);
	if(!CPublisher::mcfnS_GetInstance()->mcfn_addToQue(pCL_msg)){
		DBG_DEBUG((CG_EventLog), ("No Connection Avaiable to send the messages"));
		delete pCL_msg;
		return false;
	}

	return true;
}
