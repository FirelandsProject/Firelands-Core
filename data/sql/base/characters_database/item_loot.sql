/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `item_loot`;
CREATE TABLE IF NOT EXISTS `item_loot` (
  `guid` int unsigned NOT NULL DEFAULT '0' COMMENT 'Global unique ID of the item loot entry.',
  `owner_guid` int unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the owner. (See character.guid)',
  `itemid` int unsigned NOT NULL DEFAULT '0' COMMENT 'Item template entry ID of the item.',
  `amount` int unsigned NOT NULL DEFAULT '0' COMMENT 'Total number of the item.',
  `suffix` int unsigned NOT NULL DEFAULT '0',
  `property` int NOT NULL DEFAULT '0' COMMENT 'Item random property.',
  PRIMARY KEY (`guid`,`itemid`),
  KEY `idx_owner_guid` (`owner_guid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Item System';

DELETE FROM `item_loot`;
/*!40000 ALTER TABLE `item_loot` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_loot` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
