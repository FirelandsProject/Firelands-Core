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

-- Dumpar struktur för tabell acore_auth.build_info
DROP TABLE IF EXISTS `build_info`;
CREATE TABLE IF NOT EXISTS `build_info` (
  `build` INT NOT NULL,
  `majorVersion` INT DEFAULT NULL,
  `minorVersion` INT DEFAULT NULL,
  `bugfixVersion` INT DEFAULT NULL,
  `hotfixVersion` char(3) DEFAULT NULL,
  `winAuthSeed` VARCHAR(32) DEFAULT NULL,
  `win64AuthSeed` VARCHAR(32) DEFAULT NULL,
  `mac64AuthSeed` VARCHAR(32) DEFAULT NULL,
  `winChecksumSeed` VARCHAR(40) DEFAULT NULL,
  `macChecksumSeed` VARCHAR(40) DEFAULT NULL,
  PRIMARY KEY (`build`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- Dumpar data för tabell acore_auth.build_info: ~11 rows (ungefär)
DELETE FROM `build_info`;
/*!40000 ALTER TABLE `build_info` DISABLE KEYS */;
INSERT INTO `build_info` (`build`, `majorVersion`, `minorVersion`, `bugfixVersion`, `hotfixVersion`, `winAuthSeed`, `win64AuthSeed`, `mac64AuthSeed`, `winChecksumSeed`, `macChecksumSeed`) VALUES
	(15595, 4, 3, 4, NULL, NULL, NULL, NULL, NULL, NULL);
/*!40000 ALTER TABLE `build_info` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;
