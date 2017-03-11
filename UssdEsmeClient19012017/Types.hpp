#ifndef __TYPES_HPP
#define __TYPES_HPP

typedef struct SessionTag
{
        void *pmcv_ActiveNode;
        unsigned int mcui_SerianNo;
        SessionTag()
	{
		pmcv_ActiveNode=NULL;
		mcui_SerianNo=0;
	}
}Session;


#endif
