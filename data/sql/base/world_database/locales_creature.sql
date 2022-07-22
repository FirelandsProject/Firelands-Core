/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `locales_creature`;
CREATE TABLE IF NOT EXISTS `locales_creature` (
  `entry` mediumint unsigned NOT NULL DEFAULT '0' COMMENT 'Creature ID (See creature_template.entry).',
  `name_loc1` varchar(100) NOT NULL DEFAULT '' COMMENT 'Korean localization of name in the creature_template table table.',
  `name_loc2` varchar(100) NOT NULL DEFAULT '' COMMENT 'French localization of name in the creature_template table table.',
  `name_loc3` varchar(100) NOT NULL DEFAULT '' COMMENT 'German localization of name in the creature_template table table.',
  `name_loc4` varchar(100) NOT NULL DEFAULT '' COMMENT 'Chinese localization of name in the creature_template table table.',
  `name_loc5` varchar(100) NOT NULL DEFAULT '' COMMENT 'Taiwanese localization of name in the creature_template table table.',
  `name_loc6` varchar(100) NOT NULL DEFAULT '' COMMENT 'Spanish Spain localization of name in the creature_template table table.',
  `name_loc7` varchar(100) NOT NULL DEFAULT '' COMMENT 'Spanish Latin America localization of name.',
  `name_loc8` varchar(100) NOT NULL DEFAULT '' COMMENT 'Russian localization of name in the creature_template table table.',
  `name_loc9` varchar(100) DEFAULT NULL,
  `name_loc10` varchar(100) DEFAULT NULL,
  `name_loc11` varchar(100) DEFAULT NULL,
  `subname_loc1` varchar(100) DEFAULT NULL COMMENT 'Korean localization of subname in the creature_template table table.',
  `subname_loc2` varchar(100) DEFAULT NULL COMMENT 'French localization of subname in the creature_template table table.',
  `subname_loc3` varchar(100) DEFAULT NULL COMMENT 'German localization of subname in the creature_template table table.',
  `subname_loc4` varchar(100) DEFAULT NULL COMMENT 'Chinese localization of subname in the creature_template table table.',
  `subname_loc5` varchar(100) DEFAULT NULL COMMENT 'Taiwanese localization of subname in the creature_template table table.',
  `subname_loc6` varchar(100) DEFAULT NULL COMMENT 'Spanish Spain localization of subname in the creature_template table table.',
  `subname_loc7` varchar(100) DEFAULT NULL COMMENT 'Spanish Latin America localization of subname.',
  `subname_loc8` varchar(100) DEFAULT NULL COMMENT 'Russian localization of subname in the creature_template table table.',
  `subname_loc9` varchar(100) DEFAULT NULL,
  `subname_loc10` varchar(100) DEFAULT NULL,
  `subname_loc11` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC;

DELETE FROM `locales_creature`;
/*!40000 ALTER TABLE `locales_creature` DISABLE KEYS */;
/*!40000 ALTER TABLE `locales_creature` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
