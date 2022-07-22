/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `reputation_reward_rate`;
CREATE TABLE IF NOT EXISTS `reputation_reward_rate` (
  `faction` mediumint unsigned NOT NULL DEFAULT '0' COMMENT 'Faction (See Faction.dbc).',
  `quest_rate` float NOT NULL DEFAULT '1' COMMENT 'Rate for quest reputation.',
  `creature_rate` float NOT NULL DEFAULT '1' COMMENT 'Rate for creature kill reputation.',
  `spell_rate` float NOT NULL DEFAULT '1' COMMENT 'Rate for reputation spells.',
  PRIMARY KEY (`faction`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3;

DELETE FROM `reputation_reward_rate`;
/*!40000 ALTER TABLE `reputation_reward_rate` DISABLE KEYS */;
INSERT INTO `reputation_reward_rate` (`faction`, `quest_rate`, `creature_rate`, `spell_rate`) VALUES
	(529, 2, 1, 2),
	(609, 2, 1, 2),
	(576, 4, 1, 4),
	(1073, 2, 2, 2),
	(1119, 1.3, 1.3, 1.3),
	(1106, 1.3, 1.3, 1.3),
	(1090, 1.3, 1.3, 1.3),
	(1098, 1.3, 1.3, 1.3),
	(1091, 1.3, 1.3, 1.3),
	(970, 3, 1, 3),
	(978, 2, 1, 2),
	(941, 2, 1, 2),
	(1105, 2, 2, 2),
	(1104, 2, 2, 2);
/*!40000 ALTER TABLE `reputation_reward_rate` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
