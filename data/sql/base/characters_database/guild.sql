/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `guild`;
CREATE TABLE IF NOT EXISTS `guild` (
  `guildid` int unsigned NOT NULL DEFAULT '0' COMMENT 'The ID of the guild.',
  `name` varchar(255) NOT NULL DEFAULT '' COMMENT 'The guild name.',
  `leaderguid` int unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the character who created the guild. (See characters.guid)',
  `EmblemStyle` int NOT NULL DEFAULT '0' COMMENT 'The emblem style of the guild tabard.',
  `EmblemColor` int NOT NULL DEFAULT '0' COMMENT 'The emblem color of the guild tabard.',
  `BorderStyle` int NOT NULL DEFAULT '0' COMMENT 'The border style of the guild tabard.',
  `BorderColor` int NOT NULL DEFAULT '0' COMMENT 'The border color of the guild tabard.',
  `BackgroundColor` int NOT NULL DEFAULT '0' COMMENT 'The background color of the guild tabard.',
  `info` varchar(500) NOT NULL DEFAULT '' COMMENT 'The text message that appears in the Guild Information box.',
  `motd` varchar(128) NOT NULL DEFAULT '' COMMENT 'The text that appears in the Message Of The Day box.',
  `createdate` bigint unsigned NOT NULL DEFAULT '0' COMMENT 'The date when the guild was created.',
  `BankMoney` bigint unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guildid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Guild System';

DELETE FROM `guild`;
/*!40000 ALTER TABLE `guild` DISABLE KEYS */;
/*!40000 ALTER TABLE `guild` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
