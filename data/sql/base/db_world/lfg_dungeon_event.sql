/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `lfg_dungeon_event`;
CREATE TABLE IF NOT EXISTS `lfg_dungeon_event` (
  `dungeonId` int unsigned NOT NULL DEFAULT '0' COMMENT 'Dungeon entry from dbc',
  `eventEntry` int unsigned NOT NULL DEFAULT '0' COMMENT 'Event Associated to Dungeon Finder',
  PRIMARY KEY (`dungeonId`,`eventEntry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3;

DELETE FROM `lfg_dungeon_event`;
/*!40000 ALTER TABLE `lfg_dungeon_event` DISABLE KEYS */;
/*!40000 ALTER TABLE `lfg_dungeon_event` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
