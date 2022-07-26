/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `phase_area`;
CREATE TABLE IF NOT EXISTS `phase_area` (
  `areaId` mediumint unsigned NOT NULL DEFAULT '0',
  `entry` mediumint unsigned NOT NULL DEFAULT '0',
  `quest_start` mediumint unsigned NOT NULL DEFAULT '0',
  `quest_end` mediumint unsigned NOT NULL DEFAULT '0',
  `quest_start_status` int NOT NULL DEFAULT '64',
  `quest_end_status` int NOT NULL DEFAULT '11',
  `flags` int NOT NULL DEFAULT '0',
  `comment` text,
  PRIMARY KEY (`areaId`,`entry`,`quest_start`,`quest_end`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3;

DELETE FROM `phase_area`;
/*!40000 ALTER TABLE `phase_area` DISABLE KEYS */;
/*!40000 ALTER TABLE `phase_area` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
