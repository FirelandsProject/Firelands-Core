/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `creature_linking`;
CREATE TABLE IF NOT EXISTS `creature_linking` (
  `guid` int unsigned NOT NULL COMMENT 'This references the creature table tables unique ID.',
  `master_guid` int unsigned NOT NULL COMMENT 'This references the creature table tables unique ID.',
  `flag` mediumint unsigned NOT NULL COMMENT 'This flag determines how a linked creature will act.',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Creature Linking System';

DELETE FROM `creature_linking`;
/*!40000 ALTER TABLE `creature_linking` DISABLE KEYS */;
INSERT INTO `creature_linking` (`guid`, `master_guid`, `flag`) VALUES
	(59456, 65782, 515),
	(59457, 58910, 515),
	(59504, 58355, 515),
	(65782, 92752, 515),
	(92752, 5402, 515);
/*!40000 ALTER TABLE `creature_linking` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
