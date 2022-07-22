/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `creature_respawn`;
CREATE TABLE IF NOT EXISTS `creature_respawn` (
  `guid` int unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `respawntime` bigint unsigned NOT NULL DEFAULT '0' COMMENT 'Time till the creature should respawn.',
  `instance` mediumint unsigned NOT NULL DEFAULT '0' COMMENT 'Instance ID of the instance the creature is respawning into.',
  PRIMARY KEY (`guid`,`instance`),
  KEY `instance` (`instance`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Grid Loading System';

DELETE FROM `creature_respawn`;
/*!40000 ALTER TABLE `creature_respawn` DISABLE KEYS */;
/*!40000 ALTER TABLE `creature_respawn` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
