/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `arena_team_stats`;
CREATE TABLE IF NOT EXISTS `arena_team_stats` (
  `arenateamid` int unsigned NOT NULL DEFAULT '0',
  `rating` int unsigned NOT NULL DEFAULT '0',
  `games_week` int unsigned NOT NULL DEFAULT '0',
  `wins_week` int unsigned NOT NULL DEFAULT '0',
  `games_season` int unsigned NOT NULL DEFAULT '0',
  `wins_season` int unsigned NOT NULL DEFAULT '0',
  `rank` int unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`arenateamid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;

DELETE FROM `arena_team_stats`;
/*!40000 ALTER TABLE `arena_team_stats` DISABLE KEYS */;
/*!40000 ALTER TABLE `arena_team_stats` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
