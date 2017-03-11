#ifndef __DEFINES_H__
#define __DEFINES_H__

#define __DEBUG__



#define SUBMIT_RESP_SMPP	    	0x80000005
#define DELIVERY_REPORT_SMPP		0x80000016

#define DELIVER_CIMD				0x20
#define SUBMIT_RESP_CIMD			0x53
#define DELIVER_REPORT_CIMD			0x23
#define ALIVE_CIMD					0x40
#define SUBMIT_SM_CIMD				0x03
#define LOGINRESP                   0x51
#define DELIVER_SMRESP_CIMD			0x70

#define MSG_USSD_APP_REQ                2000
#define MSG_USSD_APP_IND                2001

#define DA							01
#define OA							02
#define UD							03
#define DCS							04
#define CO_STATUS					05
#define MSG_ID						06
#define UD_LEN						07
#define DF_MSGID					88
#define ESM							99
#define PR_ID						10
#define PR_FLAG						11
#define REG_DELV					12
#define REPL_IF_PR					13
#define UD_HEADER					18
#define USSD_DA                                                      0x32
#define USSD_OA                                                      0x31
#define USSD_UD                                                      0x04
#define USSD_DCS                                                     0x03
#define DND_STATUS			0x62
#define ESME_MSG_CONFIG         	0xc7e2
#define ESME_MSG_TERMINATE      	0xc7e3
#define ESME_ENQUIRE            	0xc7c4
#define ESME_ENQUIRE_ACK        	0xc7c5
#define HEARTBEAT		0x73
#define DND_REQUEST                     0x1030
#define DND_RESPONSE			0x1031
#define CATEGORY_ID                     0x1b
#define PSSRMSISDN                       0x1a //added for sending MSISDN to DND

#define RELEASE  0x05
#define USSDAPPREQ                      2001
typedef unsigned long 	DWORD;	/* Exactly one byte (8 bits) */
typedef unsigned char	u8;	/* Exactly one byte (8 bits) */
typedef unsigned short	u16;	/* Exactly 2 bytes (16 bits) */
typedef unsigned long	u32;	/* Exactly 4 bytes (32 bits) */

typedef char   s8;		/* Exactly one byte (8 bits) */
typedef short  s16;		/* Exactly 2 bytes (16 bits) */
typedef long   s32;		/* Exactly 4 bytes (32 bits) */


#define USSNCNF                         0x07

/**
 * MenuTree Specific addition.
 */
#define MAX_SUB_MENU                            15
#define MAX_DESC_LEN                            50
#define MAX_KEYWORD_LEN                         50
#define MAX_DEST_LEN                            15


#define BUFSIZE                                         200
#define MAX_UD_LEN                                      160
#define MSISDN_LEN                                      30


#endif
