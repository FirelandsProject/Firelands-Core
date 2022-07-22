/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `corpse`;
CREATE TABLE IF NOT EXISTS `corpse` (
  `guid` int unsigned NOT NULL DEFAULT '0' COMMENT 'The character global unique identifier of the corpse.',
  `player` int unsigned NOT NULL DEFAULT '0' COMMENT 'The character global unique identifier. (See characters.guid).',
  `position_x` float NOT NULL DEFAULT '0' COMMENT 'The x position of the character''s corpse location.',
  `position_y` float NOT NULL DEFAULT '0' COMMENT 'The y position of the character''s corpse location.',
  `position_z` float NOT NULL DEFAULT '0' COMMENT 'The z position of the character''s corpse location.',
  `orientation` float NOT NULL DEFAULT '0' COMMENT 'The orientation of the corpse. (North = 0.0, South = 3.14159)',
  `map` int unsigned NOT NULL DEFAULT '0' COMMENT 'The map ID the corpse is in. (See maps.dbc)',
  `phaseMask` int unsigned NOT NULL DEFAULT '1',
  `time` bigint unsigned NOT NULL DEFAULT '0' COMMENT 'Time of death.',
  `corpse_type` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'The Display Type of the corpse.',
  `instance` int unsigned NOT NULL DEFAULT '0' COMMENT 'The instance ID that the corpse is in.',
  PRIMARY KEY (`guid`),
  KEY `idx_type` (`corpse_type`),
  KEY `instance` (`instance`),
  KEY `Idx_player` (`player`),
  KEY `Idx_time` (`time`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Death System';

DELETE FROM `corpse`;
/*!40000 ALTER TABLE `corpse` DISABLE KEYS */;
/*!40000 ALTER TABLE `corpse` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
