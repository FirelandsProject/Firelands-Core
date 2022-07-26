/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `corpse`;
CREATE TABLE IF NOT EXISTS `corpse` (
  `corpseGuid` int unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `guid` int unsigned NOT NULL DEFAULT '0' COMMENT 'Character Global Unique Identifier',
  `posX` float NOT NULL DEFAULT '0',
  `posY` float NOT NULL DEFAULT '0',
  `posZ` float NOT NULL DEFAULT '0',
  `orientation` float NOT NULL DEFAULT '0',
  `mapId` smallint unsigned NOT NULL DEFAULT '0' COMMENT 'Map Identifier',
  `phaseMask` smallint unsigned NOT NULL DEFAULT '1',
  `displayId` int unsigned NOT NULL DEFAULT '0',
  `itemCache` text NOT NULL,
  `bytes1` int unsigned NOT NULL DEFAULT '0',
  `bytes2` int unsigned NOT NULL DEFAULT '0',
  `flags` tinyint unsigned NOT NULL DEFAULT '0',
  `dynFlags` tinyint unsigned NOT NULL DEFAULT '0',
  `time` int unsigned NOT NULL DEFAULT '0',
  `corpseType` tinyint unsigned NOT NULL DEFAULT '0',
  `instanceId` int unsigned NOT NULL DEFAULT '0' COMMENT 'Instance Identifier',
  PRIMARY KEY (`corpseGuid`),
  KEY `idx_type` (`corpseType`) USING BTREE,
  KEY `idx_time` (`time`) USING BTREE,
  KEY `idx_player` (`guid`) USING BTREE,
  KEY `idx_instance` (`instanceId`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 COMMENT='Death System';

DELETE FROM `corpse`;
/*!40000 ALTER TABLE `corpse` DISABLE KEYS */;
/*!40000 ALTER TABLE `corpse` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
