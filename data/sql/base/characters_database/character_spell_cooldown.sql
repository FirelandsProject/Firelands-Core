--
-- Table structure for table `character_spell_cooldown`
--

DROP TABLE IF EXISTS `character_spell_cooldown`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_spell_cooldown` (
  `guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID (Global Unique Identifier, Low part) of the character.',
  `spell` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The spell ID (Spell Identifier). (See Spell.dbc)',
  `item` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'If the spell was casted from an item, the item ID (Item Identifier).',
  `time` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'The time when the spell cooldown will finish, measured in Unix Time.',
  PRIMARY KEY (`guid`,`spell`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;