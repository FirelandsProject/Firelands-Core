/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `instance`;
CREATE TABLE IF NOT EXISTS `instance` (
  `id` int unsigned NOT NULL DEFAULT '0' COMMENT 'The instance ID. This number is unique to every instance.',
  `map` int unsigned NOT NULL DEFAULT '0' COMMENT 'The map ID the instance is in. (See Map.dbc)',
  `resettime` bigint unsigned NOT NULL DEFAULT '0' COMMENT 'The time when the instance will be reset, in Unix time.',
  `difficulty` tinyint unsigned NOT NULL DEFAULT '0',
  `encountersMask` int unsigned NOT NULL DEFAULT '0',
  `data` longtext COMMENT 'Specific data belonging to the individual instance.',
  PRIMARY KEY (`id`),
  KEY `map` (`map`),
  KEY `resettime` (`resettime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;

DELETE FROM `instance`;
/*!40000 ALTER TABLE `instance` DISABLE KEYS */;
/*!40000 ALTER TABLE `instance` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
