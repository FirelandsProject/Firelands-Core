--
-- Table structure for table `guild_rank`
--

DROP TABLE IF EXISTS `guild_rank`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `guild_rank` (
  `guildid` int(6) unsigned NOT NULL DEFAULT '0' COMMENT 'The guild ID that the rank is part of. (See guild.guildid)',
  `rid` int(11) unsigned NOT NULL COMMENT 'The particular rank ID. This number must be unique to each rank in a guild.',
  `rname` varchar(255) NOT NULL DEFAULT '' COMMENT 'The name of the rank that is displayed in-game.',
  `rights` int(3) unsigned NOT NULL DEFAULT '0' COMMENT 'The rights a player with this rank has in the guild.',
  `BankMoneyPerDay` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guildid`,`rid`),
  KEY `Idx_rid` (`rid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Guild System';
/*!40101 SET character_set_client = @saved_cs_client */;