/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `character_reputation`;
CREATE TABLE IF NOT EXISTS `character_reputation` (
  `guid` int unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the character. (See character.guid).',
  `faction` int unsigned NOT NULL DEFAULT '0' COMMENT 'The faction ID that the character has the given reputation in (See Faction.dbc).',
  `standing` int NOT NULL DEFAULT '0' COMMENT 'The current reputation value that the character has.',
  `flags` int NOT NULL DEFAULT '0' COMMENT 'This field is a bitmask containing flags that apply to the faction.',
  PRIMARY KEY (`guid`,`faction`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Player System';

DELETE FROM `character_reputation`;
/*!40000 ALTER TABLE `character_reputation` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_reputation` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
