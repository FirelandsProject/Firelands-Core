--
-- Table structure for table `group_instance`
--

DROP TABLE IF EXISTS `group_instance`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `group_instance` (
  `leaderGuid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The Guid of the group leader. (See characters.guid)',
  `instance` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'ID of the Instance session the group has enterd.',
  `permanent` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT 'Boolean flag if the group is bound to the Instance or not',
  PRIMARY KEY (`leaderGuid`,`instance`),
  KEY `instance` (`instance`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;