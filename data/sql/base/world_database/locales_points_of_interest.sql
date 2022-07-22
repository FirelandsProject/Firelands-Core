/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `locales_points_of_interest`;
CREATE TABLE IF NOT EXISTS `locales_points_of_interest` (
  `entry` mediumint unsigned NOT NULL DEFAULT '0' COMMENT 'Reference to points_of_interest.entry.',
  `icon_name_loc1` text COMMENT 'Korean localization of icon_name in the points_of_interest table.',
  `icon_name_loc2` text COMMENT 'French localization of icon_name in the points_of_interest table.',
  `icon_name_loc3` text COMMENT 'German localization of icon_name in the points_of_interest table.',
  `icon_name_loc4` text COMMENT 'Chinese localization of icon_name in the points_of_interest table.',
  `icon_name_loc5` text COMMENT 'Taiwanese localization of icon_name in the points_of_interest table.',
  `icon_name_loc6` text COMMENT 'Spanish Spain localization of icon_name in the points_of_interest table.',
  `icon_name_loc7` text COMMENT 'Spanish Latin America localization of icon_name.',
  `icon_name_loc8` text COMMENT 'Russian localization of icon_name in the points_of_interest table.',
  `icon_name_loc9` text,
  `icon_name_loc10` text,
  `icon_name_loc11` text,
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC;

DELETE FROM `locales_points_of_interest`;
/*!40000 ALTER TABLE `locales_points_of_interest` DISABLE KEYS */;
/*!40000 ALTER TABLE `locales_points_of_interest` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
