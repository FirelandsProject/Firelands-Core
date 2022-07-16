-- MySQL dump 10.13  Distrib 8.0.29, for macos12 (x86_64)
--
-- Host: 127.0.0.1    Database: world
-- ------------------------------------------------------
-- Server version	8.0.29

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `areatrigger_scripts`
--

DROP TABLE IF EXISTS `areatrigger_scripts`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `areatrigger_scripts` (
  `entry` int NOT NULL,
  `ScriptName` varchar(64) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `areatrigger_scripts`
--

LOCK TABLES `areatrigger_scripts` WRITE;
/*!40000 ALTER TABLE `areatrigger_scripts` DISABLE KEYS */;
INSERT INTO `areatrigger_scripts` VALUES (255,'at_sfk_gargoyle_event'),(256,'at_sfk_gargoyle_event'),(822,'at_map_chamber'),(962,'at_zumrah'),(1447,'SmartTrigger'),(1526,'at_ring_of_law'),(1726,'at_scent_larkorwi'),(1727,'at_scent_larkorwi'),(1728,'at_scent_larkorwi'),(1729,'at_scent_larkorwi'),(1730,'at_scent_larkorwi'),(1731,'at_scent_larkorwi'),(1732,'at_scent_larkorwi'),(1733,'at_scent_larkorwi'),(1734,'at_scent_larkorwi'),(1735,'at_scent_larkorwi'),(1736,'at_scent_larkorwi'),(1737,'at_scent_larkorwi'),(1738,'at_scent_larkorwi'),(1739,'at_scent_larkorwi'),(1740,'at_scent_larkorwi'),(1946,'at_nearby_scarshield_infiltrator'),(2026,'at_blackrock_stadium'),(2046,'at_dragonspire_hall'),(3066,'SmartTrigger'),(3546,'at_bring_your_orphan_to'),(3547,'at_bring_your_orphan_to'),(3548,'at_bring_your_orphan_to'),(3549,'at_bring_your_orphan_to'),(3551,'at_bring_your_orphan_to'),(3552,'at_bring_your_orphan_to'),(3746,'SmartTrigger'),(3766,'SmartTrigger'),(4016,'at_malfurion_stormrage'),(4017,'at_twilight_grove'),(4112,'at_kelthuzad_center'),(4113,'at_thaddius_entrance'),(4115,'at_faerlina_entrance'),(4119,'at_anubrekhan_entrance'),(4356,'at_bring_your_orphan_to'),(4422,'at_area_52_entrance'),(4466,'at_area_52_entrance'),(4471,'at_area_52_entrance'),(4472,'at_area_52_entrance'),(4479,'SmartTrigger'),(4497,'at_commander_dawnforge'),(4498,'SmartTrigger'),(4524,'at_nethekurse_exit'),(4560,'at_legion_teleporter'),(4591,'at_coilfang_waterfall'),(4665,'at_teron_gorefiend_entrance'),(4752,'at_nats_landing'),(4769,'SmartTrigger'),(4770,'SmartTrigger'),(4772,'SmartTrigger'),(4774,'SmartTrigger'),(4801,'SmartTrigger'),(4802,'SmartTrigger'),(4803,'SmartTrigger'),(4804,'SmartTrigger'),(4820,'at_brewfest'),(4829,'at_brewfest'),(4838,'SmartTrigger'),(4857,'SmartTrigger'),(4858,'SmartTrigger'),(4860,'SmartTrigger'),(4871,'SmartTrigger'),(4872,'SmartTrigger'),(4873,'SmartTrigger'),(4950,'SmartTrigger'),(4951,'SmartTrigger'),(4956,'SmartTrigger'),(4960,'at_nearby_messenger_torvus'),(4987,'SmartTrigger'),(4990,'SmartTrigger'),(4991,'at_skadi_gaunlet'),(5030,'SmartTrigger'),(5046,'at_sholazar_waygate'),(5047,'at_sholazar_waygate'),(5051,'SmartTrigger'),(5056,'SmartTrigger'),(5057,'SmartTrigger'),(5058,'SmartTrigger'),(5059,'SmartTrigger'),(5060,'SmartTrigger'),(5061,'SmartTrigger'),(5079,'SmartTrigger'),(5080,'SmartTrigger'),(5082,'SmartTrigger'),(5083,'SmartTrigger'),(5084,'SmartTrigger'),(5095,'SmartTrigger'),(5096,'SmartTrigger'),(5097,'SmartTrigger'),(5098,'SmartTrigger'),(5108,'at_stormwright_shelf'),(5173,'at_frostgrips_hollow'),(5187,'SmartTrigger'),(5190,'SmartTrigger'),(5284,'SmartTrigger'),(5285,'SmartTrigger'),(5286,'SmartTrigger'),(5287,'SmartTrigger'),(5291,'at_stratholme_inn_stairs_cot'),(5332,'SmartTrigger'),(5334,'SmartTrigger'),(5338,'SmartTrigger'),(5339,'SmartTrigger'),(5340,'SmartTrigger'),(5580,'at_pit_cavern_entrance'),(5581,'at_pit_cavern_end'),(5604,'at_sindragosa_lair'),(5605,'at_hor_shadow_throne'),(5616,'at_icc_start_frostwing_gauntlet'),(5617,'at_icc_start_frostwing_gauntlet'),(5618,'at_icc_start_frostwing_gauntlet'),(5632,'at_hor_intro_start'),(5633,'at_tyrannus_event_starter'),(5649,'at_icc_shutdown_traps'),(5650,'SmartTrigger'),(5660,'at_hor_uther_quel_delar_start'),(5697,'at_hor_waves_restarter'),(5698,'at_icc_saurfang_portal'),(5703,'SmartTrigger'),(5704,'SmartTrigger'),(5705,'SmartTrigger'),(5706,'SmartTrigger'),(5708,'at_blood_prince_council_start_intro'),(5709,'at_lady_deathwhisper_entrance'),(5710,'SmartTrigger'),(5711,'SmartTrigger'),(5712,'SmartTrigger'),(5714,'SmartTrigger'),(5715,'SmartTrigger'),(5716,'SmartTrigger'),(5718,'at_frozen_throne_teleport'),(5732,'at_lord_marrowgar_entrance'),(5740,'at_hor_impenetrable_door'),(5806,'at_tott_first_invader_event'),(5811,'at_hoo_brann_idle_emote'),(5834,'at_tott_lady_nazjar_gauntlet'),(5867,'at_baltharus_plateau'),(5873,'at_tott_commander_ulthok_summon'),(5876,'at_mh_hyjal_barrow_dens'),(5983,'SmartTrigger'),(5987,'at_westfall_small_time_hustler'),(5988,'at_westfall_two_shoed_lou_thugs'),(5989,'SmartTrigger'),(5993,'SmartTrigger'),(5994,'SmartTrigger'),(6014,'at_raz_corla_event'),(6016,'at_raz_obsidius_event'),(6017,'at_tott_tentacle_knockback'),(6018,'at_tott_tentacle_knockback'),(6033,'SmartTrigger'),(6070,'at_sc_slabhide_intro'),(6076,'at_sc_corborus_intro'),(6135,'SmartTrigger'),(6274,'at_totfw_jet_stream'),(6275,'at_totfw_jet_stream'),(6276,'at_totfw_jet_stream'),(6277,'at_totfw_jet_stream'),(6278,'at_totfw_jet_stream'),(6279,'at_totfw_jet_stream'),(6280,'at_totfw_jet_stream'),(6281,'at_totfw_catch_fall'),(6290,'at_totfw_jet_stream'),(6422,'at_sfk_baron_silverlaine_post_floor'),(6424,'at_sfk_outside_ivar_bloodfang'),(6425,'at_sfk_outside_troups'),(6427,'SmartTrigger'),(6431,'at_sfk_godfrey_intro'),(6437,'at_halfus_wyrmbreaker_intro'),(6442,'at_theralion_and_valiona_intro'),(6444,'at_chogall_intro'),(6454,'SmartTrigger'),(6455,'SmartTrigger'),(6477,'at_vp_catch_fall'),(6583,'at_bwd_intro'),(6593,'at_brc_corla_quest'),(6594,'at_brc_karsh_quest'),(6597,'at_brc_beauty_quest'),(6601,'at_brc_obsidius_quest'),(6625,'at_ascendant_council_intro_1'),(6626,'at_ascendant_council_intro_2'),(6627,'at_ascendant_council_intro_3'),(6646,'at_tott_neptulon_intro'),(6651,'at_vp_catch_fall'),(6652,'at_vp_catch_fall'),(6660,'at_vp_catch_fall'),(6661,'at_vp_catch_fall'),(6663,'at_vp_catch_fall'),(6667,'at_vp_catch_fall'),(6845,'at_fl_ragnaros_spawn'),(7110,'at_madness_of_deathwing_carrying_winds_jump'),(7111,'at_madness_of_deathwing_carrying_winds_jump'),(7112,'at_madness_of_deathwing_carrying_winds_jump'),(7113,'at_madness_of_deathwing_carrying_winds_jump'),(7114,'at_madness_of_deathwing_carrying_winds_jump'),(7115,'at_madness_of_deathwing_carrying_winds_jump'),(7246,'at_alizabal_intro'),(7315,'at_madness_of_deathwing_carrying_winds_land'),(7316,'at_madness_of_deathwing_carrying_winds_land'),(7317,'at_madness_of_deathwing_carrying_winds_land'),(7318,'at_madness_of_deathwing_carrying_winds_land'),(7319,'at_madness_of_deathwing_carrying_winds_land'),(7320,'at_madness_of_deathwing_carrying_winds_land'),(7344,'AreaTrigger_at_darkmoon_island_gaming_area'),(7387,'SmartTrigger');
/*!40000 ALTER TABLE `areatrigger_scripts` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-07-15 10:34:17
