/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `pet_aura`;
CREATE TABLE IF NOT EXISTS `pet_aura` (
  `guid` int unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the target affected by the aura. (See character.guid)',
  `caster_guid` bigint unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the player or creature who casted the aura. (See character.guid)',
  `item_guid` int unsigned NOT NULL DEFAULT '0' COMMENT 'Guid of the item that is casting the aura on the pet.',
  `spell` int unsigned NOT NULL DEFAULT '0' COMMENT 'The spell from which the aura was applied. (See Spell.dbc column 1)',
  `stackcount` int unsigned NOT NULL DEFAULT '1' COMMENT 'This is the total number of times the aura can be stacked on the pet.',
  `remaincharges` int unsigned NOT NULL DEFAULT '0' COMMENT 'The number of charges remaining on the aura.',
  `basepoints0` int NOT NULL DEFAULT '0' COMMENT 'Primary effect.',
  `basepoints1` int NOT NULL DEFAULT '0' COMMENT 'Secondary effect (appears to be only healing related)',
  `basepoints2` int NOT NULL DEFAULT '0' COMMENT 'Secondary effect (appears to be only damage related)',
  `periodictime0` int unsigned NOT NULL DEFAULT '0' COMMENT 'Primary effect over time.',
  `periodictime1` int unsigned NOT NULL DEFAULT '0' COMMENT 'Secondary effect (healing) over time',
  `periodictime2` int unsigned NOT NULL DEFAULT '0' COMMENT 'Secondary effect (damage) over time.',
  `maxduration` int NOT NULL DEFAULT '0' COMMENT 'The maximum duration of the aura.',
  `remaintime` int NOT NULL DEFAULT '0' COMMENT 'The time remaining in seconds on the aura. -1 means that the aura is indefinite.',
  `effIndexMask` int unsigned NOT NULL DEFAULT '0' COMMENT 'The effect index of the spell from which the aura came from.',
  PRIMARY KEY (`guid`,`caster_guid`,`item_guid`,`spell`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Pet System';

DELETE FROM `pet_aura`;
/*!40000 ALTER TABLE `pet_aura` DISABLE KEYS */;
/*!40000 ALTER TABLE `pet_aura` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
