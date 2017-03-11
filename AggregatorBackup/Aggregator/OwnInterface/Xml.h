#ifndef __XML__ 
#define __XML__
#include "xmlParser.h"
#include <string>
#include<sstream>
class Xml
{
	public:
		static std::string mcfn_setXmlValue(std::string CL_value,std::string CL_tag,bool is_setEmpty=true);
		static std::string mcfn_setXmlValueWithParent(std::string CL_value,std::string CL_parent,std::string CL_tag,bool is_setEmpty=true);
		static std::string mcfn_setXmlValueStartEndTag(std::string CL_Value,std::string CL_StartTag,std::string CL_EndTag,bool bL_IsSetEmpty=true);
		static std::string mcfn_getXmlTagValue(XMLNode &xL_parent,std::string CL_token);
		static std::string mcfn_getXmlTagValue(XMLNode &xL_parent,std::string CL_parent,std::string CL_token);
};
#endif
