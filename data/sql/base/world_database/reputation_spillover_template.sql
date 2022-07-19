CREATE DATABASE  IF NOT EXISTS `world` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `world`;
-- MySQL dump 10.13  Distrib 8.0.29, for macos12 (x86_64)
--
-- Host: 127.0.0.1    Database: world
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
-- Table structure for table `reputation_spillover_template`
--

DROP TABLE IF EXISTS `reputation_spillover_template`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `reputation_spillover_template` (
  `faction` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT 'Base faction (See Faction.dbc).',
  `faction1` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT 'Dependent faction (See Faction.dbc).',
  `rate_1` float NOT NULL DEFAULT '0' COMMENT 'Rate for faction one.',
  `rank_1` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'The topmost rank allowed.',
  `faction2` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT 'Dependent faction (See Faction.dbc).',
  `rate_2` float NOT NULL DEFAULT '0' COMMENT 'Rate for faction two.',
  `rank_2` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'The topmost rank allowed.',
  `faction3` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT 'Dependent faction (See Faction.dbc).',
  `rate_3` float NOT NULL DEFAULT '0' COMMENT 'Rate for faction three.',
  `rank_3` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'The topmost rank allowed.',
  `faction4` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT 'Dependent faction (See Faction.dbc).',
  `rate_4` float NOT NULL DEFAULT '0' COMMENT 'Rate for faction four.',
  `rank_4` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'The topmost rank allowed.',
  PRIMARY KEY (`faction`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Reputation spillover reputation gain';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `reputation_spillover_template`
--

LOCK TABLES `reputation_spillover_template` WRITE;
/*!40000 ALTER TABLE `reputation_spillover_template` DISABLE KEYS */;
INSERT INTO `reputation_spillover_template` VALUES (21,369,0.5,7,470,0.5,7,577,0.5,7,0,0,0),(47,72,0.25,7,54,0.25,7,69,0.25,7,0,0,0),(54,47,0.25,7,72,0.25,7,69,0.25,7,0,0,0),(68,76,0.25,7,81,0.25,7,530,0.25,7,0,0,0),(69,47,0.25,7,54,0.25,7,72,0.25,7,0,0,0),(72,47,0.25,7,54,0.25,7,69,0.25,7,0,0,0),(76,68,0.25,7,81,0.25,7,530,0.25,7,0,0,0),(81,76,0.25,7,68,0.25,7,530,0.25,7,0,0,0),(369,21,0.5,7,470,0.5,7,577,0.5,7,0,0,0),(470,369,0.5,7,21,0.5,7,577,0.5,7,0,0,0),(530,76,0.25,7,81,0.25,7,68,0.25,7,0,0,0),(577,369,0.5,7,470,0.5,7,21,0.5,7,0,0,0);
/*!40000 ALTER TABLE `reputation_spillover_template` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-07-19  0:45:17
