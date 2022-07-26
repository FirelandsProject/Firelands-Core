/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `spell_map`;
CREATE TABLE IF NOT EXISTS `spell_map` (
  `spell` mediumint unsigned NOT NULL DEFAULT '0',
  `map` mediumint unsigned NOT NULL DEFAULT '0',
  `quest_start` mediumint unsigned NOT NULL DEFAULT '0',
  `quest_start_active` tinyint unsigned NOT NULL DEFAULT '0',
  `quest_end` mediumint unsigned NOT NULL DEFAULT '0',
  `aura_spell` mediumint NOT NULL DEFAULT '0',
  `racemask` mediumint unsigned NOT NULL DEFAULT '0',
  `gender` tinyint unsigned NOT NULL DEFAULT '2',
  `autocast` tinyint unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`spell`,`map`,`quest_start`,`quest_start_active`,`aura_spell`,`racemask`,`gender`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3;

DELETE FROM `spell_map`;
/*!40000 ALTER TABLE `spell_map` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_map` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
