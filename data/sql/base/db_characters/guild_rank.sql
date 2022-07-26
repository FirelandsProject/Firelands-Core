/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `guild_rank`;
CREATE TABLE IF NOT EXISTS `guild_rank` (
  `guildid` int unsigned NOT NULL DEFAULT '0',
  `rid` tinyint unsigned NOT NULL,
  `rname` varchar(20) NOT NULL DEFAULT '',
  `rights` mediumint unsigned NOT NULL DEFAULT '0',
  `BankMoneyPerDay` int unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guildid`,`rid`),
  KEY `Idx_rid` (`rid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 COMMENT='Guild System';

DELETE FROM `guild_rank`;
/*!40000 ALTER TABLE `guild_rank` DISABLE KEYS */;
INSERT INTO `guild_rank` (`guildid`, `rid`, `rname`, `rights`, `BankMoneyPerDay`) VALUES
	(1, 0, 'Guild Master', 14548927, 4294967295),
	(1, 1, 'Officer', 14548927, 0),
	(1, 2, 'Veteran', 67, 0),
	(1, 3, 'Member', 67, 0),
	(1, 4, 'Noob', 67, 0),
	(1, 5, 'Initiate', 67, 0),
	(2, 0, 'Guild Master', 14548927, 0),
	(2, 1, 'Officer', 14548927, 0),
	(2, 2, 'Veteran', 67, 0),
	(2, 3, 'Member', 67, 0),
	(2, 4, 'Initiate', 67, 0),
	(3, 0, 'Guild Master', 14548927, 0),
	(3, 1, 'Officer', 14548927, 0),
	(3, 2, 'Veteran', 67, 0),
	(3, 3, 'Member', 67, 0),
	(3, 4, 'Initiate', 67, 0),
	(4, 0, 'Guild Master', 14548927, 0),
	(4, 1, 'Officer', 14548927, 0),
	(4, 2, 'Veteran', 67, 0),
	(4, 3, 'Member', 67, 0),
	(4, 4, 'Initiate', 67, 0),
	(5, 0, 'Guild Master', 14548927, 0),
	(5, 1, 'Officer', 14548927, 0),
	(5, 2, 'Veteran', 67, 0),
	(5, 3, 'Member', 67, 0),
	(5, 4, 'Initiate', 67, 0);
/*!40000 ALTER TABLE `guild_rank` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
