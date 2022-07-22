CREATE DATABASE  IF NOT EXISTS `characters` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `characters`;
--
-- Table structure for table `character_inventory`
--

DROP TABLE IF EXISTS `character_inventory`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_inventory` (
  `guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID (Global Unique Identifier) of the character. (See character.guid).',
  `bag` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'If it isn''t 0, then it is the bag''s item GUID (Global Unique Identifier).',
  `slot` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'The slot is the slot in the bag where the item is.',
  `item` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The item''s GUID. (See item_instance.guid).',
  `item_template` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The item''s template entry (Item Identifier). (See item_template.entry).',
  PRIMARY KEY (`item`),
  KEY `idx_guid` (`guid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Player System';
/*!40101 SET character_set_client = @saved_cs_client */;