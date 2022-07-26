/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `auctionhouse`;
CREATE TABLE IF NOT EXISTS `auctionhouse` (
  `id` int unsigned NOT NULL DEFAULT '0',
  `auctioneerguid` int unsigned NOT NULL DEFAULT '0',
  `itemguid` int unsigned NOT NULL DEFAULT '0',
  `itemowner` int unsigned NOT NULL DEFAULT '0',
  `buyoutprice` int unsigned NOT NULL DEFAULT '0',
  `time` int unsigned NOT NULL DEFAULT '0',
  `buyguid` int unsigned NOT NULL DEFAULT '0',
  `lastbid` int unsigned NOT NULL DEFAULT '0',
  `startbid` int unsigned NOT NULL DEFAULT '0',
  `deposit` int unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `item_guid` (`itemguid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;

DELETE FROM `auctionhouse`;
/*!40000 ALTER TABLE `auctionhouse` DISABLE KEYS */;
INSERT INTO `auctionhouse` (`id`, `auctioneerguid`, `itemguid`, `itemowner`, `buyoutprice`, `time`, `buyguid`, `lastbid`, `startbid`, `deposit`) VALUES
	(1, 92656, 17430, 1, 0, 1568781172, 0, 0, 100, 100);
/*!40000 ALTER TABLE `auctionhouse` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
