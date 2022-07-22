/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `ai_playerbot_random_bots`;
CREATE TABLE IF NOT EXISTS `ai_playerbot_random_bots` (
  `id` bigint NOT NULL AUTO_INCREMENT,
  `owner` bigint NOT NULL,
  `bot` bigint NOT NULL,
  `time` bigint NOT NULL,
  `validIn` bigint DEFAULT NULL,
  `event` varchar(45) DEFAULT NULL,
  `value` bigint DEFAULT NULL,
  `data` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `owner` (`owner`),
  KEY `bot` (`bot`),
  KEY `event` (`event`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;

DELETE FROM `ai_playerbot_random_bots`;
/*!40000 ALTER TABLE `ai_playerbot_random_bots` DISABLE KEYS */;
/*!40000 ALTER TABLE `ai_playerbot_random_bots` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
