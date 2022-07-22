--
-- Table structure for table `pet_spell_cooldown`
--

DROP TABLE IF EXISTS `pet_spell_cooldown`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pet_spell_cooldown` (
  `guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the pet. (See character_pet.id)',
  `spell` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The spell ID to which the cooldown applies.',
  `time` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'The time when the cooldown expires, in Unix time.',
  PRIMARY KEY (`guid`,`spell`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;