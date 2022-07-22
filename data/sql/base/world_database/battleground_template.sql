CREATE DATABASE  IF NOT EXISTS `world` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `world`;
--
-- Table structure for table `battleground_template`
--

DROP TABLE IF EXISTS `battleground_template`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `battleground_template` (
  `id` mediumint(8) unsigned NOT NULL COMMENT 'The battleground ID (See BattlemasterList.dbc).',
  `MinPlayersPerTeam` smallint(5) unsigned NOT NULL DEFAULT 0 COMMENT 'The minimum number of players that need to join the battleground.',
  `MaxPlayersPerTeam` smallint(5) unsigned NOT NULL DEFAULT 0 COMMENT 'Controls how many players from each team can join the battleground.',
  `AllianceStartLoc` mediumint(8) unsigned NOT NULL COMMENT 'The location where the alliance players get teleported to in the battleground.',
  `AllianceStartO` float NOT NULL COMMENT 'The orientation of the alliance players upon teleport.',
  `HordeStartLoc` mediumint(8) unsigned NOT NULL COMMENT 'The location where the horde players get teleported to in the battleground.',
  `HordeStartO` float NOT NULL COMMENT 'The orientation of the horde players upon teleport into the battleground.',
  `Comment` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
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
-- Dumping data for table `battleground_template`
--

LOCK TABLES `battleground_template` WRITE;
/*!40000 ALTER TABLE `battleground_template` DISABLE KEYS */;
INSERT INTO `battleground_template` (`id`, `MinPlayersPerTeam`, `MaxPlayersPerTeam`, `AllianceStartLoc`, `AllianceStartO`, `HordeStartLoc`, `HordeStartO`, `Comment`) VALUES 
(1,10,40,611,3.71492,610,1.16711,'Alterac Valley'),
(2,10,10,769,3.14159,770,0.00391,'Warsong Gulch'),
(3,15,15,890,3.91992,889,0.88828,'Arathi Basin'),
(4,5,5,929,0,936,3.14159,'Nagrand Arena'),
(5,5,5,939,0,940,3.14159,'Blade\'s Edge Arena'),
(6,5,5,0,0,0,0,'All Arenas'),
(7,15,15,1103,3.22092,1104,0.01649,'Eye of The Storm'),
(8,5,5,1258,0,1259,3.14159,'Ruins of Lordaeron'),
(9,8,15,1367,0,1368,0,'Strand of the Ancients'),
(10,5,5,1362,0,1363,0,'Dalaran Sewers'),
(11,5,5,1966,0,1967,0,'The Ring of Valor'),
(30,10,40,1485,0,1486,0,'Isle of Conquest'),
(32,5,40,0,0,0,0,'Random Battleground'),
(108,5,10,1726,2.55647,1727,6.18108,'Twin Peaks'),
(120,5,10,1798,5.95725,1799,1.55116,'The Battle for Gilneas');
/*!40000 ALTER TABLE `battleground_template` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-07-05 13:04:18
