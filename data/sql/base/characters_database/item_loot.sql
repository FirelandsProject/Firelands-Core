CREATE DATABASE  IF NOT EXISTS `characters` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `characters`;
--
-- Table structure for table `item_loot`
--

DROP TABLE IF EXISTS `item_loot`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_loot` (
  `guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Global unique ID of the item loot entry.',
  `owner_guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the owner. (See character.guid)',
  `itemid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Item template entry ID of the item.',
  `amount` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Total number of the item.',
  `suffix` int(11) unsigned NOT NULL DEFAULT '0',
  `property` int(11) NOT NULL DEFAULT '0' COMMENT 'Item random property.',
  PRIMARY KEY (`guid`,`itemid`),
  KEY `idx_owner_guid` (`owner_guid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Item System';
/*!40101 SET character_set_client = @saved_cs_client */;