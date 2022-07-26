/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `pet_aura`;
CREATE TABLE IF NOT EXISTS `pet_aura` (
  `guid` int unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `caster_guid` bigint unsigned NOT NULL DEFAULT '0' COMMENT 'Full Global Unique Identifier',
  `spell` mediumint unsigned NOT NULL DEFAULT '0',
  `effect_mask` tinyint unsigned NOT NULL DEFAULT '0',
  `recalculate_mask` tinyint unsigned NOT NULL DEFAULT '0',
  `stackcount` tinyint unsigned NOT NULL DEFAULT '1',
  `amount0` mediumint NOT NULL,
  `amount1` mediumint NOT NULL,
  `amount2` mediumint NOT NULL,
  `base_amount0` mediumint NOT NULL,
  `base_amount1` mediumint NOT NULL,
  `base_amount2` mediumint NOT NULL,
  `maxduration` int NOT NULL DEFAULT '0',
  `remaintime` int NOT NULL DEFAULT '0',
  `remaincharges` tinyint unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`spell`,`effect_mask`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 COMMENT='Pet System';

DELETE FROM `pet_aura`;
/*!40000 ALTER TABLE `pet_aura` DISABLE KEYS */;
/*!40000 ALTER TABLE `pet_aura` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
