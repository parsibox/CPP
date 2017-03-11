/**
 * SixDEE Telecom Solutions Pvt. Ltd.
 * Copyright 2006
 * All Rights Reserved.
 *
 * Name:		STSMsg.h
 *
 * Description:	Definitions for STSMSG message structures
 *		
 *
 * -----	---------   ---------------------------------------------
 * Issue	  Date			      Changes
 * -----	---------   ---------------------------------------------
 *
 *
**/

#ifndef STSMSG_H
#define STSMSG_H

/*
 *	Common msg structure
 */
typedef struct stshdr
{
	unsigned short	type;		/* type of message		*/
	unsigned short	id;			/* module instantiation	*/
	unsigned char	src;		/* sending module ID	*/
	unsigned char	dst;		/* destination module ID*/
	unsigned short	rsp_req;	/* response required	*/
	unsigned char   hclass;     /* generic MSG type     */
	unsigned char	status;		/* returned status		*/
	unsigned long	err_info;	/* status information	*/
	struct stshdr	*next;			/* for message chaining	*/
} STSHDR;

/*
 * control message format
 */
#define	STSMAX_PARAM	80

typedef struct stsm_frame
{
	STSHDR		hdr;
	unsigned short	len;		/* param field length	*/
	long		param[STSMAX_PARAM];
} STSMSG_SS7;

/*
 * Allow the use of STSMSF as a shorthand
 */
#define STSMSF     STSMSG_SS7

/*
 * Macro to get (unsigned char *) pointer to STSMSG parameter area:
 */
#define stsget_param(x)   ((unsigned char *)((STSMSF *)(x))->param)

/*
 * Macro to get (unsigned char *) pointer to hdr part:
 */
#define stsget_hdr(x)   ((unsigned char *)&((STSMSF *)(x))->hdr)

/*
 * Macro to get value of 1st unused unsigned bytes pointer to STSMSG hdr:
 */
#define stsget_unusedBY1(x)   (unsigned char)(*(stsget_hdr(x)+10))
/*
 * Macro to get value of 2nd unused unsigned bytes pointer to STSMSG hdr:
 */
#define stsget_unusedBY2(x)   (unsigned char)(*(stsget_hdr(x)+11))
/*
 * Macro to set value of 1st unused unsigned bytes pointer to STSMSG hdr:
 */
#define stsset_unusedBY1(x,y)   (*(stsget_hdr(x)+10)= (unsigned char)y)
/*
 * Macro to set value of 2nd unused unsigned bytes pointer to STSMSG hdr:
 */
#define stsset_unusedBY2(x,y)   (*(stsget_hdr(x)+11)= (unsigned char)y)

#endif /* STSMSG_H */
