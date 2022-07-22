CREATE DATABASE  IF NOT EXISTS `characters` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `characters`;
--
-- Table structure for table `instance`
--

DROP TABLE IF EXISTS `instance`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `instance` (
  `id` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The instance ID. This number is unique to every instance.',
  `map` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The map ID the instance is in. (See Map.dbc)',
  `resettime` bigint(40) unsigned NOT NULL DEFAULT '0' COMMENT 'The time when the instance will be reset, in Unix time.',
  `difficulty` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `encountersMask` int(10) unsigned NOT NULL DEFAULT '0',
  `data` longtext COMMENT 'Specific data belonging to the individual instance.',
  PRIMARY KEY (`id`),
  KEY `map` (`map`),
  KEY `resettime` (`resettime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;