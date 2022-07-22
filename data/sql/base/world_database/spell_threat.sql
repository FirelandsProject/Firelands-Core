/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `spell_threat`;
CREATE TABLE IF NOT EXISTS `spell_threat` (
  `entry` mediumint unsigned NOT NULL COMMENT 'The spell identifier.',
  `Threat` smallint NOT NULL COMMENT 'The value of threat to add or remove from the characters threat.',
  `multiplier` float NOT NULL DEFAULT '1' COMMENT 'Any value here will modify the spells threat with the factor given here.',
  `ap_bonus` float NOT NULL DEFAULT '0' COMMENT 'Any value here will modify the spells attack power with the factor given here.',
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC;

DELETE FROM `spell_threat`;
/*!40000 ALTER TABLE `spell_threat` DISABLE KEYS */;
INSERT INTO `spell_threat` (`entry`, `Threat`, `multiplier`, `ap_bonus`) VALUES
	(78, 20, 1, 0),
	(99, 15, 1, 0),
	(676, 104, 1, 0),
	(770, 108, 1, 0),
	(779, 0, 1.5, 0),
	(845, 10, 1, 0),
	(1160, 16, 1, 0),
	(1715, 61, 1, 0),
	(2139, 300, 1, 0),
	(5676, 0, 2, 0),
	(6343, 0, 1.75, 0),
	(6572, 155, 1, 0),
	(6807, 13, 1, 0),
	(7386, 100, 1, 0),
	(16857, 108, 1, 0),
	(17735, 200, 1, 0),
	(20243, 0, 1, 0.05),
	(20736, 100, 1, 0),
	(20925, 20, 1, 0),
	(23922, 160, 1, 0),
	(24394, 580, 1, 0),
	(33745, 182, 0.5, 0),
	(33878, 129, 1, 0),
	(52212, 0, 1.9, 0),
	(56815, 0, 1.75, 0),
	(57755, 0, 1.5, 0);
/*!40000 ALTER TABLE `spell_threat` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
