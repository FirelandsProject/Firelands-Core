CREATE DATABASE  IF NOT EXISTS `characters` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `characters`;
-- MySQL dump 10.13  Distrib 8.0.29, for macos12 (x86_64)
--
-- Host: 127.0.0.1    Database: characters
-- ------------------------------------------------------
-- Server version	5.6.51

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `mail`
--

DROP TABLE IF EXISTS `mail`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `mail` (
  `id` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Unique message id. Every new message gets a new auto incremented id.',
  `messageType` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'message_type.',
  `stationery` tinyint(3) NOT NULL DEFAULT '41' COMMENT 'The StationeryID (See Stationery.dbc).',
  `mailTemplateId` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'The Mail Template Id (See MailTemplate.dbc)',
  `sender` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Guid of the character who sends the mail.',
  `receiver` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Guid of the character who receives the mail.',
  `subject` longtext COMMENT 'The Subject of the mail.',
  `itemTextId` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The item_text ID of the massage within the mail.',
  `has_items` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Boolean value showing whether or not atems have been sent.',
  `expire_time` bigint(40) unsigned NOT NULL DEFAULT '0' COMMENT 'Current Unix Time + Unix Time till expiry.',
  `deliver_time` bigint(40) unsigned NOT NULL DEFAULT '0' COMMENT 'Current Unix Time + Unix Time till delivery.',
  `money` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Amount of money sent.',
  `cod` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Amount of money needed (COD).',
  `checked` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'checked_flag.',
  PRIMARY KEY (`id`),
  KEY `idx_receiver` (`receiver`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Mail System';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mail`
--

LOCK TABLES `mail` WRITE;
/*!40000 ALTER TABLE `mail` DISABLE KEYS */;
/*!40000 ALTER TABLE `mail` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-07-19  0:44:54
