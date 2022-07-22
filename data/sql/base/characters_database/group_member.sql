--
-- Table structure for table `group_member`
--

DROP TABLE IF EXISTS `group_member`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `group_member` (
  `groupId` int(11) unsigned NOT NULL COMMENT 'The group ID from the groups table.',
  `memberGuid` int(11) unsigned NOT NULL COMMENT 'The GUID of the character in the group.',
  `assistant` tinyint(1) unsigned NOT NULL COMMENT 'Flag stating whether or not the character is the assist.',
  `subgroup` smallint(6) unsigned NOT NULL COMMENT 'This is the group number within a raid (8 groups max)',
  PRIMARY KEY (`groupId`,`memberGuid`),
  KEY `Idx_memberGuid` (`memberGuid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Groups';
/*!40101 SET character_set_client = @saved_cs_client */;