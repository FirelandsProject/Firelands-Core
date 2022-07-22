CREATE DATABASE  IF NOT EXISTS `characters` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `characters`;
--
-- Table structure for table `character_pet`
--

DROP TABLE IF EXISTS `character_pet`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_pet` (
  `id` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The special pet ID. This is a unique identifier among all pets.',
  `entry` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The creature entry of this pet. (See creature_template.entry).',
  `owner` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The GUID of the pet''s owner. (See character.guid).',
  `modelid` int(11) unsigned DEFAULT '0' COMMENT 'The model ID to use to display the pet.',
  `CreatedBySpell` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The ID of the spell that has created this pet.',
  `PetType` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'The type of pet that this is. 0 = summoned pet, 1 = tamed pet',
  `level` int(11) unsigned NOT NULL DEFAULT '1' COMMENT 'The current level of the pet.',
  `exp` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The current experience that this pet has.',
  `Reactstate` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT 'The current reaction state of the pet (passive, aggressive, etc).',
  `name` varchar(100) DEFAULT 'Pet' COMMENT 'The pet''s name.',
  `renamed` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT 'Boolean 1 or 0. 1 = Pet has been renamed.',
  `slot` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The pet slot that the pet is in. The slot is a number between 0 and 3 inclusive.',
  `curhealth` int(11) unsigned NOT NULL DEFAULT '1' COMMENT 'The current pet health at the time it was saved to DB.',
  `curmana` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'The current pet mana at the time it was saved to DB.',
  `savetime` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'The time when the pet was last saved, in Unix time.',
  `resettalents_cost` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'This is the cost to untrain the pet talents.',
  `resettalents_time` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'How many times the pets talents have been reset.',
  `abdata` longtext COMMENT 'Data about pet action bar and action type ten pairs of action bar entry.',
  PRIMARY KEY (`id`),
  KEY `owner` (`owner`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Pet System';
/*!40101 SET character_set_client = @saved_cs_client */;
