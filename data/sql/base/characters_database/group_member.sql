/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `group_member`;
CREATE TABLE IF NOT EXISTS `group_member` (
  `groupId` int unsigned NOT NULL COMMENT 'The group ID from the groups table.',
  `memberGuid` int unsigned NOT NULL COMMENT 'The GUID of the character in the group.',
  `assistant` tinyint unsigned NOT NULL COMMENT 'Flag stating whether or not the character is the assist.',
  `subgroup` smallint unsigned NOT NULL COMMENT 'This is the group number within a raid (8 groups max)',
  PRIMARY KEY (`groupId`,`memberGuid`),
  KEY `Idx_memberGuid` (`memberGuid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Groups';

DELETE FROM `group_member`;
/*!40000 ALTER TABLE `group_member` DISABLE KEYS */;
/*!40000 ALTER TABLE `group_member` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
