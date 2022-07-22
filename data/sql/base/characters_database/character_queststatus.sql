CREATE DATABASE  IF NOT EXISTS `characters` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `characters`;
--
-- Table structure for table `character_queststatus`
--

DROP TABLE IF EXISTS `character_queststatus`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_queststatus` (
  `guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the character. (See character.guid).',
  `quest` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The quest ID. (See quest_template.entry).',
  `status` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The current quest status.',
  `rewarded` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT 'Boolean 1 or 0 representing whether the quest has been rewarded or not.',
  `explored` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT 'Boolean 1 or 0 representing if the character has explored what was needed.',
  `timer` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'Remaining time left on the quest if the quest has a timer (See quest_template).',
  `mobcount1` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Current count of the first number of kills or casts.',
  `mobcount2` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Current count of the second number of kills or casts.',
  `mobcount3` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Current count of the third number of kills or casts.',
  `mobcount4` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Current count of the forth number of kills or casts.',
  `itemcount1` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Current item count for the first item in a delivery quest, if any.',
  `itemcount2` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Current item count for the second item in a delivery quest, if any.',
  `itemcount3` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Current item count for the third item in a delivery quest, if any.',
  `itemcount4` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Current item count for the forth item in a delivery quest, if any.',
  `itemcount5` int(11) unsigned NOT NULL DEFAULT '0',
  `itemcount6` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`quest`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Player System';
/*!40101 SET character_set_client = @saved_cs_client */;