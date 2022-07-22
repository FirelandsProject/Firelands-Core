CREATE DATABASE  IF NOT EXISTS `characters` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `characters`;
--
-- Table structure for table `groups`
--

DROP TABLE IF EXISTS `groups`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `groups` (
  `groupId` int(11) unsigned NOT NULL COMMENT 'Uniquie ID for the group.',
  `leaderGuid` int(11) unsigned NOT NULL COMMENT 'Guid of the group''s leader',
  `mainTank` int(11) unsigned NOT NULL COMMENT 'Guid of the character who is that main tank.',
  `mainAssistant` int(11) unsigned NOT NULL COMMENT 'Guid of the character who is that main assist.',
  `lootMethod` tinyint(4) unsigned NOT NULL COMMENT 'This is the loot method used by the group.',
  `looterGuid` int(11) unsigned NOT NULL COMMENT 'Guid of the character who retrieves the loot.',
  `lootThreshold` tinyint(4) unsigned NOT NULL COMMENT 'The quality of the items in the loot to be rolled for.',
  `icon1` int(11) unsigned NOT NULL COMMENT 'Guid of the target that has the star raid target on it.',
  `icon2` int(11) unsigned NOT NULL COMMENT 'Guid of the target that has the circle raid target on it.',
  `icon3` int(11) unsigned NOT NULL COMMENT 'Guid of target that has the dimond raid target on it.',
  `icon4` int(11) unsigned NOT NULL COMMENT 'Guid of the target that has the triangler raid target on it.',
  `icon5` int(11) unsigned NOT NULL COMMENT 'Guid of the target that has the moon raid target on it.',
  `icon6` int(11) unsigned NOT NULL COMMENT 'Guid of the target that has the square raid target on it.',
  `icon7` int(11) unsigned NOT NULL COMMENT 'Guid of the target that has the cross raid target on it.',
  `icon8` int(11) unsigned NOT NULL COMMENT 'Guid of the target that has the skull raid target on it.',
  `groupType` tinyint(1) unsigned NOT NULL,
  `difficulty` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `raiddifficulty` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`groupId`),
  UNIQUE KEY `leaderGuid` (`leaderGuid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Groups';
/*!40101 SET character_set_client = @saved_cs_client */;