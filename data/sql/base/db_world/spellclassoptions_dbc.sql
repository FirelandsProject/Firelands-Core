/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `spellclassoptions_dbc`;
CREATE TABLE IF NOT EXISTS `spellclassoptions_dbc` (
  `Id` int unsigned NOT NULL DEFAULT '0',
  `modalNextSpell` int unsigned NOT NULL DEFAULT '0',
  `spellClassMask_1` int unsigned NOT NULL DEFAULT '0',
  `spellClassMask_2` int unsigned NOT NULL DEFAULT '0',
  `spellClassMask_3` int unsigned NOT NULL DEFAULT '0',
  `spellClassSet` int unsigned NOT NULL DEFAULT '0',
  `description` text NOT NULL,
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3;

DELETE FROM `spellclassoptions_dbc`;
/*!40000 ALTER TABLE `spellclassoptions_dbc` DISABLE KEYS */;
/*!40000 ALTER TABLE `spellclassoptions_dbc` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
