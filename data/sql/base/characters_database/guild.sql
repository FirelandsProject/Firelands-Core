--
-- Table structure for table `guild`
--

DROP TABLE IF EXISTS `guild`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `guild` (
  `guildid` int(6) unsigned NOT NULL DEFAULT '0' COMMENT 'The ID of the guild.',
  `name` varchar(255) NOT NULL DEFAULT '' COMMENT 'The guild name.',
  `leaderguid` int(6) unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the character who created the guild. (See characters.guid)',
  `EmblemStyle` int(5) NOT NULL DEFAULT '0' COMMENT 'The emblem style of the guild tabard.',
  `EmblemColor` int(5) NOT NULL DEFAULT '0' COMMENT 'The emblem color of the guild tabard.',
  `BorderStyle` int(5) NOT NULL DEFAULT '0' COMMENT 'The border style of the guild tabard.',
  `BorderColor` int(5) NOT NULL DEFAULT '0' COMMENT 'The border color of the guild tabard.',
  `BackgroundColor` int(5) NOT NULL DEFAULT '0' COMMENT 'The background color of the guild tabard.',
  `info` varchar(500) NOT NULL DEFAULT '' COMMENT 'The text message that appears in the Guild Information box.',
  `motd` varchar(128) NOT NULL DEFAULT '' COMMENT 'The text that appears in the Message Of The Day box.',
  `createdate` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'The date when the guild was created.',
  `BankMoney` bigint(20) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guildid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Guild System';
/*!40101 SET character_set_client = @saved_cs_client */;