/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `battleground_template`;
CREATE TABLE IF NOT EXISTS `battleground_template` (
  `id` mediumint unsigned NOT NULL COMMENT 'The battleground ID (See BattlemasterList.dbc).',
  `MinPlayersPerTeam` smallint unsigned NOT NULL DEFAULT '0' COMMENT 'The minimum number of players that need to join the battleground.',
  `MaxPlayersPerTeam` smallint unsigned NOT NULL DEFAULT '0' COMMENT 'Controls how many players from each team can join the battleground.',
  `AllianceStartLoc` mediumint unsigned NOT NULL COMMENT 'The location where the alliance players get teleported to in the battleground.',
  `AllianceStartO` float NOT NULL COMMENT 'The orientation of the alliance players upon teleport.',
  `HordeStartLoc` mediumint unsigned NOT NULL COMMENT 'The location where the horde players get teleported to in the battleground.',
  `HordeStartO` float NOT NULL COMMENT 'The orientation of the horde players upon teleport into the battleground.',
  `Comment` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC;

DELETE FROM `battleground_template`;
/*!40000 ALTER TABLE `battleground_template` DISABLE KEYS */;
INSERT INTO `battleground_template` (`id`, `MinPlayersPerTeam`, `MaxPlayersPerTeam`, `AllianceStartLoc`, `AllianceStartO`, `HordeStartLoc`, `HordeStartO`, `Comment`) VALUES
	(1, 10, 40, 611, 3.71492, 610, 1.16711, 'Alterac Valley'),
	(2, 10, 10, 769, 3.14159, 770, 0.00391, 'Warsong Gulch'),
	(3, 15, 15, 890, 3.91992, 889, 0.88828, 'Arathi Basin'),
	(4, 5, 5, 929, 0, 936, 3.14159, 'Nagrand Arena'),
	(5, 5, 5, 939, 0, 940, 3.14159, 'Blade\'s Edge Arena'),
	(6, 5, 5, 0, 0, 0, 0, 'All Arenas'),
	(7, 15, 15, 1103, 3.22092, 1104, 0.01649, 'Eye of The Storm'),
	(8, 5, 5, 1258, 0, 1259, 3.14159, 'Ruins of Lordaeron'),
	(9, 8, 15, 1367, 0, 1368, 0, 'Strand of the Ancients'),
	(10, 5, 5, 1362, 0, 1363, 0, 'Dalaran Sewers'),
	(11, 5, 5, 1966, 0, 1967, 0, 'The Ring of Valor'),
	(30, 10, 40, 1485, 0, 1486, 0, 'Isle of Conquest'),
	(32, 5, 40, 0, 0, 0, 0, 'Random Battleground'),
	(108, 5, 10, 1726, 2.55647, 1727, 6.18108, 'Twin Peaks'),
	(120, 5, 10, 1798, 5.95725, 1799, 1.55116, 'The Battle for Gilneas');
/*!40000 ALTER TABLE `battleground_template` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
