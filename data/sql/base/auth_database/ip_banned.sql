/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `ip_banned`;
CREATE TABLE IF NOT EXISTS `ip_banned` (
  `ip` varchar(32) NOT NULL DEFAULT '0.0.0.0' COMMENT 'The IP address that is banned.',
  `bandate` bigint NOT NULL COMMENT 'The date when the IP was first banned, in Unix time.',
  `unbandate` bigint NOT NULL COMMENT 'The date when the IP will be unbanned in Unix time.',
  `bannedby` varchar(50) NOT NULL DEFAULT '[Console]' COMMENT 'The name of the character that banned the IP.',
  `banreason` varchar(255) NOT NULL DEFAULT 'no reason' COMMENT 'The reason given for the IP ban.',
  PRIMARY KEY (`ip`,`bandate`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Banned IPs';

DELETE FROM `ip_banned`;
/*!40000 ALTER TABLE `ip_banned` DISABLE KEYS */;
/*!40000 ALTER TABLE `ip_banned` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
