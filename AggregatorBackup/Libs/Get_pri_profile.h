#ifndef _GET_PRIVATE_PROFILE_H
#define _GET_PRIVATE_PROFILE_H

#include "STSException.h"
#ifdef __LINUX__
unsigned long GetPrivateProfileSection(char *,char *,unsigned long ,char *);
unsigned long GetPrivateProfileString(char *,char *,char *,char *,unsigned long,char *);
void mcfn_getString(char *,char *,char *,bool is_Req=true);
void mcfn_getInt(char *,char *,int &,bool is_Req=true);
void mcfn_getChar(char *,char *,char &,bool is_Req=true);
void mcfn_getBool(char *,char *,bool &,bool is_Req=true);
void mcfn_setFileName(char *);

#endif

#endif

