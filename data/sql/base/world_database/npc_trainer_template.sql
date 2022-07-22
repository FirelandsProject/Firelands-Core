/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `npc_trainer_template`;
CREATE TABLE IF NOT EXISTS `npc_trainer_template` (
  `entry` mediumint unsigned NOT NULL DEFAULT '0' COMMENT 'The unique identifier for the training template.',
  `spell` mediumint unsigned NOT NULL DEFAULT '0' COMMENT 'The spell identifier. . ',
  `spellcost` int unsigned NOT NULL DEFAULT '0' COMMENT 'The cost that the player needs to pay in order to learn the spell in copper.',
  `reqskill` smallint unsigned NOT NULL DEFAULT '0' COMMENT 'The required skill to be able to learn the spell.',
  `reqskillvalue` smallint unsigned NOT NULL DEFAULT '0' COMMENT 'The minimum skill level required for the skill referenced in reqskill.',
  `reqlevel` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'The character level required in order to learn the spell.',
  UNIQUE KEY `entry_spell` (`entry`,`spell`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3;

DELETE FROM `npc_trainer_template`;
/*!40000 ALTER TABLE `npc_trainer_template` DISABLE KEYS */;
/*!40000 ALTER TABLE `npc_trainer_template` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
