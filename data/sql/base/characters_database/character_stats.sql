/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `character_stats`;
CREATE TABLE IF NOT EXISTS `character_stats` (
  `guid` int unsigned NOT NULL DEFAULT '0' COMMENT 'The guid of the character. (See character.guid).',
  `maxhealth` int unsigned NOT NULL DEFAULT '0' COMMENT 'Maximum amount of health that the character has.',
  `maxpower1` int unsigned NOT NULL DEFAULT '0' COMMENT 'Max Mana',
  `maxpower2` int unsigned NOT NULL DEFAULT '0' COMMENT 'Max Rage',
  `maxpower3` int unsigned NOT NULL DEFAULT '0' COMMENT 'Max Focus',
  `maxpower4` int unsigned NOT NULL DEFAULT '0' COMMENT 'Max Energy',
  `maxpower5` int unsigned NOT NULL DEFAULT '0' COMMENT 'Max Happiness',
  `strength` int unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current strength value.',
  `agility` int unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current agility value',
  `stamina` int unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current stamina value.',
  `intellect` int unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current intellect value.',
  `spirit` int unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current spirit value.',
  `armor` int unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current armor value.',
  `resHoly` int unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current holy resistance value.',
  `resFire` int unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current fire resistance value.',
  `resNature` int unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current nature resistance value.',
  `resFrost` int unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current frost resistance value.',
  `resShadow` int unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current shadow resistance value.',
  `resArcane` int unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current arcane resistance value.',
  `blockPct` float unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current block chance',
  `dodgePct` float unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current dodge chance.',
  `parryPct` float unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current parry chance.',
  `critPct` float unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current crit chance.',
  `rangedCritPct` float unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current ranged crit chance.',
  `spellCritPct` float unsigned NOT NULL DEFAULT '0',
  `attackPower` int unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current attackpower.',
  `rangedAttackPower` int unsigned NOT NULL DEFAULT '0' COMMENT 'Character''s current ranged attackpower.',
  `spellPower` int unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;

DELETE FROM `character_stats`;
/*!40000 ALTER TABLE `character_stats` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_stats` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
