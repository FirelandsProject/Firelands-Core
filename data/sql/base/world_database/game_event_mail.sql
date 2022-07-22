--
-- Table structure for table `game_event_mail`
--

DROP TABLE IF EXISTS `game_event_mail`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `game_event_mail` (
  `event` smallint(6) NOT NULL DEFAULT 0 COMMENT 'Event ID (See game_events.entry).',
  `raceMask` mediumint(8) unsigned NOT NULL DEFAULT 0 COMMENT 'Races of affected players.',
  `quest` mediumint(8) unsigned NOT NULL DEFAULT 0 COMMENT 'Quest (See quest_template.entry) which should be rewarded.',
  `mailTemplateId` mediumint(8) unsigned NOT NULL DEFAULT 0 COMMENT 'Mail template ID (See MailTemplate.dbc).',
  `senderEntry` mediumint(8) unsigned NOT NULL DEFAULT 0 COMMENT 'NPC entry (See creature_template.entry).',
  PRIMARY KEY (`event`,`raceMask`,`quest`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Game event system';
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
-- Dumping data for table `game_event_mail`
--

LOCK TABLES `game_event_mail` WRITE;
/*!40000 ALTER TABLE `game_event_mail` DISABLE KEYS */;
INSERT INTO `game_event_mail` (`event`, `raceMask`, `quest`, `mailTemplateId`, `senderEntry`) VALUES 
(10,1791,0,268,34365),
(46,1101,0,288,7937),
(47,690,0,289,40184),
(50,690,12420,197,27487),
(50,690,12421,212,27487),
(50,1101,12420,197,27487),
(50,1101,12421,212,27487),
(51,690,12420,198,27487),
(51,690,12421,213,27487),
(51,1101,12420,198,27487),
(51,1101,12421,213,27487),
(52,690,12420,199,27487),
(52,690,12421,214,27487),
(52,1101,12420,199,27487),
(52,1101,12421,214,27487),
(53,690,12420,200,27487),
(53,690,12421,215,27487),
(53,1101,12420,200,27487),
(53,1101,12421,215,27487),
(54,690,12420,201,27487),
(54,690,12421,216,27487),
(54,1101,12420,201,27487),
(54,1101,12421,216,27487),
(55,690,12420,202,27487),
(55,690,12421,217,27487),
(55,1101,12420,202,27487),
(55,1101,12421,217,27487),
(56,690,12421,203,27487),
(56,1101,12420,218,27487),
(57,690,12420,204,27487),
(57,690,12421,219,27487),
(57,1101,12420,204,27487),
(57,1101,12421,219,27487),
(58,690,12420,206,27487),
(58,690,12421,205,27487),
(58,1101,12420,220,27487),
(58,1101,12421,220,27487),
(59,690,12420,205,27487),
(59,690,12421,221,27487),
(59,1101,12420,206,27487),
(59,1101,12421,221,27487),
(60,690,12420,207,27487),
(60,690,12421,222,27487),
(60,1101,12420,207,27487),
(60,1101,12421,222,27487),
(61,690,12420,208,27487),
(61,690,12421,223,27487),
(61,1101,12420,208,27487),
(61,1101,12421,223,27487);
/*!40000 ALTER TABLE `game_event_mail` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-07-05 13:04:34
