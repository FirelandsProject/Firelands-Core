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
-- Table structure for table `character_aura`
--

DROP TABLE IF EXISTS `character_aura`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `character_aura` (
  `guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID (Full Global Unique Identifier) of the target affected by the aura.',
  `caster_guid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID (Global Unique Identifier) of the player who cast the aura.',
  `item_guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID (Global Unique Identifier) of the item which casted the aura.',
  `spell` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The spell from which the aura was applied. (See Spell.dbc)',
  `stackcount` int(11) unsigned NOT NULL DEFAULT '1' COMMENT 'Determines how many stacks of the spell the character has.',
  `remaincharges` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The number of charges remaining on the aura.',
  `basepoints0` int(11) NOT NULL DEFAULT '0' COMMENT 'Primary effect.',
  `basepoints1` int(11) NOT NULL DEFAULT '0' COMMENT 'Secondary effect (healing)',
  `basepoints2` int(11) NOT NULL DEFAULT '0' COMMENT 'Secondary effect (damage)',
  `periodictime0` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Primary effect over time counter.',
  `periodictime1` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Primary effect (healing) over time counter.',
  `periodictime2` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Primary effect (damage) over time counter.',
  `maxduration` int(11) NOT NULL DEFAULT '0' COMMENT 'The maximum duration of the aura.',
  `remaintime` int(11) NOT NULL DEFAULT '0' COMMENT 'The time remaining in seconds on the aura. -1 means that the aura is indefinite.',
  `effIndexMask` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The effect index of the spell from which the aura came from.',
  PRIMARY KEY (`guid`,`caster_guid`,`item_guid`,`spell`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Player System';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `character_aura`
--

LOCK TABLES `character_aura` WRITE;
/*!40000 ALTER TABLE `character_aura` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_aura` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-07-19  0:45:07