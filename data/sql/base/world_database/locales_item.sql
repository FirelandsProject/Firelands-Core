/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `locales_item`;
CREATE TABLE IF NOT EXISTS `locales_item` (
  `entry` mediumint unsigned NOT NULL DEFAULT '0' COMMENT 'Item ID (See item_template.entry).',
  `name_loc1` varchar(100) NOT NULL DEFAULT '' COMMENT 'Korean localization of name in the item_template table table.',
  `name_loc2` varchar(100) NOT NULL DEFAULT '' COMMENT 'French localization of name in the item_template table table.',
  `name_loc3` varchar(100) NOT NULL DEFAULT '' COMMENT 'German localization of name in the item_template table table.',
  `name_loc4` varchar(100) NOT NULL DEFAULT '' COMMENT 'Chinese localization of name in the item_template table table.',
  `name_loc5` varchar(100) NOT NULL DEFAULT '' COMMENT 'Taiwanese localization of name in the item_template table table.',
  `name_loc6` varchar(100) NOT NULL DEFAULT '' COMMENT 'Spanish Spain localization of name in the item_template table table.',
  `name_loc7` varchar(100) NOT NULL DEFAULT '' COMMENT 'Spanish Latin America localization of name in the item_template table table.',
  `name_loc8` varchar(100) NOT NULL DEFAULT '' COMMENT 'Russian localization of name in the item_template table table.',
  `name_loc9` varchar(100) DEFAULT NULL,
  `name_loc10` varchar(100) DEFAULT NULL,
  `name_loc11` varchar(100) DEFAULT NULL,
  `description_loc1` varchar(255) DEFAULT NULL COMMENT 'Korean localization of description in the item_template table table.',
  `description_loc2` varchar(255) DEFAULT NULL COMMENT 'French localization of description in the item_template table table.',
  `description_loc3` varchar(255) DEFAULT NULL COMMENT 'German localization of description in the item_template table table.',
  `description_loc4` varchar(255) DEFAULT NULL COMMENT 'Chinese localization of description in the item_template table table.',
  `description_loc5` varchar(255) DEFAULT NULL COMMENT 'Taiwanese localization of description in the item_template table table.',
  `description_loc6` varchar(255) DEFAULT NULL COMMENT 'Spanish Spain localization of description in the item_template table table.',
  `description_loc7` varchar(255) DEFAULT NULL COMMENT 'Spanish Latin America localization of description.',
  `description_loc8` varchar(255) DEFAULT NULL COMMENT 'Russian localization of description in the item_template table table.',
  `description_loc9` varchar(255) DEFAULT NULL,
  `description_loc10` varchar(255) DEFAULT NULL,
  `description_loc11` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC;

DELETE FROM `locales_item`;
/*!40000 ALTER TABLE `locales_item` DISABLE KEYS */;
/*!40000 ALTER TABLE `locales_item` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
