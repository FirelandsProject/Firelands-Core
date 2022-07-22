/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `npc_vendor_template`;
CREATE TABLE IF NOT EXISTS `npc_vendor_template` (
  `entry` mediumint unsigned NOT NULL DEFAULT '0' COMMENT 'The unique identifier for the vending template.',
  `item` mediumint unsigned NOT NULL DEFAULT '0' COMMENT 'Template ID of the item. ',
  `maxcount` smallint unsigned NOT NULL DEFAULT '0' COMMENT 'The maximum amount of the item a vendor should carry',
  `incrtime` int unsigned NOT NULL DEFAULT '0' COMMENT 'How frequently a vendor will restock an item having a maximum count. ',
  `ExtendedCost` mediumint unsigned NOT NULL DEFAULT '0',
  `condition_id` mediumint unsigned NOT NULL DEFAULT '0' COMMENT 'This references the conditions tables unique ID for which the entry is valid.',
  PRIMARY KEY (`entry`,`item`,`ExtendedCost`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Npc System';

DELETE FROM `npc_vendor_template`;
/*!40000 ALTER TABLE `npc_vendor_template` DISABLE KEYS */;
INSERT INTO `npc_vendor_template` (`entry`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `condition_id`) VALUES
	(33238, 4470, 0, 0, 0, 0),
	(33238, 5565, 0, 0, 0, 0),
	(33238, 16583, 0, 0, 0, 0),
	(33238, 17020, 0, 0, 0, 0),
	(33238, 17021, 0, 0, 0, 0),
	(33238, 17026, 0, 0, 0, 0),
	(33238, 17028, 0, 0, 0, 0),
	(33238, 17030, 0, 0, 0, 0),
	(33238, 17031, 0, 0, 0, 0),
	(33238, 17032, 0, 0, 0, 0),
	(33238, 17033, 0, 0, 0, 0),
	(33238, 17034, 0, 0, 0, 0),
	(33238, 21177, 0, 0, 0, 0),
	(33238, 22148, 0, 0, 0, 0),
	(33238, 33443, 0, 0, 0, 0),
	(33238, 33449, 0, 0, 0, 0),
	(33238, 33451, 0, 0, 0, 0),
	(33238, 33452, 0, 0, 0, 0),
	(33238, 33454, 0, 0, 0, 0),
	(33238, 35949, 0, 0, 0, 0),
	(33238, 35954, 0, 0, 0, 0),
	(33238, 37201, 0, 0, 0, 0),
	(33238, 44605, 0, 0, 0, 0),
	(33239, 4470, 0, 0, 0, 0),
	(33239, 5565, 0, 0, 0, 0),
	(33239, 16583, 0, 0, 0, 0),
	(33239, 17020, 0, 0, 0, 0),
	(33239, 17021, 0, 0, 0, 0),
	(33239, 17026, 0, 0, 0, 0),
	(33239, 17028, 0, 0, 0, 0),
	(33239, 17030, 0, 0, 0, 0),
	(33239, 17031, 0, 0, 0, 0),
	(33239, 17032, 0, 0, 0, 0),
	(33239, 17033, 0, 0, 0, 0),
	(33239, 17034, 0, 0, 0, 0),
	(33239, 21177, 0, 0, 0, 0),
	(33239, 22148, 0, 0, 0, 0),
	(33239, 33443, 0, 0, 0, 0),
	(33239, 33449, 0, 0, 0, 0),
	(33239, 33451, 0, 0, 0, 0),
	(33239, 33452, 0, 0, 0, 0),
	(33239, 33454, 0, 0, 0, 0),
	(33239, 35949, 0, 0, 0, 0),
	(33239, 35954, 0, 0, 0, 0),
	(33239, 37201, 0, 0, 0, 0),
	(33239, 44605, 0, 0, 0, 0);
/*!40000 ALTER TABLE `npc_vendor_template` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
