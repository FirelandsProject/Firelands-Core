/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `character_action`;
CREATE TABLE IF NOT EXISTS `character_action` (
  `guid` int unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the character (See character.id).',
  `spec` tinyint unsigned NOT NULL DEFAULT '0',
  `button` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'The ID of the button on the action bar where the action icon will be placed.',
  `action` int unsigned NOT NULL DEFAULT '0' COMMENT 'Depending on the type value, this could be various values.',
  `type` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'The type of action.',
  PRIMARY KEY (`guid`,`spec`,`button`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Player System';

DELETE FROM `character_action`;
/*!40000 ALTER TABLE `character_action` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_action` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
