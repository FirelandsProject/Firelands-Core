--
-- Table structure for table `custom_texts`
--

DROP TABLE IF EXISTS `custom_texts`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `custom_texts` (
  `entry` mediumint(8) NOT NULL COMMENT 'The unique identifier of the script text entry.',
  `content_default` text NOT NULL COMMENT 'Contains the text presented in the default language English.',
  `content_loc1` text DEFAULT NULL COMMENT 'Korean localization of content_default.',
  `content_loc2` text DEFAULT NULL COMMENT 'French localization of content_default.',
  `content_loc3` text DEFAULT NULL COMMENT 'German localization of content_default.',
  `content_loc4` text DEFAULT NULL COMMENT 'Chinese localization of content_default.',
  `content_loc5` text DEFAULT NULL COMMENT 'Taiwanese localization of content_default.',
  `content_loc6` text DEFAULT NULL COMMENT 'Spanish (Spain) localization of content_default',
  `content_loc7` text DEFAULT NULL COMMENT 'Spanish (Latin America) localization of content_default',
  `content_loc8` text DEFAULT NULL COMMENT 'Russian localization of content_default',
  `content_loc9` text DEFAULT NULL COMMENT 'Portuguese localization of content_default',
  `content_loc10` text DEFAULT NULL COMMENT 'Brazilian localization of content_default',
  `content_loc11` text DEFAULT NULL COMMENT 'Italian localization of content_default',
  `sound` mediumint(8) unsigned NOT NULL DEFAULT 0 COMMENT 'Reference to a SoundEntries.dbc table entry.',
  `type` tinyint(3) unsigned NOT NULL DEFAULT 0 COMMENT 'Selects one of various text emote types to be used for the script text.',
  `language` tinyint(3) unsigned NOT NULL DEFAULT 0 COMMENT 'A language identifier.',
  `emote` smallint(5) unsigned NOT NULL DEFAULT 0 COMMENT 'Emote ID that the creature should continually perform.',
  `comment` text DEFAULT NULL COMMENT 'This documents the script text.',
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Custom Texts';
-- ---------------------------------------- 
-- MySQL dump 10.13  Distrib 5.5.37, for macos12 (x86_64)
--
-- Host: localhost    Database: world
-- ------------------------------------------------------
-- Server version	5.5.5-10.5.8-MariaDB

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@SESSION.TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Dumping data for table `custom_texts`
--

LOCK TABLES `custom_texts` WRITE;
/*!40000 ALTER TABLE `custom_texts` DISABLE KEYS */;
/*!40000 ALTER TABLE `custom_texts` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-07-05 13:04:29
