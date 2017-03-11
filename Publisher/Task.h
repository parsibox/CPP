#ifndef __TASK_H_
#define __TASK_H_
#include<string>
#include<vector>
#include"DeliverToEsme.pb.h"
#include"DBOperations.h"

typedef enum __SubtaskStatus{
	STATUS_NONE=0,
	STATUS_ASSIGNED=1,
	STATUS_EXICUTING=2,
	STATUS_COMPLETED=3
}SUBTASK_STATUS;


class CSubTask{
	private:
		std::string meC_tableName;
		SUBTASK_STATUS meE_status;
	public:
		CSubTask(std::string CL_tableName){
			meC_tableName=CL_tableName;
			meE_status=STATUS_NONE;
		}
		SUBTASK_STATUS mcfn_getStatus(){
			return meE_status;
		}
		void mcfn_setStatus(SUBTASK_STATUS eL_status){
			meE_status=eL_status;
		}
		std::string mcfn_getTableName(){
			//DBG_DEBUG((CG_EventLog), ("Returning TableName:%s",meC_tableName.c_str()));	
			return meC_tableName.c_str();
		}
		
};

class CTask{
	private:
		bool  meb_isPause;
		int mesi_taskId;
		int mesi_msgType;
		int mesi_expiryTime;
		int mesi_langId;
		int mesi_regDel;
		int mesi_interfaceId; 
		int mesi_channelId;
		int mesi_dcs;
		std::string meC_msg;
		std::string meC_taskName;
		std::string meC_srvCode;
		std::string meC_OA;
		std::string meC_createdBy;
		SUBTASK_STATUS meE_globalStatus;
		std::vector<CSubTask *> meC_subTasks;	
	public:
		CTask(int iL_taskId,int iL_msgType,int iL_expiryTime,int iL_langId,int iL_regDel,int iL_interfaceId,std::string CL_msg,std::string CL_taskName,std::string CL_srvCode,bool bL_isPause,int iL_channelId,int iL_dcs,std::string CL_OA,std::string CL_createdBy){
			meb_isPause=bL_isPause;
			mesi_taskId=iL_taskId;
			mesi_msgType=iL_msgType;
			mesi_expiryTime=iL_expiryTime;
			mesi_langId=iL_langId;
			mesi_regDel=iL_regDel;
			mesi_interfaceId=iL_interfaceId;
			meC_msg=CL_msg;
			meC_taskName=CL_taskName;
			meC_srvCode=CL_srvCode;
			meE_globalStatus=STATUS_NONE;
			meC_OA=CL_OA;
			meC_createdBy=CL_createdBy;
			mesi_channelId=iL_channelId;
			mesi_dcs=iL_dcs;
		}
		~CTask();
		void mcfn_setTaskStatus(SUBTASK_STATUS eL_status){
			meE_globalStatus=eL_status;
		}
		
		SUBTASK_STATUS mcfn_getTaskStatus(){
			return meE_globalStatus;
		}
		
		bool mcfn_Update(CTask *pCL_updateTask){
			/*meb_isPause=bL_isPause;
                        mesi_taskId=iL_taskId;
                        mesi_msgType=iL_msgType;
                        mesi_expiryTime=iL_expiryTime;
                        mesi_langId=iL_langId;
                        mesi_regDel=iL_regDel;
                        mesi_interfaceId=iL_interfaceId;
                        meC_msg=CL_msg;
                        meC_taskName=CL_taskName;
                        meC_srvCode=CL_srvCode;*/
		}
		int mcfn_TaskId(){
			return mesi_taskId;
		}
		bool mcfn_loadSubTask();

		bool mcfn_isPause();
		bool mcfn_checkAssignment();	
		bool mcfn_isExpired(){
			(mesi_expiryTime>time(NULL))?false:true;
		}
		bool mcfn_getAssignedSubTask(CSubTask *&);
		void mcfn_setValues(DeliverToEsme *pCL_msg){
			pCL_msg->set_task_id(mesi_taskId);
			pCL_msg->set_oa(meC_OA);
			pCL_msg->set_msg(meC_msg);
			pCL_msg->set_interface_id(mesi_interfaceId);
			pCL_msg->set_reg_del(mesi_regDel);
			pCL_msg->set_channel_id(mesi_channelId);
			pCL_msg->set_expiry_time(mesi_expiryTime);
			pCL_msg->set_pid(0);
			pCL_msg->set_dcs(mesi_dcs);//as per the message need to set this value
			pCL_msg->set_lang_id(mesi_langId);
			pCL_msg->set_task_name(meC_taskName);
			pCL_msg->set_msg_type(mesi_msgType);
			pCL_msg->set_srv_code(meC_srvCode);
			pCL_msg->set_username(meC_createdBy);
		}
		bool mcfn_updateTaskTableMap(int,char,std::string);
		bool mcfn_getAllSubTasksCompleted();
};
#endif
