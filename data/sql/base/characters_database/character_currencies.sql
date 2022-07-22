--
-- Table structure for table `character_currencies`
--

DROP TABLE IF EXISTS `character_currencies`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_currencies` (
  `guid` int(11) unsigned NOT NULL DEFAULT '0',
  `id` smallint(5) unsigned NOT NULL DEFAULT '0',
  `totalCount` int(11) unsigned NOT NULL DEFAULT '0',
  `weekCount` int(11) unsigned NOT NULL DEFAULT '0',
  `seasonCount` int(11) unsigned NOT NULL DEFAULT '0',
  `flags` tinyint(2) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
