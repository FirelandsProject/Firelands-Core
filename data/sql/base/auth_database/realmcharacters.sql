/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `realmcharacters`;
CREATE TABLE IF NOT EXISTS `realmcharacters` (
  `realmid` int unsigned NOT NULL COMMENT 'The ID of the realm (See realmlist.id).',
  `acctid` int unsigned NOT NULL COMMENT 'The account ID (See account.id).',
  `numchars` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'The number of characters the account has on the realm.',
  PRIMARY KEY (`realmid`,`acctid`),
  KEY `acctid` (`acctid`),
  CONSTRAINT `realmcharacters_ibfk_1` FOREIGN KEY (`realmid`) REFERENCES `realmlist` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Realm Character Tracker';

DELETE FROM `realmcharacters`;
/*!40000 ALTER TABLE `realmcharacters` DISABLE KEYS */;
/*!40000 ALTER TABLE `realmcharacters` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
