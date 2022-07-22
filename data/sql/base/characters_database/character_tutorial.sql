/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `character_tutorial`;
CREATE TABLE IF NOT EXISTS `character_tutorial` (
  `account` bigint unsigned NOT NULL AUTO_INCREMENT COMMENT 'The account id of the user. (See account.id).',
  `tut0` int unsigned NOT NULL DEFAULT '0' COMMENT 'These values are 32bits flags.',
  `tut1` int unsigned NOT NULL DEFAULT '0' COMMENT 'These values are 32bits flags.',
  `tut2` int unsigned NOT NULL DEFAULT '0' COMMENT 'These values are 32bits flags.',
  `tut3` int unsigned NOT NULL DEFAULT '0' COMMENT 'These values are 32bits flags.',
  `tut4` int unsigned NOT NULL DEFAULT '0' COMMENT 'These values are 32bits flags.',
  `tut5` int unsigned NOT NULL DEFAULT '0' COMMENT 'These values are 32bits flags.',
  `tut6` int unsigned NOT NULL DEFAULT '0' COMMENT 'These values are 32bits flags.',
  `tut7` int unsigned NOT NULL DEFAULT '0' COMMENT 'These values are 32bits flags.',
  PRIMARY KEY (`account`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Player System';

DELETE FROM `character_tutorial`;
/*!40000 ALTER TABLE `character_tutorial` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_tutorial` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
