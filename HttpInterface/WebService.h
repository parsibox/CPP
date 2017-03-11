#ifndef __WEBSERVICE__H
#define __WEBSERVICE__H
#include <string.h> 
#include "libnavajo/AuthPAM.hh"
#include "libnavajo/libnavajo.hh"
#include "libnavajo/LogFile.hh"
#include"EventLog.h"

enum PARAM_NUMBERS
{
  MSISDN,
  MESSAGE_TYPE,
  MESSAGE,
  RESPONCE_URL,
  UNIQUE_ID,
  MAX_PARAM
};
typedef struct _paramProper
{
  std::string paramName;
  bool isMandetory;
  _paramProper(std::string name,bool isMande) :paramName(name),isMandetory(isMande)
  {}
};
const _paramProper paramProper[]= {
         _paramProper("msisdn",true),
	_paramProper("msg_type",true),
	_paramProper("message",true),
	_paramProper("resp_url",false),
	_paramProper("unique_id",false)};

class WebPush: public DynamicPage
{
  public:

  bool getPage(HttpRequest* request, HttpResponse *response);
  private:
  bool mefn_validateParam(int paramIndex,std::string paramValue); 
};


#endif
