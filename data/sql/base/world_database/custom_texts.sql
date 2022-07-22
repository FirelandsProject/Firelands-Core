/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `custom_texts`;
CREATE TABLE IF NOT EXISTS `custom_texts` (
  `entry` mediumint NOT NULL COMMENT 'The unique identifier of the script text entry.',
  `content_default` text NOT NULL COMMENT 'Contains the text presented in the default language English.',
  `content_loc1` text COMMENT 'Korean localization of content_default.',
  `content_loc2` text COMMENT 'French localization of content_default.',
  `content_loc3` text COMMENT 'German localization of content_default.',
  `content_loc4` text COMMENT 'Chinese localization of content_default.',
  `content_loc5` text COMMENT 'Taiwanese localization of content_default.',
  `content_loc6` text COMMENT 'Spanish (Spain) localization of content_default',
  `content_loc7` text COMMENT 'Spanish (Latin America) localization of content_default',
  `content_loc8` text COMMENT 'Russian localization of content_default',
  `content_loc9` text COMMENT 'Portuguese localization of content_default',
  `content_loc10` text COMMENT 'Brazilian localization of content_default',
  `content_loc11` text COMMENT 'Italian localization of content_default',
  `sound` mediumint unsigned NOT NULL DEFAULT '0' COMMENT 'Reference to a SoundEntries.dbc table entry.',
  `type` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'Selects one of various text emote types to be used for the script text.',
  `language` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'A language identifier.',
  `emote` smallint unsigned NOT NULL DEFAULT '0' COMMENT 'Emote ID that the creature should continually perform.',
  `comment` text COMMENT 'This documents the script text.',
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Custom Texts';

DELETE FROM `custom_texts`;
/*!40000 ALTER TABLE `custom_texts` DISABLE KEYS */;
/*!40000 ALTER TABLE `custom_texts` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
