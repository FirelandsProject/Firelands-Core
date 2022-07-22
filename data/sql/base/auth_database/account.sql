/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `account`;
CREATE TABLE IF NOT EXISTS `account` (
  `id` int unsigned NOT NULL AUTO_INCREMENT COMMENT 'The unique account ID.',
  `username` varchar(32) NOT NULL DEFAULT '' COMMENT 'The account user name.',
  `sha_pass_hash` varchar(40) NOT NULL DEFAULT '' COMMENT 'This field contains the encrypted SHA1 password.',
  `gmlevel` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'The account security level.',
  `sessionkey` longtext COMMENT 'The Session Key.',
  `v` longtext COMMENT 'The validated Hash Value.',
  `s` longtext COMMENT 'Password ''Salt'' Value.',
  `email` text COMMENT 'The e-mail address associated with this account.',
  `joindate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'The date when the account was created.',
  `last_ip` varchar(30) NOT NULL DEFAULT '0.0.0.0' COMMENT 'The last IP used by the person who last logged into the account.',
  `failed_logins` int unsigned NOT NULL DEFAULT '0' COMMENT 'The number of failed logins attempted on the account.',
  `locked` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'Indicates whether the account has been locked or not.',
  `last_login` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'The date when the account was last logged into.',
  `active_realm_id` int unsigned NOT NULL DEFAULT '0' COMMENT 'Which maximum expansion content a user has access to.',
  `expansion` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'Which maximum expansion content a user has access to.',
  `mutetime` bigint unsigned NOT NULL DEFAULT '0' COMMENT 'The time, in Unix time, when the account will be unmuted.',
  `locale` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'The locale used by the client logged into this account.',
  `os` varchar(3) DEFAULT '' COMMENT 'The Operating System of the connected client',
  `playerBot` bit(1) NOT NULL DEFAULT b'0' COMMENT 'Determines whether the account is a User or a PlayerBot',
  PRIMARY KEY (`id`),
  UNIQUE KEY `idx_username` (`username`),
  KEY `idx_gmlevel` (`gmlevel`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Account System';

DELETE FROM `account`;
/*!40000 ALTER TABLE `account` DISABLE KEYS */;
/*!40000 ALTER TABLE `account` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
