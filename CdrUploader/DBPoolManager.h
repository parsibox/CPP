/*
 * DBPoolManager.h
 *
 *  Created on: Jun 17, 2010
 *      Author: sunil
 */


#ifndef DBPOOLMANAGER_H_
#define DBPOOLMANAGER_H_
#include "DBConnPool.h"
#include "CriticalSection.h"
//Single tone class for DB Pool manager.

typedef std::vector<CDBConnPool*> POOL_LIST;
class CDBPoolManager{
private:
	POOL_LIST	meC_ListOfDBPools;
	//static CDBPoolManager * pmeC_Instance;
	int mesi_CurrentActiveIdx;
	CCriticalSection meC_CriticalSection;
public:
	//Singleton implementation of the class: DONOT MOVE TO PUBLIC AREA
	CDBPoolManager();
	void mcfn_removeConn(mysqlpp::Connection *pCL_Conn, int siL_PoolIdx);
	mysqlpp::Connection *mcfn_Grab(int& siLo_Idx);
	//Function to return mysqlpp where index is not required: Just to keep coding handy by not passing int :)
//	mysqlpp::Connection *mcfn_Grab(){
//		int siL_Temp = -1;
//		return mcfn_Grab(siL_Temp);
//	}
	mysqlpp::Connection *mcfn_GrabByIdx(int siL_Idx);
	void 				 mcfn_Release(const mysqlpp::Connection *pCL_Conn, int siL_PoolIdx);
	void				 mcfn_AddDB(const char *pscL_DBName, const char* pscL_Ip, const char* pscL_User, const char* pscL_Pass);
	//static CDBPoolManager *mcfnS_GetInstance();
	int	mcfn_GetNumberOfPools() const;
	std::string mcfn_LogString();
	std::list<int> mcfn_GetAllIdxOtherThanThis(int siL_Idx);

	//Added for VodafoneMega, To handle more than three DBs.
	std::list<int> mcfn_GetAllIdxFrmListOtherThanThis(std::list<int>& L_List, int siL_Idx);
	mysqlpp::Connection *mcfn_GrabFrmListOfIdx(std::list<int> L_ListOfAvailableDBIdx, int& siL_Idx);	

};

#endif /* DBPOOLMANAGER_H_ */
