/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `group_instance`;
CREATE TABLE IF NOT EXISTS `group_instance` (
  `leaderGuid` int unsigned NOT NULL DEFAULT '0' COMMENT 'The Guid of the group leader. (See characters.guid)',
  `instance` int unsigned NOT NULL DEFAULT '0' COMMENT 'ID of the Instance session the group has enterd.',
  `permanent` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'Boolean flag if the group is bound to the Instance or not',
  PRIMARY KEY (`leaderGuid`,`instance`),
  KEY `instance` (`instance`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;

DELETE FROM `group_instance`;
/*!40000 ALTER TABLE `group_instance` DISABLE KEYS */;
/*!40000 ALTER TABLE `group_instance` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
