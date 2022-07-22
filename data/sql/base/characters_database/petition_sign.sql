CREATE DATABASE  IF NOT EXISTS `characters` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `characters`;
--
-- Table structure for table `petition_sign`
--

DROP TABLE IF EXISTS `petition_sign`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `petition_sign` (
  `ownerguid` int(10) unsigned NOT NULL COMMENT 'The GUID of the owner that is trying to make the guild/arena team.',
  `petitionguid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the charter item. (See item_template.guid)',
  `playerguid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the player that has signed the charter. (See character.guid)',
  `player_account` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The account ID of the player that has signed the charter (See account.id).',
  PRIMARY KEY (`petitionguid`,`playerguid`),
  KEY `Idx_playerguid` (`playerguid`),
  KEY `Idx_ownerguid` (`ownerguid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Guild System';
/*!40101 SET character_set_client = @saved_cs_client */;
