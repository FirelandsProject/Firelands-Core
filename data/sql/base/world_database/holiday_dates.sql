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
-- Table structure for table `holiday_dates`
--

DROP TABLE IF EXISTS `holiday_dates`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `holiday_dates` (
  `id` int unsigned NOT NULL,
  `date_id` tinyint unsigned NOT NULL,
  `date_value` int unsigned NOT NULL,
  `holiday_duration` int unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`,`date_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `holiday_dates`
--

LOCK TABLES `holiday_dates` WRITE;
/*!40000 ALTER TABLE `holiday_dates` DISABLE KEYS */;
INSERT INTO `holiday_dates` VALUES (181,6,220708864,0),(181,7,238354432,0),(181,8,254885888,0),(181,9,270974976,0),(181,10,288620544,0),(181,11,305152000,0),(181,12,322256896,0),(181,13,338886656,0),(181,14,355663872,0),(181,15,372441088,0),(181,16,389218304,0),(181,17,405979136,0),(181,18,422756352,0),(201,9,221708288,0),(201,10,238469120,0),(201,11,255229952,0),(201,12,272646144,0),(201,13,289406976,0),(201,14,305610752,0),(201,15,322371584,0),(201,16,339738624,0),(201,17,356515840,0),(201,18,373293056,0),(201,19,390070272,0),(201,20,406274048,0),(201,21,423051264,0),(321,7,210092032,0),(321,8,226689024,0),(321,9,243286016,0),(321,10,260374528,0),(321,11,276955136,0),(321,12,294060032,0),(321,13,310657024,0),(321,14,344391680,0),(321,15,361168896,0),(321,16,377946112,0),(321,17,394723328,0),(321,18,411484160,0),(321,19,428261376,0),(327,7,218529792,360),(327,8,235192320,360),(327,9,252952576,360),(327,10,269484032,360),(327,11,285540352,360),(327,12,303169536,360),(327,13,319209472,360),(327,14,335921152,360),(327,15,352681984,360),(327,16,369459200,360),(327,17,386236416,360),(327,18,403013632,360),(327,19,419774464,360),(374,0,279986176,0),(374,1,287342592,0),(374,2,290471936,0),(374,3,293601280,0),(374,4,296747008,0),(374,5,304103424,0),(374,6,307232768,0),(374,7,309821440,0),(374,8,312950784,0),(374,9,320864256,0),(374,10,323452928,0),(374,11,326582272,0),(374,12,329711616,0),(374,13,337035264,0),(374,14,340197376,0),(374,15,343982080,0),(374,16,347127808,0),(374,17,353779712,0),(374,18,357613568,0),(374,19,360742912,0),(374,20,363888640,0),(374,21,371245056,0),(374,22,374374400,0),(374,23,377503744,0),(374,24,380649472,0),(374,25,388005888,0),(375,0,280444928,0),(375,1,287801344,0),(375,2,290930688,0),(375,3,294060032,0),(375,4,297205760,0),(375,5,304562176,0),(375,6,307691520,0),(375,7,310820864,0),(375,8,318816256,0),(375,9,321323008,0),(375,10,324452352,0),(375,11,328253440,0),(375,12,335577088,0),(375,13,338722816,0),(375,14,341868544,0),(375,15,344997888,0),(375,16,352321536,0),(375,17,355483648,0),(375,18,358629376,0),(375,19,361758720,0),(375,20,364347392,0),(375,21,372244480,0),(375,22,375390208,0),(375,23,377962496,0),(375,24,381108224,0),(375,25,388464640,0),(376,0,286294016,0),(376,1,288800768,0),(376,2,292601856,0),(376,3,295731200,0),(376,4,303054848,0),(376,5,306233344,0),(376,6,309362688,0),(376,7,312492032,0),(376,8,319815680,0),(376,9,322994176,0),(376,10,326123520,0),(376,11,329252864,0),(376,12,336035840,0),(376,13,339738624,0),(376,14,342327296,0),(376,15,345456640,0),(376,16,352780288,0),(376,17,355942400,0),(376,18,359088128,0),(376,19,362217472,0),(376,20,370196480,0),(376,21,372703232,0),(376,22,375848960,0),(376,23,379633664,0),(376,24,386957312,0),(376,25,389464064,0),(404,4,228982784,0),(404,5,245743616,0),(404,6,262504448,0),(404,7,279248896,0),(404,8,296009728,0),(404,9,312770560,0),(404,10,329646080,0),(404,11,346390592,0),(404,12,363167808,0),(404,13,379945024,0),(404,14,396722240,0),(404,15,413483072,0),(404,16,430260288,0),(423,3,219299840,0),(423,4,236077056,0),(423,5,252723200,0),(423,6,269713408,0),(423,7,286359552,0),(423,8,303169536,0),(423,9,319881216,0),(423,10,336707584,0),(423,11,353468416,0),(423,12,370245632,0),(423,13,387022848,0),(423,14,403800064,0),(423,15,420560896,0);
/*!40000 ALTER TABLE `holiday_dates` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-07-15 10:34:03
