-- --------------------------------------------------------
-- Värd:                         127.0.0.1
-- Serverversion:                8.0.28 - MySQL Community Server - GPL
-- Server-OS:                    Win64
-- HeidiSQL Version:             11.3.0.6295
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

-- Dumpar struktur för tabell acore_auth.version_db_auth
DROP TABLE IF EXISTS `version_db_auth`;
CREATE TABLE IF NOT EXISTS `version_db_auth` (
  `sql_rev` VARCHAR(100) NOT NULL,
  `required_rev` VARCHAR(100) DEFAULT NULL,
  `date` VARCHAR(50) DEFAULT NULL,
  `2021_11_06_00` bit(1) DEFAULT NULL,
  PRIMARY KEY (`sql_rev`),
  KEY `required` (`required_rev`),
  CONSTRAINT `required` FOREIGN KEY (`required_rev`) REFERENCES `version_db_auth` (`sql_rev`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 ROW_FORMAT=DYNAMIC COMMENT='Last applied sql update to DB';

-- Dumpar data för tabell acore_auth.version_db_auth: ~10 rows (ungefär)
DELETE FROM `version_db_auth`;
/*!40000 ALTER TABLE `version_db_auth` DISABLE KEYS */;

/*!40000 ALTER TABLE `version_db_auth` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;
