#ifndef __ICOMMONINTERFACE__H
#define __ICOMMONINTERFACE__H
#include"UssdData.pb.h"
typedef  unsigned char u8;
class ICommonInterface
{

	public:
		virtual void decode(u8*,UssdData& )=0;
		virtual void encode(u8*,UssdData& )=0;
                virtual void processRequest(u8 *) =0 ;
};
#endif
