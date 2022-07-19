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
-- Table structure for table `character_stats`
--

DROP TABLE IF EXISTS `character_stats`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `character_stats` (
  `guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The guid of the character. (See character.guid).',
  `maxhealth` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Maximum amount of health that the character has.',
  `maxpower1` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Max Mana',
  `maxpower2` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Max Rage',
  `maxpower3` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Max Focus',
  `maxpower4` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Max Energy',
  `maxpower5` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Max Happiness',
  `maxpower6` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Max Rune',
  `maxpower7` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Max Runic Power',
  `strength` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current strength value.',
  `agility` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current agility value',
  `stamina` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current stamina value.',
  `intellect` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current intellect value.',
  `spirit` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current spirit value.',
  `armor` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current armor value.',
  `resHoly` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current holy resistance value.',
  `resFire` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current fire resistance value.',
  `resNature` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current nature resistance value.',
  `resFrost` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current frost resistance value.',
  `resShadow` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current shadow resistance value.',
  `resArcane` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current arcane resistance value.',
  `blockPct` float unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current block chance',
  `dodgePct` float unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current dodge chance.',
  `parryPct` float unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current parry chance.',
  `critPct` float unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current crit chance.',
  `rangedCritPct` float unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current ranged crit chance.',
  `attackPower` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current attackpower.',
  `rangedAttackPower` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current ranged attackpower.',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `character_stats`
--

LOCK TABLES `character_stats` WRITE;
/*!40000 ALTER TABLE `character_stats` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_stats` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-07-19  0:45:14
