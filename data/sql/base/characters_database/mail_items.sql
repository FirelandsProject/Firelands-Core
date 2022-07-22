CREATE DATABASE  IF NOT EXISTS `characters` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `characters`;
--
-- Table structure for table `mail_items`
--

DROP TABLE IF EXISTS `mail_items`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mail_items` (
  `mail_id` int(11) NOT NULL DEFAULT '0' COMMENT 'The Mail Id (See mail.id).',
  `item_guid` int(11) NOT NULL DEFAULT '0' COMMENT 'The GUID of the item.',
  `item_template` int(11) NOT NULL DEFAULT '0' COMMENT 'The entry ID of the item in the item_template table.',
  `receiver` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Guid of the character who receives the mail.',
  PRIMARY KEY (`mail_id`,`item_guid`),
  KEY `idx_receiver` (`receiver`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;
/*!40101 SET character_set_client = @saved_cs_client */;