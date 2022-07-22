/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `character_inventory`;
CREATE TABLE IF NOT EXISTS `character_inventory` (
  `guid` int unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID (Global Unique Identifier) of the character. (See character.guid).',
  `bag` int unsigned NOT NULL DEFAULT '0' COMMENT 'If it isn''t 0, then it is the bag''s item GUID (Global Unique Identifier).',
  `slot` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'The slot is the slot in the bag where the item is.',
  `item` int unsigned NOT NULL DEFAULT '0' COMMENT 'The item''s GUID. (See item_instance.guid).',
  `item_template` int unsigned NOT NULL DEFAULT '0' COMMENT 'The item''s template entry (Item Identifier). (See item_template.entry).',
  PRIMARY KEY (`item`),
  KEY `idx_guid` (`guid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Player System';

DELETE FROM `character_inventory`;
/*!40000 ALTER TABLE `character_inventory` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_inventory` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
