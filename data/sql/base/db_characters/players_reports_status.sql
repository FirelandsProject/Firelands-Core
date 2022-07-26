/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `players_reports_status`;
CREATE TABLE IF NOT EXISTS `players_reports_status` (
  `guid` int unsigned NOT NULL DEFAULT '0',
  `creation_time` int unsigned NOT NULL DEFAULT '0',
  `average` float NOT NULL DEFAULT '0',
  `total_reports` bigint unsigned NOT NULL DEFAULT '0',
  `speed_reports` bigint unsigned NOT NULL DEFAULT '0',
  `fly_reports` bigint unsigned NOT NULL DEFAULT '0',
  `jump_reports` bigint unsigned NOT NULL DEFAULT '0',
  `waterwalk_reports` bigint unsigned NOT NULL DEFAULT '0',
  `teleportplane_reports` bigint unsigned NOT NULL DEFAULT '0',
  `climb_reports` bigint unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;

DELETE FROM `players_reports_status`;
/*!40000 ALTER TABLE `players_reports_status` DISABLE KEYS */;
/*!40000 ALTER TABLE `players_reports_status` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
