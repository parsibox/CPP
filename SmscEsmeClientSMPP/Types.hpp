#ifndef __TYPES_HPP
#define __TYPES_HPP

typedef struct SessionTag
{
        int mcsi_activeMenu;
	int mcsi_activeMenuNodeType; 
        unsigned int mcui_SerianNo;
	std::vector<std::string> mcC_userInputs;
        SessionTag()
	{
                mcsi_activeMenu=0;
		mcui_SerianNo=0;
		mcsi_activeMenuNodeType = 0;
		mcC_userInputs.clear();
	}
}Session;

class MenuDetails
{
  public:
     std::string mcC_userName;
     std::string mcC_menuServiceCode;
};

#endif
