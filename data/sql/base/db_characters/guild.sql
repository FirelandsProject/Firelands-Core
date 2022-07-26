/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `guild`;
CREATE TABLE IF NOT EXISTS `guild` (
  `guildid` int unsigned NOT NULL DEFAULT '0',
  `name` varchar(24) NOT NULL DEFAULT '',
  `leaderguid` int unsigned NOT NULL DEFAULT '0',
  `EmblemStyle` tinyint unsigned NOT NULL DEFAULT '0',
  `EmblemColor` tinyint unsigned NOT NULL DEFAULT '0',
  `BorderStyle` tinyint unsigned NOT NULL DEFAULT '0',
  `BorderColor` tinyint unsigned NOT NULL DEFAULT '0',
  `BackgroundColor` tinyint unsigned NOT NULL DEFAULT '0',
  `info` text NOT NULL,
  `motd` varchar(128) NOT NULL DEFAULT '',
  `createdate` int unsigned NOT NULL DEFAULT '0',
  `BankMoney` bigint unsigned NOT NULL DEFAULT '0',
  `level` int unsigned DEFAULT '1',
  `todayExperience` bigint unsigned DEFAULT '0',
  `RaidChallenge` int NOT NULL DEFAULT '0',
  `ChallengeCount` int unsigned NOT NULL DEFAULT '0',
  `RatedBGChallenge` int NOT NULL DEFAULT '0',
  `DungeonChallenge` int NOT NULL DEFAULT '0',
  `experience` bigint unsigned DEFAULT '0',
  PRIMARY KEY (`guildid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 COMMENT='Guild System';

DELETE FROM `guild`;
/*!40000 ALTER TABLE `guild` DISABLE KEYS */;
/*!40000 ALTER TABLE `guild` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
