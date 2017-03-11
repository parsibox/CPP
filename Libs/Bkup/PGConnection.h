#ifndef _PGCONNECTION_
#define _PGCONNECTION_
#include "libpq-fe.h"
#include "string.h" 
class CPGConnection
{
	public:
		PGconn *meCL_conn;
		CPGConnection();
		virtual ~CPGConnection();
		bool  mcfn_initPG(const char*, const char*, const char*, const char*);
};
#endif
