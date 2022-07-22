/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `characters`;
CREATE TABLE IF NOT EXISTS `characters` (
  `guid` int unsigned NOT NULL DEFAULT '0' COMMENT 'The character global unique identifier.',
  `account` int unsigned NOT NULL DEFAULT '0' COMMENT 'The account ID in which this character resides (See account.id) in the realm db.',
  `name` varchar(12) NOT NULL DEFAULT '' COMMENT 'The name of the character.',
  `race` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'The race of the character.',
  `class` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'The Class Id of the character (See chrclasses.dbc).',
  `gender` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'The Sex/Gender of the character.',
  `level` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'The current level of the designated player.',
  `xp` int unsigned NOT NULL DEFAULT '0' COMMENT 'The total amount of xp that the signified player has.',
  `money` bigint unsigned NOT NULL DEFAULT '0' COMMENT 'This is the amount of copper the character possesses.',
  `playerBytes` int unsigned NOT NULL DEFAULT '0' COMMENT 'This defines the physical attributes of the character.',
  `playerBytes2` int unsigned NOT NULL DEFAULT '0' COMMENT 'This defines the facial hair physical attribute of the character.',
  `playerFlags` int unsigned NOT NULL DEFAULT '0' COMMENT 'These are the player flags, such as GM mode on or off.',
  `position_x` float NOT NULL DEFAULT '0' COMMENT 'The x position of the character''s location.',
  `position_y` float NOT NULL DEFAULT '0' COMMENT 'The y position of the character''s location.',
  `position_z` float NOT NULL DEFAULT '0' COMMENT 'The z position of the character''s location.',
  `map` int unsigned NOT NULL DEFAULT '0' COMMENT 'The map ID the character is in (See maps.dbc)',
  `dungeon_difficulty` tinyint unsigned NOT NULL DEFAULT '0',
  `orientation` float NOT NULL DEFAULT '0' COMMENT 'The orientation the character is facing. (North = 0.0, South = 3.14159)',
  `taximask` longtext,
  `online` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'Records whether the character is online (1) or offline (0).',
  `cinematic` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'Boolean 1 or 0 controlling whether the start cinematic has been shown or not.',
  `totaltime` int unsigned NOT NULL DEFAULT '0' COMMENT 'The total time that the character has been active in the world.',
  `leveltime` int unsigned NOT NULL DEFAULT '0' COMMENT 'The total time the character has spent in the world at the current level.',
  `logout_time` bigint unsigned NOT NULL DEFAULT '0' COMMENT 'The time when the character last logged out, measured in Unix time.',
  `is_logout_resting` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'Boolean 1 or 0 controlling if the character is currently in a resting zone.',
  `rest_bonus` float NOT NULL DEFAULT '0' COMMENT 'This is the rest bonus for the character.',
  `resettalents_cost` int unsigned NOT NULL DEFAULT '0' COMMENT 'The cost for the character to reset its talents, measured in copper.',
  `resettalents_time` bigint unsigned NOT NULL DEFAULT '0',
  `primary_trees` varchar(10) NOT NULL DEFAULT '0 0 ',
  `trans_x` float NOT NULL DEFAULT '0' COMMENT 'The X coordinate of the character on the transport it is travelling on.',
  `trans_y` float NOT NULL DEFAULT '0' COMMENT 'The Y coordinate of the character on the transport it is travelling on.',
  `trans_z` float NOT NULL DEFAULT '0' COMMENT 'The Z coordinate of the character on the transport it is travelling on.',
  `trans_o` float NOT NULL DEFAULT '0' COMMENT 'The orientation of the character on the transport it is travelling on.',
  `transguid` bigint unsigned NOT NULL DEFAULT '0' COMMENT 'This is the transprt the character is currently travelling on.',
  `extra_flags` int unsigned NOT NULL DEFAULT '0' COMMENT 'These flags control certain player specific attributes, mostly GM features.',
  `stable_slots` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'The number of stable slots the player has available. Maximum is 2.',
  `at_login` int unsigned NOT NULL DEFAULT '0' COMMENT 'The status of the character.',
  `zone` int unsigned NOT NULL DEFAULT '0' COMMENT 'The zone ID the character is in.',
  `death_expire_time` bigint unsigned NOT NULL DEFAULT '0' COMMENT 'Time when a character can be resurrected.',
  `taxi_path` text COMMENT 'Stores the players current taxi path (TaxiPath.dbc) if logged off while on one.',
  `totalKills` int unsigned NOT NULL DEFAULT '0',
  `todayKills` smallint unsigned NOT NULL DEFAULT '0',
  `yesterdayKills` smallint unsigned NOT NULL DEFAULT '0',
  `chosenTitle` int unsigned NOT NULL DEFAULT '0',
  `watchedFaction` int unsigned NOT NULL DEFAULT '0' COMMENT 'The faction whose rep is being followed on the experience bar.',
  `drunk` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'This represents the alcoholic strength of the drink.',
  `health` int unsigned NOT NULL DEFAULT '0' COMMENT 'The character''s health when logging out.',
  `power1` int unsigned NOT NULL DEFAULT '0' COMMENT 'If a mana user, then this is a character''s mana level when logging out.',
  `power2` int unsigned NOT NULL DEFAULT '0' COMMENT 'If a warrior, then this is a character''s rage level when logging out.',
  `power3` int unsigned NOT NULL DEFAULT '0' COMMENT 'This is a hunter pet''s focus level.',
  `power4` int unsigned NOT NULL DEFAULT '0' COMMENT 'If a rogue, then this is a character''s energy level when logging out.',
  `power5` int unsigned NOT NULL DEFAULT '0' COMMENT 'This is the current active pet''s happiness level.',
  `specCount` tinyint unsigned NOT NULL DEFAULT '1',
  `activeSpec` tinyint unsigned NOT NULL DEFAULT '0',
  `exploredZones` longtext COMMENT 'This is a record of all areas discovered by the character.',
  `equipmentCache` longtext COMMENT 'This is a record of all items that are currently equipped.',
  `knownTitles` longtext,
  `actionBars` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'This represents which action bars are currently active.',
  `slot` tinyint unsigned NOT NULL DEFAULT '255',
  `deleteInfos_Account` int unsigned DEFAULT NULL COMMENT 'This is the account number from the characters table.',
  `deleteInfos_Name` varchar(12) DEFAULT NULL COMMENT 'The is the name of the character being deleted.',
  `deleteDate` bigint unsigned DEFAULT NULL COMMENT 'This is the date the character was deleted.',
  `createdDate` int unsigned DEFAULT NULL COMMENT 'This is the character creation date',
  PRIMARY KEY (`guid`),
  KEY `idx_account` (`account`),
  KEY `idx_online` (`online`),
  KEY `idx_name` (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Player System';

DELETE FROM `characters`;
/*!40000 ALTER TABLE `characters` DISABLE KEYS */;
/*!40000 ALTER TABLE `characters` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
