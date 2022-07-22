CREATE DATABASE  IF NOT EXISTS `characters` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `characters`;
--
-- Table structure for table `guild_eventlog`
--

DROP TABLE IF EXISTS `guild_eventlog`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `guild_eventlog` (
  `guildid` int(11) unsigned NOT NULL COMMENT 'Id of related guild. (See guild.guildid).',
  `LogGuid` int(11) unsigned NOT NULL COMMENT 'A unique identifier given to each log entry to distinguish it.',
  `EventType` tinyint(1) unsigned NOT NULL COMMENT 'The Guild Event Type.',
  `PlayerGuid1` int(11) unsigned NOT NULL COMMENT 'Guid of character who made the change. (See character.guid)',
  `PlayerGuid2` int(11) unsigned NOT NULL COMMENT 'Guid of character who was changed. (See character.guid)',
  `NewRank` tinyint(2) unsigned NOT NULL COMMENT 'Id of new rank of PlayerGuid2. (See guild_rank.rid)',
  `TimeStamp` bigint(20) unsigned NOT NULL COMMENT 'Unix timestamp of when the event happened.',
  PRIMARY KEY (`guildid`,`LogGuid`),
  KEY `Idx_PlayerGuid1` (`PlayerGuid1`),
  KEY `Idx_PlayerGuid2` (`PlayerGuid2`),
  KEY `Idx_LogGuid` (`LogGuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Guild Eventlog';
/*!40101 SET character_set_client = @saved_cs_client */;