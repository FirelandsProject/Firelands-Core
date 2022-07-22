/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `uptime`;
CREATE TABLE IF NOT EXISTS `uptime` (
  `realmid` int unsigned NOT NULL COMMENT 'The realm id (See realmlist.id).',
  `starttime` bigint unsigned NOT NULL DEFAULT '0' COMMENT 'The time when the server was started, in Unix time.',
  `startstring` varchar(64) NOT NULL DEFAULT '' COMMENT 'The time when the server started, formated as a readable string.',
  `uptime` bigint unsigned NOT NULL DEFAULT '0' COMMENT 'The uptime of the server, in seconds.',
  `maxplayers` smallint unsigned NOT NULL DEFAULT '0' COMMENT 'The maximum number of players connected',
  PRIMARY KEY (`realmid`,`starttime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Uptime system';

DELETE FROM `uptime`;
/*!40000 ALTER TABLE `uptime` DISABLE KEYS */;
/*!40000 ALTER TABLE `uptime` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
