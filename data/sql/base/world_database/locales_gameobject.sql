/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `locales_gameobject`;
CREATE TABLE IF NOT EXISTS `locales_gameobject` (
  `entry` mediumint unsigned NOT NULL DEFAULT '0' COMMENT 'Gameobject ID (See gameobject_template.entry).',
  `name_loc1` varchar(100) NOT NULL DEFAULT '' COMMENT 'Korean localization of name in the gameobject_template table table.',
  `name_loc2` varchar(100) NOT NULL DEFAULT '' COMMENT 'French localization of name in the gameobject_template table table.',
  `name_loc3` varchar(100) NOT NULL DEFAULT '' COMMENT 'German localization of name in the gameobject_template table table.',
  `name_loc4` varchar(100) NOT NULL DEFAULT '' COMMENT 'Chinese localization of name in the gameobject_template table table.',
  `name_loc5` varchar(100) NOT NULL DEFAULT '' COMMENT 'Taiwanese localization of name in the gameobject_template table table.',
  `name_loc6` varchar(100) NOT NULL DEFAULT '' COMMENT 'Spanish Spain localization of name in the gameobject_template table table.',
  `name_loc7` varchar(100) NOT NULL DEFAULT '' COMMENT 'Spanish Latin America localization of name.',
  `name_loc8` varchar(100) NOT NULL DEFAULT '' COMMENT 'Russian localization of name in the gameobject_template table table.',
  `name_loc9` varchar(100) NOT NULL DEFAULT '',
  `name_loc10` varchar(100) NOT NULL DEFAULT '',
  `name_loc11` varchar(100) NOT NULL DEFAULT '',
  `castbarcaption_loc1` varchar(100) NOT NULL DEFAULT '',
  `castbarcaption_loc2` varchar(100) NOT NULL DEFAULT '',
  `castbarcaption_loc3` varchar(100) NOT NULL DEFAULT '',
  `castbarcaption_loc4` varchar(100) NOT NULL DEFAULT '',
  `castbarcaption_loc5` varchar(100) NOT NULL DEFAULT '',
  `castbarcaption_loc6` varchar(100) NOT NULL DEFAULT '',
  `castbarcaption_loc7` varchar(100) NOT NULL DEFAULT '',
  `castbarcaption_loc8` varchar(100) NOT NULL DEFAULT '',
  `castbarcaption_loc9` varchar(100) NOT NULL DEFAULT '',
  `castbarcaption_loc10` varchar(100) NOT NULL DEFAULT '',
  `castbarcaption_loc11` varchar(100) NOT NULL DEFAULT '',
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC;

DELETE FROM `locales_gameobject`;
/*!40000 ALTER TABLE `locales_gameobject` DISABLE KEYS */;
/*!40000 ALTER TABLE `locales_gameobject` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
