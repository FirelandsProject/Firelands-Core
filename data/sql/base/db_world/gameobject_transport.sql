/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `gameobject_transport`;
CREATE TABLE IF NOT EXISTS `gameobject_transport` (
  `guid` int unsigned NOT NULL DEFAULT '0',
  `transport_entry` int NOT NULL DEFAULT '0',
  `go_entry` int unsigned NOT NULL DEFAULT '0',
  `TransOffsetX` float NOT NULL DEFAULT '0',
  `TransOffsetY` float NOT NULL DEFAULT '0',
  `TransOffsetZ` float NOT NULL DEFAULT '0',
  `TransOffsetO` float NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`transport_entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;

DELETE FROM `gameobject_transport`;
/*!40000 ALTER TABLE `gameobject_transport` DISABLE KEYS */;
/*!40000 ALTER TABLE `gameobject_transport` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
