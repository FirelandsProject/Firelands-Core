/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `character_guild_reputation`;
CREATE TABLE IF NOT EXISTS `character_guild_reputation` (
  `guid` int unsigned NOT NULL,
  `guildid` int unsigned NOT NULL COMMENT 'Guild Identificator',
  `disband_time` int unsigned NOT NULL DEFAULT '0',
  `weekly_rep` bigint NOT NULL DEFAULT '0',
  `total_rep` bigint NOT NULL DEFAULT '0',
  UNIQUE KEY `guid_key` (`guid`),
  KEY `guildid_key` (`guildid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 COMMENT='Characters Guild Reputation System';

DELETE FROM `character_guild_reputation`;
/*!40000 ALTER TABLE `character_guild_reputation` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_guild_reputation` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
