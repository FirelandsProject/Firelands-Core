CREATE DATABASE  IF NOT EXISTS `auth` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `auth`;
-- MySQL dump 10.13  Distrib 8.0.29, for macos12 (x86_64)
--
-- Host: 127.0.0.1    Database: auth
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
-- Table structure for table `account`
--

DROP TABLE IF EXISTS `account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `account` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT COMMENT 'The unique account ID.',
  `username` varchar(32) NOT NULL DEFAULT '' COMMENT 'The account user name.',
  `sha_pass_hash` varchar(40) NOT NULL DEFAULT '' COMMENT 'This field contains the encrypted SHA1 password.',
  `gmlevel` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'The account security level.',
  `sessionkey` longtext COMMENT 'The Session Key.',
  `v` longtext COMMENT 'The validated Hash Value.',
  `s` longtext COMMENT 'Password ''Salt'' Value.',
  `email` text COMMENT 'The e-mail address associated with this account.',
  `joindate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'The date when the account was created.',
  `last_ip` varchar(30) NOT NULL DEFAULT '0.0.0.0' COMMENT 'The last IP used by the person who last logged into the account.',
  `failed_logins` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The number of failed logins attempted on the account.',
  `locked` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Indicates whether the account has been locked or not.',
  `last_login` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'The date when the account was last logged into.',
  `active_realm_id` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Which maximum expansion content a user has access to.',
  `expansion` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Which maximum expansion content a user has access to.',
  `mutetime` bigint(40) unsigned NOT NULL DEFAULT '0' COMMENT 'The time, in Unix time, when the account will be unmuted.',
  `locale` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'The locale used by the client logged into this account.',
  `os` varchar(3) DEFAULT '' COMMENT 'The Operating System of the connected client',
  `playerBot` bit(1) NOT NULL DEFAULT b'0' COMMENT 'Determines whether the account is a User or a PlayerBot',
  PRIMARY KEY (`id`),
  UNIQUE KEY `idx_username` (`username`),
  KEY `idx_gmlevel` (`gmlevel`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Account System';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `account`
--

LOCK TABLES `account` WRITE;
/*!40000 ALTER TABLE `account` DISABLE KEYS */;
/*!40000 ALTER TABLE `account` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-07-19  0:44:52
