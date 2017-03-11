#ifndef __M3UA_HEADER__
#define __M3UA_HEADER__



class M3uaHeader
{
	public:
               enum MsgClass
               {
                     MGMT = 0,       //Management (MGMT) Messages
                     TM = 1, // Transfer Messages
                     SSNM = 2 ,// SS7 Signalling Network Management (SSNM) Messages
                     ASPSM = 3 ,//ASP State Maintenance (ASPSM) Messages
                     ASPTM = 4 ,// ASP Traffic Maintenance (ASPTM) Messages
                     RESERVED = 5, // Reserved for Other SIGTRAN Adaptation Layers
                     RKM = 9  //Routing Key Management (RKM) Messages
               }; 
		unsigned char version;
		unsigned char reserved;
		unsigned char msgClass;
		unsigned char msgType;
		int msgLength; 

		M3uaHeader()
		{
			init();
		}
		void init()
		{ 
			version = 0x01;
			reserved = 0x00;
			msgLength =0x00;
			msgClass = 0x00;
			msgType = 0x00;
		}
};

#endif;
