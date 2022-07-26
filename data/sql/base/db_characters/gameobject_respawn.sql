/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `gameobject_respawn`;
CREATE TABLE IF NOT EXISTS `gameobject_respawn` (
  `guid` int unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `respawnTime` int unsigned NOT NULL DEFAULT '0',
  `instanceId` int unsigned NOT NULL DEFAULT '0' COMMENT 'Instance Identifier',
  `mapId` smallint unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`instanceId`),
  KEY `idx_instance` (`instanceId`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 COMMENT='Grid Loading System';

DELETE FROM `gameobject_respawn`;
/*!40000 ALTER TABLE `gameobject_respawn` DISABLE KEYS */;
/*!40000 ALTER TABLE `gameobject_respawn` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
