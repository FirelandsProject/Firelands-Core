/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `guild_eventlog`;
CREATE TABLE IF NOT EXISTS `guild_eventlog` (
  `guildid` int unsigned NOT NULL COMMENT 'Id of related guild. (See guild.guildid).',
  `LogGuid` int unsigned NOT NULL COMMENT 'A unique identifier given to each log entry to distinguish it.',
  `EventType` tinyint unsigned NOT NULL COMMENT 'The Guild Event Type.',
  `PlayerGuid1` int unsigned NOT NULL COMMENT 'Guid of character who made the change. (See character.guid)',
  `PlayerGuid2` int unsigned NOT NULL COMMENT 'Guid of character who was changed. (See character.guid)',
  `NewRank` tinyint unsigned NOT NULL COMMENT 'Id of new rank of PlayerGuid2. (See guild_rank.rid)',
  `TimeStamp` bigint unsigned NOT NULL COMMENT 'Unix timestamp of when the event happened.',
  PRIMARY KEY (`guildid`,`LogGuid`),
  KEY `Idx_PlayerGuid1` (`PlayerGuid1`),
  KEY `Idx_PlayerGuid2` (`PlayerGuid2`),
  KEY `Idx_LogGuid` (`LogGuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 COMMENT='Guild Eventlog';

DELETE FROM `guild_eventlog`;
/*!40000 ALTER TABLE `guild_eventlog` DISABLE KEYS */;
/*!40000 ALTER TABLE `guild_eventlog` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
