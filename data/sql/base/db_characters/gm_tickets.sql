/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `gm_tickets`;
CREATE TABLE IF NOT EXISTS `gm_tickets` (
  `ticketId` int unsigned NOT NULL AUTO_INCREMENT,
  `guid` int unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier of ticket creator',
  `name` varchar(12) NOT NULL COMMENT 'Name of ticket creator',
  `message` text NOT NULL,
  `createTime` int unsigned NOT NULL DEFAULT '0',
  `mapId` smallint unsigned NOT NULL DEFAULT '0',
  `posX` float NOT NULL DEFAULT '0',
  `posY` float NOT NULL DEFAULT '0',
  `posZ` float NOT NULL DEFAULT '0',
  `lastModifiedTime` int unsigned NOT NULL DEFAULT '0',
  `closedBy` int NOT NULL DEFAULT '0',
  `assignedTo` int unsigned NOT NULL DEFAULT '0' COMMENT 'GUID of admin to whom ticket is assigned',
  `comment` text NOT NULL,
  `response` text NOT NULL,
  `completed` tinyint unsigned NOT NULL DEFAULT '0',
  `escalated` tinyint unsigned NOT NULL DEFAULT '0',
  `viewed` tinyint unsigned NOT NULL DEFAULT '0',
  `haveTicket` tinyint unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ticketId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 COMMENT='Player System';

DELETE FROM `gm_tickets`;
/*!40000 ALTER TABLE `gm_tickets` DISABLE KEYS */;
/*!40000 ALTER TABLE `gm_tickets` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
