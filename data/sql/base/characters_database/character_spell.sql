--
-- Table structure for table `character_spell`
--

DROP TABLE IF EXISTS `character_spell`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_spell` (
  `guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID (Global Unique Identifier) of the character. (See character.guid).',
  `spell` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The spell ID (Spell Identifier). (See Spell.dbc)',
  `active` tinyint(3) unsigned NOT NULL DEFAULT '1' COMMENT 'Boolean 1 or 0 signifying whether the spell is active (appears in the spell book',
  `disabled` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Boolean flag 0 or 1 when spell is disabled because of talents.',
  PRIMARY KEY (`guid`,`spell`),
  KEY `Idx_spell` (`spell`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Player System';
/*!40101 SET character_set_client = @saved_cs_client */;
