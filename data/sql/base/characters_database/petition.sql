/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `petition`;
CREATE TABLE IF NOT EXISTS `petition` (
  `ownerguid` int unsigned NOT NULL COMMENT 'The petition owner''s GUID. (See characters.guid)',
  `petitionguid` int unsigned DEFAULT '0' COMMENT 'The GUID of the petition item. See (item_instance.guid)',
  `name` varchar(255) NOT NULL DEFAULT '' COMMENT 'The name of the guild or arena team that the player is trying to petition.',
  PRIMARY KEY (`ownerguid`),
  UNIQUE KEY `index_ownerguid_petitionguid` (`ownerguid`,`petitionguid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Guild System';

DELETE FROM `petition`;
/*!40000 ALTER TABLE `petition` DISABLE KEYS */;
/*!40000 ALTER TABLE `petition` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
