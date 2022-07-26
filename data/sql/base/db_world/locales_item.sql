/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `locales_item`;
CREATE TABLE IF NOT EXISTS `locales_item` (
  `entry` mediumint unsigned NOT NULL DEFAULT '0',
  `name_loc1` varchar(100) NOT NULL DEFAULT '',
  `name_loc2` varchar(100) NOT NULL DEFAULT '',
  `name_loc3` varchar(100) NOT NULL DEFAULT '',
  `name_loc4` varchar(100) NOT NULL DEFAULT '',
  `name_loc5` varchar(100) NOT NULL DEFAULT '',
  `name_loc6` varchar(100) NOT NULL DEFAULT '',
  `name_loc7` varchar(100) NOT NULL DEFAULT '',
  `name_loc8` varchar(100) NOT NULL DEFAULT '',
  `description_loc1` varchar(255) DEFAULT NULL,
  `description_loc2` varchar(255) DEFAULT NULL,
  `description_loc3` varchar(255) DEFAULT NULL,
  `description_loc4` varchar(255) DEFAULT NULL,
  `description_loc5` varchar(255) DEFAULT NULL,
  `description_loc6` varchar(255) DEFAULT NULL,
  `description_loc7` varchar(255) DEFAULT NULL,
  `description_loc8` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3;

DELETE FROM `locales_item`;
/*!40000 ALTER TABLE `locales_item` DISABLE KEYS */;
INSERT INTO `locales_item` (`entry`, `name_loc1`, `name_loc2`, `name_loc3`, `name_loc4`, `name_loc5`, `name_loc6`, `name_loc7`, `name_loc8`, `description_loc1`, `description_loc2`, `description_loc3`, `description_loc4`, `description_loc5`, `description_loc6`, `description_loc7`, `description_loc8`) VALUES
	(77945, '', '', 'Furcht', '', '', '', '', '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
	(77947, '', '', 'Der Schläfer', '', '', '', '', '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
	(77949, '', '', 'Golad, Zwielicht der Aspekte', '', '', '', '', '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
	(77946, '', '', 'Rache', '', '', '', '', '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
	(77948, '', '', 'Der Träumer', '', '', '', '', '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
	(77950, '', '', 'Tiriosh, Alptraum der Zeitalter', '', '', '', '', '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
	(300001, '', '', 'Gladiator', '', '', '', '', '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
	(300002, '', '', 'Duellant', '', '', '', '', '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
	(300003, '', '', 'Rivale', '', '', '', '', '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
	(300004, '', '', 'Herausforderer', '', '', '', '', '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
	(300005, '', '', 'Erbarmungsloser Gladiator', '', '', '', '', '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
	(300006, '', '', 'Rachsüchtiger Gladiator', '', '', '', '', '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
	(300007, '', '', 'Brutaler Gladiator', '', '', '', '', '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
	(300008, '', '', 'Tödlicher Gladiator', '', '', '', '', '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
	(300009, '', '', 'Wütender Gladiator', '', '', '', '', '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
	(300010, '', '', 'Unerbittlicher Gladiator', '', '', '', '', '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
	(300011, '', '', 'Zornerfüllter Gladiator', '', '', '', '', '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
	(300012, '', '', 'Boshafter Gladiator', '', '', '', '', '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
	(300013, '', '', 'Ruchloser Gladiator', '', '', '', '', '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
	(300014, '', '', 'Kataklysmischer Gladiator', '', '', '', '', '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
	(77955, '', 'Calice d\'Alurmi', '', '', '', '', '', '', NULL, 'Ce récipient ne possède pas de boutons, de charnières, de leviers ou d\'accessoires visibles.', NULL, NULL, NULL, NULL, NULL, NULL);
/*!40000 ALTER TABLE `locales_item` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
