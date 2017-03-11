/*
 * DBOperations.h
 *
 *  Created on: Aug 28, 2013
 *      Author: surya
 */

#ifndef DBOPERATIONS_H_
#define DBOPERATIONS_H_

#include<string>
#include"DBPoolManager.h"
#define SUCCESS 0
#define TABLE_NOT_EXIST 1146 
class CDBOperations {
public:
	static int mcfn_ExecuteQuery(std::string psL_Query);
	static int mcfn_ExecuteQuery(std::string psL_Query,mysqlpp::StoreQueryResult &CL_Result);
	static std::string mcfn_ChangeIgnoreLetters(char * pscL_InputTemp);
};

#endif /* DBOPERATIONS_H_ */
