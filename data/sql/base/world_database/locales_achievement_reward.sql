/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `locales_achievement_reward`;
CREATE TABLE IF NOT EXISTS `locales_achievement_reward` (
  `entry` mediumint unsigned NOT NULL DEFAULT '0',
  `gender` tinyint NOT NULL DEFAULT '2',
  `subject_loc1` varchar(100) NOT NULL DEFAULT '',
  `subject_loc2` varchar(100) NOT NULL DEFAULT '',
  `subject_loc3` varchar(100) NOT NULL DEFAULT '',
  `subject_loc4` varchar(100) NOT NULL DEFAULT '',
  `subject_loc5` varchar(100) NOT NULL DEFAULT '',
  `subject_loc6` varchar(100) NOT NULL DEFAULT '',
  `subject_loc7` varchar(100) NOT NULL DEFAULT '',
  `subject_loc8` varchar(100) NOT NULL DEFAULT '',
  `subject_loc9` varchar(100) NOT NULL DEFAULT '',
  `subject_loc10` varchar(100) NOT NULL DEFAULT '',
  `subject_loc11` varchar(100) NOT NULL DEFAULT '',
  `text_loc1` text,
  `text_loc2` text,
  `text_loc3` text,
  `text_loc4` text,
  `text_loc5` text,
  `text_loc6` text,
  `text_loc7` text,
  `text_loc8` text,
  `text_loc9` text NOT NULL,
  `text_loc10` text NOT NULL,
  `text_loc11` text NOT NULL,
  PRIMARY KEY (`entry`,`gender`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC;

DELETE FROM `locales_achievement_reward`;
/*!40000 ALTER TABLE `locales_achievement_reward` DISABLE KEYS */;
/*!40000 ALTER TABLE `locales_achievement_reward` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
