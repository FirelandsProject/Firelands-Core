/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `locales_page_text`;
CREATE TABLE IF NOT EXISTS `locales_page_text` (
  `entry` mediumint unsigned NOT NULL DEFAULT '0' COMMENT 'Reference to page_text.entry.',
  `Text_loc1` longtext COMMENT 'Korean localization of Text in the page_text table.',
  `Text_loc2` longtext COMMENT 'French localization of Text in the page_text table.',
  `Text_loc3` longtext COMMENT 'German localization of Text in the page_text table.',
  `Text_loc4` longtext COMMENT 'Chinese localization of Text in the page_text table.',
  `Text_loc5` longtext COMMENT 'Taiwanese localization of Text in the page_text table.',
  `Text_loc6` longtext COMMENT 'Spanish Spain localization of Text in the page_text table.',
  `Text_loc7` longtext COMMENT 'Spanish Latin America localization of Text.',
  `Text_loc8` longtext COMMENT 'Russian localization of Text in the page_text table.',
  `Text_loc9` longtext,
  `Text_loc10` longtext,
  `Text_loc11` longtext,
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC;

DELETE FROM `locales_page_text`;
/*!40000 ALTER TABLE `locales_page_text` DISABLE KEYS */;
/*!40000 ALTER TABLE `locales_page_text` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
