/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `saved_variables`;
CREATE TABLE IF NOT EXISTS `saved_variables` (
  `NextArenaPointDistributionTime` bigint unsigned NOT NULL DEFAULT '0',
  `NextDailyQuestResetTime` bigint unsigned NOT NULL DEFAULT '0',
  `NextWeeklyQuestResetTime` bigint unsigned NOT NULL DEFAULT '0',
  `NextMonthlyQuestResetTime` bigint unsigned NOT NULL DEFAULT '0',
  `NextRandomBGResetTime` bigint unsigned NOT NULL DEFAULT '0',
  `cleaning_flags` int unsigned NOT NULL DEFAULT '0' COMMENT 'The flags controlling character cleanup.'
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Variable Saves';

DELETE FROM `saved_variables`;
/*!40000 ALTER TABLE `saved_variables` DISABLE KEYS */;
/*!40000 ALTER TABLE `saved_variables` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
