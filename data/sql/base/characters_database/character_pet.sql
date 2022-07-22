/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `character_pet`;
CREATE TABLE IF NOT EXISTS `character_pet` (
  `id` int unsigned NOT NULL DEFAULT '0' COMMENT 'The special pet ID. This is a unique identifier among all pets.',
  `entry` int unsigned NOT NULL DEFAULT '0' COMMENT 'The creature entry of this pet. (See creature_template.entry).',
  `owner` int unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the pet''s owner. (See character.guid).',
  `modelid` int unsigned DEFAULT '0' COMMENT 'The model ID to use to display the pet.',
  `CreatedBySpell` int unsigned NOT NULL DEFAULT '0' COMMENT 'The ID of the spell that has created this pet.',
  `PetType` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'The type of pet that this is. 0 = summoned pet, 1 = tamed pet',
  `level` int unsigned NOT NULL DEFAULT '1' COMMENT 'The current level of the pet.',
  `exp` int unsigned NOT NULL DEFAULT '0' COMMENT 'The current experience that this pet has.',
  `Reactstate` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'The current reaction state of the pet (passive, aggressive, etc).',
  `name` varchar(100) DEFAULT 'Pet' COMMENT 'The pet''s name.',
  `renamed` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'Boolean 1 or 0. 1 = Pet has been renamed.',
  `slot` int unsigned NOT NULL DEFAULT '0' COMMENT 'The pet slot that the pet is in. The slot is a number between 0 and 3 inclusive.',
  `curhealth` int unsigned NOT NULL DEFAULT '1' COMMENT 'The current pet health at the time it was saved to DB.',
  `curmana` int unsigned NOT NULL DEFAULT '0' COMMENT 'The current pet mana at the time it was saved to DB.',
  `savetime` bigint unsigned NOT NULL DEFAULT '0' COMMENT 'The time when the pet was last saved, in Unix time.',
  `resettalents_cost` int unsigned NOT NULL DEFAULT '0' COMMENT 'This is the cost to untrain the pet talents.',
  `resettalents_time` bigint unsigned NOT NULL DEFAULT '0' COMMENT 'How many times the pets talents have been reset.',
  `abdata` longtext COMMENT 'Data about pet action bar and action type ten pairs of action bar entry.',
  PRIMARY KEY (`id`),
  KEY `owner` (`owner`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Pet System';

DELETE FROM `character_pet`;
/*!40000 ALTER TABLE `character_pet` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_pet` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
