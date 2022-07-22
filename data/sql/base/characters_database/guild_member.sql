/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `guild_member`;
CREATE TABLE IF NOT EXISTS `guild_member` (
  `guildid` int unsigned NOT NULL DEFAULT '0' COMMENT 'The ID of the guild that the member is a part of. (See guild.guildid)',
  `guid` int unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the player. (See character.guid)',
  `rank` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'The rank that the player has in the guild. (See guild_rank.rid)',
  `pnote` varchar(31) NOT NULL DEFAULT '' COMMENT 'The Player Note set by the player.',
  `offnote` varchar(31) NOT NULL DEFAULT '' COMMENT 'The Officers Note set by officers in the guild.',
  `BankResetTimeMoney` int unsigned NOT NULL DEFAULT '0',
  `BankRemMoney` int unsigned NOT NULL DEFAULT '0',
  `BankResetTimeTab0` int unsigned NOT NULL DEFAULT '0',
  `BankRemSlotsTab0` int unsigned NOT NULL DEFAULT '0',
  `BankResetTimeTab1` int unsigned NOT NULL DEFAULT '0',
  `BankRemSlotsTab1` int unsigned NOT NULL DEFAULT '0',
  `BankResetTimeTab2` int unsigned NOT NULL DEFAULT '0',
  `BankRemSlotsTab2` int unsigned NOT NULL DEFAULT '0',
  `BankResetTimeTab3` int unsigned NOT NULL DEFAULT '0',
  `BankRemSlotsTab3` int unsigned NOT NULL DEFAULT '0',
  `BankResetTimeTab4` int unsigned NOT NULL DEFAULT '0',
  `BankRemSlotsTab4` int unsigned NOT NULL DEFAULT '0',
  `BankResetTimeTab5` int unsigned NOT NULL DEFAULT '0',
  `BankRemSlotsTab5` int unsigned NOT NULL DEFAULT '0',
  UNIQUE KEY `guid_key` (`guid`),
  KEY `guildid_key` (`guildid`),
  KEY `guildid_rank_key` (`guildid`,`rank`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Guild System';

DELETE FROM `guild_member`;
/*!40000 ALTER TABLE `guild_member` DISABLE KEYS */;
/*!40000 ALTER TABLE `guild_member` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
