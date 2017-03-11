#ifndef _STSPGSET_H_
#define _STSPGSET_H_


#include "libpq-fe.h"
#include "stdlib.h"
#include "string.h"
#include "STSException.h"
#include "PGConnection.h"
#define MAX_SQLQUERY_SIZE	1000
#define SIZE_CONNSTRING 	150
#define TYPE_TEXT 		0
#define TYPE_BINARY 		1
#define TYPE_DATE		2
#define INPUT_PARAMETER 	1
#define OUTPUT_PARAMETER 	0

#define SQLT_INT 		1
#define SQLT_BOOL	 	1
#define SQLT_FLOAT 		1
#define SQLT_STR		0
#define SQLT_CHR		1

#define PG_FETCH_NEXT   1
#define PG_FETCH_PREV	2
#define PG_FETCH_FIRST  3
#define PG_FETCH_LAST	4


class CSTSPGSet
{

protected:
	virtual void  mdfn_getINParamCount(int &) = 0;
        virtual void  mdfn_getOUTParamCount(int &) = 0;
	virtual const char *mdfn_getUserName() const = 0;
        virtual const char *mdfn_getPassword() const = 0;
        virtual const char *mdfn_getServerName() const = 0;
        virtual const char *mdfn_getSID() const = 0;
//	virtual void  mdfn_getDBOperation(int &)=0;
	virtual void mdfn_bindColumns() = 0;
	virtual void mdfn_getSQLString(char *pscL_Str) = 0;
        void mdfn_bindColumn(void *, int , int,short,short int);
	void mdfn_replace(const char* pscL_in, char* pscL_out);
private:
	//CPGConnection *pmeCL_conn;
        PGconn *meCL_conn;
        PGresult *meCL_res;
        char pmesc_ConnStr[SIZE_CONNSTRING];
	bool meb_isExConn;
	int  mesl_INFieldCount;
        int  mesl_OUTFieldCount;
        int  mesi_CurrRowNum;
	int  mesi_DBOperation;
	int  mesi_NumOfTuples;
        char **pmesc_paramValues;
        int         *pmesi_paramLengths;
        int         *pmesi_paramFormats;
        int          mesi_resultFormat;
        void *       pme_OutParam[100];
	int 	     pmesi_OutParamType[100];
	int 	     pmesi_OutParamLength[100];
        void mefn_CreateConnStr(char* =NULL, char* =NULL, char* =NULL, char* =NULL);
        void mefn_prepareToExecute();
	bool mefn_fetch(int);
	bool mefn_makeConnection(char*);
public:
     	void mcfn_setConn(PGconn *);
	bool mcfn_open();
        bool mcfn_execute();
        bool mcfn_moveNext();
        bool mcfn_movePrev();
	bool mcfn_moveFirst();
	bool mcfn_moveLast();
	void mcfn_close();
	void mcfn_setDBOperation(int);
	CSTSPGSet(char* =NULL, char* =NULL, char* =NULL, char* =NULL);
        CSTSPGSet(CPGConnection * = NULL);
        ~CSTSPGSet();
};

inline
void CSTSPGSet::mcfn_setDBOperation(int siL_Value)
{

	mesi_DBOperation = siL_Value;
}
inline
bool CSTSPGSet::mcfn_moveNext()
{
	return mefn_fetch(PG_FETCH_NEXT);
}

inline
bool CSTSPGSet::mcfn_movePrev()
{
	return mefn_fetch(PG_FETCH_PREV);
}

inline
bool CSTSPGSet::mcfn_moveLast()
{
	return mefn_fetch(PG_FETCH_LAST);
}

inline
bool CSTSPGSet::mcfn_moveFirst()
{
	return mefn_fetch(PG_FETCH_FIRST);
}
#endif

