/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `mail`;
CREATE TABLE IF NOT EXISTS `mail` (
  `id` int unsigned NOT NULL DEFAULT '0' COMMENT 'Unique message id. Every new message gets a new auto incremented id.',
  `messageType` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'message_type.',
  `stationery` tinyint NOT NULL DEFAULT '41' COMMENT 'The StationeryID (See Stationery.dbc).',
  `mailTemplateId` mediumint unsigned NOT NULL DEFAULT '0' COMMENT 'The Mail Template Id (See MailTemplate.dbc)',
  `sender` int unsigned NOT NULL DEFAULT '0' COMMENT 'Guid of the character who sends the mail.',
  `receiver` int unsigned NOT NULL DEFAULT '0' COMMENT 'Guid of the character who receives the mail.',
  `subject` longtext COMMENT 'The Subject of the mail.',
  `body` longtext,
  `has_items` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'Boolean value showing whether or not atems have been sent.',
  `expire_time` bigint unsigned NOT NULL DEFAULT '0' COMMENT 'Current Unix Time + Unix Time till expiry.',
  `deliver_time` bigint unsigned NOT NULL DEFAULT '0' COMMENT 'Current Unix Time + Unix Time till delivery.',
  `money` bigint unsigned NOT NULL DEFAULT '0' COMMENT 'Amount of money sent.',
  `cod` bigint unsigned NOT NULL DEFAULT '0' COMMENT 'Amount of money needed (COD).',
  `checked` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'checked_flag.',
  PRIMARY KEY (`id`),
  KEY `idx_receiver` (`receiver`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Mail System';

DELETE FROM `mail`;
/*!40000 ALTER TABLE `mail` DISABLE KEYS */;
/*!40000 ALTER TABLE `mail` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
