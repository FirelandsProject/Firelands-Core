/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `character_aura`;
CREATE TABLE IF NOT EXISTS `character_aura` (
  `guid` int unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID (Full Global Unique Identifier) of the target affected by the aura.',
  `caster_guid` bigint unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID (Global Unique Identifier) of the player who cast the aura.',
  `item_guid` int unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID (Global Unique Identifier) of the item which casted the aura.',
  `spell` int unsigned NOT NULL DEFAULT '0' COMMENT 'The spell from which the aura was applied. (See Spell.dbc)',
  `stackcount` int unsigned NOT NULL DEFAULT '1' COMMENT 'Determines how many stacks of the spell the character has.',
  `remaincharges` int unsigned NOT NULL DEFAULT '0' COMMENT 'The number of charges remaining on the aura.',
  `basepoints0` int NOT NULL DEFAULT '0' COMMENT 'Primary effect.',
  `basepoints1` int NOT NULL DEFAULT '0' COMMENT 'Secondary effect (healing)',
  `basepoints2` int NOT NULL DEFAULT '0' COMMENT 'Secondary effect (damage)',
  `periodictime0` int unsigned NOT NULL DEFAULT '0' COMMENT 'Primary effect over time counter.',
  `periodictime1` int unsigned NOT NULL DEFAULT '0' COMMENT 'Primary effect (healing) over time counter.',
  `periodictime2` int unsigned NOT NULL DEFAULT '0' COMMENT 'Primary effect (damage) over time counter.',
  `maxduration` int NOT NULL DEFAULT '0' COMMENT 'The maximum duration of the aura.',
  `remaintime` int NOT NULL DEFAULT '0' COMMENT 'The time remaining in seconds on the aura. -1 means that the aura is indefinite.',
  `effIndexMask` int unsigned NOT NULL DEFAULT '0' COMMENT 'The effect index of the spell from which the aura came from.',
  PRIMARY KEY (`guid`,`caster_guid`,`item_guid`,`spell`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Player System';

DELETE FROM `character_aura`;
/*!40000 ALTER TABLE `character_aura` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_aura` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
