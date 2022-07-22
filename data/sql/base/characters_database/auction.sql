--
-- Table structure for table `auction`
--

DROP TABLE IF EXISTS `auction`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `auction` (
  `id` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Unique identifier for every auction.',
  `houseid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The AuctionHouseId (See AuctionHouse.dbc)',
  `itemguid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the item that is up for auction. (See item_instance.guid)',
  `item_template` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The entry of the item up for auction. See (item_template.entry)',
  `item_count` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The stack count of the item',
  `item_randompropertyid` int(11) NOT NULL DEFAULT '0' COMMENT 'A link to the Item''s RandomProperty Id (See item_template.Randompropertyid).',
  `itemowner` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the owner of the item up for auction. (See character.guid)',
  `buyoutprice` bigint(40) NOT NULL DEFAULT '0' COMMENT 'The buyout price of the item in copper.',
  `time` bigint(40) unsigned NOT NULL DEFAULT '0' COMMENT 'The time when this auction will end, measured in Unix time.',
  `moneyTime` bigint(40) unsigned NOT NULL DEFAULT '0',
  `buyguid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the highest bidder. See (character.guid)',
  `lastbid` bigint(40) NOT NULL DEFAULT '0' COMMENT 'The amount of copper of the last bid made on the item.',
  `startbid` bigint(40) NOT NULL DEFAULT '0' COMMENT 'The amount of copper of the starting bid made on the item.',
  `deposit` bigint(40) NOT NULL DEFAULT '0' COMMENT 'The amount of copper spent on the deposit.',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;