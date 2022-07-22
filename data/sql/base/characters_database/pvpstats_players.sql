/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `pvpstats_players`;
CREATE TABLE IF NOT EXISTS `pvpstats_players` (
  `battleground_id` bigint unsigned NOT NULL,
  `character_guid` int unsigned NOT NULL,
  `score_killing_blows` mediumint unsigned NOT NULL,
  `score_deaths` mediumint unsigned NOT NULL,
  `score_honorable_kills` mediumint unsigned NOT NULL,
  `score_bonus_honor` mediumint unsigned NOT NULL,
  `score_damage_done` mediumint unsigned NOT NULL,
  `score_healing_done` mediumint unsigned NOT NULL,
  `attr_1` mediumint unsigned NOT NULL DEFAULT '0',
  `attr_2` mediumint unsigned NOT NULL DEFAULT '0',
  `attr_3` mediumint unsigned NOT NULL DEFAULT '0',
  `attr_4` mediumint unsigned NOT NULL DEFAULT '0',
  `attr_5` mediumint unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`battleground_id`,`character_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;

DELETE FROM `pvpstats_players`;
/*!40000 ALTER TABLE `pvpstats_players` DISABLE KEYS */;
/*!40000 ALTER TABLE `pvpstats_players` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
