--
-- Table structure for table `character_skills`
--

DROP TABLE IF EXISTS `character_skills`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_skills` (
  `guid` int(11) unsigned NOT NULL COMMENT 'The GUID (Global Unique Identifier) of the character. (See character.guid).',
  `skill` mediumint(9) unsigned NOT NULL COMMENT 'The list of skills a character knows. A listing of those can be found in here.',
  `value` mediumint(9) unsigned NOT NULL COMMENT 'The current skill rank(value) of the character skills a character has.',
  `max` mediumint(9) unsigned NOT NULL COMMENT 'The highest possible value for the given skill within a given rank.',
  PRIMARY KEY (`guid`,`skill`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Player System';
/*!40101 SET character_set_client = @saved_cs_client */;