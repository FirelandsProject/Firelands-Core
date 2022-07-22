/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `warden`;
CREATE TABLE IF NOT EXISTS `warden` (
  `id` smallint unsigned NOT NULL COMMENT 'Check ID from warden_checks',
  `build` smallint unsigned NOT NULL COMMENT 'Client build',
  `type` tinyint unsigned DEFAULT NULL COMMENT 'Check Type',
  `data` varchar(48) NOT NULL DEFAULT '',
  `str` varchar(20) NOT NULL DEFAULT '',
  `address` int unsigned NOT NULL DEFAULT '0',
  `length` tinyint unsigned NOT NULL DEFAULT '0',
  `result` varchar(24) NOT NULL DEFAULT '',
  `comment` varchar(50) DEFAULT '' COMMENT 'Description of what the check is',
  PRIMARY KEY (`id`,`build`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC;

DELETE FROM `warden`;
/*!40000 ALTER TABLE `warden` DISABLE KEYS */;
/*!40000 ALTER TABLE `warden` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
