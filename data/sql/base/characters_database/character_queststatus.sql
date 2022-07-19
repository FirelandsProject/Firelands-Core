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
-- Table structure for table `character_queststatus`
--

DROP TABLE IF EXISTS `character_queststatus`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `character_queststatus` (
  `guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the character. (See character.guid).',
  `quest` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The quest ID. (See quest_template.entry).',
  `status` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The current quest status.',
  `rewarded` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT 'Boolean 1 or 0 representing whether the quest has been rewarded or not.',
  `explored` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT 'Boolean 1 or 0 representing if the character has explored what was needed.',
  `timer` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'Remaining time left on the quest if the quest has a timer (See quest_template).',
  `mobcount1` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Current count of the first number of kills or casts.',
  `mobcount2` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Current count of the second number of kills or casts.',
  `mobcount3` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Current count of the third number of kills or casts.',
  `mobcount4` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Current count of the forth number of kills or casts.',
  `itemcount1` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Current item count for the first item in a delivery quest, if any.',
  `itemcount2` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Current item count for the second item in a delivery quest, if any.',
  `itemcount3` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Current item count for the third item in a delivery quest, if any.',
  `itemcount4` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Current item count for the forth item in a delivery quest, if any.',
  PRIMARY KEY (`guid`,`quest`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Player System';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `character_queststatus`
--

LOCK TABLES `character_queststatus` WRITE;
/*!40000 ALTER TABLE `character_queststatus` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_queststatus` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-07-19  0:44:55
