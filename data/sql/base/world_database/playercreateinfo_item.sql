/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `playercreateinfo_item`;
CREATE TABLE IF NOT EXISTS `playercreateinfo_item` (
  `race` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'A bit-mask corresponding to races that should get the spell.',
  `class` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'A bit-mask corresponding to class that should get the spell.',
  `itemid` mediumint unsigned NOT NULL DEFAULT '0' COMMENT 'The item a character should receive upon creation.',
  `amount` tinyint unsigned NOT NULL DEFAULT '1' COMMENT 'The number of copies of the item to be awarded.',
  KEY `playercreateinfo_race_class_index` (`race`,`class`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3;

DELETE FROM `playercreateinfo_item`;
/*!40000 ALTER TABLE `playercreateinfo_item` DISABLE KEYS */;
/*!40000 ALTER TABLE `playercreateinfo_item` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
