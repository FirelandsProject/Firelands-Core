/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `mail_items`;
CREATE TABLE IF NOT EXISTS `mail_items` (
  `mail_id` int NOT NULL DEFAULT '0' COMMENT 'The Mail Id (See mail.id).',
  `item_guid` int NOT NULL DEFAULT '0' COMMENT 'The GUID of the item.',
  `item_template` int NOT NULL DEFAULT '0' COMMENT 'The entry ID of the item in the item_template table.',
  `receiver` int unsigned NOT NULL DEFAULT '0' COMMENT 'Guid of the character who receives the mail.',
  PRIMARY KEY (`mail_id`,`item_guid`),
  KEY `idx_receiver` (`receiver`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC;

DELETE FROM `mail_items`;
/*!40000 ALTER TABLE `mail_items` DISABLE KEYS */;
/*!40000 ALTER TABLE `mail_items` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
