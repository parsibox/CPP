#!/bin/bash

db_ip=127.0.0.1
db_user=cmsuser
db_pwd=cmsuser
db_name=CMS_USSD

echo "Welcome to scriptUser to create new task"
echo "Schdule Name:"
read scheduleName

echo "Select From the below list of options:"
echo "1.USSN "
echo "2.USSR"
read user_opt1

channel_type=2
if [ $user_opt1 = 1 ]
then 
echo "USSN String :"
read msgString
msg_type=1
else
echo "Selecte Menu Name From Follwoing :"
menu_res=`mysql -h$db_ip -u$db_user -p$db_pwd -A $db_name -e"select distinct(MENU_NAME) from MENU_DETAILS where username='testuser'"|tail -1`
echo "Menu Name:"
read menuName
msg_type=2

fi
echo "Read End Time (HH:MM:SS):"
read endtime
echo "File Name:"
read fileName
tableName="DATA_TABLE"
today=`date '+%Y/%m/%d'`

QTASK_PROFILE=` printf "insert into SCHEDULE_PROFILE (SCHEDULE_NAME,MSG_TYPE,CHANNEL_ID,SCHEDULE_TIME,END_TIME,MSG_TEXT,OA,SERVICE_CODE,STATUS,INTERFACE_ID,CREATED_BY) values ('%s', '%d', '2','current_timestamp','%s %s' '%s', '%s', '2','3', 'testuser');" $scheduleName $msg_type $today $endtime $msgString $oa $serviceCode `;

echo $QTASK_PROFILE
#mysql -h$db_ip -u$db_user -p$db_pwd -A $db_name -e"$QTASK_PROFILE"

QSCH_ID=`printf "select SCHEDULE_ID from SCHEDULE_PROFILE  where SCHEDULE_NAME='%s' order by SCHEDULE_ID desc limit 1;" $scheduleName`;
#echo $STATETID;
mysql -h$db_ip -u$db_user -p$db_pwd -A $db_name -e"load data local infile '$fileName' into table $tableName fields terminated by ','lines terminated by '\n' (DA) set SCHEDULE_ID =$TID";

TID=`mysql -h$db_ip -u$db_user -p$db_pwd -A $db_name -e"$QSCH_ID"|tail -1`
TASK_TABLE_MAP=`printf "insert into SCHEDULE_TABLE_MAPPING (SCHEDULE_ID, TABLE_NAME,FILE_NAME,FILE_STATUS,STATUS) VALUES
( '%d', '%s', '%s', '83', 'E'); " $TID $tableName $fileName;
#echo $TASK_TABLE_MAP;
mysql -h$db_ip -u$db_user -p$db_pwd -A $db_name -e"$TASK_TABLE_MAP"

