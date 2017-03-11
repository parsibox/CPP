#ifndef __TYPES_HPP
#define __TYPES_HPP

typedef struct SessionTag
{
        int mcsi_activeMenu; 
        unsigned int mcui_SerianNo;
        SessionTag()
	{
                mcsi_activeMenu=0;
		mcui_SerianNo=0;
	}
}Session;

class MenuDetails
{
  public:
     std::string mcC_userName;
     std::string mcC_menuServiceCode;
};

#endif
