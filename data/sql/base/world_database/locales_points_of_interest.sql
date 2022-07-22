--
-- Table structure for table `locales_points_of_interest`
--

DROP TABLE IF EXISTS `locales_points_of_interest`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `locales_points_of_interest` (
  `entry` mediumint(8) unsigned NOT NULL DEFAULT 0 COMMENT 'Reference to points_of_interest.entry.',
  `icon_name_loc1` text DEFAULT NULL COMMENT 'Korean localization of icon_name in the points_of_interest table.',
  `icon_name_loc2` text DEFAULT NULL COMMENT 'French localization of icon_name in the points_of_interest table.',
  `icon_name_loc3` text DEFAULT NULL COMMENT 'German localization of icon_name in the points_of_interest table.',
  `icon_name_loc4` text DEFAULT NULL COMMENT 'Chinese localization of icon_name in the points_of_interest table.',
  `icon_name_loc5` text DEFAULT NULL COMMENT 'Taiwanese localization of icon_name in the points_of_interest table.',
  `icon_name_loc6` text DEFAULT NULL COMMENT 'Spanish Spain localization of icon_name in the points_of_interest table.',
  `icon_name_loc7` text DEFAULT NULL COMMENT 'Spanish Latin America localization of icon_name.',
  `icon_name_loc8` text DEFAULT NULL COMMENT 'Russian localization of icon_name in the points_of_interest table.',
  `icon_name_loc9` text DEFAULT NULL,
  `icon_name_loc10` text DEFAULT NULL,
  `icon_name_loc11` text DEFAULT NULL,
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;
/*!40101 SET character_set_client = @saved_cs_client */;
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
-- Dumping data for table `locales_points_of_interest`
--

LOCK TABLES `locales_points_of_interest` WRITE;
/*!40000 ALTER TABLE `locales_points_of_interest` DISABLE KEYS */;
/*!40000 ALTER TABLE `locales_points_of_interest` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-07-05 13:04:45
