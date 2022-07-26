/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `character_aura`;
CREATE TABLE IF NOT EXISTS `character_aura` (
  `guid` int unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `caster_guid` bigint unsigned NOT NULL DEFAULT '0' COMMENT 'Full Global Unique Identifier',
  `item_guid` bigint unsigned NOT NULL DEFAULT '0',
  `spell` mediumint unsigned NOT NULL DEFAULT '0',
  `effect_mask` tinyint unsigned NOT NULL DEFAULT '0',
  `recalculate_mask` tinyint unsigned NOT NULL DEFAULT '0',
  `stackcount` tinyint unsigned NOT NULL DEFAULT '1',
  `amount0` int NOT NULL DEFAULT '0',
  `amount1` int NOT NULL DEFAULT '0',
  `amount2` int NOT NULL DEFAULT '0',
  `base_amount0` int NOT NULL DEFAULT '0',
  `base_amount1` int NOT NULL DEFAULT '0',
  `base_amount2` int NOT NULL DEFAULT '0',
  `maxduration` int NOT NULL DEFAULT '0',
  `remaintime` int NOT NULL DEFAULT '0',
  `remaincharges` tinyint unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`caster_guid`,`item_guid`,`spell`,`effect_mask`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 COMMENT='Player System';

DELETE FROM `character_aura`;
/*!40000 ALTER TABLE `character_aura` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_aura` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
