/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `character_skills`;
CREATE TABLE IF NOT EXISTS `character_skills` (
  `guid` int unsigned NOT NULL COMMENT 'The GUID (Global Unique Identifier) of the character. (See character.guid).',
  `skill` mediumint unsigned NOT NULL COMMENT 'The list of skills a character knows. A listing of those can be found in here.',
  `value` mediumint unsigned NOT NULL COMMENT 'The current skill rank(value) of the character skills a character has.',
  `max` mediumint unsigned NOT NULL COMMENT 'The highest possible value for the given skill within a given rank.',
  PRIMARY KEY (`guid`,`skill`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Player System';

DELETE FROM `character_skills`;
/*!40000 ALTER TABLE `character_skills` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_skills` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
