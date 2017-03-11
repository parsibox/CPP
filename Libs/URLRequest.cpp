#include "URLRequest.h"

size_t fnG_WriteData(void *ptr, size_t size, size_t nmemb, void *stream)
{
	int written = fwrite(ptr, size, nmemb, (FILE *)stream);
	return written;	
}

size_t fnG_WriteDataToBuffer(void *ptr, size_t size, size_t nmemb, void *stream)
{
	register int nL_realsize = size * nmemb; 
	struct URLResponseBuffer *pSL_mem = (struct URLResponseBuffer*)stream; 
	pSL_mem->pmesc_Buff = (char *)realloc(pSL_mem->pmesc_Buff, pSL_mem->mesl_Size + nL_realsize + 1); 
	if (pSL_mem->pmesc_Buff) 
	{ 
		memcpy(&(pSL_mem->pmesc_Buff[pSL_mem->mesl_Size]), ptr, nL_realsize); 
		pSL_mem->mesl_Size += nL_realsize; 
		pSL_mem->pmesc_Buff[pSL_mem->mesl_Size] = 0; 
	} 
	return nL_realsize; 

}


STSHttp::STSHttp()
{
	mepv_Handle = curl_easy_init();
	if(mepv_Handle!=0x00)
	{
		memset(mesc_URL,0x00,sizeof(mesc_URL));
		memset(meS_Param,0x00,sizeof(meS_Param));
		memset(mesc_postdata,0x00,sizeof(mesc_postdata));
		mesl_NumParam=0x00;
		memset(mesc_OutPutFileName,0x00,sizeof(mesc_OutPutFileName));
		memset(&meS_resp,0x00,sizeof(URLResponseBuffer));
		pmeS_headerList = NULL;
	}
	else
	{
		printf("CURL not initialised\n");
		strcpy(mesc_Error,"CURL not initialised");

	}

}

const char *STSHttp::mcfn_GetErrorStr() const{
	    return mesc_Error;
}

STSHttp::~STSHttp()
{
	curl_easy_cleanup(mepv_Handle);
	curl_slist_free_all(pmeS_headerList);
	if(meS_resp.pmesc_Buff != 0x00)
		free(meS_resp.pmesc_Buff);

}

void STSHttp::mcfn_initialise()
{
	memset(mesc_URL,0x00,sizeof(mesc_URL));
	memset(meS_Param,0x00,sizeof(meS_Param));
	memset(mesc_postdata,0x00,sizeof(mesc_postdata));
	mesl_NumParam=0x00;
	memset(mesc_OutPutFileName,0x00,sizeof(mesc_OutPutFileName));
	memset(mesc_Error,0x00,sizeof(mesc_Error));
	if(meS_resp.pmesc_Buff != 0x00)
		free(meS_resp.pmesc_Buff);
	memset(&meS_resp,0x00,sizeof(meS_resp));
	pmeS_headerList = NULL;
        
}

void STSHttp::mefn_SetOutputFileName(char* scL_fname)
{
	if(mepv_Handle!=0x00)
	{	
		strcpy(mesc_OutPutFileName,scL_fname);   
	}
	else
	{
		printf("CURL not initialised\n");
		strcpy(mesc_Error,"CURL not initialised");
	}

}

void STSHttp::mcfn_SetURL(char* scL_url)
{
	if(mepv_Handle!=0x00)
	{	
		strcpy(mesc_URL,scL_url);   
	}
	else
	{
		printf("CURL not initialised\n");
		strcpy(mesc_Error,"CURL not initialised");
	}
}

void STSHttp::mcfn_AddParam(char* scL_Pname, char* scL_Pval)
{
	strcpy(meS_Param[mesl_NumParam].mesc_ParamName,scL_Pname);
	strcpy(meS_Param[mesl_NumParam++].mesc_ParamValue,scL_Pval);
}

bool STSHttp::mcfn_ExecuteGETFile(char* scL_fname,unsigned int u32L_Time)
{
	if(mepv_Handle!=0x00)
	{	
		if(mesl_NumParam >0x00)
		{
			char scL_list[1000];
			memset(scL_list,0x00,sizeof(scL_list));
			int sL_Index = 0x00;
			strcpy(scL_list,"?");
			while(sL_Index < mesl_NumParam)
			{
				if(sL_Index !=0x00)
					strcat(scL_list,"&");
				strcat(scL_list,meS_Param[sL_Index].mesc_ParamName);
				strcat(scL_list,"=");
				strcat(scL_list,meS_Param[sL_Index++].mesc_ParamValue);
			}
			strcat(mesc_URL,scL_list);
		}
		if(strlen(mesc_URL)>255)
		{
			printf("URL Size too big for GET request\n");
			strcpy(mesc_Error,"URL Size too big for GET request");
			return false;
		}
		curl_easy_setopt(mepv_Handle, CURLOPT_URL,mesc_URL);
		curl_easy_setopt(mepv_Handle, CURLOPT_ERRORBUFFER,mesc_Error);
		curl_easy_setopt(mepv_Handle, CURLOPT_NOPROGRESS, 1);
		//curl_easy_setopt(mepv_Handle, (CURLoption)CURLOPT_MUTE, 1);
		curl_easy_setopt(mepv_Handle, CURLOPT_WRITEFUNCTION, fnG_WriteData);
		if(u32L_Time != 0)
		{
			curl_easy_setopt(mepv_Handle, CURLOPT_TIMEOUT , u32L_Time);
			curl_easy_setopt(mepv_Handle, CURLOPT_CONNECTTIMEOUT , u32L_Time);
			curl_easy_setopt(mepv_Handle, CURLOPT_NOSIGNAL , 1);
		}
		mefn_SetOutputFileName(scL_fname);
		FILE* pL_outputfilefd = fopen(mesc_OutPutFileName,"w");
		if (pL_outputfilefd == NULL)
		{            
			return false;
		}
		curl_easy_setopt(mepv_Handle,CURLOPT_WRITEDATA ,pL_outputfilefd);
		if(curl_easy_perform(mepv_Handle)==0x00)
		{
			fclose(pL_outputfilefd);
			return true;
		}
		else
		{
			printf("Error = %s\n",mesc_Error);
			fclose(pL_outputfilefd);
			return false;
		}
	}
	else
	{
		printf("CURL not initialised\n");
		strcpy(mesc_Error,"CURL not initialised");
		return false;

	}
}

void STSHttp::mcfn_SetPostData(char* scL_pdata)
{	
	strcpy(mesc_postdata,scL_pdata);
}

bool STSHttp::mcfn_ExecutePOSTFile(char* scL_fname,unsigned int u32L_Time)
{
	if(mepv_Handle!=0x00)
	{
		if(mesc_postdata!=0x00)
			curl_easy_setopt(mepv_Handle, CURLOPT_POSTFIELDS,mesc_postdata);
		curl_easy_setopt(mepv_Handle,CURLOPT_URL,mesc_URL);
		curl_easy_setopt(mepv_Handle,CURLOPT_ERRORBUFFER,mesc_Error);
		curl_easy_setopt(mepv_Handle,CURLOPT_NOPROGRESS, 1);
		//curl_easy_setopt(mepv_Handle,(CURLoption)CURLOPT_MUTE, 1);
		curl_easy_setopt(mepv_Handle,CURLOPT_WRITEFUNCTION, fnG_WriteData);
		if(u32L_Time != 0)
		{
			curl_easy_setopt(mepv_Handle, CURLOPT_TIMEOUT , u32L_Time);
			curl_easy_setopt(mepv_Handle, CURLOPT_CONNECTTIMEOUT , u32L_Time);
			curl_easy_setopt(mepv_Handle, CURLOPT_NOSIGNAL , 1);
		}
		mefn_SetOutputFileName(scL_fname);
		FILE* pL_outputfilefd = fopen(mesc_OutPutFileName,"w");
		if (pL_outputfilefd == NULL)
		{            
			return false;
		}
		curl_easy_setopt(mepv_Handle,CURLOPT_WRITEDATA ,pL_outputfilefd);
		if(curl_easy_perform(mepv_Handle)==0x00)
		{
			fclose(pL_outputfilefd);
			return true;
		}
		else
		{
			printf("Error = %s\n",mesc_Error);
			fclose(pL_outputfilefd);
			return false;
		}
	}
	else
	{
		printf("CURL not initialised\n");
		strcpy(mesc_Error,"CURL not initialised");
		return false;
	}

}

bool STSHttp::mcfn_ExecuteGETBuffer(unsigned int u32L_Time)
{
	if(mepv_Handle!=0x00)
	{	
		if(mesl_NumParam >0x00)
		{
			char scL_list[1000];
			memset(scL_list,0x00,sizeof(scL_list));
			int sL_Index = 0x00;
			strcpy(scL_list,"?");
			while(sL_Index < mesl_NumParam)
			{
				if(sL_Index !=0x00)
					strcat(scL_list,"&");
				strcat(scL_list,meS_Param[sL_Index].mesc_ParamName);
				strcat(scL_list,"=");
				strcat(scL_list,meS_Param[sL_Index++].mesc_ParamValue);
			}
			strcat(mesc_URL,scL_list);
		}
		if(strlen(mesc_URL)>255)
		{
			printf("URL Size too big for GET request\n");
			strcpy(mesc_Error,"URL Size too big for GET request");
			return false;
		}
		curl_easy_setopt(mepv_Handle, CURLOPT_URL,mesc_URL);
		curl_easy_setopt(mepv_Handle, CURLOPT_ERRORBUFFER,mesc_Error);
		curl_easy_setopt(mepv_Handle, CURLOPT_NOPROGRESS, 1);
		//curl_easy_setopt(mepv_Handle, (CURLoption)CURLOPT_MUTE, 1);
		curl_easy_setopt(mepv_Handle, CURLOPT_WRITEFUNCTION, fnG_WriteDataToBuffer);
		if(u32L_Time != 0)
		{
			curl_easy_setopt(mepv_Handle, CURLOPT_TIMEOUT , u32L_Time);
			curl_easy_setopt(mepv_Handle, CURLOPT_CONNECTTIMEOUT , u32L_Time);
			curl_easy_setopt(mepv_Handle, CURLOPT_NOSIGNAL , 1);
		}
		if(meS_resp.pmesc_Buff != 0x00)
			free(meS_resp.pmesc_Buff);
		memset(&meS_resp,0x00,sizeof(meS_resp));
		curl_easy_setopt(mepv_Handle,CURLOPT_WRITEDATA ,(void*)&meS_resp);
		if(curl_easy_perform(mepv_Handle)==0x00)
		{
			return true;
		}
		else
		{
			printf("Error = %s\n",mesc_Error);
			return false;
		}
	}
	else
	{
		printf("CURL not initialised\n");
		strcpy(mesc_Error,"CURL not initialised");
		return false;

	}
}

bool STSHttp::mcfn_ExecutePOSTBuffer(unsigned int u32L_Time)
{
	if(mepv_Handle!=0x00)
	{
		if(mesc_postdata!=0x00)
			curl_easy_setopt(mepv_Handle, CURLOPT_POSTFIELDS,mesc_postdata);
		curl_easy_setopt(mepv_Handle,CURLOPT_URL,mesc_URL);
		curl_easy_setopt(mepv_Handle,CURLOPT_ERRORBUFFER,mesc_Error);
		curl_easy_setopt(mepv_Handle,CURLOPT_NOPROGRESS, 1);
		//curl_easy_setopt(mepv_Handle,(CURLoption)CURLOPT_MUTE, 1);
		curl_easy_setopt(mepv_Handle,CURLOPT_WRITEFUNCTION, fnG_WriteDataToBuffer);
		if(u32L_Time != 0)
		{
			curl_easy_setopt(mepv_Handle, CURLOPT_TIMEOUT , u32L_Time);
			curl_easy_setopt(mepv_Handle, CURLOPT_CONNECTTIMEOUT , u32L_Time);
			curl_easy_setopt(mepv_Handle, CURLOPT_NOSIGNAL , 1);
		}
		if(meS_resp.pmesc_Buff != 0x00)
			free(meS_resp.pmesc_Buff);
		memset(&meS_resp,0x00,sizeof(meS_resp));
		curl_easy_setopt(mepv_Handle,CURLOPT_WRITEDATA ,(void*)&meS_resp); 
		if(pmeS_headerList!=NULL)
		 curl_easy_setopt(mepv_Handle, CURLOPT_HTTPHEADER, pmeS_headerList);
		if(curl_easy_perform(mepv_Handle)==0x00)
		{		
			return true;
		}
		else
		{
			printf("Error = %s\n",mesc_Error);	  
			return false;
		}
	}
	else
	{
		printf("CURL not initialised\n");
		strcpy(mesc_Error,"CURL not initialised");
		return false;
	}

}

char* STSHttp::mcfn_GetResponseBuffer()
{
	if(meS_resp.pmesc_Buff)
		return meS_resp.pmesc_Buff;
	else
		return 0x00;
}

bool STSHttp::mcfn_setHeader(char* headerStr)
{
 	pmeS_headerList = curl_slist_append(pmeS_headerList, "Content-Type: application/json");
}
