/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `pvpstats_players`;
CREATE TABLE IF NOT EXISTS `pvpstats_players` (
  `battleground_id` int DEFAULT NULL,
  `character_guid` int DEFAULT NULL,
  `winner` int DEFAULT NULL,
  `score_killing_blows` int DEFAULT NULL,
  `score_deaths` int DEFAULT NULL,
  `score_honorable_kills` int DEFAULT NULL,
  `score_bonus_honor` int DEFAULT NULL,
  `score_damage_done` int DEFAULT NULL,
  `score_healing_done` int DEFAULT NULL,
  `attr_1` int DEFAULT NULL,
  `attr_2` int DEFAULT NULL,
  `attr_3` int DEFAULT NULL,
  `attr_4` int DEFAULT NULL,
  `attr_5` int DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

DELETE FROM `pvpstats_players`;
/*!40000 ALTER TABLE `pvpstats_players` DISABLE KEYS */;
/*!40000 ALTER TABLE `pvpstats_players` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
