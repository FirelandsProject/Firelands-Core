/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `db_version`;
CREATE TABLE IF NOT EXISTS `db_version` (
  `version` int NOT NULL COMMENT 'The Version of the Release',
  `structure` int NOT NULL COMMENT 'The current core structure level.',
  `content` int NOT NULL COMMENT 'The current core content level.',
  `description` varchar(30) NOT NULL DEFAULT '' COMMENT 'A short description of the latest database revision.',
  `comment` varchar(150) DEFAULT '' COMMENT 'A comment about the latest database revision.',
  PRIMARY KEY (`version`,`structure`,`content`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 ROW_FORMAT=COMPACT COMMENT='Used DB version notes';

DELETE FROM `db_version`;
/*!40000 ALTER TABLE `db_version` DISABLE KEYS */;
INSERT INTO `db_version` (`version`, `structure`, `content`, `description`, `comment`) VALUES
	(22, 3, 2, 'freeze_gm_command', 'Base DB version FDB_22_03_002');
/*!40000 ALTER TABLE `db_version` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
