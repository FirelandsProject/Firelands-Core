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
-- Table structure for table `instance_template`
--

DROP TABLE IF EXISTS `instance_template`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `instance_template` (
  `map` smallint(5) unsigned NOT NULL COMMENT 'Map ID (See Map.dbc).',
  `parent` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT 'Map ID of the parent instance See Map.dbc).',
  `levelMin` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Minimum allowed player level.',
  `levelMax` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Maximum allowed player level.',
  `maxPlayers` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Maximal number of players.',
  `reset_delay` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Instance reset delay (days).',
  `ghostEntranceMap` smallint(5) unsigned NOT NULL COMMENT 'Map ID (See Map.dbc) for ghost entrance.',
  `ghostEntranceX` float NOT NULL COMMENT 'X position of ghost entrance.',
  `ghostEntranceY` float NOT NULL COMMENT 'Y position of ghost entrance.',
  PRIMARY KEY (`map`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `instance_template`
--

LOCK TABLES `instance_template` WRITE;
/*!40000 ALTER TABLE `instance_template` DISABLE KEYS */;
INSERT INTO `instance_template` VALUES (33,0,20,26,10,0,0,-230.989,1571.57),(34,0,22,34,10,0,0,-8762.38,848.01),(36,0,17,26,10,0,0,-11207.8,1681.15),(43,0,17,24,10,0,1,-751.131,-2209.24),(47,0,24,40,10,0,1,-4459.45,-1660.21),(48,0,20,34,10,0,1,4249.12,748.387),(70,0,41,51,10,0,0,-6060.18,-2955),(90,0,24,40,10,0,0,-5162.66,931.599),(109,0,50,0,10,0,0,-10170.1,-3995.97),(129,0,33,47,10,0,1,-4662.88,-2535.87),(189,0,29,48,10,0,0,2892.24,-811.264),(209,0,43,54,10,0,1,-6790.58,-2891.28),(229,0,55,0,10,3,0,-7522.53,-1233.04),(230,0,48,60,5,0,0,-7178.1,-928.639),(249,0,60,60,40,5,1,-4753.31,-3752.42),(289,0,58,0,5,0,0,1274.78,-2552.56),(309,0,60,60,20,3,0,-11916.1,-1224.58),(329,0,58,0,5,0,0,3392.32,-3378.48),(349,0,40,58,10,0,1,-1432.7,2924.98),(389,0,13,22,10,0,1,1816.76,-4423.37),(409,0,60,60,40,7,0,-7510.56,-1036.7),(429,0,55,0,5,0,1,-3908.03,1130),(469,0,60,60,40,7,0,-7663.41,-1218.67),(509,0,60,60,20,3,1,-8114.46,1526.37),(531,0,60,60,40,7,1,-8111.72,1526.79),(533,0,60,60,40,7,0,0,0);
/*!40000 ALTER TABLE `instance_template` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-07-19  0:45:50
