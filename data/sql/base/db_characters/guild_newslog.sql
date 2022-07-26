/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `guild_newslog`;
CREATE TABLE IF NOT EXISTS `guild_newslog` (
  `guildid` int unsigned NOT NULL DEFAULT '0',
  `TimeStamp` int unsigned NOT NULL DEFAULT '0',
  `PlayerGuid` int unsigned NOT NULL DEFAULT '0',
  `LogGuid` int unsigned NOT NULL DEFAULT '0',
  `Flags` int unsigned NOT NULL DEFAULT '0',
  `EventType` tinyint unsigned NOT NULL DEFAULT '0',
  `Value` int unsigned NOT NULL DEFAULT '0',
  KEY `Idx_PlayerGuid` (`PlayerGuid`) USING BTREE,
  KEY `Idx_LogGuid` (`LogGuid`) USING BTREE,
  KEY `guildid_key` (`guildid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 COMMENT='Guild Newslog';

DELETE FROM `guild_newslog`;
/*!40000 ALTER TABLE `guild_newslog` DISABLE KEYS */;
/*!40000 ALTER TABLE `guild_newslog` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
