--
-- Table structure for table `character_reputation`
--

DROP TABLE IF EXISTS `character_reputation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_reputation` (
  `guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the character. (See character.guid).',
  `faction` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The faction ID that the character has the given reputation in (See Faction.dbc).',
  `standing` int(11) NOT NULL DEFAULT '0' COMMENT 'The current reputation value that the character has.',
  `flags` int(11) NOT NULL DEFAULT '0' COMMENT 'This field is a bitmask containing flags that apply to the faction.',
  PRIMARY KEY (`guid`,`faction`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Player System';
/*!40101 SET character_set_client = @saved_cs_client */;