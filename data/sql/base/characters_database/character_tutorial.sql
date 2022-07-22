CREATE DATABASE  IF NOT EXISTS `characters` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `characters`;
--
-- Table structure for table `character_tutorial`
--

DROP TABLE IF EXISTS `character_tutorial`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_tutorial` (
  `account` bigint(20) unsigned NOT NULL AUTO_INCREMENT COMMENT 'The account id of the user. (See account.id).',
  `tut0` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'These values are 32bits flags.',
  `tut1` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'These values are 32bits flags.',
  `tut2` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'These values are 32bits flags.',
  `tut3` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'These values are 32bits flags.',
  `tut4` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'These values are 32bits flags.',
  `tut5` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'These values are 32bits flags.',
  `tut6` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'These values are 32bits flags.',
  `tut7` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'These values are 32bits flags.',
  PRIMARY KEY (`account`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Player System';
/*!40101 SET character_set_client = @saved_cs_client */;