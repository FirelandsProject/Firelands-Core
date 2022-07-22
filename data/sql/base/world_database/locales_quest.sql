/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `locales_quest`;
CREATE TABLE IF NOT EXISTS `locales_quest` (
  `entry` mediumint unsigned NOT NULL DEFAULT '0' COMMENT 'Reference to quest_template.entry.',
  `Title_loc1` text COMMENT 'Korean localization of Title in the quest_template table.',
  `Title_loc2` text COMMENT 'French localization of Title in the quest_template table.',
  `Title_loc3` text COMMENT 'German localization of Title in the quest_template table.',
  `Title_loc4` text COMMENT 'Chinese localization of Title in the quest_template table.',
  `Title_loc5` text COMMENT 'Taiwanese localization of Title in the quest_template table.',
  `Title_loc6` text COMMENT 'Spanish Spain localization of Title in the quest_template table.',
  `Title_loc7` text COMMENT 'Spanish Latin America localization of Title.',
  `Title_loc8` text COMMENT 'Russian localization of Title in the quest_template table.',
  `Title_loc9` longtext,
  `Title_loc10` longtext,
  `Title_loc11` longtext,
  `Details_loc1` text COMMENT 'Korean localization of Details in the quest_template table.',
  `Details_loc2` text COMMENT 'French localization of Details in the quest_template table.',
  `Details_loc3` text COMMENT 'German localization of Details in the quest_template table.',
  `Details_loc4` text COMMENT 'Chinese localization of Details in the quest_template table.',
  `Details_loc5` text COMMENT 'Taiwanese localization of Details in the quest_template table.',
  `Details_loc6` text COMMENT 'Spanish Spain localization of Details in the quest_template table.',
  `Details_loc7` text COMMENT 'Spanish Latin America localization of Details.',
  `Details_loc8` text COMMENT 'Russian localization of Details in the quest_template table.',
  `Details_loc9` longtext,
  `Details_loc10` longtext,
  `Details_loc11` longtext,
  `Objectives_loc1` text COMMENT 'Korean localization of Objectives in the quest_template table.',
  `Objectives_loc2` text COMMENT 'French localization of Objectives in the quest_template table.',
  `Objectives_loc3` text COMMENT 'German localization of Objectives in the quest_template table.',
  `Objectives_loc4` text COMMENT 'Chinese localization of Objectives in the quest_template table.',
  `Objectives_loc5` text COMMENT 'Taiwanese localization of Objectives in the quest_template table.',
  `Objectives_loc6` text COMMENT 'Spanish Spain localization of Objectives in the quest_template table.',
  `Objectives_loc7` text COMMENT 'Spanish Latin America localization of Objectives.',
  `Objectives_loc8` text COMMENT 'Russian localization of Objectives in the quest_template table.',
  `Objectives_loc9` longtext,
  `Objectives_loc10` longtext,
  `Objectives_loc11` longtext,
  `OfferRewardText_loc1` text COMMENT 'Korean localization of OfferRewardText in the quest_template table.',
  `OfferRewardText_loc2` text COMMENT 'French localization of OfferRewardText in the quest_template table.',
  `OfferRewardText_loc3` text COMMENT 'German localization of OfferRewardText in the quest_template table.',
  `OfferRewardText_loc4` text COMMENT 'Chinese localization of OfferRewardText in the quest_template table.',
  `OfferRewardText_loc5` text COMMENT 'Taiwanese localization of OfferRewardText in the quest_template table.',
  `OfferRewardText_loc6` text COMMENT 'Spanish Spain localization of OfferRewardText.',
  `OfferRewardText_loc7` text COMMENT 'Spanish Latin America localization of OfferRewardText.',
  `OfferRewardText_loc8` text COMMENT 'Russian localization of OfferRewardText in the quest_template table.',
  `OfferRewardText_loc9` longtext,
  `OfferRewardText_loc10` longtext,
  `OfferRewardText_loc11` longtext,
  `RequestItemsText_loc1` text COMMENT 'Korean localization of RequestItemsText in the quest_template table.',
  `RequestItemsText_loc2` text COMMENT 'French localization of RequestItemsText in the quest_template table.',
  `RequestItemsText_loc3` text COMMENT 'German localization of RequestItemsText in the quest_template table.',
  `RequestItemsText_loc4` text COMMENT 'Chinese localization of RequestItemsText in the quest_template table.',
  `RequestItemsText_loc5` text COMMENT 'Taiwanese localization of RequestItemsText in the quest_template table.',
  `RequestItemsText_loc6` text COMMENT 'Spanish Spain localization of RequestItemsText in the quest_template table.',
  `RequestItemsText_loc7` text COMMENT 'Spanish Latin America localization of RequestItemsText.',
  `RequestItemsText_loc8` text COMMENT 'Russian localization of RequestItemsText in the quest_template table.',
  `RequestItemsText_loc9` longtext,
  `RequestItemsText_loc10` longtext,
  `RequestItemsText_loc11` longtext,
  `EndText_loc1` text COMMENT 'Korean localization of EndText in the quest_template table.',
  `EndText_loc2` text COMMENT 'French localization of EndText in the quest_template table.',
  `EndText_loc3` text COMMENT 'German localization of EndText in the quest_template table.',
  `EndText_loc4` text COMMENT 'Chinese localization of EndText in the quest_template table.',
  `EndText_loc5` text COMMENT 'Taiwanese localization of EndText in the quest_template table.',
  `EndText_loc6` text COMMENT 'Spanish Spain localization of EndText in the quest_template table.',
  `EndText_loc7` text COMMENT 'Spanish Latin America localization of EndText.',
  `EndText_loc8` text COMMENT 'Russian localization of EndText in the quest_template table.',
  `EndText_loc9` longtext,
  `EndText_loc10` longtext,
  `EndText_loc11` longtext,
  `CompletedText_loc1` text,
  `CompletedText_loc2` text,
  `CompletedText_loc3` text,
  `CompletedText_loc4` text,
  `CompletedText_loc5` text,
  `CompletedText_loc6` text,
  `CompletedText_loc7` text,
  `CompletedText_loc8` text,
  `CompletedText_loc9` text,
  `CompletedText_loc10` text,
  `CompletedText_loc11` text,
  `ObjectiveText1_loc1` text COMMENT 'Korean localization of ObjectiveText1 in the quest_template table.',
  `ObjectiveText1_loc2` text COMMENT 'French localization of ObjectiveText1 in the quest_template table.',
  `ObjectiveText1_loc3` text COMMENT 'German localization of ObjectiveText1 in the quest_template table.',
  `ObjectiveText1_loc4` text COMMENT 'Chinese localization of ObjectiveText1 in the quest_template table.',
  `ObjectiveText1_loc5` text COMMENT 'Taiwanese localization of ObjectiveText1 in the quest_template table.',
  `ObjectiveText1_loc6` text COMMENT 'Spanish Spain localization of ObjectiveText1 in the quest_template table.',
  `ObjectiveText1_loc7` text COMMENT 'Spanish Latin America localization of ObjectiveText1.',
  `ObjectiveText1_loc8` text COMMENT 'Russian localization of ObjectiveText1 in the quest_template table.',
  `ObjectiveText1_loc9` longtext,
  `ObjectiveText1_loc10` longtext,
  `ObjectiveText1_loc11` longtext,
  `ObjectiveText2_loc1` text COMMENT 'Korean localization of ObjectiveText2 in the quest_template table.',
  `ObjectiveText2_loc2` text COMMENT 'French localization of ObjectiveText2 in the quest_template table.',
  `ObjectiveText2_loc3` text COMMENT 'German localization of ObjectiveText2 in the quest_template table.',
  `ObjectiveText2_loc4` text COMMENT 'Chinese localization of ObjectiveText2 in the quest_template table.',
  `ObjectiveText2_loc5` text COMMENT 'Taiwanese localization of ObjectiveText2 in the quest_template table.',
  `ObjectiveText2_loc6` text COMMENT 'Spanish Spain localization of ObjectiveText2 in the quest_template table.',
  `ObjectiveText2_loc7` text COMMENT 'Spanish Latin America localization of ObjectiveText2.',
  `ObjectiveText2_loc8` text COMMENT 'Russian localization of ObjectiveText2 in the quest_template table.',
  `ObjectiveText2_loc9` longtext,
  `ObjectiveText2_loc10` longtext,
  `ObjectiveText2_loc11` longtext,
  `ObjectiveText3_loc1` text COMMENT 'Korean localization of ObjectiveText3 in the quest_template table.',
  `ObjectiveText3_loc2` text COMMENT 'French localization of ObjectiveText3 in the quest_template table.',
  `ObjectiveText3_loc3` text COMMENT 'German localization of ObjectiveText3 in the quest_template table.',
  `ObjectiveText3_loc4` text COMMENT 'Chinese localization of ObjectiveText3 in the quest_template table.',
  `ObjectiveText3_loc5` text COMMENT 'Taiwanese localization of ObjectiveText3 in the quest_template table.',
  `ObjectiveText3_loc6` text COMMENT 'Spanish Spain localization of ObjectiveText3 in the quest_template table.',
  `ObjectiveText3_loc7` text COMMENT 'Spanish Latin America localization of ObjectiveText3.',
  `ObjectiveText3_loc8` text COMMENT 'Russian localization of ObjectiveText3 in the quest_template table.',
  `ObjectiveText3_loc9` longtext,
  `ObjectiveText3_loc10` longtext,
  `ObjectiveText3_loc11` longtext,
  `ObjectiveText4_loc1` text COMMENT 'Korean localization of ObjectiveText4 in the quest_template table.',
  `ObjectiveText4_loc2` text COMMENT 'French localization of ObjectiveText4 in the quest_template table.',
  `ObjectiveText4_loc3` text COMMENT 'German localization of ObjectiveText4 in the quest_template table.',
  `ObjectiveText4_loc4` text COMMENT 'Chinese localization of ObjectiveText4 in the quest_template table.',
  `ObjectiveText4_loc5` text COMMENT 'Taiwanese localization of ObjectiveText4 in the quest_template table.',
  `ObjectiveText4_loc6` text COMMENT 'Spanish Spain localization of ObjectiveText4 in the quest_template table.',
  `ObjectiveText4_loc7` text COMMENT 'Spanish Latin America localization of ObjectiveText4.',
  `ObjectiveText4_loc8` text COMMENT 'Russian localization of ObjectiveText4 in the quest_template table.',
  `ObjectiveText4_loc9` longtext,
  `ObjectiveText4_loc10` longtext,
  `ObjectiveText4_loc11` longtext,
  `PortraitGiverName_loc1` text,
  `PortraitGiverName_loc2` text,
  `PortraitGiverName_loc3` text,
  `PortraitGiverName_loc4` text,
  `PortraitGiverName_loc5` text,
  `PortraitGiverName_loc6` text,
  `PortraitGiverName_loc7` text,
  `PortraitGiverName_loc8` text,
  `PortraitGiverName_loc9` longtext,
  `PortraitGiverName_loc10` longtext,
  `PortraitGiverName_loc11` longtext,
  `PortraitGiverText_loc1` text,
  `PortraitGiverText_loc2` text,
  `PortraitGiverText_loc3` text,
  `PortraitGiverText_loc4` text,
  `PortraitGiverText_loc5` text,
  `PortraitGiverText_loc6` text,
  `PortraitGiverText_loc7` text,
  `PortraitGiverText_loc8` text,
  `PortraitGiverText_loc9` longtext,
  `PortraitGiverText_loc10` longtext,
  `PortraitGiverText_loc11` longtext,
  `PortraitTurnInName_loc1` text,
  `PortraitTurnInName_loc2` text,
  `PortraitTurnInName_loc3` text,
  `PortraitTurnInName_loc4` text,
  `PortraitTurnInName_loc5` text,
  `PortraitTurnInName_loc6` text,
  `PortraitTurnInName_loc7` text,
  `PortraitTurnInName_loc8` text,
  `PortraitTurnInName_loc9` longtext,
  `PortraitTurnInName_loc10` longtext,
  `PortraitTurnInName_loc11` longtext,
  `PortraitTurnInText_loc1` text,
  `PortraitTurnInText_loc2` text,
  `PortraitTurnInText_loc3` text,
  `PortraitTurnInText_loc4` text,
  `PortraitTurnInText_loc5` text,
  `PortraitTurnInText_loc6` text,
  `PortraitTurnInText_loc7` text,
  `PortraitTurnInText_loc8` text,
  `PortraitTurnInText_loc9` longtext,
  `PortraitTurnInText_loc10` longtext,
  `PortraitTurnInText_loc11` longtext,
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC;

DELETE FROM `locales_quest`;
/*!40000 ALTER TABLE `locales_quest` DISABLE KEYS */;
/*!40000 ALTER TABLE `locales_quest` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
