/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `character_battleground_data`;
CREATE TABLE IF NOT EXISTS `character_battleground_data` (
  `guid` int unsigned NOT NULL DEFAULT '0' COMMENT 'The character battleground datar global unique identifier.',
  `instance_id` int unsigned NOT NULL DEFAULT '0' COMMENT 'The unique ID of the battleground instance the character has entered.',
  `team` int unsigned NOT NULL DEFAULT '0' COMMENT 'Neutral, Horde, or Alliance',
  `join_x` float NOT NULL DEFAULT '0' COMMENT 'The character''s X coordinate in the battleground.',
  `join_y` float NOT NULL DEFAULT '0' COMMENT 'The character''s Y coordinate in the battleground.',
  `join_z` float NOT NULL DEFAULT '0' COMMENT 'The character''s Z coordinate in the battleground.',
  `join_o` float NOT NULL DEFAULT '0' COMMENT 'The character''s orientation in the battleground.',
  `join_map` int NOT NULL DEFAULT '0' COMMENT 'Battleground map ID where the character will spawn.',
  `taxi_start` int NOT NULL DEFAULT '0',
  `taxi_end` int NOT NULL DEFAULT '0',
  `mount_spell` int NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Player System';

DELETE FROM `character_battleground_data`;
/*!40000 ALTER TABLE `character_battleground_data` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_battleground_data` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
