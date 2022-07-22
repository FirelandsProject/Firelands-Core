/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `character_spell`;
CREATE TABLE IF NOT EXISTS `character_spell` (
  `guid` int unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID (Global Unique Identifier) of the character. (See character.guid).',
  `spell` int unsigned NOT NULL DEFAULT '0' COMMENT 'The spell ID (Spell Identifier). (See Spell.dbc)',
  `active` tinyint unsigned NOT NULL DEFAULT '1' COMMENT 'Boolean 1 or 0 signifying whether the spell is active (appears in the spell book',
  `disabled` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'Boolean flag 0 or 1 when spell is disabled because of talents.',
  PRIMARY KEY (`guid`,`spell`),
  KEY `Idx_spell` (`spell`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Player System';

DELETE FROM `character_spell`;
/*!40000 ALTER TABLE `character_spell` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_spell` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
