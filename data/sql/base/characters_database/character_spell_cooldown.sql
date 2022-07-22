/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `character_spell_cooldown`;
CREATE TABLE IF NOT EXISTS `character_spell_cooldown` (
  `guid` int unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID (Global Unique Identifier, Low part) of the character.',
  `spell` int unsigned NOT NULL DEFAULT '0' COMMENT 'The spell ID (Spell Identifier). (See Spell.dbc)',
  `item` int unsigned NOT NULL DEFAULT '0' COMMENT 'If the spell was casted from an item, the item ID (Item Identifier).',
  `time` bigint unsigned NOT NULL DEFAULT '0' COMMENT 'The time when the spell cooldown will finish, measured in Unix Time.',
  PRIMARY KEY (`guid`,`spell`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;

DELETE FROM `character_spell_cooldown`;
/*!40000 ALTER TABLE `character_spell_cooldown` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_spell_cooldown` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
