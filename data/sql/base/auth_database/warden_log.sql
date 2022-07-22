/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `warden_log`;
CREATE TABLE IF NOT EXISTS `warden_log` (
  `entry` int unsigned NOT NULL AUTO_INCREMENT COMMENT 'Log entry ID',
  `check` smallint unsigned NOT NULL COMMENT 'Failed Warden check ID',
  `action` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'Action taken (enum WardenActions)',
  `account` int unsigned NOT NULL COMMENT 'The account ID of the player.',
  `guid` int unsigned NOT NULL DEFAULT '0' COMMENT 'Player GUID',
  `map` int unsigned DEFAULT NULL COMMENT 'The map id. (See map.dbc)',
  `position_x` float DEFAULT NULL COMMENT 'The x location of the player.',
  `position_y` float DEFAULT NULL COMMENT 'The y location of the player.',
  `position_z` float DEFAULT NULL COMMENT 'The z location of the player.',
  `date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'The date/time when the log entry was raised, in Unix time.',
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Warden log of failed checks';

DELETE FROM `warden_log`;
/*!40000 ALTER TABLE `warden_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `warden_log` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
