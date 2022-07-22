--
-- Table structure for table `character_stats`
--

DROP TABLE IF EXISTS `character_stats`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_stats` (
  `guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The guid of the character. (See character.guid).',
  `maxhealth` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Maximum amount of health that the character has.',
  `maxpower1` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Max Mana',
  `maxpower2` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Max Rage',
  `maxpower3` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Max Focus',
  `maxpower4` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Max Energy',
  `maxpower5` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Max Happiness',
  `strength` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current strength value.',
  `agility` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current agility value',
  `stamina` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current stamina value.',
  `intellect` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current intellect value.',
  `spirit` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current spirit value.',
  `armor` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current armor value.',
  `resHoly` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current holy resistance value.',
  `resFire` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current fire resistance value.',
  `resNature` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current nature resistance value.',
  `resFrost` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current frost resistance value.',
  `resShadow` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current shadow resistance value.',
  `resArcane` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current arcane resistance value.',
  `blockPct` float unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current block chance',
  `dodgePct` float unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current dodge chance.',
  `parryPct` float unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current parry chance.',
  `critPct` float unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current crit chance.',
  `rangedCritPct` float unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current ranged crit chance.',
  `spellCritPct` float unsigned NOT NULL DEFAULT '0',
  `attackPower` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current attackpower.',
  `rangedAttackPower` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current ranged attackpower.',
  `spellPower` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;