--
-- Table structure for table `skill_fishing_base_level`
--

DROP TABLE IF EXISTS `skill_fishing_base_level`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `skill_fishing_base_level` (
  `entry` mediumint(8) unsigned NOT NULL DEFAULT 0 COMMENT 'Area ID (See AreaTable.dbc).',
  `skill` smallint(6) NOT NULL DEFAULT 0 COMMENT 'Fishing skill value.',
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Fishing system';
/*!40101 SET character_set_client = @saved_cs_client */;
-- ---------------------------------------- 
-- MySQL dump 10.13  Distrib 5.5.37, for macos12 (x86_64)
--
-- Host: localhost    Database: world
-- ------------------------------------------------------
-- Server version	5.5.5-10.5.8-MariaDB

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@SESSION.TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Dumping data for table `skill_fishing_base_level`
--

LOCK TABLES `skill_fishing_base_level` WRITE;
/*!40000 ALTER TABLE `skill_fishing_base_level` DISABLE KEYS */;
INSERT INTO `skill_fishing_base_level` (`entry`, `skill`) VALUES 
(1,-70),
(2,-20),
(8,130),
(10,55),
(11,55),
(12,-70),
(14,-70),
(15,130),
(16,205),
(17,-20),
(28,205),
(33,130),
(35,55),
(36,130),
(37,130),
(38,-20),
(40,-20),
(41,330),
(44,55),
(45,130),
(46,330),
(47,205),
(65,380),
(66,380),
(67,455),
(85,-70),
(115,-20),
(130,-20),
(139,330),
(141,-70),
(148,-20),
(150,55),
(210,455),
(214,55),
(215,-70),
(249,330),
(267,55),
(294,55),
(295,55),
(297,205),
(298,55),
(299,55),
(300,130),
(305,-20),
(306,-20),
(307,205),
(327,130),
(328,130),
(331,55),
(357,205),
(361,205),
(368,-70),
(373,-70),
(374,-70),
(375,-70),
(382,-20),
(385,-20),
(386,5),
(387,5),
(388,5),
(391,-20),
(392,-20),
(393,-70),
(394,380),
(400,130),
(401,-20),
(405,130),
(406,55),
(414,55),
(440,205),
(445,-20),
(448,-20),
(449,-20),
(454,-20),
(456,-20),
(463,55),
(490,205),
(493,205),
(495,380),
(513,130),
(518,130),
(537,205),
(598,130),
(618,330),
(636,55),
(702,-70),
(718,-20),
(719,-20),
(720,-20),
(796,130),
(878,205),
(896,55),
(1039,55),
(1108,205),
(1112,330),
(1116,205),
(1117,205),
(1120,205),
(1121,205),
(1222,330),
(1227,330),
(1234,205),
(1377,330),
(1417,205),
(1477,205),
(1497,-20),
(1519,-20),
(1537,-20),
(1578,55),
(1581,-20),
(1637,-20),
(1638,-20),
(1657,-20),
(1977,330),
(2017,330),
(2057,330),
(2077,-20),
(2079,130),
(2100,205),
(2318,130),
(2403,130),
(2406,130),
(2408,130),
(2521,205),
(2597,330),
(2717,330),
(2817,405),
(3140,330),
(3317,205),
(3430,-70),
(3433,-20),
(3455,250),
(3456,460),
(3460,-70),
(3461,-70),
(3470,-70),
(3472,-70),
(3473,-70),
(3479,-70),
(3482,-70),
(3483,280),
(3508,-20),
(3513,-20),
(3514,-20),
(3515,-20),
(3518,380),
(3519,355),
(3520,280),
(3521,305),
(3523,380),
(3524,-70),
(3525,-20),
(3528,-70),
(3537,380),
(3558,-70),
(3586,-20),
(3595,-20),
(3607,305),
(3614,395),
(3621,395),
(3625,280),
(3636,395),
(3653,305),
(3655,355),
(3656,305),
(3658,305),
(3659,355),
(3690,405),
(3691,405),
(3692,405),
(3711,435),
(3716,305),
(3805,330),
(3859,405),
(3911,-70),
(3979,485),
(3980,380),
(4022,380),
(4039,380),
(4043,380),
(4055,380),
(4058,380),
(4080,355),
(4109,380),
(4113,380),
(4123,380),
(4134,380),
(4151,380),
(4166,380),
(4197,435),
(4256,380),
(4257,380),
(4258,430),
(4273,460),
(4290,435),
(4374,395),
(4395,435),
(4411,-20),
(4479,380),
(4493,460),
(4560,435),
(4567,435),
(4646,380),
(4722,460),
(4742,460),
(4747,55),
(4813,455),
(5031,350),
(5034,430),
(5038,350),
(5044,350),
(5389,350),
(5534,350),
(5535,350),
(5536,350),
(5538,350),
(5545,350),
(5546,350);
/*!40000 ALTER TABLE `skill_fishing_base_level` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-07-05 13:05:01
