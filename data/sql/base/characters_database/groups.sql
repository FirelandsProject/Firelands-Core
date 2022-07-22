/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `groups`;
CREATE TABLE IF NOT EXISTS `groups` (
  `groupId` int unsigned NOT NULL COMMENT 'Uniquie ID for the group.',
  `leaderGuid` int unsigned NOT NULL COMMENT 'Guid of the group''s leader',
  `mainTank` int unsigned NOT NULL COMMENT 'Guid of the character who is that main tank.',
  `mainAssistant` int unsigned NOT NULL COMMENT 'Guid of the character who is that main assist.',
  `lootMethod` tinyint unsigned NOT NULL COMMENT 'This is the loot method used by the group.',
  `looterGuid` int unsigned NOT NULL COMMENT 'Guid of the character who retrieves the loot.',
  `lootThreshold` tinyint unsigned NOT NULL COMMENT 'The quality of the items in the loot to be rolled for.',
  `icon1` int unsigned NOT NULL COMMENT 'Guid of the target that has the star raid target on it.',
  `icon2` int unsigned NOT NULL COMMENT 'Guid of the target that has the circle raid target on it.',
  `icon3` int unsigned NOT NULL COMMENT 'Guid of target that has the dimond raid target on it.',
  `icon4` int unsigned NOT NULL COMMENT 'Guid of the target that has the triangler raid target on it.',
  `icon5` int unsigned NOT NULL COMMENT 'Guid of the target that has the moon raid target on it.',
  `icon6` int unsigned NOT NULL COMMENT 'Guid of the target that has the square raid target on it.',
  `icon7` int unsigned NOT NULL COMMENT 'Guid of the target that has the cross raid target on it.',
  `icon8` int unsigned NOT NULL COMMENT 'Guid of the target that has the skull raid target on it.',
  `groupType` tinyint unsigned NOT NULL,
  `difficulty` tinyint unsigned NOT NULL DEFAULT '0',
  `raiddifficulty` int unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`groupId`),
  UNIQUE KEY `leaderGuid` (`leaderGuid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Groups';

DELETE FROM `groups`;
/*!40000 ALTER TABLE `groups` DISABLE KEYS */;
/*!40000 ALTER TABLE `groups` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
