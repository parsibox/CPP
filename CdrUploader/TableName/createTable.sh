db_ip=127.0.0.1
db_user=cmsuser
db_pwd=cmsuser
db_name=CMS_USSD
month=0

while [ $month -le 1 ]
do
day=1
while [ $day -le 31 ]
do

DROP_TABLE=`printf "drop table mdr_%02d_%02d " $month $day`

CREATE_TABLE=`printf "create table mdr_%02d_%02d (
 SEQ_NUM int(11) NOT NULL AUTO_INCREMENT,
  SCHEDULE_ID int(11) DEFAULT NULL,
  MSG_ID int(11) DEFAULT NULL,
  DA varchar(20) DEFAULT NULL,
  MSG_DATA varchar(200) DEFAULT NULL COMMENT '''In Case of USSR Traversal path will be preset in this field''',
  SENDER_SESSIONID int(11) DEFAULT NULL,
  RECEIVER_SESSIONID int(11) DEFAULT NULL,
  STATUS int(11) DEFAULT NULL,
  CHARGING_STATUS int(11) DEFAULT NULL,
  SESSION_STARTTIME timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  SESSION_ENDTIME timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  URL varchar(256) DEFAULT NULL,
  URL_STATUS int(11) DEFAULT NULL,
  PRIMARY KEY (SEQ_NUM)
) ENGINE=InnoDB " $month $day`
echo $DROP_TABLE

day=`expr $day + 1 `
mysql -h$db_ip -u$db_user -p$db_pwd -A $db_name -e"$DROP_TABLE"
#echo "$CREATE_TABLE"
#mysql -h$db_ip -u$db_user -p$db_pwd -A $db_name -e"$CREATE_TABLE"
done 
month=`expr $month + 1 `

done
