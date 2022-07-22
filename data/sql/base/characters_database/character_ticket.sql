/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `character_ticket`;
CREATE TABLE IF NOT EXISTS `character_ticket` (
  `ticket_id` int unsigned NOT NULL AUTO_INCREMENT COMMENT 'A unique ticket ID.',
  `guid` int unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the character sending the ticket. (See character.guid).',
  `ticket_text` text COMMENT 'The ticket description text; the text written by the player.',
  `response_text` text COMMENT 'The ticket solution text; the text written by the GM.',
  `ticket_lastchange` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Stores the time when this ticket was last changed.',
  PRIMARY KEY (`ticket_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Player System';

DELETE FROM `character_ticket`;
/*!40000 ALTER TABLE `character_ticket` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_ticket` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
