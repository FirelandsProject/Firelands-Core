-- MySQL dump 10.13  Distrib 8.0.29, for macos12 (x86_64)
--
-- Host: 127.0.0.1    Database: characters
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
-- Table structure for table `worldstates`
--

DROP TABLE IF EXISTS `worldstates`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `worldstates` (
  `entry` int unsigned NOT NULL DEFAULT '0',
  `value` int unsigned NOT NULL DEFAULT '0',
  `comment` tinytext CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci,
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='Variable Saves';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `worldstates`
--

LOCK TABLES `worldstates` WRITE;
/*!40000 ALTER TABLE `worldstates` DISABLE KEYS */;
INSERT INTO `worldstates` VALUES (1,0,NULL),(2,0,NULL),(3,0,NULL),(4,0,NULL),(5,0,NULL),(6,0,NULL),(7,0,NULL),(8,0,NULL),(9,0,NULL),(10,0,NULL),(11,0,NULL),(12,0,NULL),(13,0,NULL),(14,0,NULL),(15,0,NULL),(16,0,NULL),(17,0,NULL),(18,0,NULL),(19,0,NULL),(20,0,NULL),(21,1657435111,NULL),(22,0,NULL),(23,0,NULL),(24,0,NULL),(25,0,NULL),(26,0,NULL),(27,0,NULL),(28,0,NULL),(29,0,NULL),(30,0,NULL),(31,0,NULL),(32,0,NULL),(33,1657444201,NULL),(34,0,NULL),(35,0,NULL),(36,0,NULL),(37,0,NULL),(38,0,NULL),(39,0,NULL),(40,0,NULL),(41,0,NULL),(42,0,NULL),(43,1657435111,NULL),(44,0,NULL),(45,0,NULL),(46,0,NULL),(47,0,NULL),(48,0,NULL),(49,0,NULL),(50,0,NULL),(51,0,NULL),(52,0,NULL),(53,0,NULL),(54,0,NULL),(55,0,NULL),(56,0,NULL),(57,0,NULL),(58,0,NULL),(59,0,NULL),(60,1657435111,NULL),(61,0,NULL),(62,0,NULL),(63,0,NULL),(64,0,NULL),(65,0,NULL),(66,0,NULL),(67,0,NULL),(68,0,NULL),(69,1657444201,NULL),(70,0,NULL),(71,0,NULL),(72,0,NULL),(73,0,NULL),(74,0,NULL),(75,0,NULL),(76,0,NULL),(77,0,NULL),(78,0,NULL),(79,0,NULL),(80,0,NULL),(81,0,NULL),(82,0,NULL),(83,0,NULL),(3781,9000000,NULL),(3801,0,NULL),(3802,1,NULL),(5332,600000,NULL),(5334,2,NULL),(5344,0,NULL),(5384,1,NULL),(5385,0,NULL),(5387,1,NULL),(5546,0,NULL),(5547,0,NULL),(20001,0,'NextArenaPointDistributionTime'),(20002,1657681200,'NextWeeklyQuestResetTime'),(20003,1657519200,'NextBGRandomDailyResetTime'),(20004,0,'cleaning_flags'),(20006,1657519200,'NextGuildDailyResetTime'),(20007,1659322800,'NextMonthlyQuestResetTime'),(20008,1657508400,'NextDailyQuestResetTime'),(20050,4,NULL),(20051,1657864800,NULL);
/*!40000 ALTER TABLE `worldstates` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-07-15 10:34:47
