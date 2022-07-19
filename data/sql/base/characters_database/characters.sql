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
-- Table structure for table `characters`
--

DROP TABLE IF EXISTS `characters`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `characters` (
  `guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The character global unique identifier.',
  `account` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The account ID in which this character resides (See account.id) in the realm db.',
  `name` varchar(12) NOT NULL DEFAULT '' COMMENT 'The name of the character.',
  `race` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'The race of the character.',
  `class` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'The Class Id of the character (See chrclasses.dbc).',
  `gender` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'The Sex/Gender of the character.',
  `level` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'The current level of the designated player.',
  `xp` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'The total amount of xp that the signified player has.',
  `money` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'This is the amount of copper the character possesses.',
  `playerBytes` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'This defines the physical attributes of the character.',
  `playerBytes2` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'This defines the facial hair physical attribute of the character.',
  `playerFlags` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'These are the player flags, such as GM mode on or off.',
  `position_x` float NOT NULL DEFAULT '0' COMMENT 'The x position of the character''s location.',
  `position_y` float NOT NULL DEFAULT '0' COMMENT 'The y position of the character''s location.',
  `position_z` float NOT NULL DEFAULT '0' COMMENT 'The z position of the character''s location.',
  `map` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The map ID the character is in (See maps.dbc)',
  `orientation` float NOT NULL DEFAULT '0' COMMENT 'The orientation the character is facing. (North = 0.0, South = 3.14159)',
  `taximask` longtext,
  `online` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Records whether the character is online (1) or offline (0).',
  `cinematic` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Boolean 1 or 0 controlling whether the start cinematic has been shown or not.',
  `totaltime` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The total time that the character has been active in the world.',
  `leveltime` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The total time the character has spent in the world at the current level.',
  `logout_time` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'The time when the character last logged out, measured in Unix time.',
  `is_logout_resting` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Boolean 1 or 0 controlling if the character is currently in a resting zone.',
  `rest_bonus` float NOT NULL DEFAULT '0' COMMENT 'This is the rest bonus for the character.',
  `resettalents_cost` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The cost for the character to reset its talents, measured in copper.',
  `resettalents_time` bigint(20) unsigned NOT NULL DEFAULT '0',
  `trans_x` float NOT NULL DEFAULT '0' COMMENT 'The X coordinate of the character on the transport it is travelling on.',
  `trans_y` float NOT NULL DEFAULT '0' COMMENT 'The Y coordinate of the character on the transport it is travelling on.',
  `trans_z` float NOT NULL DEFAULT '0' COMMENT 'The Z coordinate of the character on the transport it is travelling on.',
  `trans_o` float NOT NULL DEFAULT '0' COMMENT 'The orientation of the character on the transport it is travelling on.',
  `transguid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'This is the transprt the character is currently travelling on.',
  `extra_flags` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'These flags control certain player specific attributes, mostly GM features.',
  `stable_slots` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT 'The number of stable slots the player has available. Maximum is 2.',
  `at_login` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The status of the character.',
  `zone` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The zone ID the character is in.',
  `death_expire_time` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'Time when a character can be resurrected.',
  `taxi_path` text COMMENT 'Stores the players current taxi path (TaxiPath.dbc) if logged off while on one.',
  `honor_highest_rank` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'This shows the highest honor rank ever earned.',
  `honor_standing` int(11) unsigned NOT NULL DEFAULT '0',
  `stored_honor_rating` float NOT NULL DEFAULT '0' COMMENT 'This is the current amount of honor points.',
  `stored_dishonorable_kills` int(11) NOT NULL DEFAULT '0' COMMENT 'The is the total dishonorable kills made by the character.',
  `stored_honorable_kills` int(11) NOT NULL DEFAULT '0' COMMENT 'The is the total honorable kills made by the character.',
  `watchedFaction` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'The faction whose rep is being followed on the experience bar.',
  `drunk` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT 'This represents the alcoholic strength of the drink.',
  `health` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'The character''s health when logging out.',
  `power1` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'If a mana user, then this is a character''s mana level when logging out.',
  `power2` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'If a warrior, then this is a character''s rage level when logging out.',
  `power3` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'This is a hunter pet''s focus level.',
  `power4` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'If a rogue, then this is a character''s energy level when logging out.',
  `power5` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'This is the current active pet''s happiness level.',
  `exploredZones` longtext COMMENT 'This is a record of all areas discovered by the character.',
  `equipmentCache` longtext COMMENT 'This is a record of all items that are currently equipped.',
  `ammoId` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'This is the item_template Entry ID of the ammo currently equipped.',
  `actionBars` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'This represents which action bars are currently active.',
  `deleteInfos_Account` int(11) unsigned DEFAULT NULL COMMENT 'This is the account number from the characters table.',
  `deleteInfos_Name` varchar(12) DEFAULT NULL COMMENT 'The is the name of the character being deleted.',
  `deleteDate` bigint(20) unsigned DEFAULT NULL COMMENT 'This is the date the character was deleted,',
  `createdDate` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`),
  KEY `idx_account` (`account`),
  KEY `idx_online` (`online`),
  KEY `idx_name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Player System';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `characters`
--

LOCK TABLES `characters` WRITE;
/*!40000 ALTER TABLE `characters` DISABLE KEYS */;
/*!40000 ALTER TABLE `characters` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-07-19  0:45:00
