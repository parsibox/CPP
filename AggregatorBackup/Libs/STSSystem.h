/**
 * SixDEE Telecom Solutions Pvt. Ltd.
 * Copyright 2006
 * All Rights Reserved.
 *
 * Name:		STSSystem.h
 *
 * Description:	Definitions for Shorthand Data Types
 *		
 *
 * -----	---------   ---------------------------------------------
 * Issue	  Date			      Changes
 * -----	---------   ---------------------------------------------
 *
 *
**/

#ifndef STSSYSTEM_H
#define STSSYSTEM_H

typedef unsigned char	u8;		/* Exactly one byte (8 bits) */
typedef unsigned short	u16;	/* Exactly 2 bytes (16 bits) */

typedef char   s8;				/* Exactly one byte (8 bits) */
typedef short  s16;				/* Exactly 2 bytes (16 bits) */

#ifdef __LINUX32__
typedef unsigned long   u32;    /* Exactly 4 bytes (32 bits) */
typedef long		s32;	   /* Exactly 4 bytes (32 bits) */
#endif

#ifdef __LINUX64__
typedef unsigned int   u32;    /* Exactly 4 bytes (32 bits) */
typedef int			   s32;	   /* Exactly 4 bytes (32 bits) */
typedef unsigned long  u64;    /* Exactly 4 bytes (32 bits) */
typedef long		   s64;	   /* Exactly 4 bytes (32 bits) */
#endif




#endif

