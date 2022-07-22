/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `petition_sign`;
CREATE TABLE IF NOT EXISTS `petition_sign` (
  `ownerguid` int unsigned NOT NULL COMMENT 'The GUID of the owner that is trying to make the guild/arena team.',
  `petitionguid` int unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the charter item. (See item_template.guid)',
  `playerguid` int unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the player that has signed the charter. (See character.guid)',
  `player_account` int unsigned NOT NULL DEFAULT '0' COMMENT 'The account ID of the player that has signed the charter (See account.id).',
  PRIMARY KEY (`petitionguid`,`playerguid`),
  KEY `Idx_playerguid` (`playerguid`),
  KEY `Idx_ownerguid` (`ownerguid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Guild System';

DELETE FROM `petition_sign`;
/*!40000 ALTER TABLE `petition_sign` DISABLE KEYS */;
/*!40000 ALTER TABLE `petition_sign` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
