/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `disables`;
CREATE TABLE IF NOT EXISTS `disables` (
  `sourceType` int unsigned NOT NULL COMMENT 'Type of disable, see enum DisableType',
  `entry` int unsigned NOT NULL COMMENT 'Main entry for disable, type-specific',
  `flags` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'Flag bitfield, type-specific',
  `data` int unsigned NOT NULL DEFAULT '0' COMMENT 'Additional data, type-specific',
  `comment` varchar(255) DEFAULT NULL COMMENT 'Description of disable cause',
  PRIMARY KEY (`sourceType`,`entry`,`data`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Mangos disables system';

DELETE FROM `disables`;
/*!40000 ALTER TABLE `disables` DISABLE KEYS */;
/*!40000 ALTER TABLE `disables` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
