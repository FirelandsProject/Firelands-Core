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
-- Table structure for table `milling_loot_template`
--

DROP TABLE IF EXISTS `milling_loot_template`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `milling_loot_template` (
  `Entry` int unsigned NOT NULL DEFAULT '0',
  `Item` int unsigned NOT NULL DEFAULT '0',
  `Reference` int unsigned NOT NULL DEFAULT '0',
  `Chance` float NOT NULL DEFAULT '100',
  `QuestRequired` tinyint(1) NOT NULL DEFAULT '0',
  `IsCurrency` tinyint(1) NOT NULL DEFAULT '0',
  `LootMode` smallint unsigned NOT NULL DEFAULT '1',
  `GroupId` tinyint unsigned NOT NULL DEFAULT '0',
  `MinCount` tinyint unsigned NOT NULL DEFAULT '1',
  `MaxCount` tinyint unsigned NOT NULL DEFAULT '1',
  `Comment` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`Entry`,`Item`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='Loot System';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `milling_loot_template`
--

LOCK TABLES `milling_loot_template` WRITE;
/*!40000 ALTER TABLE `milling_loot_template` DISABLE KEYS */;
INSERT INTO `milling_loot_template` VALUES (765,11900,11900,100,0,0,1,0,1,1,NULL),(785,11901,11901,100,0,0,1,0,1,1,NULL),(2447,11900,11900,100,0,0,1,0,1,1,NULL),(2449,11900,11900,100,0,0,1,0,1,1,NULL),(2450,11902,11902,100,0,0,1,0,1,1,NULL),(2452,11902,11902,100,0,0,1,0,1,1,NULL),(2453,11903,11903,100,0,0,1,0,1,1,NULL),(3355,11904,11904,100,0,0,1,0,1,1,NULL),(3356,11905,11905,100,0,0,1,0,1,1,NULL),(3357,11905,11905,100,0,0,1,0,1,1,NULL),(3358,11907,11907,100,0,0,1,0,1,1,NULL),(3369,11904,11904,100,0,0,1,0,1,1,NULL),(3818,11906,11906,100,0,0,1,0,1,1,NULL),(3819,11907,11907,100,0,0,1,0,1,1,NULL),(3820,11903,11903,100,0,0,1,0,1,1,NULL),(3821,11906,11906,100,0,0,1,0,1,1,NULL),(4625,11908,11908,100,0,0,1,0,1,1,NULL),(8831,11908,11908,100,0,0,1,0,1,1,NULL),(8836,11908,11908,100,0,0,1,0,1,1,NULL),(8838,11908,11908,100,0,0,1,0,1,1,NULL),(8839,11909,11909,100,0,0,1,0,1,1,NULL),(8845,11909,11909,100,0,0,1,0,1,1,NULL),(8846,11909,11909,100,0,0,1,0,1,1,NULL),(13463,11910,11910,100,0,0,1,0,1,1,NULL),(13464,11910,11910,100,0,0,1,0,1,1,NULL),(13465,11911,11911,100,0,0,1,0,1,1,NULL),(13466,11911,11911,100,0,0,1,0,1,1,NULL),(13467,11911,11911,100,0,0,1,0,1,1,NULL),(22785,11913,11913,100,0,0,1,0,1,1,NULL),(22786,11912,11912,100,0,0,1,0,1,1,NULL),(22787,11912,11912,100,0,0,1,0,1,1,NULL),(22789,11912,11912,100,0,0,1,0,1,1,NULL),(22790,11914,11914,100,0,0,1,0,1,1,NULL),(22791,11914,11914,100,0,0,1,0,1,1,NULL),(22792,11914,11914,100,0,0,1,0,1,1,NULL),(22793,11914,11914,100,0,0,1,0,1,1,NULL),(36901,11916,11916,100,0,0,1,0,1,1,NULL),(36903,11918,11918,100,0,0,1,0,1,1,NULL),(36904,11916,11916,100,0,0,1,0,1,1,NULL),(36905,11917,11917,100,0,0,1,0,1,1,NULL),(36906,11917,11917,100,0,0,1,0,1,1,NULL),(36907,11915,11915,100,0,0,1,0,1,1,NULL),(37921,11916,11916,100,0,0,1,0,1,1,NULL),(39969,11915,11915,100,0,0,1,0,1,1,NULL),(39970,11915,11915,100,0,0,1,0,1,1,NULL),(52983,11919,11919,100,0,0,1,0,1,1,'Cinderbloom - Ashen Pigment, Burning Embers'),(52984,11920,11920,100,0,0,1,0,1,1,'Stormvine - Ashen Pigment, Burning Embers'),(52985,11920,11920,100,0,0,1,0,1,1,'Azshara\'s Veil - Ashen Pigment, Burning Embers'),(52986,11919,11919,100,0,0,1,0,1,1,'Heartblossom - Ashen Pigment, Burning Embers'),(52987,11921,11921,100,0,0,1,0,1,1,'Twilight Jasmine - Ashen Pigment, Burning Embers'),(52988,11921,11921,100,0,0,1,0,1,1,'Whiptail - Ashen Pigment, Burning Embers');
/*!40000 ALTER TABLE `milling_loot_template` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-07-15 10:33:46
