/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `lfg_dungeon_rewards`;
CREATE TABLE IF NOT EXISTS `lfg_dungeon_rewards` (
  `dungeonId` int unsigned NOT NULL DEFAULT '0' COMMENT 'Dungeon entry from dbc',
  `maxLevel` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'Max level at which this reward is rewarded',
  `firstQuestId` int unsigned NOT NULL DEFAULT '0' COMMENT 'Quest id with rewards for first dungeon this day',
  `otherQuestId` int unsigned NOT NULL DEFAULT '0' COMMENT 'Quest id with rewards for Nth dungeon this day',
  PRIMARY KEY (`dungeonId`,`maxLevel`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3;

DELETE FROM `lfg_dungeon_rewards`;
/*!40000 ALTER TABLE `lfg_dungeon_rewards` DISABLE KEYS */;
INSERT INTO `lfg_dungeon_rewards` (`dungeonId`, `maxLevel`, `firstQuestId`, `otherQuestId`) VALUES
	(258, 15, 24881, 24889),
	(258, 25, 24882, 24890),
	(258, 34, 24883, 24891),
	(258, 45, 24884, 24892),
	(258, 55, 24885, 24893),
	(258, 60, 24886, 24894),
	(259, 64, 24887, 24895),
	(259, 70, 24888, 24896),
	(260, 70, 24922, 24923),
	(261, 80, 24790, 24791),
	(262, 80, 24788, 24789),
	(285, 85, 25482, 0),
	(286, 85, 25484, 0),
	(287, 85, 25483, 0),
	(288, 85, 25485, 0),
	(301, 85, 28905, 28906),
	(300, 85, 28907, 28908),
	(434, 85, 29185, 29183),
	(416, 85, 30177, 29183),
	(417, 85, 30177, 29183);
/*!40000 ALTER TABLE `lfg_dungeon_rewards` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
