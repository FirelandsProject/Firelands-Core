/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `worldsafelocs_dbc`;
CREATE TABLE IF NOT EXISTS `worldsafelocs_dbc` (
  `Id` int unsigned NOT NULL,
  `MapId` int unsigned NOT NULL DEFAULT '0',
  `x` float NOT NULL DEFAULT '0',
  `y` float NOT NULL DEFAULT '0',
  `z` float NOT NULL DEFAULT '0',
  `Comment` text NOT NULL,
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3;

DELETE FROM `worldsafelocs_dbc`;
/*!40000 ALTER TABLE `worldsafelocs_dbc` DISABLE KEYS */;
INSERT INTO `worldsafelocs_dbc` (`Id`, `MapId`, `x`, `y`, `z`, `Comment`) VALUES
	(1900, 648, 1768.83, 2011.97, 220.939, 'The Lost Isles - Sky Falls GY'),
	(1901, 648, 710, 1825, 104.699, 'The Lost Isles - Oomlot Village GY'),
	(1902, 648, 623.562, 1960.35, 13.7934, 'The Lost Isles - Ruins of Vashj\'elan GY'),
	(1903, 648, -8387, 1289, 92.7854, 'Kezan - KTC Headquarters GY'),
	(1904, 659, 1768, 2011, 221.028, 'The Lost Isles - Sky Falls GY'),
	(1905, 0, -1698.29, 1370.73, 23.0201, 'Ruins of Gilneas - Lord Hiram Creed Graveyard'),
	(1906, 0, -11405.9, -2156.49, 34.865, 'Deadwind Pass - Nalice Graveyard');
/*!40000 ALTER TABLE `worldsafelocs_dbc` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
