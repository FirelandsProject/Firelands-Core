/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `arena_log`;
CREATE TABLE IF NOT EXISTS `arena_log` (
  `endTime` int DEFAULT NULL,
  `duration` int DEFAULT NULL,
  `winnerTeam` mediumint DEFAULT NULL,
  `looserTeam` mediumint DEFAULT NULL,
  `winnerRating` mediumint DEFAULT NULL,
  `winnerMMR` mediumint DEFAULT NULL,
  `looserRating` mediumint DEFAULT NULL,
  `looserMMR` mediumint DEFAULT NULL,
  `winpl1` text,
  `winpl2` text,
  `winpl3` text,
  `winpl4` text,
  `winpl5` text,
  `loosepl1` text,
  `loosepl2` text,
  `loosepl3` text,
  `loosepl4` text,
  `loosepl5` text,
  `windmg` int DEFAULT NULL,
  `winheal` int DEFAULT NULL,
  `winkill` tinyint DEFAULT NULL,
  `loosedmg` int DEFAULT NULL,
  `looseheal` int DEFAULT NULL,
  `loosekill` tinyint DEFAULT NULL,
  `winIP1` text,
  `winIP2` text,
  `winIP3` text,
  `winIP4` text,
  `winIP5` text,
  `looseIP1` text,
  `looseIP2` text,
  `looseIP3` text,
  `looseIP4` text,
  `looseIP5` text,
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

DELETE FROM `arena_log`;
/*!40000 ALTER TABLE `arena_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `arena_log` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
