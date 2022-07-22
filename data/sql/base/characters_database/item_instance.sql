--
-- Table structure for table `item_instance`
--

DROP TABLE IF EXISTS `item_instance`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_instance` (
  `guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the item. This number is unique for each item instance.',
  `owner_guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the character who has ownership of this item. (See character.guid)',
  `data` longtext COMMENT 'Much like the playerbytes fields in the characters table.',
  `text` longtext,
  PRIMARY KEY (`guid`),
  KEY `idx_owner_guid` (`owner_guid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Item System';
/*!40101 SET character_set_client = @saved_cs_client */;