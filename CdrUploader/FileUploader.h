/* FileProperties.h
 * 	created by Ramesh Kumar on 30 AUG 2012
 * 	Version: 1.0.0.1 Changes: Initial version
 */

#ifndef __FILEPROP_
#define __FILEPROP_

#include <list>
#include <filesystem/v3/convenience.hpp>
#include <regex.hpp>
#include<STSOSUtil.h>

namespace fs = boost::filesystem;

struct fileunit{
	long time;
	boost::filesystem3::path path;
};
class CFileUploader
{
	//members containing Database details
	static int mesl_UploaderCount;
	int mesl_UploaderNo;
	char pmesc_DBIp[50];
	char pmesc_DBName[50];
	char pmesc_DBUser[50];
	char pmesc_DBPwd[50];
	//members containing Table Details
	char pmesc_Columns[2000];
	//members containing File Details
	char pmesc_FileInputPath[500];
	char pmesc_FileOutPath[500];
	char pmesc_FileErrorOutPath[500];
	char pmesc_FileRegExp[50];
	char pmesc_LineSep[20];
	char pmesc_FieldSep[20];
	//Replication Details
	bool meb_ReplicationRequired;
	char pmesc_ReplicationPath[500];
	//function name for each table-name framing
	char pmesc_Function[50];
	//data member containing the filenames in a list.
	std::list<fileunit> pmeC_ListOfFiles;
	THREAD_HANDLE meC_Thread;
	//Private Functions
public:
	CFileUploader(char* DBIp,char* DBName,char* username,char* passwd,char* Table,char* Cols,char* FileInPath,\
						char* FileOutPath,char* FileErrorPath,char* FileRegExp,char* LineSep,char* FieldSep,bool ReplicationRequired);
	~CFileUploader();
	void mcfn_init();
	void mcfn_processFiles();
};

inline
bool mefn_mysort(fileunit a, fileunit b){
	if(a.time > b.time)
		return false;
	else
		return true;
}

#endif
