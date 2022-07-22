/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `locales_gossip_menu_option`;
CREATE TABLE IF NOT EXISTS `locales_gossip_menu_option` (
  `menu_id` smallint unsigned NOT NULL DEFAULT '0' COMMENT 'Reference to gossip_menu_option.menu_id.',
  `id` smallint unsigned NOT NULL DEFAULT '0' COMMENT 'Reference to gossip_menu_option.id.',
  `option_text_loc1` text COMMENT 'Korean localization of option_text in the gossip_menu_option table table.',
  `option_text_loc2` text COMMENT 'French localization of option_text in the gossip_menu_option table table.',
  `option_text_loc3` text COMMENT 'German localization of option_text in the gossip_menu_option table table.',
  `option_text_loc4` text COMMENT 'Chinese localization of option_text in the gossip_menu_option table table.',
  `option_text_loc5` text COMMENT 'Taiwanese localization of option_text in the gossip_menu_option table table.',
  `option_text_loc6` text COMMENT 'Spanish Spain localization of option_text.',
  `option_text_loc7` text COMMENT 'Spanish Latin America localization of option_text.',
  `option_text_loc8` text COMMENT 'Russian localization of option_text in the gossip_menu_option table table.',
  `option_text_loc9` text,
  `option_text_loc10` text,
  `option_text_loc11` text,
  `box_text_loc1` text COMMENT 'Korean localization of box_text in the gossip_menu_option table table.',
  `box_text_loc2` text COMMENT 'French localization of box_text in the gossip_menu_option table table.',
  `box_text_loc3` text COMMENT 'German localization of box_text in the gossip_menu_option table table.',
  `box_text_loc4` text COMMENT 'Chinese localization of box_text in the gossip_menu_option table table.',
  `box_text_loc5` text COMMENT 'Taiwanese localization of box_text in the gossip_menu_option table table.',
  `box_text_loc6` text COMMENT 'Spanish Spain localization of box_text in the gossip_menu_option table table.',
  `box_text_loc7` text COMMENT 'Spanish Latin America localization of box_text.',
  `box_text_loc8` text COMMENT 'Russian localization of box_text in the gossip_menu_option table table.',
  `box_text_loc9` text,
  `box_text_loc10` text,
  `box_text_loc11` text,
  PRIMARY KEY (`menu_id`,`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC;

DELETE FROM `locales_gossip_menu_option`;
/*!40000 ALTER TABLE `locales_gossip_menu_option` DISABLE KEYS */;
/*!40000 ALTER TABLE `locales_gossip_menu_option` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
