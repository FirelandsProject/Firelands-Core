--
-- Table structure for table `character_homebind`
--

DROP TABLE IF EXISTS `character_homebind`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_homebind` (
  `guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID (Global Unique Identifier) of the character. (See character.guid).',
  `map` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The Map Identifier where the character gets teleported to. (See Maps.dbc).',
  `zone` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The Zone Identifier where the character gets teleported to.',
  `position_x` float NOT NULL DEFAULT '0' COMMENT 'The x position where the character gets teleported to.',
  `position_y` float NOT NULL DEFAULT '0' COMMENT 'The y position where the character gets teleported to.',
  `position_z` float NOT NULL DEFAULT '0' COMMENT 'The z position where the character gets teleported to.',
  PRIMARY KEY (`guid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Player System';
/*!40101 SET character_set_client = @saved_cs_client */;