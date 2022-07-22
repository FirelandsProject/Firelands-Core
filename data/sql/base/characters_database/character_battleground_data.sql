--
-- Table structure for table `character_battleground_data`
--

DROP TABLE IF EXISTS `character_battleground_data`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_battleground_data` (
  `guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The character battleground datar global unique identifier.',
  `instance_id` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The unique ID of the battleground instance the character has entered.',
  `team` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Neutral, Horde, or Alliance',
  `join_x` float NOT NULL DEFAULT '0' COMMENT 'The character''s X coordinate in the battleground.',
  `join_y` float NOT NULL DEFAULT '0' COMMENT 'The character''s Y coordinate in the battleground.',
  `join_z` float NOT NULL DEFAULT '0' COMMENT 'The character''s Z coordinate in the battleground.',
  `join_o` float NOT NULL DEFAULT '0' COMMENT 'The character''s orientation in the battleground.',
  `join_map` int(11) NOT NULL DEFAULT '0' COMMENT 'Battleground map ID where the character will spawn.',
  `taxi_start` int(11) NOT NULL DEFAULT '0',
  `taxi_end` int(11) NOT NULL DEFAULT '0',
  `mount_spell` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Player System';
/*!40101 SET character_set_client = @saved_cs_client */;