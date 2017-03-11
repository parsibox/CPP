#include "Xml.h"

std::string Xml::mcfn_setXmlValue(std::string CL_value,std::string CL_tag,bool is_setEmpty)
{
	std::stringstream CL_xmlValue;
	if(CL_value.empty()){
		if(is_setEmpty)
			CL_xmlValue<<"<"<<CL_tag<<"/>";
		else
			return "";
	}
	else
		CL_xmlValue<<"<"<<CL_tag<<">"<<CL_value<<"</"<<CL_tag<<">";
	return CL_xmlValue.str();
}

std::string Xml::mcfn_setXmlValueWithParent(std::string CL_value,std::string CL_parent,std::string CL_tag,bool is_setEmpty)
{
	std::stringstream CL_xmlValue("");
	if(CL_value.empty()){
		if(is_setEmpty)
			CL_xmlValue<<"<"<<CL_parent<<">"<<"</"<<CL_tag<<">"<<"</"<<CL_parent<<">";
		else
			return "";
	}
	CL_xmlValue<<"<"<<CL_parent<<">"<<"<"<<CL_tag<<">"<<CL_value<<"</"<<CL_tag<<">"<<"</"<<CL_parent<<">";
	return CL_xmlValue.str();
}
std::string Xml::mcfn_setXmlValueStartEndTag(std::string CL_Value,std::string CL_StartTag,std::string CL_EndTag,bool bL_IsSetEmpty){
	std::stringstream CL_XmlValue("");
	if(CL_Value.empty()){
		if(bL_IsSetEmpty)
			CL_XmlValue<<"<"<<CL_StartTag<<"></"<<CL_EndTag<<">";
		else
			return "";
	}
	CL_XmlValue<<"<"<<CL_StartTag<<">"<<CL_Value<<"</"<<CL_EndTag<<">";
	return CL_XmlValue.str();
}


std::string Xml::mcfn_getXmlTagValue(XMLNode &xL_parent,std::string CL_token)
{
	XMLNode xL_tempNode=xL_parent.getChildNode(CL_token.c_str());
	if((!xL_tempNode.isEmpty()) && (xL_tempNode.getText() != NULL))
		return xL_tempNode.getText();
	else 
		return "";
}

std::string Xml::mcfn_getXmlTagValue(XMLNode &xL_parent,std::string CL_parent,std::string CL_token)
{
	std::string CL_return="";
	XMLNode xL_tempNode=xL_parent.getChildNode(CL_parent.c_str());
	if(!xL_tempNode.isEmpty()){
		CL_return=mcfn_getXmlTagValue(xL_tempNode,CL_token.c_str());
		if(CL_return.empty()||CL_return.length()==0){
			return mcfn_getXmlTagValue(xL_tempNode,"Meta",CL_token.c_str());
		}
	}
	return CL_return;
}	


