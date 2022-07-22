CREATE DATABASE  IF NOT EXISTS `world` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `world`;
--
-- Table structure for table `locales_quest`
--

DROP TABLE IF EXISTS `locales_quest`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `locales_quest` (
  `entry` mediumint(8) unsigned NOT NULL DEFAULT 0 COMMENT 'Reference to quest_template.entry.',
  `Title_loc1` text DEFAULT NULL COMMENT 'Korean localization of Title in the quest_template table.',
  `Title_loc2` text DEFAULT NULL COMMENT 'French localization of Title in the quest_template table.',
  `Title_loc3` text DEFAULT NULL COMMENT 'German localization of Title in the quest_template table.',
  `Title_loc4` text DEFAULT NULL COMMENT 'Chinese localization of Title in the quest_template table.',
  `Title_loc5` text DEFAULT NULL COMMENT 'Taiwanese localization of Title in the quest_template table.',
  `Title_loc6` text DEFAULT NULL COMMENT 'Spanish Spain localization of Title in the quest_template table.',
  `Title_loc7` text DEFAULT NULL COMMENT 'Spanish Latin America localization of Title.',
  `Title_loc8` text DEFAULT NULL COMMENT 'Russian localization of Title in the quest_template table.',
  `Title_loc9` longtext DEFAULT NULL,
  `Title_loc10` longtext DEFAULT NULL,
  `Title_loc11` longtext DEFAULT NULL,
  `Details_loc1` text DEFAULT NULL COMMENT 'Korean localization of Details in the quest_template table.',
  `Details_loc2` text DEFAULT NULL COMMENT 'French localization of Details in the quest_template table.',
  `Details_loc3` text DEFAULT NULL COMMENT 'German localization of Details in the quest_template table.',
  `Details_loc4` text DEFAULT NULL COMMENT 'Chinese localization of Details in the quest_template table.',
  `Details_loc5` text DEFAULT NULL COMMENT 'Taiwanese localization of Details in the quest_template table.',
  `Details_loc6` text DEFAULT NULL COMMENT 'Spanish Spain localization of Details in the quest_template table.',
  `Details_loc7` text DEFAULT NULL COMMENT 'Spanish Latin America localization of Details.',
  `Details_loc8` text DEFAULT NULL COMMENT 'Russian localization of Details in the quest_template table.',
  `Details_loc9` longtext DEFAULT NULL,
  `Details_loc10` longtext DEFAULT NULL,
  `Details_loc11` longtext DEFAULT NULL,
  `Objectives_loc1` text DEFAULT NULL COMMENT 'Korean localization of Objectives in the quest_template table.',
  `Objectives_loc2` text DEFAULT NULL COMMENT 'French localization of Objectives in the quest_template table.',
  `Objectives_loc3` text DEFAULT NULL COMMENT 'German localization of Objectives in the quest_template table.',
  `Objectives_loc4` text DEFAULT NULL COMMENT 'Chinese localization of Objectives in the quest_template table.',
  `Objectives_loc5` text DEFAULT NULL COMMENT 'Taiwanese localization of Objectives in the quest_template table.',
  `Objectives_loc6` text DEFAULT NULL COMMENT 'Spanish Spain localization of Objectives in the quest_template table.',
  `Objectives_loc7` text DEFAULT NULL COMMENT 'Spanish Latin America localization of Objectives.',
  `Objectives_loc8` text DEFAULT NULL COMMENT 'Russian localization of Objectives in the quest_template table.',
  `Objectives_loc9` longtext DEFAULT NULL,
  `Objectives_loc10` longtext DEFAULT NULL,
  `Objectives_loc11` longtext DEFAULT NULL,
  `OfferRewardText_loc1` text DEFAULT NULL COMMENT 'Korean localization of OfferRewardText in the quest_template table.',
  `OfferRewardText_loc2` text DEFAULT NULL COMMENT 'French localization of OfferRewardText in the quest_template table.',
  `OfferRewardText_loc3` text DEFAULT NULL COMMENT 'German localization of OfferRewardText in the quest_template table.',
  `OfferRewardText_loc4` text DEFAULT NULL COMMENT 'Chinese localization of OfferRewardText in the quest_template table.',
  `OfferRewardText_loc5` text DEFAULT NULL COMMENT 'Taiwanese localization of OfferRewardText in the quest_template table.',
  `OfferRewardText_loc6` text DEFAULT NULL COMMENT 'Spanish Spain localization of OfferRewardText.',
  `OfferRewardText_loc7` text DEFAULT NULL COMMENT 'Spanish Latin America localization of OfferRewardText.',
  `OfferRewardText_loc8` text DEFAULT NULL COMMENT 'Russian localization of OfferRewardText in the quest_template table.',
  `OfferRewardText_loc9` longtext DEFAULT NULL,
  `OfferRewardText_loc10` longtext DEFAULT NULL,
  `OfferRewardText_loc11` longtext DEFAULT NULL,
  `RequestItemsText_loc1` text DEFAULT NULL COMMENT 'Korean localization of RequestItemsText in the quest_template table.',
  `RequestItemsText_loc2` text DEFAULT NULL COMMENT 'French localization of RequestItemsText in the quest_template table.',
  `RequestItemsText_loc3` text DEFAULT NULL COMMENT 'German localization of RequestItemsText in the quest_template table.',
  `RequestItemsText_loc4` text DEFAULT NULL COMMENT 'Chinese localization of RequestItemsText in the quest_template table.',
  `RequestItemsText_loc5` text DEFAULT NULL COMMENT 'Taiwanese localization of RequestItemsText in the quest_template table.',
  `RequestItemsText_loc6` text DEFAULT NULL COMMENT 'Spanish Spain localization of RequestItemsText in the quest_template table.',
  `RequestItemsText_loc7` text DEFAULT NULL COMMENT 'Spanish Latin America localization of RequestItemsText.',
  `RequestItemsText_loc8` text DEFAULT NULL COMMENT 'Russian localization of RequestItemsText in the quest_template table.',
  `RequestItemsText_loc9` longtext DEFAULT NULL,
  `RequestItemsText_loc10` longtext DEFAULT NULL,
  `RequestItemsText_loc11` longtext DEFAULT NULL,
  `EndText_loc1` text DEFAULT NULL COMMENT 'Korean localization of EndText in the quest_template table.',
  `EndText_loc2` text DEFAULT NULL COMMENT 'French localization of EndText in the quest_template table.',
  `EndText_loc3` text DEFAULT NULL COMMENT 'German localization of EndText in the quest_template table.',
  `EndText_loc4` text DEFAULT NULL COMMENT 'Chinese localization of EndText in the quest_template table.',
  `EndText_loc5` text DEFAULT NULL COMMENT 'Taiwanese localization of EndText in the quest_template table.',
  `EndText_loc6` text DEFAULT NULL COMMENT 'Spanish Spain localization of EndText in the quest_template table.',
  `EndText_loc7` text DEFAULT NULL COMMENT 'Spanish Latin America localization of EndText.',
  `EndText_loc8` text DEFAULT NULL COMMENT 'Russian localization of EndText in the quest_template table.',
  `EndText_loc9` longtext DEFAULT NULL,
  `EndText_loc10` longtext DEFAULT NULL,
  `EndText_loc11` longtext DEFAULT NULL,
  `CompletedText_loc1` text DEFAULT NULL,
  `CompletedText_loc2` text DEFAULT NULL,
  `CompletedText_loc3` text DEFAULT NULL,
  `CompletedText_loc4` text DEFAULT NULL,
  `CompletedText_loc5` text DEFAULT NULL,
  `CompletedText_loc6` text DEFAULT NULL,
  `CompletedText_loc7` text DEFAULT NULL,
  `CompletedText_loc8` text DEFAULT NULL,
  `CompletedText_loc9` text DEFAULT NULL,
  `CompletedText_loc10` text DEFAULT NULL,
  `CompletedText_loc11` text DEFAULT NULL,
  `ObjectiveText1_loc1` text DEFAULT NULL COMMENT 'Korean localization of ObjectiveText1 in the quest_template table.',
  `ObjectiveText1_loc2` text DEFAULT NULL COMMENT 'French localization of ObjectiveText1 in the quest_template table.',
  `ObjectiveText1_loc3` text DEFAULT NULL COMMENT 'German localization of ObjectiveText1 in the quest_template table.',
  `ObjectiveText1_loc4` text DEFAULT NULL COMMENT 'Chinese localization of ObjectiveText1 in the quest_template table.',
  `ObjectiveText1_loc5` text DEFAULT NULL COMMENT 'Taiwanese localization of ObjectiveText1 in the quest_template table.',
  `ObjectiveText1_loc6` text DEFAULT NULL COMMENT 'Spanish Spain localization of ObjectiveText1 in the quest_template table.',
  `ObjectiveText1_loc7` text DEFAULT NULL COMMENT 'Spanish Latin America localization of ObjectiveText1.',
  `ObjectiveText1_loc8` text DEFAULT NULL COMMENT 'Russian localization of ObjectiveText1 in the quest_template table.',
  `ObjectiveText1_loc9` longtext DEFAULT NULL,
  `ObjectiveText1_loc10` longtext DEFAULT NULL,
  `ObjectiveText1_loc11` longtext DEFAULT NULL,
  `ObjectiveText2_loc1` text DEFAULT NULL COMMENT 'Korean localization of ObjectiveText2 in the quest_template table.',
  `ObjectiveText2_loc2` text DEFAULT NULL COMMENT 'French localization of ObjectiveText2 in the quest_template table.',
  `ObjectiveText2_loc3` text DEFAULT NULL COMMENT 'German localization of ObjectiveText2 in the quest_template table.',
  `ObjectiveText2_loc4` text DEFAULT NULL COMMENT 'Chinese localization of ObjectiveText2 in the quest_template table.',
  `ObjectiveText2_loc5` text DEFAULT NULL COMMENT 'Taiwanese localization of ObjectiveText2 in the quest_template table.',
  `ObjectiveText2_loc6` text DEFAULT NULL COMMENT 'Spanish Spain localization of ObjectiveText2 in the quest_template table.',
  `ObjectiveText2_loc7` text DEFAULT NULL COMMENT 'Spanish Latin America localization of ObjectiveText2.',
  `ObjectiveText2_loc8` text DEFAULT NULL COMMENT 'Russian localization of ObjectiveText2 in the quest_template table.',
  `ObjectiveText2_loc9` longtext DEFAULT NULL,
  `ObjectiveText2_loc10` longtext DEFAULT NULL,
  `ObjectiveText2_loc11` longtext DEFAULT NULL,
  `ObjectiveText3_loc1` text DEFAULT NULL COMMENT 'Korean localization of ObjectiveText3 in the quest_template table.',
  `ObjectiveText3_loc2` text DEFAULT NULL COMMENT 'French localization of ObjectiveText3 in the quest_template table.',
  `ObjectiveText3_loc3` text DEFAULT NULL COMMENT 'German localization of ObjectiveText3 in the quest_template table.',
  `ObjectiveText3_loc4` text DEFAULT NULL COMMENT 'Chinese localization of ObjectiveText3 in the quest_template table.',
  `ObjectiveText3_loc5` text DEFAULT NULL COMMENT 'Taiwanese localization of ObjectiveText3 in the quest_template table.',
  `ObjectiveText3_loc6` text DEFAULT NULL COMMENT 'Spanish Spain localization of ObjectiveText3 in the quest_template table.',
  `ObjectiveText3_loc7` text DEFAULT NULL COMMENT 'Spanish Latin America localization of ObjectiveText3.',
  `ObjectiveText3_loc8` text DEFAULT NULL COMMENT 'Russian localization of ObjectiveText3 in the quest_template table.',
  `ObjectiveText3_loc9` longtext DEFAULT NULL,
  `ObjectiveText3_loc10` longtext DEFAULT NULL,
  `ObjectiveText3_loc11` longtext DEFAULT NULL,
  `ObjectiveText4_loc1` text DEFAULT NULL COMMENT 'Korean localization of ObjectiveText4 in the quest_template table.',
  `ObjectiveText4_loc2` text DEFAULT NULL COMMENT 'French localization of ObjectiveText4 in the quest_template table.',
  `ObjectiveText4_loc3` text DEFAULT NULL COMMENT 'German localization of ObjectiveText4 in the quest_template table.',
  `ObjectiveText4_loc4` text DEFAULT NULL COMMENT 'Chinese localization of ObjectiveText4 in the quest_template table.',
  `ObjectiveText4_loc5` text DEFAULT NULL COMMENT 'Taiwanese localization of ObjectiveText4 in the quest_template table.',
  `ObjectiveText4_loc6` text DEFAULT NULL COMMENT 'Spanish Spain localization of ObjectiveText4 in the quest_template table.',
  `ObjectiveText4_loc7` text DEFAULT NULL COMMENT 'Spanish Latin America localization of ObjectiveText4.',
  `ObjectiveText4_loc8` text DEFAULT NULL COMMENT 'Russian localization of ObjectiveText4 in the quest_template table.',
  `ObjectiveText4_loc9` longtext DEFAULT NULL,
  `ObjectiveText4_loc10` longtext DEFAULT NULL,
  `ObjectiveText4_loc11` longtext DEFAULT NULL,
  `PortraitGiverName_loc1` text DEFAULT NULL,
  `PortraitGiverName_loc2` text DEFAULT NULL,
  `PortraitGiverName_loc3` text DEFAULT NULL,
  `PortraitGiverName_loc4` text DEFAULT NULL,
  `PortraitGiverName_loc5` text DEFAULT NULL,
  `PortraitGiverName_loc6` text DEFAULT NULL,
  `PortraitGiverName_loc7` text DEFAULT NULL,
  `PortraitGiverName_loc8` text DEFAULT NULL,
  `PortraitGiverName_loc9` longtext DEFAULT NULL,
  `PortraitGiverName_loc10` longtext DEFAULT NULL,
  `PortraitGiverName_loc11` longtext DEFAULT NULL,
  `PortraitGiverText_loc1` text DEFAULT NULL,
  `PortraitGiverText_loc2` text DEFAULT NULL,
  `PortraitGiverText_loc3` text DEFAULT NULL,
  `PortraitGiverText_loc4` text DEFAULT NULL,
  `PortraitGiverText_loc5` text DEFAULT NULL,
  `PortraitGiverText_loc6` text DEFAULT NULL,
  `PortraitGiverText_loc7` text DEFAULT NULL,
  `PortraitGiverText_loc8` text DEFAULT NULL,
  `PortraitGiverText_loc9` longtext DEFAULT NULL,
  `PortraitGiverText_loc10` longtext DEFAULT NULL,
  `PortraitGiverText_loc11` longtext DEFAULT NULL,
  `PortraitTurnInName_loc1` text DEFAULT NULL,
  `PortraitTurnInName_loc2` text DEFAULT NULL,
  `PortraitTurnInName_loc3` text DEFAULT NULL,
  `PortraitTurnInName_loc4` text DEFAULT NULL,
  `PortraitTurnInName_loc5` text DEFAULT NULL,
  `PortraitTurnInName_loc6` text DEFAULT NULL,
  `PortraitTurnInName_loc7` text DEFAULT NULL,
  `PortraitTurnInName_loc8` text DEFAULT NULL,
  `PortraitTurnInName_loc9` longtext DEFAULT NULL,
  `PortraitTurnInName_loc10` longtext DEFAULT NULL,
  `PortraitTurnInName_loc11` longtext DEFAULT NULL,
  `PortraitTurnInText_loc1` text DEFAULT NULL,
  `PortraitTurnInText_loc2` text DEFAULT NULL,
  `PortraitTurnInText_loc3` text DEFAULT NULL,
  `PortraitTurnInText_loc4` text DEFAULT NULL,
  `PortraitTurnInText_loc5` text DEFAULT NULL,
  `PortraitTurnInText_loc6` text DEFAULT NULL,
  `PortraitTurnInText_loc7` text DEFAULT NULL,
  `PortraitTurnInText_loc8` text DEFAULT NULL,
  `PortraitTurnInText_loc9` longtext DEFAULT NULL,
  `PortraitTurnInText_loc10` longtext DEFAULT NULL,
  `PortraitTurnInText_loc11` longtext DEFAULT NULL,
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
-- Dumping data for table `locales_quest`
--

LOCK TABLES `locales_quest` WRITE;
/*!40000 ALTER TABLE `locales_quest` DISABLE KEYS */;
/*!40000 ALTER TABLE `locales_quest` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-07-05 13:04:46
