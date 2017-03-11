#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Get_pri_profile.h"

char *psCG_FileName;
unsigned long GetPrivateProfileSection(char *pscL_Sec,char *pscL_Data,unsigned long ulL_Size,char *pscL_Filename)
{
	
	if((pscL_Sec == NULL) || (pscL_Filename ==NULL) || (pscL_Data == NULL) || (ulL_Size <= 0))
		return 0;

	FILE *fp;
	char scL_Section[100];

	if(strlen(pscL_Sec) > 96)
		return 0;


	fp=fopen(pscL_Filename,"r");
	
	if(fp==NULL)
	{
		return 0;
	}
	
	scL_Section[0]='[';
	scL_Section[1]='\0';
	strcat(scL_Section,pscL_Sec);
	strcat(scL_Section,"]");
	
	char scL_Databuffer[100];
	char *pscL_Delimiter = pscL_Data;
	char *pscL_DataBuffer;
	unsigned long ulL_Len;
	
	int ulL_SecLen = 0;
	unsigned long ulL_Length = 0x00;
	fgets(scL_Databuffer,100,fp);
	while(!feof(fp))
	{
		if(scL_Databuffer[0]=='[')
		{
			ulL_SecLen = strlen(scL_Databuffer);
			
			if(scL_Databuffer[ulL_SecLen - 1] == '\n')
				ulL_SecLen--;

			if(scL_Databuffer[ulL_SecLen - 1] == '\r')
				ulL_SecLen--;

			scL_Databuffer[ulL_SecLen] = '\0';

			if(strcmp(scL_Databuffer,scL_Section)==0)
			{
				fgets(scL_Databuffer,100,fp);
				while(scL_Databuffer[0]!='[')
				{
					pscL_DataBuffer = scL_Databuffer;

					while(*pscL_DataBuffer == ' ' || *pscL_DataBuffer == '\t' || *pscL_DataBuffer == '\n')
						pscL_DataBuffer++;

					if(pscL_DataBuffer[0] != '\0')
					{
						if(ulL_Length >= (ulL_Size - 1))
						{
							*pscL_Delimiter++='\0';
							fclose(fp);
							return ++ulL_Length;
						}
						ulL_Len=strlen(pscL_DataBuffer);

						if(pscL_DataBuffer[ulL_Len - 1] == '\n')
							ulL_Len--;

						if(pscL_DataBuffer[ulL_Len - 1] == '\r')
							ulL_Len--;

						ulL_Length = ulL_Length + ulL_Len + 1;
						strcpy(pscL_Delimiter,pscL_DataBuffer);
						pscL_Delimiter[ulL_Len] = '\0';
						pscL_Delimiter= pscL_Delimiter + ulL_Len + 1;
					}
					fgets(scL_Databuffer,100,fp);
					if(feof(fp))
						break;
				}
				break; 
			}
		}
	fgets(scL_Databuffer,100,fp);
	}
    fclose(fp);
	return ulL_Length; 
}



unsigned long GetPrivateProfileString(char *pscL_Sec,char *pscL_Key,char *pscL_Defaultstr, char *pscL_Data,unsigned long ulL_Size,char *pscL_Filename)
{
	
	if((pscL_Sec == NULL) || (pscL_Key == NULL) || (pscL_Defaultstr == NULL) || (pscL_Filename ==NULL) || (pscL_Data == NULL) || (ulL_Size <= 0))
		return 0;
	FILE *fp;
	char scL_Section[100];

	if(strlen(pscL_Sec) > 96)
	   return 0;
    
	fp=fopen(pscL_Filename,"r");

	if(fp==NULL)
	{
	  return 0;
	}

	
	scL_Section[0]='[';
	scL_Section[1]='\0';
	strcat(scL_Section,pscL_Sec);
	strcat(scL_Section,"]");
	
	char scL_Databuffer[1025];
	char *pscL_Delimiter;
	char *pscL_DataBuffer;
	unsigned long ulL_Len=0;
	strcpy(pscL_Data,pscL_Defaultstr);
	int ulL_SecLen = 0;
	fgets(scL_Databuffer,1025,fp);
	while(!feof(fp))
	{
		if(scL_Databuffer[0]=='[')
		{
			ulL_SecLen = strlen(scL_Databuffer);
			
			if(scL_Databuffer[ulL_SecLen - 1] == '\n')
				ulL_SecLen--;

			if(scL_Databuffer[ulL_SecLen - 1] == '\r')
				ulL_SecLen--;

			scL_Databuffer[ulL_SecLen] = '\0';

			if(strcmp(scL_Databuffer,scL_Section)==0)
			{
				fgets(scL_Databuffer,1024,fp);

				while(scL_Databuffer[0] != '[')
				{
					pscL_DataBuffer = scL_Databuffer;

					while(*pscL_DataBuffer == ' ' || *pscL_DataBuffer == '\t' || *pscL_DataBuffer == '\n')
						pscL_DataBuffer++;

					if(pscL_DataBuffer[0] != '\0')
					{
						pscL_Delimiter = strchr(pscL_DataBuffer,'=');
						if(pscL_Delimiter != NULL)
						{
							*pscL_Delimiter = '\0';
							pscL_Delimiter++;
							if(strcmp(pscL_DataBuffer,pscL_Key) == 0)
							{
								ulL_Len = strlen(pscL_Delimiter);
								if(pscL_Delimiter[ulL_Len - 1] == '\n')
									ulL_Len--;

								if(pscL_Delimiter[ulL_Len - 1] == '\r')
									ulL_Len--;

								if(ulL_Len > (ulL_Size - 1))
									ulL_Len = ulL_Size -1;
								if(ulL_Len != 0)
									strncpy(pscL_Data,pscL_Delimiter,ulL_Len);
								pscL_Data[ulL_Len] = '\0';
								fclose(fp);
								return (ulL_Len);
							}
						}
					}
					fgets(scL_Databuffer,1024,fp);
					if(feof(fp))
						break;
				}
			}
		}
	fgets(scL_Databuffer,1024,fp);
	}
	fclose(fp);
    return (ulL_Len);
}
void mcfn_setFileName(char *pscL_FileName){
psCG_FileName=pscL_FileName;
}
void mcfn_getString(char *sL_Section,char *sL_Filed,char *pcL_Destination,bool is_Req)
{
	char pscL_Param[1024]={0x00};
	char pscL_ErrStrg[300];
	memset(pscL_Param,0x00,sizeof(pscL_Param));
	GetPrivateProfileString(sL_Section,sL_Filed,"-1",pscL_Param,sizeof(pscL_Param),psCG_FileName);
	if(strcmp(pscL_Param,"-1") == 0)
	{
		GetPrivateProfileString("COMMON",sL_Filed,"-1",pscL_Param,sizeof(pscL_Param),psCG_FileName);
		if(strcmp(pscL_Param,"-1") == 0){
			sprintf(pscL_ErrStrg,"Configuration Parameter %s key missing in section %s\n",sL_Filed,sL_Section);
			throw STSGenException(1,pscL_ErrStrg);
		}
	}
	if(is_Req &&strlen(pscL_Param)==0)
	{
		sprintf(pscL_ErrStrg,"Configuration Parameter %s value  missing in section %s\n",sL_Filed,sL_Section);
		throw STSGenException(2,pscL_ErrStrg);

	}
	strcpy(pcL_Destination,pscL_Param);
	//printf("%s=%s\n",sL_Filed,pcL_Destination);
}
void mcfn_getInt(char *sL_Section,char *sL_Filed,int &siL_Destination,bool is_Req)
{
        char pscL_Param[200]={0x00};
	char pscL_ErrStrg[300];
        memset(pscL_Param,0x00,sizeof(pscL_Param));
        GetPrivateProfileString(sL_Section,sL_Filed,"*",pscL_Param,sizeof(pscL_Param),psCG_FileName);
	if(strcmp(pscL_Param,"*") == 0)
	{
		GetPrivateProfileString("COMMON",sL_Filed,"*",pscL_Param,sizeof(pscL_Param),psCG_FileName);
		if(strcmp(pscL_Param,"*") == 0)
		{
			sprintf(pscL_ErrStrg,"Configuration Parameter %s key missing in section %s\n",sL_Filed,sL_Section);
			throw STSGenException(1,pscL_ErrStrg);
		}
	}
        if(is_Req && strlen(pscL_Param)==0)
        {
                sprintf(pscL_ErrStrg,"Configuration Parameter %s value  missing in section %s\n",sL_Filed,sL_Section);
		throw STSGenException(2,pscL_ErrStrg);
        }
        siL_Destination=atoi(pscL_Param);
        //printf("%s=%d\n",sL_Filed,siL_Destination);

}
void mcfn_getChar(char *sL_Section,char *sL_Filed,char &cL_Destination,bool is_Req)
{
        char pscL_Param[200]={0x00};
	/*char pscL_ErrStrg[300];
        memset(pscL_Param,0x00,sizeof(pscL_Param));
        GetPrivateProfileString(sL_Section,sL_Filed,"-1",pscL_Param,sizeof(pscL_Param),psCG_FileName);
        if(strcmp(pscL_Param,"-1") == 0)
        {
                printf("Configuration Parameter %s key missing in section %s\n",sL_Filed,sL_Section);
                exit(1);
        }
        if(is_Req && strlen(pscL_Param)==0)
        {
                printf("Configuration Parameter %s value  missing in section %s\n",sL_Filed,sL_Section);
                exit(1);

        }*/
	mcfn_getString(sL_Section,sL_Filed,pscL_Param,is_Req);
        cL_Destination=pscL_Param[0];
        //printf("%s=%c\n",sL_Filed,cL_Destination);

}
void mcfn_getBool(char *sL_Section,char *sL_Filed,bool &bL_Destination,bool is_Req)
{
        char cL_Param=0x00;
        mcfn_getChar(sL_Section,sL_Filed,cL_Param,is_Req);
        (cL_Param=='y'||cL_Param=='Y')?bL_Destination=true:bL_Destination=false;
}



