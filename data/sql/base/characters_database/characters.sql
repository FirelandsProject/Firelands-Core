CREATE DATABASE  IF NOT EXISTS `characters` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `characters`;
--
-- Table structure for table `characters`
--

DROP TABLE IF EXISTS `characters`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `characters` (
  `guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The character global unique identifier.',
  `account` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The account ID in which this character resides (See account.id) in the realm db.',
  `name` varchar(12) NOT NULL DEFAULT '' COMMENT 'The name of the character.',
  `race` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'The race of the character.',
  `class` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'The Class Id of the character (See chrclasses.dbc).',
  `gender` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'The Sex/Gender of the character.',
  `level` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'The current level of the designated player.',
  `xp` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'The total amount of xp that the signified player has.',
  `money` bigint(40) unsigned NOT NULL DEFAULT '0' COMMENT 'This is the amount of copper the character possesses.',
  `playerBytes` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'This defines the physical attributes of the character.',
  `playerBytes2` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'This defines the facial hair physical attribute of the character.',
  `playerFlags` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'These are the player flags, such as GM mode on or off.',
  `position_x` float NOT NULL DEFAULT '0' COMMENT 'The x position of the character''s location.',
  `position_y` float NOT NULL DEFAULT '0' COMMENT 'The y position of the character''s location.',
  `position_z` float NOT NULL DEFAULT '0' COMMENT 'The z position of the character''s location.',
  `map` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The map ID the character is in (See maps.dbc)',
  `dungeon_difficulty` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `orientation` float NOT NULL DEFAULT '0' COMMENT 'The orientation the character is facing. (North = 0.0, South = 3.14159)',
  `taximask` longtext,
  `online` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Records whether the character is online (1) or offline (0).',
  `cinematic` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Boolean 1 or 0 controlling whether the start cinematic has been shown or not.',
  `totaltime` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The total time that the character has been active in the world.',
  `leveltime` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The total time the character has spent in the world at the current level.',
  `logout_time` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'The time when the character last logged out, measured in Unix time.',
  `is_logout_resting` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Boolean 1 or 0 controlling if the character is currently in a resting zone.',
  `rest_bonus` float NOT NULL DEFAULT '0' COMMENT 'This is the rest bonus for the character.',
  `resettalents_cost` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The cost for the character to reset its talents, measured in copper.',
  `resettalents_time` bigint(20) unsigned NOT NULL DEFAULT '0',
  `primary_trees` varchar(10) NOT NULL DEFAULT '0 0 ',
  `trans_x` float NOT NULL DEFAULT '0' COMMENT 'The X coordinate of the character on the transport it is travelling on.',
  `trans_y` float NOT NULL DEFAULT '0' COMMENT 'The Y coordinate of the character on the transport it is travelling on.',
  `trans_z` float NOT NULL DEFAULT '0' COMMENT 'The Z coordinate of the character on the transport it is travelling on.',
  `trans_o` float NOT NULL DEFAULT '0' COMMENT 'The orientation of the character on the transport it is travelling on.',
  `transguid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'This is the transprt the character is currently travelling on.',
  `extra_flags` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'These flags control certain player specific attributes, mostly GM features.',
  `stable_slots` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT 'The number of stable slots the player has available. Maximum is 2.',
  `at_login` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The status of the character.',
  `zone` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The zone ID the character is in.',
  `death_expire_time` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'Time when a character can be resurrected.',
  `taxi_path` text COMMENT 'Stores the players current taxi path (TaxiPath.dbc) if logged off while on one.',
  `totalKills` int(10) unsigned NOT NULL DEFAULT '0',
  `todayKills` smallint(5) unsigned NOT NULL DEFAULT '0',
  `yesterdayKills` smallint(5) unsigned NOT NULL DEFAULT '0',
  `chosenTitle` int(10) unsigned NOT NULL DEFAULT '0',
  `watchedFaction` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'The faction whose rep is being followed on the experience bar.',
  `drunk` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'This represents the alcoholic strength of the drink.',
  `health` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'The character''s health when logging out.',
  `power1` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'If a mana user, then this is a character''s mana level when logging out.',
  `power2` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'If a warrior, then this is a character''s rage level when logging out.',
  `power3` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'This is a hunter pet''s focus level.',
  `power4` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'If a rogue, then this is a character''s energy level when logging out.',
  `power5` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'This is the current active pet''s happiness level.',
  `specCount` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `activeSpec` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `exploredZones` longtext COMMENT 'This is a record of all areas discovered by the character.',
  `equipmentCache` longtext COMMENT 'This is a record of all items that are currently equipped.',
  `knownTitles` longtext,
  `actionBars` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'This represents which action bars are currently active.',
  `slot` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `deleteInfos_Account` int(11) unsigned DEFAULT NULL COMMENT 'This is the account number from the characters table.',
  `deleteInfos_Name` varchar(12) DEFAULT NULL COMMENT 'The is the name of the character being deleted.',
  `deleteDate` bigint(20) unsigned DEFAULT NULL COMMENT 'This is the date the character was deleted,',
  `createdDate` int unsigned DEFAULT NULL COMMENT 'This is the character creation date',
  PRIMARY KEY (`guid`),
  KEY `idx_account` (`account`),
  KEY `idx_online` (`online`),
  KEY `idx_name` (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Player System';
-- Table structure for table `corpse`
--

DROP TABLE IF EXISTS `corpse`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `corpse` (
  `guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The character global unique identifier of the corpse.',
  `player` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The character global unique identifier. (See characters.guid).',
  `position_x` float NOT NULL DEFAULT '0' COMMENT 'The x position of the character''s corpse location.',
  `position_y` float NOT NULL DEFAULT '0' COMMENT 'The y position of the character''s corpse location.',
  `position_z` float NOT NULL DEFAULT '0' COMMENT 'The z position of the character''s corpse location.',
  `orientation` float NOT NULL DEFAULT '0' COMMENT 'The orientation of the corpse. (North = 0.0, South = 3.14159)',
  `map` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The map ID the corpse is in. (See maps.dbc)',
  `phaseMask` int(11) unsigned NOT NULL DEFAULT '1',
  `time` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'Time of death.',
  `corpse_type` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'The Display Type of the corpse.',
  `instance` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The instance ID that the corpse is in.',
  PRIMARY KEY (`guid`),
  KEY `idx_type` (`corpse_type`),
  KEY `instance` (`instance`),
  KEY `Idx_player` (`player`),
  KEY `Idx_time` (`time`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Death System';
/*!40101 SET character_set_client = @saved_cs_client */;