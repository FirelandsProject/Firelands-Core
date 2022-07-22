--
-- Table structure for table `character_ticket`
--

DROP TABLE IF EXISTS `character_ticket`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_ticket` (
  `ticket_id` int(11) unsigned NOT NULL AUTO_INCREMENT COMMENT 'A unique ticket ID.',
  `guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the character sending the ticket. (See character.guid).',
  `ticket_text` text COMMENT 'The ticket description text; the text written by the player.',
  `response_text` text COMMENT 'The ticket solution text; the text written by the GM.',
  `ticket_lastchange` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Stores the time when this ticket was last changed.',
  PRIMARY KEY (`ticket_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Player System';
/*!40101 SET character_set_client = @saved_cs_client */;