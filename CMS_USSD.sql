-- MySQL dump 10.13  Distrib 5.1.44, for pc-linux-gnu (i686)
--
-- Host: 192.168.220.128    Database: CMS_USSD
-- ------------------------------------------------------
-- Server version	5.1.44-community

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `CHANNEL_TABLEMAPPING`
--

DROP TABLE IF EXISTS `CHANNEL_TABLEMAPPING`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `CHANNEL_TABLEMAPPING` (
  `REFID` int(11) NOT NULL AUTO_INCREMENT,
  `CHANNEL_NAME` varchar(20) DEFAULT NULL,
  `SUBCHANNEL_NAME` varchar(20) DEFAULT NULL,
  PRIMARY KEY (`REFID`)
) ENGINE=MyISAM AUTO_INCREMENT=5 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `CHANNEL_TABLEMAPPING`
--

LOCK TABLES `CHANNEL_TABLEMAPPING` WRITE;
/*!40000 ALTER TABLE `CHANNEL_TABLEMAPPING` DISABLE KEYS */;
INSERT INTO `CHANNEL_TABLEMAPPING` VALUES (1,'USSD','USSN'),(2,'USSD','USSR'),(3,'SMS','FLASH'),(4,'SMS','WAP');
/*!40000 ALTER TABLE `CHANNEL_TABLEMAPPING` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ERRORCODE_DETAILS`
--

DROP TABLE IF EXISTS `ERRORCODE_DETAILS`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ERRORCODE_DETAILS` (
  `SEQ_NUM` int(11) NOT NULL AUTO_INCREMENT,
  `ERROR_CODE` int(11) DEFAULT NULL,
  `ERROR_DESCRIPTION` varchar(128) DEFAULT NULL,
  `IS_RETRY` int(11) DEFAULT '0' COMMENT '''1-to Retry,0-No Retry''',
  PRIMARY KEY (`SEQ_NUM`)
) ENGINE=InnoDB AUTO_INCREMENT=256 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ERRORCODE_DETAILS`
--

LOCK TABLES `ERRORCODE_DETAILS` WRITE;
/*!40000 ALTER TABLE `ERRORCODE_DETAILS` DISABLE KEYS */;
INSERT INTO `ERRORCODE_DETAILS` VALUES (1,0,'No Error',0),(2,1,'The subscriber is unknown',0),(3,2,'The Base_station is unknown',0),(4,3,'The MSC is unknown',0),(5,4,'The Loc_area is unknown',0),(6,5,'The subscriber is undentified',0),(7,6,'The roaming number is unallocated',0),(8,7,'The equipment is unknown',0),(9,8,'The roaming is not be allowed',0),(10,9,'The MS is illegal',0),(11,10,'The bearer service is not be provided',0),(12,11,'The television is not be provided',0),(13,12,'The bearer capability is not sufficient or the equitment is illegal',0),(14,13,'The call has been barred',0),(15,14,'The forwarding is violated',0),(16,15,'The CUG reject',0),(17,16,'The SS operation is illegal',0),(18,17,'The state is SS_error',0),(19,18,'The SS is not available',0),(20,19,'The SS_Subscription is violated',0),(21,20,'The SS is incompatible',0),(22,21,'The facility is not supported',0),(23,23,'The target_base_station is invalid',0),(24,24,'There is not the available radio resourse',0),(25,25,'There is nor the available handover number',0),(26,26,'The subsequent handover is failure',0),(27,27,'The subscriber is absent',0),(28,29,'The subscriber is busy',0),(29,30,'The radio is congested',0),(30,31,'The call is impossible completed or the subscriber for MT_SMS is busy',0),(31,32,'The SM_delivery is failure',0),(32,33,'The message_waiting_list is full',0),(33,34,'The system is failure',0),(34,35,'The data is missed',0),(35,36,'The date value is unexpected',0),(36,37,'The passward registion is failed',0),(37,38,'The passward check is not pass',0),(38,39,'There is not the roaming number',0),(39,40,'The tracing buffer is full',0),(40,43,'The num of PW attempts is violated',0),(41,44,'The map_number has changed',0),(42,50,'The error is in MS',0),(43,51,'The SMS_Lower_layers capability is not provided',0),(44,52,'The memory has exceeded',0),(45,53,'The SC is congested',0),(46,54,'The MS is not a SC subscriber',0),(47,55,'The SME address is invalid',0),(48,56,'The subscriber without para is absent',0),(49,253,'The dialog creation is failed',0),(50,254,'The MS is released',0),(51,1001,'Account is not found or does not exist',0),(52,1002,'Account type is not valid',0),(53,1003,'The account has been bound too many users',0),(54,1004,'The account has already existed',0),(55,1005,'The account does not exist',0),(56,1006,'The account to be modified conflicked with the exist ones',0),(57,1007,'The account is in use now',0),(58,1008,'The account count has reached its limit',0),(59,1009,'The password is not valid',0),(60,1010,'The service count has reached its limit',0),(61,1011,'The interface count has reached its limit',0),(62,1012,'The service can not be found',0),(63,1013,'The service number is not valid',0),(64,1014,'The interface can not be found',0),(65,1015,'The OMC Server can not monitored',0),(66,1016,'Interface can not monitor itself',0),(67,1017,'The task count has reached its limit',0),(68,1018,'The task can not be found',0),(69,1019,'The statistic tasks count has reached its limit',0),(70,1020,'The Ussd Dialogs count has reached its limit',0),(71,1021,'The Ussd Dialog can not be found',0),(72,1022,'The Ussd sring seems in bad format',0),(73,1023,'The Ussd Entity has exit',0),(74,1024,'Ussd Switch operation has failed',0),(75,1025,'END_LOAD_USER message recieved while the loading operation has not completed',0),(76,1026,'The user loader has already existed',0),(77,1027,'Max user request count has been reached',0),(78,1028,'Max user count has been reached',0),(79,1029,'The User has already exist',0),(80,1030,'User can not be found',0),(81,1031,'Package was currupted',0),(82,1032,'Create failed (usually refers to \'not enough memory\')',0),(83,1033,'Insufficient rights',0),(84,1034,'Operation time out',0),(85,1035,'Invalid parameters',0),(86,1036,'Internal error',0),(87,1037,'The max server information count has been reached',0),(88,1038,'The Server informaion can not be found',0),(89,1039,'The server information has already exist',0),(90,1040,'The file that contains the server information can not be found',0),(91,1041,'The snapshot has already exist',0),(92,1042,'The snapshot can not be found',0),(93,1043,'The service code can not be found in the route table',0),(94,1044,'There is no bind info can be found in the route table',0),(95,1045,'There is no interface bounded in this number segment',0),(96,1046,'The account is not a service account',0),(97,1047,'The account does not support any service',0),(98,1048,'The number segment can not be found',0),(99,1049,'There is no such authentication type',0),(100,1050,'Authentication failed',0),(101,1051,'Query thread info operation failed',0),(102,1052,'Interface can not be found',0),(103,1053,'Unknown error',0),(104,1054,'Illegal server type',0),(105,1055,'There is no MSISDN in the message and MSISDN can not be found based on the IMSI',0),(106,1056,'Interface has already blocked',0),(107,1057,'Dialog Time Out',0),(108,1058,'Exceed Max Scp Account Countœ?200œ?',0),(109,1059,'Scp Account existed',0),(110,1060,'Scp Account not existed',0),(111,1061,'Can not modify Scp Account with ScpIF',0),(112,1062,'IP and Port of Scp Account existed',0),(113,1063,'Scp Account conflicted',0),(114,1064,'Exceed Max IF Count of SCP Account',0),(115,1065,'ScpIF not existed',0),(116,1066,'ScpIF Logined with this account existed',0),(117,1067,'Delete ScpIF Error',0),(118,1068,'Invalid ScpIF Param',0),(119,1069,'Create ScpIF Failure',0),(120,1070,'Delete PPS Nosegment Error',0),(121,1071,'Add PPS Nosegment Error',0),(122,1072,'Modify PPS Nosegment Error',0),(123,1073,'PPS Nosegment not existed',0),(124,1074,'PPS Route not found',0),(125,1075,'PPS Nosegment existed',0),(126,1076,'Scp CommFee Auth Failed',0),(127,1077,'Scp InfoFee Auth Failed',0),(128,1078,'Scp Connect Failed',0),(129,1079,'Scp Timeout',0),(130,1080,'Not PPS User',0),(131,1081,'Auth failed because can­?t find route',0),(132,1082,'Auth failed because can­?t find servicecode',0),(133,1083,'Auth failed because can­?t find user',0),(134,1084,'MS invalid operation',0),(135,1085,'Sp charge again',0),(136,1086,'Is PPS User',0),(137,1087,'Service Code is in use',0),(138,1088,'Ussd Dialog reach maximum interactive count',0),(139,1089,'USSD dialog switch failed',0),(140,2001,'Invalid SPID',0),(141,2002,'Wrong Auth Information',0),(142,2003,'Reach maximum dialogs',0),(143,2004,'Can\'t find dialog',0),(144,2005,'Switch dialog failed',0),(145,2006,'MS response timeout',0),(146,2007,'SP response timeout',0),(147,2008,'Dialog response timeout',0),(148,2009,'No matched route',0),(149,2010,'Dialog version is wrong',0),(150,2011,'Dialog type is wrong',0),(151,2012,'Format of UssdString is wrong',0),(152,2013,'Service Access number is wrong',0),(153,2014,'Invalid User',0),(154,2015,'Map_Open failed',0),(155,2016,'        Map_Open failed',0),(156,2017,'MS release call',0),(157,2018,'MS network breaddown',0),(158,2019,'UAP Version is too high',0),(159,2020,'User has no enough balance',0),(160,2021,'Can\'t find Service Information',0),(161,2022,'Fail to lookup',0),(162,5001,'Client systems have not been initiated',0),(163,5002,'client systems already been initiated',0),(164,5003,'Object create failure',0),(165,5004,'Handle not found',0),(166,5005,'No message or message queue is empty',0),(167,5006,'Login failure for invalid account or password',0),(168,5007,'Invalid parameters',0),(169,5008,'Internal error',0),(170,5009,'Time out error',0),(171,5010,'Null Object',0),(172,5011,'Reach maximum interface count',0),(173,5012,'Reach maximum ussddialog count',0),(174,5013,'Ussddialog object has not been initiated',0),(175,5014,'Ussddialog not found',0),(176,5015,'Ussddialog version error',0),(177,5016,'Message is null or message length error',0),(178,5017,'Current message is not from service',0),(179,5018,'Current message is not from MS',0),(180,5019,'Invalid ussddialog message operation type',0),(181,5020,'Client close dialog',0),(182,5021,'Callback functions not registered',0),(183,5022,'Message not supported by current status',0),(184,5023,'Invalid callback functions',0),(185,5024,'Reach maximum thread called count',0),(186,5025,'Invalid function calling',0),(187,5026,'Invalid MSISDN',0),(188,5027,'Invalid TimeOut',0),(189,6001,'Account is not fount or does not exist',0),(190,6002,'Account type is not valid',0),(191,6003,'The account has been bound too many users',0),(192,6004,'The account has been existed',0),(193,6005,'The account does not exist',0),(194,6006,'The account to be modified conflicked with the exist ones',0),(195,6007,'The account is in use now',0),(196,6008,'The account has reached its limit',0),(197,6009,'The password is not valid',0),(198,6010,'The service count has reached its limit',0),(199,6011,'The interface count has reached its limit',0),(200,6012,'The service can not be fount',0),(201,6013,'The service number is not valid',0),(202,6014,'The interface can not be found',0),(203,6015,'The OMC Server can not be minitored',0),(204,6016,'Interface can not monitor itself',0),(205,6017,'The task count has reached its limit',0),(206,6018,'The task can not be found',0),(207,6019,'The statistic tasks count has reached its limit',0),(208,6020,'The Ussd Dialogs count has reached its limit',0),(209,6021,'The Ussd Dialog can not be found',0),(210,6022,'The Ussd string seems in bad format',0),(211,6023,'The Ussd Entity has exit',0),(212,6024,'Ussd Switch operation has failed',0),(213,6025,'END_LOAD_USER message recerved while the loading operation has not completed',0),(214,6026,'The user loader has already existed',0),(215,6027,'Max user requesr count has been reached',0),(216,6028,'Max user count has been reached',0),(217,6030,'User can not been found',0),(218,6031,'Package was currupted',0),(219,6032,'Create failed(usually refers to \'not enough memory\')',0),(220,6033,'Insufficient rights',0),(221,6034,'Operation time out',0),(222,6035,'Invalid parameters',0),(223,6036,'Internal error',0),(224,6037,'The max server information count has been reached',0),(225,6038,'The Server information can not be found',0),(226,6039,'The server information has already exist',0),(227,6040,'The file that contains the server information can not be found',0),(228,6041,'The snapshot has already exist',0),(229,6042,'The snapshot can not be found',0),(230,6043,'The service code can not be found in the route table',0),(231,6044,'There is no bind info can be found about this account',0),(232,6045,'There is no interface bounded in this number segment',0),(233,6046,'The account is not a service account',0),(234,6047,'The account doed not support any service',0),(235,6048,'The number segment can not be found',0),(236,6049,'There is no such authentication type',0),(237,6050,'Authentication failed',0),(238,6051,'Query thread info operation failed',0),(239,6052,'Interface can not be found',0),(240,6053,'Unknown error',0),(241,6054,'Invalid server type',0),(242,6055,'Interface has already blocked',0);
/*!40000 ALTER TABLE `ERRORCODE_DETAILS` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ESME_DETAILS`
--

DROP TABLE IF EXISTS `ESME_DETAILS`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ESME_DETAILS` (
  `ESME_ID` int(10) DEFAULT NULL,
  `ESME_IP` varchar(24) DEFAULT NULL,
  `ESME_PORT` int(11) DEFAULT NULL,
  `SMSC_ID` int(11) DEFAULT NULL,
  `THROWTTEL_QSIZE` int(11) DEFAULT NULL,
  `ESME_SECIP` varchar(24) DEFAULT NULL,
  `ESME_SECPORT` int(11) DEFAULT NULL,
  `SEQ_NUM` int(11) NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`SEQ_NUM`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ESME_DETAILS`
--

LOCK TABLES `ESME_DETAILS` WRITE;
/*!40000 ALTER TABLE `ESME_DETAILS` DISABLE KEYS */;
INSERT INTO `ESME_DETAILS` VALUES (1,'127.0.0.1',9898,1,10000,'127.0.0.1',9898,1);
/*!40000 ALTER TABLE `ESME_DETAILS` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `MDR_01_01`
--

DROP TABLE IF EXISTS `MDR_01_01`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `MDR_01_01` (
  `SEQ_NUM` int(11) NOT NULL AUTO_INCREMENT,
  `SCHEDULE_ID` int(11) DEFAULT NULL,
  `MSG_ID` int(11) DEFAULT NULL,
  `DA` varchar(20) DEFAULT NULL,
  `MSG_DATA` varchar(200) DEFAULT NULL COMMENT '''In Case of USSR Traversal path will be preset in this field''',
  `SENDER_SESSIONID` int(11) DEFAULT NULL,
  `RECEIVER_SESSIONID` int(11) DEFAULT NULL,
  `STATUS` int(11) DEFAULT NULL,
  `CHARGING_STATUS` int(11) DEFAULT NULL,
  `SESSION_STARTTIME` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `SESSION_ENDTIME` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`SEQ_NUM`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `MDR_01_01`
--

LOCK TABLES `MDR_01_01` WRITE;
/*!40000 ALTER TABLE `MDR_01_01` DISABLE KEYS */;
/*!40000 ALTER TABLE `MDR_01_01` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `MENU_DETAILS`
--

DROP TABLE IF EXISTS `MENU_DETAILS`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `MENU_DETAILS` (
  `SeqNum` int(11) NOT NULL AUTO_INCREMENT,
  `MENU_ID` int(11) DEFAULT NULL,
  `PARENT_ID` int(11) DEFAULT NULL,
  `DTMF_ID` int(11) DEFAULT NULL,
  `MESSAGE` varchar(64) DEFAULT NULL,
  `SERVICE_CODE` varchar(20) DEFAULT NULL,
  `CHARGING_UNITS` int(11) DEFAULT NULL,
  `LANG_ID` int(11) DEFAULT NULL,
  `MENU_NAME` varchar(64) DEFAULT NULL,
  `USER_NAME` varchar(32) DEFAULT NULL COMMENT '''for PSSR use value as pssr''',
  PRIMARY KEY (`SeqNum`),
  UNIQUE KEY `menuIdx` (`MENU_ID`,`PARENT_ID`,`SERVICE_CODE`,`USER_NAME`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `MENU_DETAILS`
--

LOCK TABLES `MENU_DETAILS` WRITE;
/*!40000 ALTER TABLE `MENU_DETAILS` DISABLE KEYS */;
/*!40000 ALTER TABLE `MENU_DETAILS` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `PUBLISHER_SMSC_MAP`
--

DROP TABLE IF EXISTS `PUBLISHER_SMSC_MAP`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `PUBLISHER_SMSC_MAP` (
  `SEQ_NUM` int(11) NOT NULL AUTO_INCREMENT,
  `MODULE_ID` int(11) DEFAULT NULL,
  `SMSC_ID` int(11) DEFAULT NULL,
  PRIMARY KEY (`SEQ_NUM`),
  UNIQUE KEY `uniqMapping` (`MODULE_ID`,`SMSC_ID`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `PUBLISHER_SMSC_MAP`
--

LOCK TABLES `PUBLISHER_SMSC_MAP` WRITE;
/*!40000 ALTER TABLE `PUBLISHER_SMSC_MAP` DISABLE KEYS */;
INSERT INTO `PUBLISHER_SMSC_MAP` VALUES (1,10,1);
/*!40000 ALTER TABLE `PUBLISHER_SMSC_MAP` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `SCHEDULE_PROFILE`
--

DROP TABLE IF EXISTS `SCHEDULE_PROFILE`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `SCHEDULE_PROFILE` (
  `SCHEDULE_ID` int(11) NOT NULL AUTO_INCREMENT,
  `SCHEDULE_NAME` varchar(40) DEFAULT NULL,
  `MSG_TYPE` int(11) DEFAULT NULL,
  `TOTAL_COUNT` int(11) DEFAULT NULL,
  `SUBMIT_COUNT` int(11) DEFAULT NULL,
  `CHANNEL_ID` int(11) DEFAULT NULL,
  `SCHEDULE_TIME` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `END_TIME` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `MSG_TEXT` varchar(1024) DEFAULT NULL,
  `LANG_ID` int(11) DEFAULT '0',
  `OA` varchar(20) DEFAULT NULL,
  `SERVICE_CODE` varchar(32) DEFAULT NULL,
  `DLR_REG` int(11) DEFAULT '0',
  `STATUS` int(11) DEFAULT NULL,
  `INTERFACE_ID` int(11) DEFAULT '1',
  `IS_PAUSE` int(11) DEFAULT '0',
  `CREATED_BY` varchar(40) DEFAULT '',
  PRIMARY KEY (`SCHEDULE_ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `SCHEDULE_PROFILE`
--

LOCK TABLES `SCHEDULE_PROFILE` WRITE;
/*!40000 ALTER TABLE `SCHEDULE_PROFILE` DISABLE KEYS */;
/*!40000 ALTER TABLE `SCHEDULE_PROFILE` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `SCHEDULE_TABLE_MAPPING`
--

DROP TABLE IF EXISTS `SCHEDULE_TABLE_MAPPING`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `SCHEDULE_TABLE_MAPPING` (
  `SEQ_NUM` int(11) NOT NULL AUTO_INCREMENT,
  `SCHEDULED_ID` int(11) DEFAULT NULL,
  `TABLE_NAME` varchar(32) DEFAULT NULL,
  `FILE_NAME` varchar(40) DEFAULT NULL,
  `STATUS` char(1) DEFAULT 'P',
  `FILE_STAUS` int(11) DEFAULT '1',
  PRIMARY KEY (`SEQ_NUM`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `SCHEDULE_TABLE_MAPPING`
--

LOCK TABLES `SCHEDULE_TABLE_MAPPING` WRITE;
/*!40000 ALTER TABLE `SCHEDULE_TABLE_MAPPING` DISABLE KEYS */;
/*!40000 ALTER TABLE `SCHEDULE_TABLE_MAPPING` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `SMSC_DETAILS`
--

DROP TABLE IF EXISTS `SMSC_DETAILS`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `SMSC_DETAILS` (
  `SMSC_ID` int(11) DEFAULT NULL,
  `SMSC_IP` varchar(24) DEFAULT NULL,
  `SMSC_PORT` int(11) DEFAULT NULL,
  `USER_NAME` varchar(24) DEFAULT NULL,
  `PASSWORD` varchar(24) DEFAULT NULL,
  `SYS_TYPE` varchar(24) DEFAULT NULL,
  `CONN_TYPE` varchar(2) DEFAULT NULL,
  `TON` int(11) DEFAULT NULL,
  `NPI` int(11) DEFAULT NULL,
  `ADDR_RANGE` varchar(24) DEFAULT NULL,
  `MSG_PER_SEC` int(11) DEFAULT NULL,
  `INTERFACE_VERSION` int(11) DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `SMSC_DETAILS`
--

LOCK TABLES `SMSC_DETAILS` WRITE;
/*!40000 ALTER TABLE `SMSC_DETAILS` DISABLE KEYS */;
INSERT INTO `SMSC_DETAILS` VALUES (1,'127.0.0.1',3014,'ussdclient','ussdclient','TR','TR',1,1,'',100,10);
/*!40000 ALTER TABLE `SMSC_DETAILS` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `STATUS_DETAILS`
--

DROP TABLE IF EXISTS `STATUS_DETAILS`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `STATUS_DETAILS` (
  `STATUS_ID` int(2) NOT NULL,
  `STATUS_NAME` varchar(30) NOT NULL,
  `DESCRIPTION` varchar(50) NOT NULL,
  `CREATE_USER` varchar(30) NOT NULL DEFAULT 'ADMIN',
  `CREATE_DATE` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `CHANGE_USER` varchar(30) DEFAULT NULL,
  `CHANGE_DATE` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`STATUS_ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `STATUS_DETAILS`
--

LOCK TABLES `STATUS_DETAILS` WRITE;
/*!40000 ALTER TABLE `STATUS_DETAILS` DISABLE KEYS */;
INSERT INTO `STATUS_DETAILS` VALUES (1,'TASK_PENDING','Task pending for Campaign Manager Approval','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(4,'TASK_SCHEDULED','SCHEDULED','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(5,'TASK_EXECUTING','EXECUTING','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(6,'TASK_SUBMITTED','SUBMITTED','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(7,'TASK_COMPLETED','COMPLETED','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(2,'TASK_APPROVED','Task approved by Service provider','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(3,'TASK_REJECTED_BY_SP','Task rejected by Service Provider','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(9,'MSG_PENDING','Messages pending to be delivered','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(10,'MSG_SUBMITTED','Message submitted to smsc','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(11,'MSG_SUBMIT_FAILED','Negative submit message response','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(12,'MSG_DELIVERY_FAILED','Delivery failed','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(13,'MSG_DELIVERED','Delivery Success','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(15,'MSG_REJECTED_BY_CM','Msg Rejected by CP','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(31,'FILE_PROCESSING','File being Processed by Backend Profiling','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(32,'TASK_CANCEL_REQUEST','Task Cancel Request by CP after SP Approval','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(33,'TASK_CANCELED_BY_SP','Task Canceled by SP','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(34,'MSG_CANCEL_REQUEST','Msg Cancel Request by CE before CP approval','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(35,'MSG_CANCELED_BY_SP','Msg Canceled by SP','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(50,'MSG_EXECUTED','Sending to PC','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(51,'REJECT_BW','Rejecting BW while uploading from file','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(71,'FILE_UPLOAD_INP','File upload in process','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(70,'FILE_UPLOAD_START','Start uploading file','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(72,'FILE_UPLOAD_DONE','File upload completed','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(73,'FILE_UPLOAD_ERR','Error in file upload','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(81,'FILE_DUMP_WAITING','Ready for file uploader','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(82,'FILE_DUMP_INP','File dumping in process','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(83,'FILE_DUMP_CMPLTD','File dump completed','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(84,'FILE_DUMP_ERR','Error while dumping file','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(48,'TASK_EMERGENCY_CANCELED_BY_SP','Task Emergency Canceled by SP ','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(85,'MSG_APPROVED','Msg Approved by SP','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(91,'TASK_APPROVED_BY_CM','Task Approved by Campaign Manager','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(93,'TASK_REJECTED_BY_CM','Task Rejected by Campaign Manager','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(95,'TASK_CANCEL_REQUEST_AFTER_CM_A','Task Cancel Request by CE after CM Approval','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(39,'MSG_CANCEL_REQUEST_AFTER_CP_AP','Msg Cancel Request by CE after CP approval','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(38,'MSG_CANCEL_REQUEST_AFTER_APPRO','Msg Cancel request by CP after SP approval','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(40,'MSG_APPROVED_BY_CP','Message Approved by CP','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(0,'MSG_PENDING_ON_CP','message pending on CP to approval','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(16,'MSG_REJECTED_BY_SP','Msg Rejected by SP','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(43,'MSG_EMERGENCY_CANCELED_BY_SP','Message Emergency Canceled by SP ','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(97,'TASK_CANCEL_REQUEST_EVEN_SP_AP','task cancel request by CE after both CP and SP app','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(98,'TASK_CANCEL_REQUEST_MOVE_TO_SP','task cancel req move from CP to SP coming from CE','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(44,'MSG_CANCEL_REQUEST_EVEN_SP_APP','msg cancel request by CE after both CP and SP app','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(45,'MSG_CANCEL_REQUEST_MOVE_TO_SP','msg cancel req move from CP to SP coming from CE','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(46,'TASK_CANCELED_BY_CP','task canceled by CP','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(47,'TASK_CANCELED_BY_CE','task canceled  by CE','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(87,'MSG_CANCELED_BY_CP','msg canceled by CP','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(88,'MSG_CANCELED_BY_CE','msg canceled by CE','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(100,'Problem in File','Problem in File','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(89,'TASK_PAUSED','Task Paused','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(74,'TASK_CANCELED','TASK_CANCLED','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(75,'MSG_CANCELED','MSG_CANCELED','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(125,'TASK_EXPIRED','TASK HAS BEEN EXPIRED','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00');
/*!40000 ALTER TABLE `STATUS_DETAILS` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2014-04-23 22:42:23
