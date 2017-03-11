-- MySQL dump 10.13  Distrib 5.1.44, for pc-linux-gnu (i686)
--
-- Host: 127.0.0.1    Database: CMS_USSD
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
-- Table structure for table `channel_tablemapping`
--

DROP TABLE IF EXISTS `channel_tablemapping`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `channel_tablemapping` (
  `REFID` int(11) NOT NULL AUTO_INCREMENT,
  `CHANNEL_NAME` varchar(20) DEFAULT NULL,
  `SUBCHANNEL_NAME` varchar(20) DEFAULT NULL,
  `STATUS` char(1) DEFAULT 'A',
  PRIMARY KEY (`REFID`)
) ENGINE=MyISAM AUTO_INCREMENT=5 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `channel_tablemapping`
--

LOCK TABLES `channel_tablemapping` WRITE;
/*!40000 ALTER TABLE `channel_tablemapping` DISABLE KEYS */;
/*!40000 ALTER TABLE `channel_tablemapping` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `data_table`
--

DROP TABLE IF EXISTS `data_table`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `data_table` (
  `MSG_ID` int(11) NOT NULL AUTO_INCREMENT,
  `DA` varchar(20) DEFAULT NULL,
  `SCHEDULE_ID` int(11) DEFAULT NULL,
  `STATUS` int(11) DEFAULT '0',
  PRIMARY KEY (`MSG_ID`)
) ENGINE=MyISAM AUTO_INCREMENT=44 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `data_table`
--

LOCK TABLES `data_table` WRITE;
/*!40000 ALTER TABLE `data_table` DISABLE KEYS */;
INSERT INTO `data_table` VALUES (1,'2349090390046',1,1),(2,'2349090390046',2,1),(3,'2349090390046',3,1),(4,'2349090390046',4,1),(5,'2349090390046',5,1),(6,'2349090390046',6,1),(7,'2349090390046',7,1),(8,'2349090390046',8,1),(9,'2349090390046',9,1),(10,'2349090390046',10,1),(11,'2349090390046',11,1),(12,'2.34909E+12\r',12,0),(13,'2.34909E+12\r',12,0),(14,'2.34909E+12\r',12,0),(15,'2.34909E+12\r',12,0),(16,'2.34909E+12\r',13,0),(17,'2.34909E+12\r',13,0),(18,'2.34909E+12\r',13,0),(19,'2.34909E+12\r',13,0),(20,'2.34909E+12\r',14,0),(21,'2.34909E+12\r',14,0),(22,'2.34909E+12\r',14,0),(23,'2.34909E+12\r',14,0),(24,'2.34909E+12\r',15,0),(25,'2.34909E+12\r',15,0),(26,'2.34909E+12\r',15,0),(27,'2.34909E+12\r',15,0),(28,'2.34909E+12\r',16,0),(29,'2.34909E+12\r',16,0),(30,'2.34909E+12\r',16,0),(31,'2.34909E+12\r',16,0),(32,'2349090390046\r',17,0),(33,'2348033705949',17,0),(34,'\r',18,0),(35,'2349090390046\r',18,0),(36,'2348033705949\r',18,0),(37,'2348033705948\r',18,0),(38,'2348033705950',18,0),(39,'',19,0),(40,'2349090390046',19,0),(41,'2348033705949',19,0),(42,'2348033705948',19,0),(43,'2348033705950',19,0);
/*!40000 ALTER TABLE `data_table` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `errorcode_details`
--

DROP TABLE IF EXISTS `errorcode_details`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `errorcode_details` (
  `seq_num` int(11) NOT NULL AUTO_INCREMENT,
  `error_code` int(11) DEFAULT NULL,
  `error_description` varchar(128) DEFAULT NULL,
  `is_retry` int(11) DEFAULT '0' COMMENT '''1-to retry,0-no retry''',
  PRIMARY KEY (`seq_num`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `errorcode_details`
--

LOCK TABLES `errorcode_details` WRITE;
/*!40000 ALTER TABLE `errorcode_details` DISABLE KEYS */;
/*!40000 ALTER TABLE `errorcode_details` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `esme_details`
--

DROP TABLE IF EXISTS `esme_details`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `esme_details` (
  `esme_id` int(10) DEFAULT NULL,
  `esme_ip` varchar(24) DEFAULT NULL,
  `esme_port` int(11) DEFAULT NULL,
  `smsc_id` int(11) DEFAULT NULL,
  `throwttel_qsize` int(11) DEFAULT NULL,
  `esme_secip` varchar(24) DEFAULT NULL,
  `esme_secport` int(11) DEFAULT NULL,
  `seq_num` int(11) NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`seq_num`)
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `esme_details`
--

LOCK TABLES `esme_details` WRITE;
/*!40000 ALTER TABLE `esme_details` DISABLE KEYS */;
INSERT INTO `esme_details` VALUES (1,'127.0.0.1',9898,1,1000,'127.0.0.1',9898,1),(2,'127.0.0.1',9999,2,1000,'127.0.0.1',9999,2);
/*!40000 ALTER TABLE `esme_details` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mdr_01_01`
--

DROP TABLE IF EXISTS `mdr_01_01`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mdr_01_01` (
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
  `URL` varchar(256) DEFAULT NULL,
  `URL_STATUS` int(11) DEFAULT NULL,
  PRIMARY KEY (`SEQ_NUM`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mdr_01_01`
--

LOCK TABLES `mdr_01_01` WRITE;
/*!40000 ALTER TABLE `mdr_01_01` DISABLE KEYS */;
INSERT INTO `mdr_01_01` VALUES (1,2,12,'7676724614','1212',121,1212,1,0,'2014-10-31 16:17:20','0000-00-00 00:00:00','',0),(2,2,13,'9493024705','mdk',121,1212,0,0,'2014-10-31 16:17:20','2014-10-31 16:17:25','',0),(3,2,14,'9493024704','mdk',121,1212,0,0,'2014-10-31 16:17:20','2014-10-31 16:17:25','',0);
/*!40000 ALTER TABLE `mdr_01_01` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `menu_details`
--

DROP TABLE IF EXISTS `menu_details`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `menu_details` (
  `seq_id` int(11) NOT NULL AUTO_INCREMENT,
  `user_name` varchar(40) DEFAULT NULL,
  `menu_service_code` varchar(16) DEFAULT NULL,
  `menu_id` int(11) DEFAULT NULL,
  `dtmf_id` varchar(3) DEFAULT NULL,
  `next_menu_id` varchar(10) DEFAULT NULL,
  `message` varchar(40) DEFAULT NULL,
  `info_msg` varchar(160) DEFAULT NULL,
  `end_msg` varchar(160) DEFAULT NULL,
  `url` varchar(300) DEFAULT NULL,
  `status` varchar(1) DEFAULT 'A',
  PRIMARY KEY (`seq_id`),
  UNIQUE KEY `menuidx` (`user_name`,`menu_service_code`,`menu_id`,`dtmf_id`)
) ENGINE=MyISAM AUTO_INCREMENT=7 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `menu_details`
--

LOCK TABLES `menu_details` WRITE;
/*!40000 ALTER TABLE `menu_details` DISABLE KEYS */;
INSERT INTO `menu_details` VALUES (1,'icmuser','1257',0,'1','1','Songs','select your choice:','','','A'),(2,'icmuser','1257',0,'2','2','Dailogs','','','','A'),(3,'icmuser','1257',1,'1','','Folk Songs','Select Song type','thanks for chosing Folk songs','','A'),(4,'icmuser','1257',1,'2','','Melody Songs','','thanks for chosing melody songs','','A'),(5,'icmuser','1257',2,'1','','Comedy',' chose dailog type','thanks for chosing comedy dailogs','','A'),(6,'icmuser','1257',2,'2','','warning','',' thank for chosing warning dailogs','','A');
/*!40000 ALTER TABLE `menu_details` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `menu_details1`
--

DROP TABLE IF EXISTS `menu_details1`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `menu_details1` (
  `seq_id` int(11) NOT NULL AUTO_INCREMENT,
  `user_name` varchar(40) DEFAULT NULL,
  `menu_service_code` varchar(16) DEFAULT NULL,
  `menu_id` int(11) DEFAULT NULL,
  `dtmf_id` varchar(3) DEFAULT NULL,
  `next_menu_id` varchar(10) DEFAULT NULL,
  `message` varchar(40) DEFAULT NULL,
  `info_msg` varchar(160) DEFAULT NULL,
  `end_msg` varchar(160) DEFAULT NULL,
  `url` varchar(300) DEFAULT NULL,
  `status` varchar(1) DEFAULT 'A',
  PRIMARY KEY (`seq_id`),
  UNIQUE KEY `menuidx` (`user_name`,`menu_service_code`,`menu_id`,`dtmf_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `menu_details1`
--

LOCK TABLES `menu_details1` WRITE;
/*!40000 ALTER TABLE `menu_details1` DISABLE KEYS */;
/*!40000 ALTER TABLE `menu_details1` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `menu_retry_plan`
--

DROP TABLE IF EXISTS `menu_retry_plan`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `menu_retry_plan` (
  `plan_id` int(11) DEFAULT NULL,
  `interval` int(11) DEFAULT NULL,
  `interval_ration` varchar(128) DEFAULT NULL,
  `status` varchar(1) DEFAULT 'A'
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `menu_retry_plan`
--

LOCK TABLES `menu_retry_plan` WRITE;
/*!40000 ALTER TABLE `menu_retry_plan` DISABLE KEYS */;
INSERT INTO `menu_retry_plan` VALUES (1,10,'1:1','A');
/*!40000 ALTER TABLE `menu_retry_plan` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mo_menu_map`
--

DROP TABLE IF EXISTS `mo_menu_map`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mo_menu_map` (
  `seq_no` int(11) NOT NULL AUTO_INCREMENT,
  `mo_srv_code` varchar(32) DEFAULT NULL,
  `username` varchar(40) DEFAULT NULL,
  `menu_srv_code` varchar(32) DEFAULT NULL,
  `STATUS` char(1) DEFAULT 'A',
  PRIMARY KEY (`seq_no`),
  UNIQUE KEY `mosrvidx` (`mo_srv_code`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mo_menu_map`
--

LOCK TABLES `mo_menu_map` WRITE;
/*!40000 ALTER TABLE `mo_menu_map` DISABLE KEYS */;
/*!40000 ALTER TABLE `mo_menu_map` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `plan_profile`
--

DROP TABLE IF EXISTS `plan_profile`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `plan_profile` (
  `plan_id` int(11) DEFAULT NULL,
  `user_name` varchar(40) DEFAULT NULL,
  `error_code` int(11) DEFAULT NULL,
  `msg_type` int(11) DEFAULT NULL,
  `retry_count` int(11) DEFAULT NULL,
  `status` varchar(1) DEFAULT 'A'
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `plan_profile`
--

LOCK TABLES `plan_profile` WRITE;
/*!40000 ALTER TABLE `plan_profile` DISABLE KEYS */;
INSERT INTO `plan_profile` VALUES (1,'icmuser',-1,-1,-1,'A');
/*!40000 ALTER TABLE `plan_profile` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `publisher_smsc_map`
--

DROP TABLE IF EXISTS `publisher_smsc_map`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `publisher_smsc_map` (
  `SEQ_NUM` int(11) NOT NULL AUTO_INCREMENT,
  `MODULE_ID` int(11) DEFAULT NULL,
  `SMSC_ID` int(11) DEFAULT NULL,
  PRIMARY KEY (`SEQ_NUM`),
  UNIQUE KEY `uniqMapping` (`MODULE_ID`,`SMSC_ID`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `publisher_smsc_map`
--

LOCK TABLES `publisher_smsc_map` WRITE;
/*!40000 ALTER TABLE `publisher_smsc_map` DISABLE KEYS */;
INSERT INTO `publisher_smsc_map` VALUES (1,10,1);
/*!40000 ALTER TABLE `publisher_smsc_map` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `schedule_profile`
--

DROP TABLE IF EXISTS `schedule_profile`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `schedule_profile` (
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
  `menu_service_code` varchar(32) DEFAULT NULL,
  `DLR_REG` int(11) DEFAULT '0',
  `STATUS` int(11) DEFAULT NULL,
  `INTERFACE_ID` int(11) DEFAULT '1',
  `IS_PAUSE` int(11) DEFAULT '0',
  `CREATED_BY` varchar(40) DEFAULT '',
  `DCS` int(11) DEFAULT NULL,
  PRIMARY KEY (`SCHEDULE_ID`)
) ENGINE=MyISAM AUTO_INCREMENT=20 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `schedule_profile`
--

LOCK TABLES `schedule_profile` WRITE;
/*!40000 ALTER TABLE `schedule_profile` DISABLE KEYS */;
INSERT INTO `schedule_profile` VALUES (1,'testUSSN',1,1,0,2,'2015-03-11 14:34:20','2014-10-23 12:35:35','this is test message',0,'123',NULL,0,7,3,0,'imsuser',NULL),(2,'testUSSR',2,1,16,2,'2016-01-25 13:47:00','2014-10-23 12:37:43',NULL,0,'123','1257',0,7,3,0,'icmuser',NULL),(3,'testn',1,1,0,2,'2015-02-26 13:59:39','2014-10-23 12:45:25','test message please ignore',0,'123',NULL,0,0,3,0,'imsuser',NULL),(4,'da',1,1,0,2,'2015-03-02 14:34:05','2014-10-25 05:29:27','test message',0,'12212',NULL,0,7,3,0,'icmuser',NULL),(5,'da',1,1,0,2,'2015-02-26 13:59:39','2014-10-25 05:29:27','test message',0,'12212',NULL,0,0,3,0,'icmuser',NULL),(6,'da',1,1,0,2,'2015-02-26 13:59:39','2014-10-25 05:35:21','dadaff',0,'12123',NULL,0,0,3,0,'icmuser',NULL),(7,'da',1,1,0,2,'2015-02-26 13:59:39','2014-10-25 05:37:04','dada`',0,'2123',NULL,0,0,3,0,'icmuser',NULL),(8,'da',1,1,0,2,'2015-02-26 13:59:39','2014-10-25 05:38:44','dasd',0,'adad',NULL,0,0,3,0,'icmuser',NULL),(9,'da',1,1,0,2,'2015-02-26 13:59:39','2014-10-25 05:44:57','adada',0,'a123',NULL,0,0,3,0,'icmuser',NULL),(10,'da',1,1,0,2,'2015-02-26 13:59:39','2014-10-25 05:44:57','adada',0,'a123',NULL,0,0,3,0,'icmuser',NULL),(11,'da',1,1,0,2,'2015-02-26 13:59:39','2014-10-25 05:44:57','adada',0,'a123',NULL,0,0,3,0,'icmuser',NULL),(12,'test1',2,4,0,2,'2015-02-26 13:59:39','2014-10-28 16:00:27',NULL,0,'123','123',0,0,3,0,'imsuser',NULL),(13,'ussr',2,4,0,2,'2015-02-26 13:59:39','2014-10-28 16:20:21',NULL,0,'123','123',0,0,3,0,'icmuser',NULL),(14,'twest',2,4,0,2,'2015-02-26 13:59:39','2014-10-28 16:27:03','',0,'123','123',0,0,3,0,'icmuser',NULL),(15,'name ',1,4,0,2,'2015-02-26 13:59:39','2014-10-28 16:30:45','test mesage',0,'121',NULL,0,0,3,0,'icmuser',15),(16,'tesr',2,4,0,2,'2015-02-26 13:59:39','2014-10-28 16:31:56','',0,'123','123',0,0,3,0,'icmuser',15),(17,'test1',2,2,0,2,'2015-02-26 13:59:39','2014-10-28 16:37:50','',0,'123','123',0,0,3,0,'icmuser',15),(18,'te111',2,5,0,2,'2015-02-26 13:59:39','2014-10-28 16:40:24','',0,'123','123',0,0,3,0,'icmuser',15),(19,'da',2,5,0,2,'2015-02-26 13:59:39','2014-10-27 16:46:25','',0,'123','123',0,0,3,0,'icmuser',15);
/*!40000 ALTER TABLE `schedule_profile` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `schedule_table_mapping`
--

DROP TABLE IF EXISTS `schedule_table_mapping`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `schedule_table_mapping` (
  `SEQ_NUM` int(11) NOT NULL AUTO_INCREMENT,
  `SCHEDULE_ID` int(11) DEFAULT NULL,
  `TABLE_NAME` varchar(32) DEFAULT NULL,
  `FILE_NAME` varchar(40) DEFAULT NULL,
  `STATUS` char(1) DEFAULT 'P',
  `FILE_STAUS` int(11) DEFAULT '1',
  PRIMARY KEY (`SEQ_NUM`)
) ENGINE=MyISAM AUTO_INCREMENT=20 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `schedule_table_mapping`
--

LOCK TABLES `schedule_table_mapping` WRITE;
/*!40000 ALTER TABLE `schedule_table_mapping` DISABLE KEYS */;
INSERT INTO `schedule_table_mapping` VALUES (1,1,'DATA_TABLE','D:\\ICM/temp/223212/da1414067808499.csv','C',1),(2,2,'DATA_TABLE','D:\\ICM/temp/223212/da1414067885382.csv','C',1),(3,3,'DATA_TABLE','D:\\IMS/23102014/da1414068452178.csv','0',1),(4,4,'DATA_TABLE','D:\\IMS/25102014/da1414214983097.csv','C',1),(5,5,'DATA_TABLE','D:\\IMS/25102014/da1414215069920.csv','0',1),(6,6,'DATA_TABLE','D:\\IMS/25102014/da1414215339618.csv','0',1),(7,7,'DATA_TABLE','D:\\IMS/25102014/da1414215439212.csv','0',1),(8,8,'DATA_TABLE','D:\\IMS/25102014/da1414215538207.csv','0',1),(9,9,'DATA_TABLE','D:\\IMS/25102014/da1414215911202.csv','0',1),(10,10,'DATA_TABLE','D:\\IMS/25102014/da1414216088449.csv','0',1),(11,11,'DATA_TABLE','D:\\IMS/25102014/da1414216151540.csv','0',1),(12,12,'DATA_TABLE','D:\\IMS/28102014/da1414512062703.csv','0',1),(13,13,'DATA_TABLE','D:\\IMS/28102014/da1414513329389.csv','0',1),(14,14,'DATA_TABLE','D:\\IMS/28102014/da1414513640113.csv','0',1),(15,15,'DATA_TABLE','D:\\IMS/28102014/da1414513870907.csv','0',1),(16,16,'DATA_TABLE','D:\\IMS/28102014/da1414513932796.csv','0',1),(17,17,'DATA_TABLE','D:\\IMS/28102014/m21414514287441.csv','0',1),(18,18,'DATA_TABLE','D:\\IMS/28102014/m21414514469377.csv','0',1),(19,19,'DATA_TABLE','D:\\IMS/28102014/m21414514802918.csv','0',1);
/*!40000 ALTER TABLE `schedule_table_mapping` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `smsc_details`
--

DROP TABLE IF EXISTS `smsc_details`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `smsc_details` (
  `smsc_id` int(11) DEFAULT NULL,
  `smsc_ip` varchar(24) DEFAULT NULL,
  `smsc_port` int(11) DEFAULT NULL,
  `user_name` varchar(24) DEFAULT NULL,
  `password` varchar(24) DEFAULT NULL,
  `sys_type` varchar(24) DEFAULT NULL,
  `conn_type` varchar(2) DEFAULT NULL,
  `ton` int(11) DEFAULT NULL,
  `npi` int(11) DEFAULT NULL,
  `addr_range` varchar(24) DEFAULT NULL,
  `msg_per_sec` int(11) DEFAULT NULL,
  `interface_version` int(11) DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `smsc_details`
--

LOCK TABLES `smsc_details` WRITE;
/*!40000 ALTER TABLE `smsc_details` DISABLE KEYS */;
INSERT INTO `smsc_details` VALUES (1,'127.0.0.1',3014,'ussdclient','password','TR','TR',1,1,'',100,10),(2,'127.0.0.1',8888,'aggrigator','aggrigator','TR','TR',1,1,'',100,10);
/*!40000 ALTER TABLE `smsc_details` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `status_details`
--

DROP TABLE IF EXISTS `status_details`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `status_details` (
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
-- Dumping data for table `status_details`
--

LOCK TABLES `status_details` WRITE;
/*!40000 ALTER TABLE `status_details` DISABLE KEYS */;
INSERT INTO `status_details` VALUES (1,'TASK_PENDING','Task pending for Campaign Manager Approval','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(4,'TASK_SCHEDULED','SCHEDULED','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(5,'TASK_EXECUTING','EXECUTING','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(6,'TASK_SUBMITTED','SUBMITTED','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(7,'TASK_COMPLETED','COMPLETED','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(2,'TASK_APPROVED','Task approved by Service provider','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(3,'TASK_REJECTED_BY_SP','Task rejected by Service Provider','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(9,'MSG_PENDING','Messages pending to be delivered','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(10,'MSG_SUBMITTED','Message submitted to smsc','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(11,'MSG_SUBMIT_FAILED','Negative submit message response','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(12,'MSG_DELIVERY_FAILED','Delivery failed','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(13,'MSG_DELIVERED','Delivery Success','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(15,'MSG_REJECTED_BY_CM','Msg Rejected by CP','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(31,'FILE_PROCESSING','File being Processed by Backend Profiling','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(32,'TASK_CANCEL_REQUEST','Task Cancel Request by CP after SP Approval','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(33,'TASK_CANCELED_BY_SP','Task Canceled by SP','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(34,'MSG_CANCEL_REQUEST','Msg Cancel Request by CE before CP approval','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(35,'MSG_CANCELED_BY_SP','Msg Canceled by SP','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(50,'MSG_EXECUTED','Sending to PC','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(51,'REJECT_BW','Rejecting BW while uploading from file','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(71,'FILE_UPLOAD_INP','File upload in process','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(70,'FILE_UPLOAD_START','Start uploading file','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(72,'FILE_UPLOAD_DONE','File upload completed','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(73,'FILE_UPLOAD_ERR','Error in file upload','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(81,'FILE_DUMP_WAITING','Ready for file uploader','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(82,'FILE_DUMP_INP','File dumping in process','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(83,'FILE_DUMP_CMPLTD','File dump completed','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(84,'FILE_DUMP_ERR','Error while dumping file','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(48,'TASK_EMERGENCY_CANCELED_BY_SP','Task Emergency Canceled by SP ','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(85,'MSG_APPROVED','Msg Approved by SP','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(91,'TASK_APPROVED_BY_CM','Task Approved by Campaign Manager','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(93,'TASK_REJECTED_BY_CM','Task Rejected by Campaign Manager','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(95,'TASK_CANCEL_REQUEST_AFTER_CM_A','Task Cancel Request by CE after CM Approval','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(39,'MSG_CANCEL_REQUEST_AFTER_CP_AP','Msg Cancel Request by CE after CP approval','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(38,'MSG_CANCEL_REQUEST_AFTER_APPRO','Msg Cancel request by CP after SP approval','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(40,'MSG_APPROVED_BY_CP','Message Approved by CP','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(0,'MSG_PENDING_ON_CP','message pending on CP to approval','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(16,'MSG_REJECTED_BY_SP','Msg Rejected by SP','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(43,'MSG_EMERGENCY_CANCELED_BY_SP','Message Emergency Canceled by SP ','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(97,'TASK_CANCEL_REQUEST_EVEN_SP_AP','task cancel request by CE after both CP and SP app','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(98,'TASK_CANCEL_REQUEST_MOVE_TO_SP','task cancel req move from CP to SP coming from CE','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(44,'MSG_CANCEL_REQUEST_EVEN_SP_APP','msg cancel request by CE after both CP and SP app','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(45,'MSG_CANCEL_REQUEST_MOVE_TO_SP','msg cancel req move from CP to SP coming from CE','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(46,'TASK_CANCELED_BY_CP','task canceled by CP','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(47,'TASK_CANCELED_BY_CE','task canceled  by CE','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(87,'MSG_CANCELED_BY_CP','msg canceled by CP','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(88,'MSG_CANCELED_BY_CE','msg canceled by CE','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(100,'Problem in File','Problem in File','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(89,'TASK_PAUSED','Task Paused','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(74,'TASK_CANCELED','TASK_CANCLED','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(75,'MSG_CANCELED','MSG_CANCELED','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00'),(125,'TASK_EXPIRED','TASK HAS BEEN EXPIRED','ADMIN','0000-00-00 00:00:00',NULL,'0000-00-00 00:00:00');
/*!40000 ALTER TABLE `status_details` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `userdetails`
--

DROP TABLE IF EXISTS `userdetails`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `userdetails` (
  `seqNum` int(11) DEFAULT NULL,
  `username` varchar(20) DEFAULT NULL,
  `password` varchar(20) DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `userdetails`
--

LOCK TABLES `userdetails` WRITE;
/*!40000 ALTER TABLE `userdetails` DISABLE KEYS */;
INSERT INTO `userdetails` VALUES (1,'imsuser','imsuser'),(2,'icmuser','icmuser');
/*!40000 ALTER TABLE `userdetails` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2016-11-09 21:36:05
