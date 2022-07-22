/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `pool_creature_template`;
CREATE TABLE IF NOT EXISTS `pool_creature_template` (
  `id` int unsigned NOT NULL DEFAULT '0' COMMENT 'Creature ID (See creature_template.entry).',
  `pool_entry` mediumint unsigned NOT NULL DEFAULT '0' COMMENT 'Pool ID (See pool_template.entry).',
  `chance` float unsigned NOT NULL DEFAULT '0' COMMENT 'Chance, %.',
  `description` varchar(255) NOT NULL COMMENT 'Description.',
  PRIMARY KEY (`id`),
  KEY `pool_idx` (`pool_entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC;

DELETE FROM `pool_creature_template`;
/*!40000 ALTER TABLE `pool_creature_template` DISABLE KEYS */;
INSERT INTO `pool_creature_template` (`id`, `pool_entry`, `chance`, `description`) VALUES
	(832, 335, 0, 'Dust Devil'),
	(23777, 14361, 0, 'Proto-Drake Egg'),
	(28362, 334, 0, 'Glinting Dirt'),
	(31887, 255, 0, 'Ebon Blade Marker');
/*!40000 ALTER TABLE `pool_creature_template` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
