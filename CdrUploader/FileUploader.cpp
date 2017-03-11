/*
 * FileProperties.cpp
 *
 *  Created on: Aug 30, 2012
 *      Author: ramesh
 */
#include "FileUploader.h"
#include "EventLog.h"
#include "DBPoolManager.h"
#include<dlfcn.h>

extern CEventLog CG_EventLog;
const char scG_Delimiter = '_';

int CFileUploader::mesl_UploaderCount = 0;
static FNVOID mcfn_processThread(void* pvL_This)
{
	try{
		(static_cast<CFileUploader*>(pvL_This))->mcfn_processFiles();
	}
	catch(std::bad_cast e){
		DBG_ERROR((CG_EventLog),("Unable to start the mcfn_processFiles function"));
	}
	return NULL;
}

CFileUploader::CFileUploader(char* DBIp,char* DBName,char* username,char* passwd,char* Function,char* Cols,char* FileInPath,\
		char* FileOutPath,char* FileErrorPath,char* FileRegExp,char* LineSep,char* FieldSep,bool ReplicationRequired)
{
	strcpy(pmesc_DBIp,DBIp);
	strcpy(pmesc_DBName,DBName);
	strcpy(pmesc_DBUser,username);
	strcpy(pmesc_DBPwd,passwd);
	strcpy(pmesc_Function,Function);
	strcpy(pmesc_Columns,Cols);
	strcpy(pmesc_FileInputPath,FileInPath);
	strcpy(pmesc_FileOutPath,FileOutPath);
	strcpy(pmesc_FileErrorOutPath,FileErrorPath);
	strcpy(pmesc_FileRegExp,FileRegExp);
	strcpy(pmesc_LineSep,LineSep);
	strcpy(pmesc_FieldSep,FieldSep);
	meb_ReplicationRequired = ReplicationRequired;
	mesl_UploaderCount++;
	mesl_UploaderNo = mesl_UploaderCount;
	mcfn_init();
	CSTSOSUtil::mcfn_beginAndDetachThread(meC_Thread,mcfn_processThread,this);
	sleep(1);
}

CFileUploader::~CFileUploader()
{
	//killing the thread
	pthread_cancel(meC_Thread);
	sleep(5);
}

void CFileUploader::mcfn_init()
{
	//Checking if the directories specified exists.
	namespace fs = boost::filesystem;
	fs::path someDir(pmesc_FileInputPath);
	fs::path OutDir(pmesc_FileOutPath);
	fs::path ErrorDir(pmesc_FileErrorOutPath);
	if(!(fs::exists(someDir) && fs::is_directory(someDir)))
	{
		DBG_INFO((CG_EventLog),("Directory '%s' doesn't exist, exiting",pmesc_FileInputPath));
		exit(1);
	}
	if(!(fs::exists(OutDir) && fs::is_directory(OutDir)))
	{
		DBG_INFO((CG_EventLog),("Directory '%s' doesn't exist, exiting",pmesc_FileOutPath));
		exit(1);
	}
	if(!(fs::exists(ErrorDir) && fs::is_directory(ErrorDir)))
	{
		DBG_INFO((CG_EventLog),("Directory '%s' doesn't exist, exiting",pmesc_FileErrorOutPath));
		exit(1);
	}

}
void CFileUploader::mcfn_processFiles()
{
	DBG_INFO((CG_EventLog),("mcfn_processFiles >>"));
	try{
		CDBPoolManager *pCL_PoolManager = new CDBPoolManager();
		DBG_INFO((CG_EventLog),("Addding the database"));
		//Add the Database to the connection pool object.
		//DBG_INFO((CG_EventLog),("Ip:%s,DB:%s,User:%s,Pwd:%s",pmesc_DBIp,pmesc_DBName,pmesc_DBUser,pmesc_DBPwd));
		pCL_PoolManager->mcfn_AddDB(pmesc_DBName,pmesc_DBIp,pmesc_DBUser,pmesc_DBPwd);
		//Assign an easier name for the file system name-space of the boost library.
		namespace fs = boost::filesystem;
		boost::regex_constants::syntax_option_type flags = boost::regex_constants::basic;
		boost::regex Expr;
		//Get the file regular expression from the configuration file.
		Expr.assign(pmesc_FileRegExp,flags);
		//Set the path to the directory where the files are present
		fs::path someDir(pmesc_FileInputPath);
		//Loading the function from the .so file
		//The 3 arguments for the below function are the file-name, table-name(will be returned in this argument) and the delimiter(defalut value '_')
		typedef void (*pfn_getTableName)(char*,char*,char);
		pfn_getTableName Funct;
		void* Handle = dlopen("./Table.so",RTLD_LAZY|RTLD_GLOBAL);
		if(Handle == NULL)
		{
			fprintf(stdout,"Error loading the dynamic library, Reason:%s",dlerror());
			exit(1);
		}
		Funct = (pfn_getTableName)dlsym(Handle,pmesc_Function);
		if(Funct == NULL)
		{
			fprintf(stdout,"Error loading the function, Reason:%s\n",dlerror());
			exit(1);
		}
		//Finished loading the pointer to the function from the .so file
		//Declaration of the iterator
		fs::directory_iterator itr;
		//Get a connection to the database
		int siL_idx = -1;
		mysqlpp::Connection *pcL_Conn = NULL;
		while(1)
		{
			pcL_Conn = pCL_PoolManager->mcfn_Grab(siL_idx);
			if(pcL_Conn == NULL)
			{
				DBG_ERROR((CG_EventLog),("No connection found to the database..Exiting"));
				sleep(1);
				//CDBPoolManager::mcfnS_GetInstance() -> mcfn_Release(pcL_Conn,siL_idx);
				continue;
			}
			break;
		}
		while(1)
		{
			try{
				//mcfn_init();
				//Iterate through each file.
				for(fs::directory_iterator file_itr(someDir);file_itr!=itr; ++file_itr)
				{
					//Check if the file is a regular file
					if(fs::is_regular_file(file_itr->status()))
					{
						fs::path p = file_itr->path();
						//checks if the current file matches the pattern in Expression
						if(!boost::regex_search(file_itr->path().filename().c_str(), Expr))
							continue;
						fileunit fu;
						fu.time = fs::last_write_time(file_itr->path());
						fu.path = file_itr->path();//.filename();
						//Insert the filename and the time-stamp into the list
						pmeC_ListOfFiles.push_back(fu);
					}
				}
				//Sorts the records in the list in the ascending order of their time-stamps
				pmeC_ListOfFiles.sort(mefn_mysort);
				if(pmeC_ListOfFiles.size() <= 0)
				{
					DBG_VERBOSE((CG_EventLog),("[U%d]No files in the directory,sleep for 1 secs",mesl_UploaderNo));
					sleep(1);
					continue;
				}

				//start processing the records in the list
				std::list<fileunit>::iterator pCL_itr = pmeC_ListOfFiles.begin();
				char pscL_TableName[100]={0x00};
				mysqlpp::Query CL_Query = pcL_Conn->query();
				while(pCL_itr != pmeC_ListOfFiles.end())
				{
					memset(pscL_TableName,0x00,sizeof(pscL_TableName));
					//Get the table name which is based on the file name.
					DBG_INFO((CG_EventLog),("[U%d]%s,%s",mesl_UploaderNo,pscL_TableName,(char*)pCL_itr->path.filename().c_str()));
					Funct((char*)pCL_itr->path.filename().c_str(),pscL_TableName,scG_Delimiter);
					DBG_INFO((CG_EventLog),("[U%d]%s,%s",mesl_UploaderNo,pscL_TableName,(char*)pCL_itr->path.filename().c_str()));
					CL_Query.reset();
					if(strlen(pmesc_Columns))
						CL_Query<<"load data local infile '"<<pCL_itr->path.c_str()<<"' into table "<<pscL_TableName<<" fields terminated by '"<<pmesc_FieldSep<<"' lines terminated by '"<<pmesc_LineSep<<"' ("<<pmesc_Columns<<") ";//set CREATE_TIME = now()";//str_to_date('"<<date<<"','%Y_%m_%d_%H_%i_%s')";
					else
						CL_Query<<"load data local infile '"<<pCL_itr->path.c_str()<<"' into table "<<pscL_TableName<<" fields terminated by '"<<pmesc_FieldSep<<"' lines terminated by '"<<pmesc_LineSep<<"'";
					DBG_INFO((CG_EventLog),("[U%d]Query:%s",mesl_UploaderNo,CL_Query.str().c_str()));
					mysqlpp::SimpleResult CL_Res;
					timeval start_time;
					gettimeofday(&start_time,NULL);
					CL_Res = CL_Query.execute();
					if(!CL_Res)
					{
						pCL_PoolManager->mcfn_removeConn(pcL_Conn,siL_idx);
						while(1)
						{
							pcL_Conn = pCL_PoolManager->mcfn_Grab(siL_idx);
							if(pcL_Conn == NULL)
							{
								DBG_ERROR((CG_EventLog),("No connection found to the database..Exiting"));
								sleep(1);
								//CDBPoolManager::mcfnS_GetInstance() -> mcfn_Release(pcL_Conn,siL_idx);
								continue;
							}
							CL_Query = pcL_Conn->query();
							break;
						}
						CL_Res = CL_Query.execute();
					}
					/*while(!(CL_Res=CL_Query.execute()))
					{
						DBG_ERROR((CG_EventLog),("[%d][%d]Unable to upload the file into the DB..retrying:%s",CL_Query.errnum(),mesl_UploaderNo,CL_Query.error()));
						DBG_INFO((CG_EventLog),("Query:%s",CL_Query.str().c_str()));
						if(CL_Query.errnum() >= 2000)
						{
							while(1)
							{
								DBG_INFO((CG_EventLog),("[%d]Mysqlserver connection gone away, Reconnecting",mesl_UploaderNo));
								pCL_PoolManager -> mcfn_Release(pcL_Conn,siL_idx);
								pcL_Conn = pCL_PoolManager->mcfn_Grab(siL_idx);
								if(pcL_Conn == NULL)
								{
									DBG_ERROR((CG_EventLog),("[%d]No connection found to the database..Trying again",mesl_UploaderNo));
									sleep(3);
									continue;
								}
								CL_Query = pcL_Conn->query();
								break;
							}
							continue;
						}
						if(CL_Query.errnum() == 2)
							break;
						break;
						sleep(1);
					}*/
					timeval end_time;
					gettimeofday(&end_time,NULL);
					char pscL_Dest[500] = {0x00};
					char pscL_Ext[500]={0x00};
					char pscL_ChangedFileName[150]={0x00};
					if(!CL_Res)
					{
						//if replication is required, copy it to the errorpath and move it to the source path of the replicating uploader
						if(meb_ReplicationRequired){
							DBG_ERROR((CG_EventLog),("[U%d]File '%s' uploading is failed, Replication required, hence copying to the error location,Reason:[%d]%s",mesl_UploaderNo,pCL_itr->path.c_str(),CL_Query.errnum(),CL_Query.error()));
							sprintf(pscL_Dest,"%s/%s",pmesc_FileErrorOutPath,pCL_itr->path.filename().c_str());
							fs::copy_file(pCL_itr->path.c_str(),pscL_Dest,fs::copy_option::overwrite_if_exists);
						}
						else{
							DBG_ERROR((CG_EventLog),("[U%d]File '%s' uploading is failed, moving it to error location,Reason:[%d]%s",mesl_UploaderNo,pCL_itr->path.c_str(),CL_Query.errnum(),CL_Query.error()));
							sprintf(pscL_Dest,"%s/%s",pmesc_FileErrorOutPath,pCL_itr->path.filename().c_str());
							fs::rename(pCL_itr->path.c_str(),pscL_Dest);
							++pCL_itr;
							continue;
						}
					}
					DBG_INFO((CG_EventLog),("[U%d]TimeLapsed:%u.%u,No of rows updated:%d",mesl_UploaderNo,end_time.tv_sec-start_time.tv_sec,\
														end_time.tv_usec-start_time.tv_usec,CL_Res.rows()));
					//FileName is changed
					sprintf(pscL_ChangedFileName,"%s.ext",pCL_itr->path.filename().c_str());
					//InputPath+ChangedFileName
					sprintf(pscL_Ext,"%s/%s",pmesc_FileInputPath,pscL_ChangedFileName);
					//OutputPath+ChangedFileName
					sprintf(pscL_Dest,"%s/%s",pmesc_FileOutPath,pscL_ChangedFileName);
					fs::rename(pCL_itr->path.c_str(),pscL_Ext);
					fs::rename(pscL_Ext,pscL_Dest);
					DBG_INFO((CG_EventLog),("[U%d]Moved the file '%s' to destination folder %s",mesl_UploaderNo,pscL_Ext,pscL_Dest));
					++pCL_itr;
				}
				pmeC_ListOfFiles.clear();
			}//try
			catch(fs::filesystem_error e){
				DBG_CRITICAL((CG_EventLog),("[U%d]Exception occurred:%s",mesl_UploaderNo,e.what()));
				pmeC_ListOfFiles.clear();
				sleep(1);
				continue;
			}
			catch(...){
				DBG_CRITICAL((CG_EventLog),("[U%d]Unknown exception caught",mesl_UploaderNo));
				pmeC_ListOfFiles.clear();
				sleep(1);
				continue;
			}
		}//while(1)
	}//try
	catch(fs::filesystem_error e){
		DBG_CRITICAL((CG_EventLog),("Exception occurred:%s",e.what()));
		exit(1);
	}
	catch(...){
		DBG_CRITICAL((CG_EventLog),("Unknown exception caught"));
		exit(1);
	}
}
