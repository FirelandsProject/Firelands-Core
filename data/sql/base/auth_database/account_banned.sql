/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `account_banned`;
CREATE TABLE IF NOT EXISTS `account_banned` (
  `id` int unsigned NOT NULL COMMENT 'The account ID (See account.id).',
  `bandate` bigint NOT NULL DEFAULT '0' COMMENT 'The date when the account was banned, in Unix time.',
  `unbandate` bigint NOT NULL DEFAULT '0' COMMENT 'The date when the account will be automatically unbanned.',
  `bannedby` varchar(50) NOT NULL COMMENT 'The character that banned the account.',
  `banreason` varchar(255) NOT NULL COMMENT 'The reason for the ban.',
  `active` tinyint NOT NULL DEFAULT '1' COMMENT 'Is the ban is currently active or not.',
  PRIMARY KEY (`id`,`bandate`),
  CONSTRAINT `account_banned_ibfk_1` FOREIGN KEY (`id`) REFERENCES `account` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Ban List';

DELETE FROM `account_banned`;
/*!40000 ALTER TABLE `account_banned` DISABLE KEYS */;
/*!40000 ALTER TABLE `account_banned` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
