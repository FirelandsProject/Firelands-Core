CREATE DATABASE  IF NOT EXISTS `world` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `world`;
--
-- Table structure for table `player_xp_for_level`
--

DROP TABLE IF EXISTS `player_xp_for_level`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `player_xp_for_level` (
  `lvl` int(3) unsigned NOT NULL COMMENT 'The player''s level.',
  `xp_for_next_level` int(10) unsigned NOT NULL COMMENT 'The experience needed to upgrade from the value in lvl field to lvl +1.',
  PRIMARY KEY (`lvl`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
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
-- Dumping data for table `player_xp_for_level`
--

LOCK TABLES `player_xp_for_level` WRITE;
/*!40000 ALTER TABLE `player_xp_for_level` DISABLE KEYS */;
INSERT INTO `player_xp_for_level` (`lvl`, `xp_for_next_level`) VALUES 
(1,400),
(2,900),
(3,1400),
(4,2100),
(5,2800),
(6,3600),
(7,4500),
(8,5400),
(9,6500),
(10,7600),
(11,8700),
(12,9800),
(13,11000),
(14,12300),
(15,13600),
(16,15000),
(17,16400),
(18,17800),
(19,19300),
(20,20800),
(21,22400),
(22,24000),
(23,25500),
(24,27200),
(25,28900),
(26,30500),
(27,32200),
(28,33900),
(29,36300),
(30,38800),
(31,41600),
(32,44600),
(33,48000),
(34,51400),
(35,55000),
(36,58700),
(37,62400),
(38,66200),
(39,70200),
(40,74300),
(41,78500),
(42,82800),
(43,87100),
(44,91600),
(45,96300),
(46,101000),
(47,105800),
(48,110700),
(49,115700),
(50,120900),
(51,126100),
(52,131500),
(53,137000),
(54,142500),
(55,148200),
(56,154000),
(57,159900),
(58,165800),
(59,172000),
(60,290000),
(61,317000),
(62,349000),
(63,386000),
(64,428000),
(65,475000),
(66,527000),
(67,585000),
(68,648000),
(69,717000),
(79,1336640),
(78,1323120),
(77,1309920),
(76,1296560),
(75,1283360),
(74,1270320),
(73,1257440),
(72,1244560),
(71,1231680),
(70,1219040),
(80,1686300),
(81,2121500),
(82,4004000),
(83,5203400),
(84,9165100);
/*!40000 ALTER TABLE `player_xp_for_level` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-07-05 13:04:51
