--
-- Table structure for table `ahbot_category`
--

DROP TABLE IF EXISTS `ahbot_category`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ahbot_category` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT 'Unique identifier of the category',
  `category` varchar(45) DEFAULT NULL COMMENT 'An Item category Id',
  `multiplier` decimal(20,2) NOT NULL COMMENT 'The cost multiplier',
  `max_auction_count` bigint(20) NOT NULL COMMENT 'The max number of items of this category in the AH. ',
  `expire_time` bigint(20) NOT NULL COMMENT 'The UNIX time when the item will expire.',
  PRIMARY KEY (`id`),
  KEY `helper` (`category`,`multiplier`,`expire_time`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;