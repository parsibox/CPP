#ifndef __DBCONNPOOL_H
#define __DBCONNPOOL_H

#include <mysql++.h>
#include <EventLog.h>
#include "ConfigParams.h"
extern CEventLog CG_EventLog;
extern CConfigParams CG_Cfg;

// Define a concrete ConnectionPool derivative.  Takes connection
// parameters as inputs to its ctor, which it uses to create the
// connections we're called upon to make.  Note that we also declare
// a global pointer to an object of this type, which we create soon
// after startup; this should be a common usage pattern, as what use
// are multiple pools?
class CDBConnPool : public mysqlpp::ConnectionPool
{
public:
    // The object's only constructor
    CDBConnPool(const char* db, const char* server,
            const char* user, const char* password) :
            	db_(db ? db : ""),
            	server_(server ? server : ""),
            	user_(user ? user : ""),
            	password_(password ? password : "")
    {
    	//intentionally left blank
    }

    // The destructor.  We _must_ call ConnectionPool::clear() here,
    // because our superclass can't do it for us.
    ~CDBConnPool()
    {
        clear();
    }

    int mcfn_Size() const{
    	return size();
    }
    std::string mcfn_LogString(){
    	return  "DBName:" + db_ + ",ServerIp:" + server_ + ",User:" + user_ + ",Password:" + password_;
    }
protected:
    // Superclass overrides
    mysqlpp::Connection* create()
    {
        // Create connection using the parameters we were passed upon
        // creation.  This could be something much more complex, but for
        // the purposes of the example, this suffices.
        DBG_VERBOSE((CG_EventLog), ("Creating connection object1"));
//        mysqlpp::Connection *pCL_Conn = new mysqlpp::Connection(false);
	mysqlpp::Connection *pCL_Conn = NULL;
	try{
        pCL_Conn = new mysqlpp::Connection(
                db_.empty() ? 0 : db_.c_str(),
                server_.empty() ? 0 : server_.c_str(),
                user_.empty() ? 0 : user_.c_str(),
                password_.empty() ? "" : password_.c_str());
//        if(!pCL_Conn->connect(db_.empty() ? 0 : db_.c_str(),
//                server_.empty() ? 0 : server_.c_str(),
//                user_.empty() ? 0 : user_.c_str(),
//                password_.empty() ? "" : password_.c_str())){
//        	return NULL;
//        }
//	mysqlpp::MultiStatementsOption* opt = new mysqlpp::MultiStatementsOption(true);
//	pCL_Conn->set_option(opt);
	if(pCL_Conn)
		pCL_Conn->disable_exceptions();
	}catch(mysqlpp::Exception e){
		DBG_ERROR((CG_EventLog), ("Unable to connect to server:%s", e.what()));
		delete pCL_Conn;
		pCL_Conn = NULL;
	}
        return pCL_Conn;
    }

    void destroy(mysqlpp::Connection* cp)
    {
        // Our superclass can't know how we created the Connection, so
        // it delegates destruction to us, to be safe.
        DBG_INFO((CG_EventLog), ("Destroying the db connection1"));
        delete cp;
    }

    unsigned int max_idle_time()
    {
        // Set our idle time at an example-friendly 3 seconds.  A real
        // pool would return some fraction of the server's connection
        // idle timeout instead.
        return 86400 * 4;
    }

private:
    // Our connection parameters
    std::string db_, server_, user_, password_;
};

#endif
