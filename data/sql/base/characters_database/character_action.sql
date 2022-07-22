--
-- Table structure for table `character_action`
--

DROP TABLE IF EXISTS `character_action`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_action` (
  `guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the character (See character.id).',
  `spec` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `button` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'The ID of the button on the action bar where the action icon will be placed.',
  `action` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Depending on the type value, this could be various values.',
  `type` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'The type of action.',
  PRIMARY KEY (`guid`,`spec`,`button`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Player System';
/*!40101 SET character_set_client = @saved_cs_client */;