--
-- Table structure for table `character_aura`
--

DROP TABLE IF EXISTS `character_aura`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_aura` (
  `guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID (Full Global Unique Identifier) of the target affected by the aura.',
  `caster_guid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID (Global Unique Identifier) of the player who cast the aura.',
  `item_guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID (Global Unique Identifier) of the item which casted the aura.',
  `spell` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The spell from which the aura was applied. (See Spell.dbc)',
  `stackcount` int(11) unsigned NOT NULL DEFAULT '1' COMMENT 'Determines how many stacks of the spell the character has.',
  `remaincharges` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The number of charges remaining on the aura.',
  `basepoints0` int(11) NOT NULL DEFAULT '0' COMMENT 'Primary effect.',
  `basepoints1` int(11) NOT NULL DEFAULT '0' COMMENT 'Secondary effect (healing)',
  `basepoints2` int(11) NOT NULL DEFAULT '0' COMMENT 'Secondary effect (damage)',
  `periodictime0` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Primary effect over time counter.',
  `periodictime1` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Primary effect (healing) over time counter.',
  `periodictime2` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Primary effect (damage) over time counter.',
  `maxduration` int(11) NOT NULL DEFAULT '0' COMMENT 'The maximum duration of the aura.',
  `remaintime` int(11) NOT NULL DEFAULT '0' COMMENT 'The time remaining in seconds on the aura. -1 means that the aura is indefinite.',
  `effIndexMask` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The effect index of the spell from which the aura came from.',
  PRIMARY KEY (`guid`,`caster_guid`,`item_guid`,`spell`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Player System';
/*!40101 SET character_set_client = @saved_cs_client */;