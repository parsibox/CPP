#include "STSPGSet.h"

CSTSPGSet::CSTSPGSet(char* pscL_hostAddr, char* pscL_dbName, char* pscL_user, char* pscL_password)
{
	mesl_INFieldCount=0;
	mesl_OUTFieldCount=0;
	meb_isExConn=false;
	memset(pmesc_ConnStr, 0x00, sizeof(pmesc_ConnStr));
	meCL_res=NULL;
	if(pscL_hostAddr==NULL || pscL_dbName==NULL || pscL_user==NULL || pscL_password==NULL )
        {
		meCL_conn=NULL;
        }
	else
	{
		mefn_CreateConnStr(pscL_hostAddr, pscL_dbName, pscL_user, pscL_password);
		//fprintf(stdout, "\nIn constructor: Connection String: [%s]\n", pmesc_ConnStr);
		meCL_conn=PQconnectdb(pmesc_ConnStr);
		if(PQstatus(meCL_conn)!=CONNECTION_OK)
                {
                        fprintf(stdout, "\nConnection to Database failed:\nReason>>%s", PQerrorMessage(meCL_conn));
                        meCL_conn=NULL;
                }
	}
}
CSTSPGSet::CSTSPGSet(CPGConnection* pCL_temp)
{
	meCL_conn=pCL_temp->meCL_conn;
	meb_isExConn=true;
}

CSTSPGSet::~CSTSPGSet()
{
	mcfn_close();
}

void CSTSPGSet::mefn_CreateConnStr(char* pscL_hostAddr, char* pscL_dbName, char* pscL_user, char* pscL_password)
{
	if(pscL_hostAddr==NULL || pscL_dbName==NULL || pscL_user==NULL || pscL_password==NULL )
	{

		sprintf(pmesc_ConnStr, "hostaddr=%s dbname=%s user=%s password=%s", \ 
				mdfn_getServerName(), mdfn_getSID(),  \
				mdfn_getUserName(), mdfn_getPassword());
	}
	else
	{
		sprintf(pmesc_ConnStr, "hostaddr=%s dbname=%s user=%s password=%s", pscL_hostAddr, pscL_dbName,  \
				pscL_user, pscL_password);
	}
}

bool CSTSPGSet::mcfn_open()
{
	bool bL_returnValue=true;
	if(meCL_conn==NULL)
	{
		mefn_CreateConnStr();
		//fprintf(stdout, "\nConnection String: [%s]\n", pmesc_ConnStr);
		meCL_conn=PQconnectdb(pmesc_ConnStr);
		if(PQstatus(meCL_conn)!=CONNECTION_OK)
		{
			fprintf(stdout, "\nConnection to Database failed:\nReason>>%s", PQerrorMessage(meCL_conn));
			bL_returnValue=false;
		}
		
	}
	if(bL_returnValue)
	{
		mdfn_getINParamCount(mesl_INFieldCount);
                mdfn_getOUTParamCount(mesl_OUTFieldCount);
		mefn_prepareToExecute();
		mdfn_bindColumns();
	}
	return bL_returnValue;
}

void CSTSPGSet::mefn_prepareToExecute()
{
	pmesc_paramValues=new char*[mesl_INFieldCount];
	pmesi_paramLengths=new int[mesl_INFieldCount];
	pmesi_paramFormats=new int[mesl_INFieldCount];
	mesi_CurrRowNum=-1;	
}

bool CSTSPGSet::mcfn_execute()
{
	
	bool bL_returnValue=true;
	char pscL_SQLString[MAX_SQLQUERY_SIZE];
	memset(pscL_SQLString, 0x00, sizeof(pscL_SQLString));
	int siL_DBOperation=mesi_DBOperation;
	//mdfn_getDBOperation(siL_DBOperation);
	
	char pscL_TempSQLString[MAX_SQLQUERY_SIZE];

	mdfn_getSQLString(pscL_TempSQLString);
	//mdfn_replace(pscL_TempSQLString, pscL_SQLString);
	strcpy(pscL_SQLString,pscL_TempSQLString);
	//fprintf(stdout, "Sql String: %s\n", pscL_SQLString);
	if(pscL_SQLString[0]== '\0')
		throw STSGenException(2001, "Cannot execute null query string\n"); 
	
	if(meCL_conn== NULL)
                throw STSGenException(2006,"Used without opening\n");

	//PQsetnonblocking(meCL_conn, 1);	
	if(PQstatus(meCL_conn)!=CONNECTION_OK)
		return false;
	meCL_res = PQexecParams(meCL_conn,  pscL_SQLString, mesl_INFieldCount, NULL, pmesc_paramValues,  \
			pmesi_paramLengths, pmesi_paramFormats, TYPE_BINARY);

	switch(PQresultStatus(meCL_res))
	{
		case PGRES_EMPTY_QUERY:
		case PGRES_COPY_OUT:                        
		case PGRES_BAD_RESPONSE:                    
		case PGRES_FATAL_ERROR: 
			fprintf(stdout, "Query Execution failed: %s\n",PQerrorMessage(meCL_conn));
			if(meCL_res!=NULL)
			{
				PQclear(meCL_res); 
				meCL_res=NULL;
			}
			bL_returnValue=false;
	}	
	return bL_returnValue;
}

void CSTSPGSet::mcfn_close()
{
	delete[] pmesc_paramValues;
	delete[] pmesi_paramFormats;
	delete[] pmesi_paramLengths;
	pmesc_paramValues=NULL;
	pmesi_paramFormats=NULL;
	pmesi_paramLengths=NULL;
	mesi_CurrRowNum=-1;
	mesl_INFieldCount=0;
	mesl_OUTFieldCount=0;	

	if(meCL_res!=NULL)
	{
                PQclear(meCL_res);
                meCL_res=NULL;
        }
	
	if(meCL_conn!=NULL && !meb_isExConn)
	{
		PQfinish(meCL_conn);
		meCL_conn=NULL;
	}
	
/*	if(meCL_res!=NULL)
	{
		PQclear(meCL_res);
		meCL_res=NULL;	
	}*/
}

void CSTSPGSet::mdfn_bindColumn(void *pL_param, int siL_ParamLen, int siL_ParamType, short siL_ParamPos, short siL_mode)
{
	if(siL_mode==INPUT_PARAMETER)
	{
		
		if(mesl_INFieldCount==0)
			throw STSGenException(2002, "Input field count not set\n");
		
		if(siL_ParamPos<0 || siL_ParamPos>mesl_INFieldCount)
			throw STSGenException(2003, "Input field position is out of bound\n");
		
		pmesc_paramValues[siL_ParamPos]=new char[siL_ParamLen];
		memset(pmesc_paramValues[siL_ParamPos], 0x00, siL_ParamLen);
		
		if(siL_ParamType==TYPE_BINARY)
		{
			for(int siL_j=siL_ParamLen-1, siL_k=0; siL_j>=0 ;siL_j--, siL_k++)
                        {
                                memcpy(pmesc_paramValues[siL_ParamPos]+siL_k, (char*)pL_param+siL_j, 1);
                        }
		}
		if(siL_ParamType==TYPE_TEXT || siL_ParamType==TYPE_DATE)
		{
			strcpy(pmesc_paramValues[siL_ParamPos], (char*)pL_param);
		}

		pmesi_paramLengths[siL_ParamPos]=siL_ParamLen;
		if(siL_ParamType==TYPE_DATE)
			pmesi_paramFormats[siL_ParamPos]=0;
		else
		pmesi_paramFormats[siL_ParamPos]=siL_ParamType;
	}
	if(siL_mode==OUTPUT_PARAMETER)
	{
		
		if(mesl_OUTFieldCount==0)
			throw STSGenException(2004, "Output field count not set\n");

		if( siL_ParamPos<0 || siL_ParamPos> mesl_OUTFieldCount)
			throw STSGenException(2005, "Output field position is out of bound\n");

		pme_OutParam[siL_ParamPos]=pL_param;
		pmesi_OutParamLength[siL_ParamPos]=siL_ParamLen;
		pmesi_OutParamType[siL_ParamPos]=siL_ParamType;	
	}
	
}

bool CSTSPGSet::mefn_fetch(int siL_direction)
{

	mesi_NumOfTuples=PQntuples(meCL_res);
	switch(siL_direction)
	{
		case PG_FETCH_LAST:
			mesi_CurrRowNum=mesi_NumOfTuples-1;
			break;
		case PG_FETCH_FIRST:
			mesi_CurrRowNum=0;
			break;
		case PG_FETCH_PREV:
			mesi_CurrRowNum>=0?mesi_CurrRowNum--:mesi_CurrRowNum;
			break;
		case PG_FETCH_NEXT:
			mesi_CurrRowNum<mesi_NumOfTuples?mesi_CurrRowNum++:mesi_CurrRowNum;
			break;
	}	
	if(mesi_CurrRowNum<0 || mesi_CurrRowNum>=mesi_NumOfTuples)
	{
//		fprintf(stdout, "\n*********End of ResultSet\n");
		return false;
	}

	char *pscL_temp;
	for(int siL_i=0; siL_i<mesl_OUTFieldCount; siL_i++)
	{
		pscL_temp= PQgetvalue(meCL_res, mesi_CurrRowNum, siL_i);
		int siL_len=PQgetlength(meCL_res, mesi_CurrRowNum, siL_i);	
		if(pscL_temp == NULL)
		{
			return false;
		}
		
		if(pmesi_OutParamType[siL_i]==TYPE_TEXT)
		{
			strcpy((char*)pme_OutParam[siL_i],pscL_temp);
		}
		
		if(pmesi_OutParamType[siL_i]==TYPE_BINARY)
		{
			for(int siL_j=pmesi_OutParamLength[siL_i]-1, siL_k=0; siL_j>=0 ;siL_j--, siL_k++)
			{
				memcpy((char*)(pme_OutParam[siL_i])+siL_k, pscL_temp+siL_j, 1);
			}
		}
		
	}
	return true;
}
void CSTSPGSet::mdfn_replace(const char* pscL_in, char* pscL_out)
{
	if(mesl_INFieldCount==0)
	{
		strcpy(pscL_out, pscL_in); return;
	}
        char pscL_tempStr[1000];
        strcpy(pscL_tempStr, pscL_in);
        char *pscL_temp1;
        char *pscL_temp2;
        char pscL_insStr[10];
        int siL_i=1;
	int siL_j=0;
        while(pscL_temp1=index(pscL_tempStr+siL_j,':'))
        {
		
		siL_j=(pscL_temp1-pscL_tempStr)+1;

		while(pscL_tempStr[siL_j]!=0x00 && strchr("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_1234567890", pscL_tempStr[siL_j]))
			siL_j++;
		pscL_temp2=pscL_tempStr+siL_j;

                *pscL_temp1=0x00;
		if(pmesi_paramFormats[siL_i-1]==TYPE_DATE)
		{
			sprintf(pscL_insStr, "$%d::timestamp", siL_i);
		}
		else
			sprintf(pscL_insStr, "$%d", siL_i);
                siL_i++;
                strcat(pscL_tempStr, pscL_insStr);
                strcat(pscL_tempStr, pscL_temp2);
        }
        strcpy(pscL_out, pscL_tempStr);
}
