CREATE DATABASE  IF NOT EXISTS `characters` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `characters`;
--
-- Table structure for table `mail`
--

DROP TABLE IF EXISTS `mail`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mail` (
  `id` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Unique message id. Every new message gets a new auto incremented id.',
  `messageType` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'message_type.',
  `stationery` tinyint(3) NOT NULL DEFAULT '41' COMMENT 'The StationeryID (See Stationery.dbc).',
  `mailTemplateId` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'The Mail Template Id (See MailTemplate.dbc)',
  `sender` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Guid of the character who sends the mail.',
  `receiver` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Guid of the character who receives the mail.',
  `subject` longtext COMMENT 'The Subject of the mail.',
  `body` longtext,
  `has_items` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Boolean value showing whether or not atems have been sent.',
  `expire_time` bigint(40) unsigned NOT NULL DEFAULT '0' COMMENT 'Current Unix Time + Unix Time till expiry.',
  `deliver_time` bigint(40) unsigned NOT NULL DEFAULT '0' COMMENT 'Current Unix Time + Unix Time till delivery.',
  `money` bigint(40) unsigned NOT NULL DEFAULT '0' COMMENT 'Amount of money sent.',
  `cod` bigint(40) unsigned NOT NULL DEFAULT '0' COMMENT 'Amount of money needed (COD).',
  `checked` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'checked_flag.',
  PRIMARY KEY (`id`),
  KEY `idx_receiver` (`receiver`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Mail System';
/*!40101 SET character_set_client = @saved_cs_client */;
