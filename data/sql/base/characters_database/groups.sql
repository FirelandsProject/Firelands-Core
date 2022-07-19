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
-- Table structure for table `groups`
--

DROP TABLE IF EXISTS `groups`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `groups` (
  `groupId` int(11) unsigned NOT NULL COMMENT 'Uniquie ID for the group.',
  `leaderGuid` int(11) unsigned NOT NULL COMMENT 'Guid of the group''s leader',
  `mainTank` int(11) unsigned NOT NULL COMMENT 'Guid of the character who is that main tank.',
  `mainAssistant` int(11) unsigned NOT NULL COMMENT 'Guid of the character who is that main assist.',
  `lootMethod` tinyint(4) unsigned NOT NULL COMMENT 'This is the loot method used by the group.',
  `looterGuid` int(11) unsigned NOT NULL COMMENT 'Guid of the character who retrieves the loot.',
  `lootThreshold` tinyint(4) unsigned NOT NULL COMMENT 'The quality of the items in the loot to be rolled for.',
  `icon1` int(11) unsigned NOT NULL COMMENT 'Guid of the target that has the star raid target on it.',
  `icon2` int(11) unsigned NOT NULL COMMENT 'Guid of the target that has the circle raid target on it.',
  `icon3` int(11) unsigned NOT NULL COMMENT 'Guid of target that has the dimond raid target on it.',
  `icon4` int(11) unsigned NOT NULL COMMENT 'Guid of the target that has the triangler raid target on it.',
  `icon5` int(11) unsigned NOT NULL COMMENT 'Guid of the target that has the moon raid target on it.',
  `icon6` int(11) unsigned NOT NULL COMMENT 'Guid of the target that has the square raid target on it.',
  `icon7` int(11) unsigned NOT NULL COMMENT 'Guid of the target that has the cross raid target on it.',
  `icon8` int(11) unsigned NOT NULL COMMENT 'Guid of the target that has the skull raid target on it.',
  `isRaid` tinyint(1) unsigned NOT NULL COMMENT 'Flag set for if group is part of a raid or not.',
  PRIMARY KEY (`groupId`),
  UNIQUE KEY `leaderGuid` (`leaderGuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Groups';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `groups`
--

LOCK TABLES `groups` WRITE;
/*!40000 ALTER TABLE `groups` DISABLE KEYS */;
/*!40000 ALTER TABLE `groups` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-07-19  0:45:04
