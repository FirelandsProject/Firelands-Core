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
-- Table structure for table `character_pet`
--

DROP TABLE IF EXISTS `character_pet`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `character_pet` (
  `id` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The special pet ID. This is a unique identifier among all pets.',
  `entry` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The creature entry of this pet. (See creature_template.entry).',
  `owner` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the pet''s owner. (See character.guid).',
  `modelid` int(11) unsigned DEFAULT '0' COMMENT 'The model ID to use to display the pet.',
  `CreatedBySpell` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The ID of the spell that has created this pet.',
  `PetType` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'The type of pet that this is. 0 = summoned pet, 1 = tamed pet',
  `level` int(11) unsigned NOT NULL DEFAULT '1' COMMENT 'The current level of the pet.',
  `exp` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The current experience that this pet has.',
  `Reactstate` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT 'The current reaction state of the pet (passive, aggressive, etc).',
  `loyaltypoints` int(11) NOT NULL DEFAULT '0' COMMENT 'The current amount of loyalty points the pet has.',
  `loyalty` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The pet''s loyalty level.',
  `trainpoint` int(11) NOT NULL DEFAULT '0' COMMENT 'Total training points available to spend.',
  `name` varchar(100) DEFAULT 'Pet' COMMENT 'The pet''s name.',
  `renamed` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT 'Boolean 1 or 0. 1 = Pet has been renamed.',
  `slot` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The pet slot that the pet is in. The slot is a number between 0 and 3 inclusive.',
  `curhealth` int(11) unsigned NOT NULL DEFAULT '1' COMMENT 'The current pet health at the time it was saved to DB.',
  `curmana` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The current pet mana at the time it was saved to DB.',
  `curhappiness` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The current pet happiness.',
  `savetime` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'The time when the pet was last saved, in Unix time.',
  `resettalents_cost` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'This is the cost to untrain the pet talents.',
  `resettalents_time` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'How many times the pets talents have been reset.',
  `abdata` longtext COMMENT 'Data about pet action bar and action type ten pairs of action bar entry.',
  `teachspelldata` longtext COMMENT 'This field holds IDs of spells that have been taught to this pet.',
  PRIMARY KEY (`id`),
  KEY `owner` (`owner`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Pet System';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `character_pet`
--

LOCK TABLES `character_pet` WRITE;
/*!40000 ALTER TABLE `character_pet` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_pet` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-07-19  0:45:16
