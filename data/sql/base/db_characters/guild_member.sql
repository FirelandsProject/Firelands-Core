/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `guild_member`;
CREATE TABLE IF NOT EXISTS `guild_member` (
  `guildid` int unsigned NOT NULL COMMENT 'Guild Identificator',
  `guid` int unsigned NOT NULL,
  `rank` tinyint unsigned NOT NULL,
  `pnote` varchar(31) NOT NULL DEFAULT '',
  `offnote` varchar(31) NOT NULL DEFAULT '',
  `activity` bigint NOT NULL,
  `profession1_skillID` int NOT NULL DEFAULT '0',
  `weekReputation` int NOT NULL DEFAULT '0',
  `profession2_rank` int NOT NULL DEFAULT '0',
  `profession2_skillID` int NOT NULL DEFAULT '0',
  `profession1_rank` int NOT NULL DEFAULT '0',
  `weekActivity` int NOT NULL DEFAULT '0',
  `profession1_level` int NOT NULL DEFAULT '0',
  `profession2_level` int NOT NULL DEFAULT '0',
  UNIQUE KEY `guid_key` (`guid`) USING BTREE,
  KEY `guildid_key` (`guildid`) USING BTREE,
  KEY `guildid_rank_key` (`guildid`,`rank`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 COMMENT='Guild System';

DELETE FROM `guild_member`;
/*!40000 ALTER TABLE `guild_member` DISABLE KEYS */;
/*!40000 ALTER TABLE `guild_member` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
