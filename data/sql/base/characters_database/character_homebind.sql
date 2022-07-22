/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `character_homebind`;
CREATE TABLE IF NOT EXISTS `character_homebind` (
  `guid` int unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID (Global Unique Identifier) of the character. (See character.guid).',
  `map` int unsigned NOT NULL DEFAULT '0' COMMENT 'The Map Identifier where the character gets teleported to. (See Maps.dbc).',
  `zone` int unsigned NOT NULL DEFAULT '0' COMMENT 'The Zone Identifier where the character gets teleported to.',
  `position_x` float NOT NULL DEFAULT '0' COMMENT 'The x position where the character gets teleported to.',
  `position_y` float NOT NULL DEFAULT '0' COMMENT 'The y position where the character gets teleported to.',
  `position_z` float NOT NULL DEFAULT '0' COMMENT 'The z position where the character gets teleported to.',
  PRIMARY KEY (`guid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Player System';

DELETE FROM `character_homebind`;
/*!40000 ALTER TABLE `character_homebind` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_homebind` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
