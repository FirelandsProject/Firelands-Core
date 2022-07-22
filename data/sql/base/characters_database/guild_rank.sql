/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `guild_rank`;
CREATE TABLE IF NOT EXISTS `guild_rank` (
  `guildid` int unsigned NOT NULL DEFAULT '0' COMMENT 'The guild ID that the rank is part of. (See guild.guildid)',
  `rid` int unsigned NOT NULL COMMENT 'The particular rank ID. This number must be unique to each rank in a guild.',
  `rname` varchar(255) NOT NULL DEFAULT '' COMMENT 'The name of the rank that is displayed in-game.',
  `rights` int unsigned NOT NULL DEFAULT '0' COMMENT 'The rights a player with this rank has in the guild.',
  `BankMoneyPerDay` int unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guildid`,`rid`),
  KEY `Idx_rid` (`rid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Guild System';

DELETE FROM `guild_rank`;
/*!40000 ALTER TABLE `guild_rank` DISABLE KEYS */;
/*!40000 ALTER TABLE `guild_rank` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
