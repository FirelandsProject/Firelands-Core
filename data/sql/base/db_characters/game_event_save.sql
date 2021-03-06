/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `game_event_save`;
CREATE TABLE IF NOT EXISTS `game_event_save` (
  `eventEntry` tinyint unsigned NOT NULL,
  `state` tinyint unsigned NOT NULL DEFAULT '1',
  `next_start` int unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`eventEntry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;

DELETE FROM `game_event_save`;
/*!40000 ALTER TABLE `game_event_save` DISABLE KEYS */;
INSERT INTO `game_event_save` (`eventEntry`, `state`, `next_start`) VALUES
	(33, 4, 0),
	(34, 3, 1724210478),
	(35, 4, 0),
	(36, 3, 1724210478),
	(37, 4, 0),
	(38, 3, 1724210478),
	(39, 4, 0),
	(40, 3, 1724210478),
	(41, 4, 0),
	(42, 3, 1724210478),
	(43, 4, 0),
	(44, 3, 1724210478),
	(45, 4, 0),
	(46, 3, 1724210478),
	(47, 3, 1724210478);
/*!40000 ALTER TABLE `game_event_save` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
