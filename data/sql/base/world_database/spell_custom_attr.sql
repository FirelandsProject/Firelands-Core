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
-- Table structure for table `spell_custom_attr`
--

DROP TABLE IF EXISTS `spell_custom_attr`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `spell_custom_attr` (
  `entry` int unsigned NOT NULL DEFAULT '0' COMMENT 'spell id',
  `attributes` int unsigned NOT NULL DEFAULT '0' COMMENT 'SpellCustomAttributes',
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='SpellInfo custom attributes';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_custom_attr`
--

LOCK TABLES `spell_custom_attr` WRITE;
/*!40000 ALTER TABLE `spell_custom_attr` DISABLE KEYS */;
INSERT INTO `spell_custom_attr` VALUES (53,131072),(703,131072),(1130,64),(1725,64),(1776,65536),(3600,64),(5221,131072),(6785,131072),(6788,64),(7159,131072),(7268,4096),(8627,131072),(8676,131072),(11196,64),(12540,65536),(13579,65536),(15582,131072),(15657,131072),(18500,32768),(21987,131072),(22416,131072),(23959,131072),(24340,8),(24698,65536),(26029,4),(26558,8),(26789,8),(28456,65536),(28884,8),(29425,65536),(31436,8),(32375,64),(32592,64),(33086,32768),(34709,20480),(34940,65536),(35009,64),(35181,8),(36837,8),(36862,65536),(37433,4),(37685,131072),(38863,65536),(38903,8),(39897,64),(40810,8),(41276,8),(42384,8),(42650,64),(42772,524288),(43138,262144),(43140,4),(43215,4),(43263,64),(43264,64),(43267,8),(43268,8),(43419,262144),(45145,1048576),(45150,8),(47666,4096),(48642,32768),(49749,32768),(49882,32768),(52743,65536),(52890,32768),(53454,32768),(55550,524288),(56690,32768),(57467,8),(58563,131072),(58831,64),(58832,64),(58833,64),(58834,64),(58838,64),(59446,32768),(59685,524288),(60586,32768),(60776,32768),(60864,32768),(60881,32768),(62383,32768),(62544,32768),(62626,32768),(62709,32768),(62775,32768),(62874,32768),(63003,32768),(63010,32768),(63124,131072),(63278,32768),(63293,4),(64125,32768),(64126,32768),(64342,32768),(64422,32776),(64588,32768),(64590,32768),(64591,32768),(64619,4),(64686,32768),(64688,8),(64777,32768),(65147,32768),(65239,32768),(65919,32768),(66378,32768),(66479,32768),(66717,262144),(66765,8),(66809,8),(67097,32768),(67098,32768),(67099,32768),(67331,8),(67333,8),(67858,32768),(67859,32768),(67860,32768),(68321,32768),(68498,32768),(68504,32768),(68505,32768),(68873,4),(69293,32768),(69409,32768),(70324,4),(70461,4),(70492,8),(71904,8),(72133,4),(72255,32768),(72293,4096),(72347,4096),(72373,8),(72409,32768),(72444,32768),(72445,32768),(72446,32768),(72447,32768),(72448,32768),(72449,32768),(72505,8),(72624,8),(72625,8),(73788,4),(73789,4),(73790,4),(73797,32768),(73798,32768),(73799,32768),(74117,2),(74439,32768),(75731,4096),(77679,8),(77833,2),(79353,8192),(79461,28672),(80206,4),(81067,16),(86299,16),(86309,16),(88188,8),(91884,16),(92758,8),(92968,8),(92969,8),(92970,8),(93992,16),(98474,8),(100212,8),(100213,8),(100214,8),(105069,8),(106444,8192),(108094,8),(109631,8192),(109632,8192),(109633,8192);
/*!40000 ALTER TABLE `spell_custom_attr` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-07-15 10:34:16