/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `vehicle_accessory`;
CREATE TABLE IF NOT EXISTS `vehicle_accessory` (
  `guid` mediumint NOT NULL DEFAULT '0',
  `accessory_entry` mediumint unsigned NOT NULL DEFAULT '0',
  `seat_id` tinyint NOT NULL DEFAULT '0',
  `minion` tinyint unsigned NOT NULL DEFAULT '0',
  `description` text NOT NULL,
  `summontype` tinyint unsigned NOT NULL DEFAULT '6' COMMENT 'see enum TempSummonType',
  `summontimer` int unsigned NOT NULL DEFAULT '30000' COMMENT 'timer, only relevant for certain summontypes',
  PRIMARY KEY (`guid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=FIXED;

DELETE FROM `vehicle_accessory`;
/*!40000 ALTER TABLE `vehicle_accessory` DISABLE KEYS */;
INSERT INTO `vehicle_accessory` (`guid`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES
	(35502, 46447, 0, 0, '', 6, 30000),
	(34162, 46447, 0, 0, '', 6, 30000);
/*!40000 ALTER TABLE `vehicle_accessory` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
