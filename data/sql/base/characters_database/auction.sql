/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `auction`;
CREATE TABLE IF NOT EXISTS `auction` (
  `id` int unsigned NOT NULL DEFAULT '0' COMMENT 'Unique identifier for every auction.',
  `houseid` int unsigned NOT NULL DEFAULT '0' COMMENT 'The AuctionHouseId (See AuctionHouse.dbc)',
  `itemguid` int unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the item that is up for auction. (See item_instance.guid)',
  `item_template` int unsigned NOT NULL DEFAULT '0' COMMENT 'The entry of the item up for auction. See (item_template.entry)',
  `item_count` int unsigned NOT NULL DEFAULT '0' COMMENT 'The stack count of the item',
  `item_randompropertyid` int NOT NULL DEFAULT '0' COMMENT 'A link to the Item''s RandomProperty Id (See item_template.Randompropertyid).',
  `itemowner` int unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the owner of the item up for auction. (See character.guid)',
  `buyoutprice` bigint NOT NULL DEFAULT '0' COMMENT 'The buyout price of the item in copper.',
  `time` bigint unsigned NOT NULL DEFAULT '0' COMMENT 'The time when this auction will end, measured in Unix time.',
  `moneyTime` bigint unsigned NOT NULL DEFAULT '0',
  `buyguid` int unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the highest bidder. See (character.guid)',
  `lastbid` bigint NOT NULL DEFAULT '0' COMMENT 'The amount of copper of the last bid made on the item.',
  `startbid` bigint NOT NULL DEFAULT '0' COMMENT 'The amount of copper of the starting bid made on the item.',
  `deposit` bigint NOT NULL DEFAULT '0' COMMENT 'The amount of copper spent on the deposit.',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;

DELETE FROM `auction`;
/*!40000 ALTER TABLE `auction` DISABLE KEYS */;
/*!40000 ALTER TABLE `auction` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
