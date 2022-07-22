/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `item_instance`;
CREATE TABLE IF NOT EXISTS `item_instance` (
  `guid` int unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the item. This number is unique for each item instance.',
  `owner_guid` int unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the character who has ownership of this item. (See character.guid)',
  `data` longtext COMMENT 'Much like the playerbytes fields in the characters table.',
  `text` longtext,
  PRIMARY KEY (`guid`),
  KEY `idx_owner_guid` (`owner_guid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Item System';

DELETE FROM `item_instance`;
/*!40000 ALTER TABLE `item_instance` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_instance` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
