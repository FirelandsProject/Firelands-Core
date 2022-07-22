--
-- Table structure for table `locales_gossip_menu_option`
--

DROP TABLE IF EXISTS `locales_gossip_menu_option`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `locales_gossip_menu_option` (
  `menu_id` smallint(6) unsigned NOT NULL DEFAULT 0 COMMENT 'Reference to gossip_menu_option.menu_id.',
  `id` smallint(6) unsigned NOT NULL DEFAULT 0 COMMENT 'Reference to gossip_menu_option.id.',
  `option_text_loc1` text DEFAULT NULL COMMENT 'Korean localization of option_text in the gossip_menu_option table table.',
  `option_text_loc2` text DEFAULT NULL COMMENT 'French localization of option_text in the gossip_menu_option table table.',
  `option_text_loc3` text DEFAULT NULL COMMENT 'German localization of option_text in the gossip_menu_option table table.',
  `option_text_loc4` text DEFAULT NULL COMMENT 'Chinese localization of option_text in the gossip_menu_option table table.',
  `option_text_loc5` text DEFAULT NULL COMMENT 'Taiwanese localization of option_text in the gossip_menu_option table table.',
  `option_text_loc6` text DEFAULT NULL COMMENT 'Spanish Spain localization of option_text.',
  `option_text_loc7` text DEFAULT NULL COMMENT 'Spanish Latin America localization of option_text.',
  `option_text_loc8` text DEFAULT NULL COMMENT 'Russian localization of option_text in the gossip_menu_option table table.',
  `option_text_loc9` text DEFAULT NULL,
  `option_text_loc10` text DEFAULT NULL,
  `option_text_loc11` text DEFAULT NULL,
  `box_text_loc1` text DEFAULT NULL COMMENT 'Korean localization of box_text in the gossip_menu_option table table.',
  `box_text_loc2` text DEFAULT NULL COMMENT 'French localization of box_text in the gossip_menu_option table table.',
  `box_text_loc3` text DEFAULT NULL COMMENT 'German localization of box_text in the gossip_menu_option table table.',
  `box_text_loc4` text DEFAULT NULL COMMENT 'Chinese localization of box_text in the gossip_menu_option table table.',
  `box_text_loc5` text DEFAULT NULL COMMENT 'Taiwanese localization of box_text in the gossip_menu_option table table.',
  `box_text_loc6` text DEFAULT NULL COMMENT 'Spanish Spain localization of box_text in the gossip_menu_option table table.',
  `box_text_loc7` text DEFAULT NULL COMMENT 'Spanish Latin America localization of box_text.',
  `box_text_loc8` text DEFAULT NULL COMMENT 'Russian localization of box_text in the gossip_menu_option table table.',
  `box_text_loc9` text DEFAULT NULL,
  `box_text_loc10` text DEFAULT NULL,
  `box_text_loc11` text DEFAULT NULL,
  PRIMARY KEY (`menu_id`,`id`)
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
-- Dumping data for table `locales_gossip_menu_option`
--

LOCK TABLES `locales_gossip_menu_option` WRITE;
/*!40000 ALTER TABLE `locales_gossip_menu_option` DISABLE KEYS */;
/*!40000 ALTER TABLE `locales_gossip_menu_option` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-07-05 13:04:44
