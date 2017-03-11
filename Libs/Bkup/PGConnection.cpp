#include "PGConnection.h"
CPGConnection::CPGConnection()
{
                meCL_conn=NULL;
}

bool CPGConnection::mcfn_initPG(const char* pscL_hostAddr, const char* pscL_dbName, const char* pscL_user, const char* pscL_password)
{
	//printf("%s--%s--%s--%s\n",pscL_hostAddr,pscL_dbName,pscL_user,pscL_password);
	//puts("00000000");
	char pscL_ConnStr[500];
	pscL_ConnStr[0]='\0';
	if(meCL_conn==NULL)
	{
	      sprintf(pscL_ConnStr, "hostaddr=%s dbname=%s user=%s password=%s", pscL_hostAddr, pscL_dbName,  \
                                pscL_user, pscL_password);
//		puts("***********************!!!!!");
//		puts(pscL_ConnStr);
		meCL_conn=PQconnectdb(pscL_ConnStr);
                if(PQstatus(meCL_conn)!=CONNECTION_OK)
                {
                        fprintf(stdout, "\nConnection to Database failed:\nReason>>%s", PQerrorMessage(meCL_conn));
                        meCL_conn=NULL;
			return false;
                }
		else
		{
//			puts("Connection to database succeed");
			return true;
		}
	}
	return true;	
}
CPGConnection::~CPGConnection()
{
        if(meCL_conn!=NULL)
        {
                PQfinish(meCL_conn);
		meCL_conn=NULL;
        }
}
