CREATE DATABASE  IF NOT EXISTS `characters` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `characters`;
--
-- Table structure for table `pet_aura`
--

DROP TABLE IF EXISTS `pet_aura`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pet_aura` (
  `guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the target affected by the aura. (See character.guid)',
  `caster_guid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the player or creature who casted the aura. (See character.guid)',
  `item_guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Guid of the item that is casting the aura on the pet.',
  `spell` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The spell from which the aura was applied. (See Spell.dbc column 1)',
  `stackcount` int(11) unsigned NOT NULL DEFAULT '1' COMMENT 'This is the total number of times the aura can be stacked on the pet.',
  `remaincharges` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The number of charges remaining on the aura.',
  `basepoints0` int(11) NOT NULL DEFAULT '0' COMMENT 'Primary effect.',
  `basepoints1` int(11) NOT NULL DEFAULT '0' COMMENT 'Secondary effect (appears to be only healing related)',
  `basepoints2` int(11) NOT NULL DEFAULT '0' COMMENT 'Secondary effect (appears to be only damage related)',
  `periodictime0` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Primary effect over time.',
  `periodictime1` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Secondary effect (healing) over time',
  `periodictime2` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Secondary effect (damage) over time.',
  `maxduration` int(11) NOT NULL DEFAULT '0' COMMENT 'The maximum duration of the aura.',
  `remaintime` int(11) NOT NULL DEFAULT '0' COMMENT 'The time remaining in seconds on the aura. -1 means that the aura is indefinite.',
  `effIndexMask` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The effect index of the spell from which the aura came from.',
  PRIMARY KEY (`guid`,`caster_guid`,`item_guid`,`spell`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Pet System';
/*!40101 SET character_set_client = @saved_cs_client */;