/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `character_queststatus`;
CREATE TABLE IF NOT EXISTS `character_queststatus` (
  `guid` int unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the character. (See character.guid).',
  `quest` int unsigned NOT NULL DEFAULT '0' COMMENT 'The quest ID. (See quest_template.entry).',
  `status` int unsigned NOT NULL DEFAULT '0' COMMENT 'The current quest status.',
  `rewarded` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'Boolean 1 or 0 representing whether the quest has been rewarded or not.',
  `explored` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'Boolean 1 or 0 representing if the character has explored what was needed.',
  `timer` bigint unsigned NOT NULL DEFAULT '0' COMMENT 'Remaining time left on the quest if the quest has a timer (See quest_template).',
  `mobcount1` int unsigned NOT NULL DEFAULT '0' COMMENT 'Current count of the first number of kills or casts.',
  `mobcount2` int unsigned NOT NULL DEFAULT '0' COMMENT 'Current count of the second number of kills or casts.',
  `mobcount3` int unsigned NOT NULL DEFAULT '0' COMMENT 'Current count of the third number of kills or casts.',
  `mobcount4` int unsigned NOT NULL DEFAULT '0' COMMENT 'Current count of the forth number of kills or casts.',
  `itemcount1` int unsigned NOT NULL DEFAULT '0' COMMENT 'Current item count for the first item in a delivery quest, if any.',
  `itemcount2` int unsigned NOT NULL DEFAULT '0' COMMENT 'Current item count for the second item in a delivery quest, if any.',
  `itemcount3` int unsigned NOT NULL DEFAULT '0' COMMENT 'Current item count for the third item in a delivery quest, if any.',
  `itemcount4` int unsigned NOT NULL DEFAULT '0' COMMENT 'Current item count for the forth item in a delivery quest, if any.',
  `itemcount5` int unsigned NOT NULL DEFAULT '0',
  `itemcount6` int unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`quest`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Player System';

DELETE FROM `character_queststatus`;
/*!40000 ALTER TABLE `character_queststatus` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_queststatus` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
