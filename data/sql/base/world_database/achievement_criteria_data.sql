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
-- Table structure for table `achievement_criteria_data`
--

DROP TABLE IF EXISTS `achievement_criteria_data`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `achievement_criteria_data` (
  `criteria_id` int NOT NULL,
  `type` tinyint unsigned NOT NULL DEFAULT '0',
  `value1` int unsigned NOT NULL DEFAULT '0',
  `value2` int unsigned NOT NULL DEFAULT '0',
  `ScriptName` varchar(64) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  PRIMARY KEY (`criteria_id`,`type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='Achievment system';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `achievement_criteria_data`
--

LOCK TABLES `achievement_criteria_data` WRITE;
/*!40000 ALTER TABLE `achievement_criteria_data` DISABLE KEYS */;
INSERT INTO `achievement_criteria_data` VALUES (100,0,0,0,''),(102,0,0,0,''),(123,0,0,0,''),(140,0,0,0,''),(166,0,0,0,''),(176,0,0,0,''),(177,0,0,0,''),(221,0,0,0,''),(222,0,0,0,''),(223,0,0,0,''),(224,0,0,0,''),(225,0,0,0,''),(308,0,0,0,''),(409,0,0,0,''),(414,0,0,0,''),(416,0,0,0,''),(417,0,0,0,''),(418,0,0,0,''),(419,0,0,0,''),(420,0,0,0,''),(421,0,0,0,''),(422,0,0,0,''),(423,0,0,0,''),(424,0,0,0,''),(426,0,0,0,''),(427,0,0,0,''),(436,0,0,0,''),(437,0,0,0,''),(438,0,0,0,''),(439,0,0,0,''),(440,0,0,0,''),(477,0,0,0,''),(480,0,0,0,''),(481,0,0,0,''),(482,0,0,0,''),(483,0,0,0,''),(489,0,0,0,''),(490,0,0,0,''),(491,0,0,0,''),(522,13,8,0,''),(524,0,0,0,''),(525,0,0,0,''),(526,0,0,0,''),(527,0,0,0,''),(528,0,0,0,''),(529,0,0,0,''),(530,0,0,0,''),(531,0,0,0,''),(532,0,0,0,''),(533,0,0,0,''),(534,0,0,0,''),(535,0,0,0,''),(536,0,0,0,''),(537,0,0,0,''),(538,0,0,0,''),(539,0,0,0,''),(540,0,0,0,''),(541,0,0,0,''),(542,0,0,0,''),(544,0,0,0,''),(545,0,0,0,''),(546,0,0,0,''),(547,0,0,0,''),(548,0,0,0,''),(549,0,0,0,''),(550,0,0,0,''),(551,0,0,0,''),(552,0,0,0,''),(594,0,0,0,''),(595,0,0,0,''),(596,0,0,0,''),(598,0,0,0,''),(599,0,0,0,''),(601,0,0,0,''),(602,0,0,0,''),(603,0,0,0,''),(604,0,0,0,''),(605,0,0,0,''),(606,0,0,0,''),(607,0,0,0,''),(611,17,0,0,''),(758,0,0,0,''),(1234,11,0,0,'achievement_bg_control_all_nodes'),(1235,17,1590,1590,''),(1236,17,0,0,''),(1237,0,0,0,''),(1238,17,0,0,''),(1239,11,0,0,'achievement_bg_control_all_nodes'),(1240,0,0,0,''),(1241,0,0,0,''),(1242,11,0,0,'achievement_bg_av_perfection'),(1243,0,0,0,''),(1244,0,0,0,''),(1245,0,0,0,''),(1246,0,0,0,''),(1247,0,0,0,''),(1801,0,0,0,''),(1802,0,0,0,''),(1803,11,0,0,'achievement_bg_av_perfection'),(1804,11,0,0,'achievement_bg_av_perfection'),(1805,11,0,0,'achievement_bg_av_perfection'),(1806,11,0,0,'achievement_bg_av_perfection'),(1807,11,0,0,'achievement_bg_av_perfection'),(1808,11,0,0,'achievement_bg_av_perfection'),(1809,11,0,0,'achievement_bg_av_perfection'),(1810,11,0,0,'achievement_bg_av_perfection'),(1811,11,0,0,'achievement_bg_av_perfection'),(1812,11,0,0,'achievement_bg_av_perfection'),(1813,11,0,0,'achievement_bg_av_perfection'),(1814,11,0,0,'achievement_bg_av_perfection'),(1815,11,0,0,'achievement_bg_av_perfection'),(1816,11,0,0,'achievement_bg_av_perfection'),(1817,11,0,0,'achievement_bg_av_perfection'),(1818,11,0,0,'achievement_bg_av_perfection'),(1819,11,0,0,'achievement_bg_av_perfection'),(1824,0,0,0,''),(1825,11,0,0,'achievement_bg_av_perfection'),(1826,11,0,0,'achievement_bg_av_perfection'),(1831,0,0,0,''),(1832,0,0,0,''),(2070,0,0,0,''),(2072,0,0,0,''),(2342,19,213,4,''),(2343,19,213,4,''),(2344,19,213,4,''),(2345,19,213,4,''),(2346,19,213,4,''),(2347,19,213,4,''),(2348,19,213,4,''),(2349,19,213,4,''),(2350,19,213,4,''),(2351,19,213,4,''),(2353,19,213,4,''),(2355,19,213,4,''),(2356,19,213,4,''),(2357,19,213,4,''),(2358,8,185,3,''),(2408,0,0,0,''),(2412,8,185,3,''),(3209,0,0,0,''),(3210,0,0,0,''),(3211,0,0,0,''),(3212,0,0,0,''),(3213,0,0,0,''),(3214,0,0,0,''),(3215,0,0,0,''),(3239,0,0,0,''),(3240,0,0,0,''),(3241,0,0,0,''),(3242,0,0,0,''),(3243,0,0,0,''),(3244,0,0,0,''),(3245,0,0,0,''),(3246,0,0,0,''),(3247,0,0,0,''),(3248,0,0,0,''),(3249,0,0,0,''),(3250,0,0,0,''),(3251,0,0,0,''),(3252,0,0,0,''),(3253,0,0,0,''),(3254,0,0,0,''),(3255,0,0,0,''),(3256,0,0,0,''),(3257,0,0,0,''),(3258,0,0,0,''),(3259,0,0,0,''),(3260,0,0,0,''),(3261,0,0,0,''),(3262,0,0,0,''),(3263,0,0,0,''),(3264,0,0,0,''),(3265,0,0,0,''),(3266,0,0,0,''),(3268,0,0,0,''),(3270,0,0,0,''),(3271,0,0,0,''),(3272,0,0,0,''),(3273,0,0,0,''),(3274,0,0,0,''),(3275,0,0,0,''),(3353,0,0,0,''),(3368,0,0,0,''),(3369,0,0,0,''),(3370,0,0,0,''),(3371,0,0,0,''),(3372,0,0,0,''),(3384,8,1800,3,''),(3386,11,0,0,'achievement_everything_counts'),(3387,11,0,0,'achievement_everything_counts'),(3388,11,0,0,'achievement_everything_counts'),(3389,11,0,0,'achievement_everything_counts'),(3515,0,0,0,''),(3516,0,0,0,''),(3517,0,0,0,''),(3518,0,0,0,''),(3519,0,0,0,''),(3520,0,0,0,''),(3521,0,0,0,''),(3522,0,0,0,''),(3523,0,0,0,''),(3524,0,0,0,''),(3525,0,0,0,''),(3526,0,0,0,''),(3615,1,1412,0,''),(3622,0,0,0,''),(3680,0,0,0,''),(3684,5,23505,0,''),(3685,7,34976,0,''),(3693,11,0,0,'achievement_bg_control_all_nodes'),(3698,7,23335,0,''),(3699,7,23333,0,''),(3723,19,187,3,''),(3724,19,187,3,''),(3725,19,187,3,''),(3726,19,187,3,''),(3727,19,187,3,''),(3728,19,187,3,''),(3729,19,187,3,''),(3730,19,187,3,''),(3731,19,187,3,''),(3732,19,187,3,''),(3733,19,187,3,''),(3734,19,187,3,''),(3735,19,187,3,''),(3736,19,187,3,''),(3737,19,187,3,''),(3738,19,187,3,''),(3739,1,25679,0,''),(3741,1,25677,0,''),(3744,1,6368,0,''),(3745,1,620,0,''),(3746,1,2442,0,''),(3747,1,6827,0,''),(3748,1,883,0,''),(3749,1,19665,0,''),(3750,1,890,0,''),(3751,1,13321,0,''),(3752,1,4166,0,''),(3753,1,5951,0,''),(3754,1,9600,0,''),(3755,1,721,0,''),(3756,1,2098,0,''),(3757,1,1933,0,''),(3758,1,17467,0,''),(3759,1,10685,0,''),(3760,1,1420,0,''),(3761,1,2620,0,''),(3804,11,0,0,'achievement_resilient_victory'),(3814,0,0,0,''),(3815,0,0,0,''),(3821,0,0,0,''),(3822,0,0,0,''),(3826,5,26157,0,''),(3827,5,26272,0,''),(3828,5,26273,0,''),(3829,5,26274,0,''),(3879,7,23505,0,''),(3880,7,23451,0,''),(3881,15,3,0,''),(3881,16,372,0,''),(3882,5,48890,0,''),(3929,1,8403,0,''),(3929,11,0,0,'achievement_flirt_with_disaster_perf_check'),(3929,15,3,0,''),(3931,1,9099,0,''),(3931,11,0,0,'achievement_flirt_with_disaster_perf_check'),(3931,15,3,0,''),(3936,5,44827,0,''),(3936,16,141,0,''),(3937,5,44825,0,''),(3937,16,141,0,''),(3938,5,44824,0,''),(3938,16,141,0,''),(4090,0,0,0,''),(4111,0,0,0,''),(4112,5,55005,0,''),(4227,1,8403,0,''),(4227,11,0,0,'achievement_flirt_with_disaster_perf_check'),(4227,15,3,0,''),(4230,5,55000,0,''),(4230,16,141,0,''),(4240,11,0,0,'achievement_watch_him_die'),(4244,11,0,0,'achievement_hadronox_denied'),(4439,0,0,0,''),(4501,0,0,0,''),(4502,0,0,0,''),(4504,0,0,0,''),(4505,0,0,0,''),(4506,0,0,0,''),(4507,0,0,0,''),(4508,0,0,0,''),(4509,0,0,0,''),(4510,0,0,0,''),(4511,0,0,0,''),(4512,0,0,0,''),(4513,0,0,0,''),(4514,0,0,0,''),(4515,0,0,0,''),(4516,0,0,0,''),(4517,0,0,0,''),(4518,0,0,0,''),(4519,0,0,0,''),(4520,0,0,0,''),(4521,0,0,0,''),(4522,0,0,0,''),(4523,0,0,0,''),(4768,25,4,0,''),(4769,25,4,0,''),(4770,25,4,0,''),(4771,25,4,0,''),(4772,25,4,0,''),(4773,25,4,0,''),(4774,25,4,0,''),(4775,25,4,0,''),(4776,25,4,0,''),(4777,25,4,0,''),(4778,25,4,0,''),(4779,25,4,0,''),(4780,25,4,0,''),(4781,25,4,0,''),(4782,25,4,0,''),(4783,25,4,0,''),(4784,25,4,0,''),(4785,25,4,0,''),(4786,25,4,0,''),(4944,0,0,0,''),(4946,11,0,0,'achievement_killed_exp_or_honor_target'),(4948,8,1,0,''),(4949,8,3,0,''),(4950,8,2,0,''),(4951,8,4,0,''),(4952,8,5,0,''),(4953,8,7,0,''),(4954,8,9,0,''),(4955,8,6,0,''),(4956,8,10,0,''),(4957,8,11,0,''),(4958,8,8,0,''),(4966,0,0,0,''),(4967,0,0,0,''),(4968,0,0,0,''),(4969,0,0,0,''),(4970,0,0,0,''),(4971,0,0,0,''),(4972,0,0,0,''),(4973,0,0,0,''),(4974,0,0,0,''),(4975,0,0,0,''),(4976,0,0,0,''),(4977,0,0,0,''),(4978,0,0,0,''),(4979,0,0,0,''),(4980,0,0,0,''),(4981,0,0,0,''),(4982,0,0,0,''),(4983,0,0,0,''),(4991,0,0,0,''),(5008,0,0,0,''),(5018,21,8,0,''),(5031,0,0,0,''),(5032,0,0,0,''),(5033,0,0,0,''),(5034,0,0,0,''),(5035,0,0,0,''),(5036,0,0,0,''),(5037,0,0,0,''),(5038,0,0,0,''),(5039,0,0,0,''),(5203,0,0,0,''),(5213,21,7,0,''),(5214,21,11,0,''),(5215,21,2,0,''),(5216,21,5,0,''),(5217,21,9,0,''),(5218,21,3,0,''),(5219,21,6,0,''),(5220,21,1,0,''),(5221,21,4,0,''),(5229,21,0,7,''),(5230,21,0,10,''),(5231,21,0,11,''),(5232,21,0,3,''),(5233,21,0,1,''),(5234,21,0,4,''),(5235,21,0,2,''),(5236,21,0,6,''),(5237,21,0,8,''),(5238,21,0,5,''),(5258,0,57064,0,''),(5272,1,3057,0,''),(5272,16,141,0,''),(5377,0,0,0,''),(5378,0,0,0,''),(5379,0,0,0,''),(5380,0,0,0,''),(5381,0,0,0,''),(5382,0,0,0,''),(5383,0,0,0,''),(5384,0,0,0,''),(5436,0,0,0,''),(5437,0,0,0,''),(5438,0,0,0,''),(5439,0,0,0,''),(5440,0,0,0,''),(5441,0,0,0,''),(5442,0,0,0,''),(5443,0,0,0,''),(5444,0,0,0,''),(5445,0,0,0,''),(5446,0,0,0,''),(5447,0,0,0,''),(5448,0,0,0,''),(5449,0,0,0,''),(5450,0,0,0,''),(5451,0,0,0,''),(5452,0,0,0,''),(5453,0,0,0,''),(5454,0,0,0,''),(5455,0,0,0,''),(5456,0,0,0,''),(5457,0,0,0,''),(5458,0,0,0,''),(5459,0,0,0,''),(5460,0,0,0,''),(5461,0,0,0,''),(5462,0,0,0,''),(5463,0,0,0,''),(5464,0,0,0,''),(5465,0,0,0,''),(5466,0,0,0,''),(5467,0,0,0,''),(5468,0,0,0,''),(5469,8,50,3,''),(5529,0,0,0,''),(5541,11,0,0,'achievement_arena_2v2_kills'),(5542,11,0,0,'achievement_arena_3v3_kills'),(5543,11,0,0,'achievement_arena_5v5_kills'),(5597,23,62,0,''),(5599,23,71,0,''),(5600,23,80,0,''),(5601,0,0,0,''),(5602,0,0,0,''),(5603,0,0,0,''),(5605,7,23333,0,''),(5605,11,0,0,'achievement_save_the_day'),(5606,7,23335,0,''),(5606,11,0,0,'achievement_save_the_day'),(5621,0,0,0,''),(5622,0,0,0,''),(5623,0,0,0,''),(5624,0,0,0,''),(5625,0,0,0,''),(5626,0,0,0,''),(5627,0,0,0,''),(5628,0,0,0,''),(5629,0,0,0,''),(5630,0,0,0,''),(5631,0,0,0,''),(5745,0,0,0,''),(5746,0,0,0,''),(5772,2,0,7,''),(5774,2,0,10,''),(5775,2,0,11,''),(5776,2,0,3,''),(5777,2,0,1,''),(5778,2,0,4,''),(5779,2,0,2,''),(5780,2,0,6,''),(5781,2,0,8,''),(5782,2,0,5,''),(5816,19,213,4,''),(5817,19,213,4,''),(5820,0,0,0,''),(5888,0,0,0,''),(5889,0,0,0,''),(5890,0,0,0,''),(5896,0,0,0,''),(5897,0,0,0,''),(5898,0,0,0,''),(5899,0,0,0,''),(6225,1,5661,0,''),(6226,1,26044,0,''),(6228,1,739,0,''),(6229,1,927,0,''),(6230,1,1444,0,''),(6231,1,8140,0,''),(6232,1,5489,0,''),(6233,1,12336,0,''),(6234,1,1351,0,''),(6235,1,5484,0,''),(6237,2,6,2,''),(6238,2,1,1,''),(6239,2,7,6,''),(6240,2,11,4,''),(6241,2,4,5,''),(6242,2,3,8,''),(6243,2,8,7,''),(6244,2,2,3,''),(6245,2,9,10,''),(6246,2,5,11,''),(6247,16,141,0,''),(6248,16,141,0,''),(6249,16,141,0,''),(6261,5,21848,0,''),(6261,7,21848,0,''),(6261,16,141,0,''),(6302,2,6,2,''),(6312,2,9,7,''),(6313,2,6,1,''),(6314,2,5,4,''),(6315,2,7,2,''),(6316,2,11,6,''),(6317,2,4,8,''),(6318,2,1,5,''),(6319,2,8,10,''),(6320,2,2,11,''),(6321,2,3,3,''),(6324,0,0,0,''),(6325,0,0,0,''),(6326,0,0,0,''),(6327,0,0,0,''),(6328,0,0,0,''),(6329,0,0,0,''),(6330,0,0,0,''),(6331,0,0,0,''),(6343,1,16111,0,''),(6344,1,16111,0,''),(6345,1,16111,0,''),(6346,1,16111,0,''),(6347,1,16111,0,''),(6436,0,0,0,''),(6440,0,0,0,''),(6441,0,0,0,''),(6444,0,0,0,''),(6445,0,0,0,''),(6446,7,52418,0,''),(6447,0,0,0,''),(6486,0,0,0,''),(6487,0,0,0,''),(6488,0,0,0,''),(6489,0,0,0,''),(6490,0,0,0,''),(6491,0,0,0,''),(6492,0,0,0,''),(6493,0,0,0,''),(6494,0,0,0,''),(6495,0,0,0,''),(6496,0,0,0,''),(6497,0,0,0,''),(6498,0,0,0,''),(6499,0,0,0,''),(6501,0,0,0,''),(6502,0,0,0,''),(6503,0,0,0,''),(6505,0,0,0,''),(6506,0,0,0,''),(6507,0,0,0,''),(6508,0,0,0,''),(6509,0,0,0,''),(6510,0,0,0,''),(6511,0,0,0,''),(6512,0,0,0,''),(6513,0,0,0,''),(6514,0,0,0,''),(6515,0,0,0,''),(6516,0,0,0,''),(6517,0,0,0,''),(6518,0,0,0,''),(6587,0,0,0,''),(6588,0,0,0,''),(6589,0,0,0,''),(6590,0,0,0,''),(6591,0,0,0,''),(6592,0,0,0,''),(6593,0,0,0,''),(6594,0,0,0,''),(6595,0,0,0,''),(6596,0,0,0,''),(6597,0,0,0,''),(6598,0,0,0,''),(6599,0,0,0,''),(6641,5,58818,0,''),(6641,16,201,0,''),(6642,5,58818,0,''),(6642,16,201,0,''),(6643,5,58818,0,''),(6643,16,201,0,''),(6644,5,58818,0,''),(6644,16,201,0,''),(6651,5,58818,0,''),(6651,16,201,0,''),(6652,5,58818,0,''),(6652,16,201,0,''),(6653,5,58818,0,''),(6653,16,201,0,''),(6654,5,58818,0,''),(6654,16,201,0,''),(6655,5,58818,0,''),(6655,16,201,0,''),(6656,5,58818,0,''),(6656,16,201,0,''),(6657,5,58818,0,''),(6657,16,201,0,''),(6659,5,58818,0,''),(6659,16,201,0,''),(6662,1,31261,0,''),(6758,0,0,0,''),(6759,0,0,0,''),(6760,0,0,0,''),(6761,0,0,0,''),(6764,0,0,0,''),(6765,0,0,0,''),(6766,0,0,0,''),(6767,0,0,0,''),(6768,0,0,0,''),(6770,0,0,0,''),(6771,0,0,0,''),(6772,0,0,0,''),(6773,0,0,0,''),(6774,0,0,0,''),(6775,0,0,0,''),(6776,0,0,0,''),(6777,0,0,0,''),(6778,0,0,0,''),(6779,0,0,0,''),(6780,0,0,0,''),(6781,0,0,0,''),(6782,0,0,0,''),(6783,0,0,0,''),(6784,0,0,0,''),(6785,0,0,0,''),(6786,0,0,0,''),(6789,23,53,0,''),(6799,3,5,0,''),(6800,11,0,0,'achievement_sickly_gazelle'),(6802,13,8,0,''),(6803,11,0,0,'achievement_defenseless'),(6804,0,0,0,''),(6835,7,52092,0,''),(6937,0,0,0,''),(7020,7,23335,0,''),(7021,7,23333,0,''),(7136,5,55817,0,''),(7146,13,8,0,''),(7147,13,8,0,''),(7148,13,8,0,''),(7149,13,8,0,''),(7150,13,8,0,''),(7151,13,8,0,''),(7152,13,8,0,''),(7153,13,8,0,''),(7154,13,8,0,''),(7155,13,8,0,''),(7156,13,8,0,''),(7157,13,8,0,''),(7158,13,8,0,''),(7159,13,20,0,''),(7160,13,20,0,''),(7161,13,20,0,''),(7162,13,20,0,''),(7163,13,20,0,''),(7164,13,20,0,''),(7165,13,20,0,''),(7166,13,20,0,''),(7167,13,20,0,''),(7168,13,20,0,''),(7169,13,20,0,''),(7170,13,20,0,''),(7171,13,20,0,''),(7172,13,20,0,''),(7174,13,8,0,''),(7175,13,20,0,''),(7177,5,49460,1,''),(7178,5,49346,1,''),(7179,5,49464,1,''),(7185,13,20,0,''),(7186,13,20,0,''),(7187,13,20,0,''),(7188,13,20,0,''),(7189,13,8,0,''),(7190,13,8,0,''),(7191,13,8,0,''),(7231,11,0,0,'achievement_on_the_rocks'),(7264,11,0,0,'achievement_safety_dance'),(7265,11,0,0,'achievement_momma_said_knock_you_out'),(7315,11,0,0,'achievement_intense_cold'),(7316,11,0,0,'achievement_chaos_theory'),(7317,11,0,0,'achievement_respect_your_elders'),(7318,11,0,0,'achievement_king_dred'),(7319,11,0,0,'achievement_less_rabi'),(7320,11,0,0,'achievement_dehydration'),(7321,11,0,0,'achievement_shatter_resistant'),(7322,11,0,0,'achievement_incredible_hulk'),(7323,11,0,0,'achievement_ruby_void'),(7324,11,0,0,'achievement_emerald_void'),(7325,11,0,0,'achievement_amber_void'),(7328,11,0,0,'achievement_twilight_assist'),(7329,11,0,0,'achievement_twilight_duo'),(7330,11,0,0,'achievement_twilight_zone'),(7331,11,0,0,'achievement_twilight_assist'),(7332,11,0,0,'achievement_twilight_duo'),(7333,11,0,0,'achievement_twilight_zone'),(7359,11,0,0,'achievement_volunteer_work'),(7361,11,0,0,'achievement_oh_novos'),(7363,11,0,0,'achievement_snakes_whyd_it_have_to_be_snakes'),(7365,0,0,0,''),(7408,23,45,0,''),(7412,23,42,0,''),(7415,23,43,0,''),(7416,23,42,0,''),(7418,23,44,0,''),(7419,23,43,0,''),(7548,11,0,0,'achievement_safety_dance'),(7549,11,0,0,'achievement_momma_said_knock_you_out'),(7567,11,0,0,'achievement_the_hundred_club'),(7568,11,0,0,'achievement_the_hundred_club'),(7573,1,30249,0,''),(7573,11,0,0,'achievement_denyin_the_scion'),(7574,1,30249,0,''),(7574,11,0,0,'achievement_denyin_the_scion'),(7577,11,0,0,'achievement_split_personality'),(7579,11,0,0,'achievement_consumption_junction'),(7583,11,0,0,'achievement_share_the_love'),(7587,11,0,0,'achievement_void_dance'),(7590,11,0,0,'achievement_brann_spankin_new'),(7593,11,0,0,'achievement_abuse_the_ooze'),(7595,11,0,0,'achievement_girl_love_to_skadi'),(7598,11,0,0,'achievement_kings_bane'),(7600,18,0,0,''),(7601,18,0,0,''),(7604,11,0,0,'achievement_thaddius_shocking'),(7605,11,0,0,'achievement_thaddius_shocking'),(7608,13,8,0,''),(7609,13,20,0,''),(7612,11,0,0,'achievement_spore_loser'),(7613,11,0,0,'achievement_spore_loser'),(7614,11,0,0,'achievement_just_cant_get_enough'),(7615,11,0,0,'achievement_just_cant_get_enough'),(7622,0,0,0,''),(7623,0,0,0,''),(7625,11,0,0,'achievement_bg_sa_artillery'),(7626,11,0,0,'achievement_not_even_a_scratch'),(7628,11,0,0,'achievement_bg_sa_artillery'),(7629,7,52418,0,''),(7632,0,0,0,''),(7634,11,0,0,'achievement_not_even_a_scratch'),(7635,0,0,0,''),(7636,0,0,0,''),(7636,11,0,0,'achievement_bg_sa_defense_of_ancients'),(7666,0,0,0,''),(7703,11,0,0,'achievement_wg_didnt_stand_a_chance'),(7740,0,0,0,''),(7740,11,0,0,'achievement_bg_sa_defense_of_ancients'),(7769,0,0,0,''),(7850,0,0,0,''),(7851,0,0,0,''),(7852,0,0,0,''),(7853,0,0,0,''),(7854,0,0,0,''),(7855,0,0,0,''),(7856,0,0,0,''),(7857,0,0,0,''),(7858,0,0,0,''),(7859,0,0,0,''),(7860,0,0,0,''),(7861,0,0,0,''),(7862,0,0,0,''),(7863,0,0,0,''),(7864,0,0,0,''),(7865,0,0,0,''),(7866,0,0,0,''),(7867,0,0,0,''),(7868,0,0,0,''),(8100,0,0,0,''),(8101,0,0,0,''),(8102,0,0,0,''),(8103,0,0,0,''),(8104,0,0,0,''),(8105,0,0,0,''),(8106,0,0,0,''),(8107,0,0,0,''),(8108,0,0,0,''),(8109,0,0,0,''),(8110,0,0,0,''),(8111,0,0,0,''),(8112,0,0,0,''),(8113,0,0,0,''),(8114,0,0,0,''),(8115,0,0,0,''),(8116,0,0,0,''),(8117,0,0,0,''),(8118,0,0,0,''),(8119,0,0,0,''),(8120,0,0,0,''),(8121,0,0,0,''),(8122,0,0,0,''),(8758,0,0,0,''),(8759,0,0,0,''),(8760,0,0,0,''),(8761,0,0,0,''),(8762,0,0,0,''),(8763,0,0,0,''),(8764,0,0,0,''),(8765,0,0,0,''),(8798,0,0,0,''),(8799,0,0,0,''),(8800,0,0,0,''),(8801,0,0,0,''),(8802,0,0,0,''),(8803,0,0,0,''),(9058,24,65,0,''),(9060,0,0,0,''),(9061,0,0,0,''),(9118,16,181,0,''),(9119,0,0,0,''),(9119,16,181,0,''),(9120,0,0,0,''),(9120,16,181,0,''),(9121,0,0,0,''),(9122,0,0,0,''),(9123,0,0,0,''),(9124,2,0,10,''),(9124,9,18,0,''),(9124,10,1,0,''),(9138,0,0,0,''),(9139,0,0,0,''),(9140,0,0,0,''),(9141,0,0,0,''),(9142,0,0,0,''),(9143,2,0,11,''),(9143,9,18,0,''),(9143,10,1,0,''),(9144,2,0,1,''),(9144,9,18,0,''),(9144,10,1,0,''),(9145,2,0,3,''),(9145,9,18,0,''),(9145,10,1,0,''),(9146,2,0,7,''),(9146,9,18,0,''),(9146,10,1,0,''),(9147,2,0,4,''),(9147,9,18,0,''),(9147,10,1,0,''),(9148,2,0,2,''),(9148,9,18,0,''),(9148,10,1,0,''),(9149,2,0,6,''),(9149,9,18,0,''),(9149,10,1,0,''),(9150,2,0,8,''),(9150,9,18,0,''),(9150,10,1,0,''),(9151,2,0,5,''),(9151,9,18,0,''),(9151,10,1,0,''),(9178,0,0,0,''),(9179,0,0,0,''),(9180,0,0,0,''),(9181,0,0,0,''),(9199,0,0,0,''),(9200,0,0,0,''),(9201,0,0,0,''),(9299,1,29328,0,''),(9300,1,31685,0,''),(9301,1,28407,0,''),(9302,1,24746,0,''),(9303,1,32498,0,''),(9304,1,31889,0,''),(9305,1,6653,0,''),(9306,1,9700,0,''),(9307,1,31890,0,''),(9308,1,26503,0,''),(9309,1,28093,0,''),(9310,1,28440,0,''),(9358,0,0,0,''),(9359,0,0,0,''),(9360,0,0,0,''),(9361,0,0,0,''),(9362,0,0,0,''),(9363,0,0,0,''),(9364,0,0,0,''),(9365,0,0,0,''),(9366,0,0,0,''),(9367,0,0,0,''),(9368,0,0,0,''),(9369,0,0,0,''),(9370,0,0,0,''),(9371,0,0,0,''),(9372,0,0,0,''),(9373,0,0,0,''),(9374,0,0,0,''),(9421,0,0,0,''),(9619,0,0,0,''),(9718,23,42,0,''),(9719,23,44,0,''),(9720,23,43,0,''),(9721,23,42,0,''),(9798,5,63034,0,''),(9798,11,0,0,'achievement_tilted'),(9818,0,0,0,''),(9838,1,3057,0,''),(9838,16,141,0,''),(9839,1,3057,0,''),(9839,16,141,0,''),(9858,5,62181,0,''),(9858,7,61980,0,''),(9918,0,0,0,''),(9919,0,0,0,''),(9920,0,0,0,''),(9921,0,0,0,''),(9922,0,0,0,''),(10044,11,0,0,'achievement_unbroken'),(10045,11,0,0,'achievement_unbroken'),(10046,11,0,0,'achievement_three_car_garage_chopper'),(10047,11,0,0,'achievement_three_car_garage_siege'),(10048,11,0,0,'achievement_three_car_garage_demolisher'),(10049,11,0,0,'achievement_three_car_garage_chopper'),(10050,11,0,0,'achievement_three_car_garage_siege'),(10051,11,0,0,'achievement_three_car_garage_demolisher'),(10054,11,0,0,'achievement_shutout'),(10055,11,0,0,'achievement_shutout'),(10056,11,0,0,'achievement_orbital_bombardment'),(10057,11,0,0,'achievement_orbital_devastation'),(10058,11,0,0,'achievement_nuked_from_orbit'),(10059,11,0,0,'achievement_orbital_bombardment'),(10060,11,0,0,'achievement_orbital_devastation'),(10061,11,0,0,'achievement_nuked_from_orbit'),(10062,11,0,0,'achievement_quick_shave'),(10063,11,0,0,'achievement_quick_shave'),(10066,11,0,0,'achievement_iron_dwarf_medium_rare'),(10067,11,0,0,'achievement_iron_dwarf_medium_rare'),(10068,11,0,0,'achievement_ignis_shattered'),(10069,11,0,0,'achievement_ignis_shattered'),(10074,11,0,0,'achievement_nerf_engineering'),(10075,11,0,0,'achievement_nerf_engineering'),(10077,11,0,0,'achievement_nerf_gravity_bombs'),(10079,11,0,0,'achievement_nerf_gravity_bombs'),(10082,11,0,0,'achievement_assembly_i_choose_you'),(10083,11,0,0,'achievement_assembly_i_choose_you'),(10084,11,0,0,'achievement_assembly_i_choose_you'),(10085,11,0,0,'achievement_assembly_i_choose_you'),(10086,11,0,0,'achievement_assembly_i_choose_you'),(10087,11,0,0,'achievement_assembly_i_choose_you'),(10088,5,58501,0,''),(10089,5,58501,0,''),(10173,11,0,0,'achievement_shadowdodger'),(10184,11,0,0,'achievement_crazy_cat_lady'),(10185,18,0,0,''),(10187,1,33134,0,''),(10187,7,65157,0,''),(10189,1,33134,0,''),(10189,7,65157,0,''),(10218,11,0,0,'achievement_orbit_uary'),(10219,11,0,0,'achievement_orbit_uary'),(10220,11,0,0,'achievement_heartbreaker'),(10221,11,0,0,'achievement_heartbreaker'),(10223,18,0,0,''),(10224,0,0,0,''),(10225,0,0,0,''),(10226,0,0,0,''),(10227,0,0,0,''),(10228,0,0,0,''),(10229,18,0,0,''),(10238,18,0,0,''),(10239,18,0,0,''),(10240,18,0,0,''),(10241,18,0,0,''),(10243,11,0,0,'achievement_nine_lives'),(10247,18,0,0,''),(10248,18,0,0,''),(10258,18,0,0,''),(10259,18,0,0,''),(10260,18,0,0,''),(10261,18,0,0,''),(10279,18,0,0,''),(10287,11,0,0,'achievement_i_ll_take_you_all_on'),(10288,11,0,0,'achievement_i_ll_take_you_all_on'),(10296,18,0,0,''),(10304,5,62320,0,''),(10305,11,0,0,'achievement_dont_stand_in_the_lightning'),(10306,11,0,0,'achievement_shadowdodger'),(10309,11,0,0,'achievement_dont_stand_in_the_lightning'),(10311,11,0,0,'achievement_i_ll_take_you_all_on'),(10312,11,0,0,'achievement_i_ll_take_you_all_on'),(10313,5,62320,0,''),(10388,18,0,0,''),(10391,5,58818,0,''),(10391,16,201,0,''),(10399,11,0,0,'achievement_nine_lives'),(10400,11,0,0,'achievement_crazy_cat_lady'),(10409,18,0,0,''),(10410,18,0,0,''),(10412,18,0,0,''),(10414,18,0,0,''),(10415,18,0,0,''),(10416,18,0,0,''),(10417,18,0,0,''),(10418,5,58501,0,''),(10419,5,58501,0,''),(10420,5,58501,0,''),(10421,5,58501,0,''),(10440,11,0,0,'achievement_lose_your_illusion'),(10441,1,2784,0,''),(10441,16,141,0,''),(10442,1,2784,0,''),(10442,16,141,0,''),(10443,1,2784,0,''),(10443,16,141,0,''),(10445,11,0,0,'achievement_getting_back_to_nature'),(10447,11,0,0,'achievement_knock_on_wood'),(10448,11,0,0,'achievement_knock_knock_on_wood'),(10449,11,0,0,'achievement_knock_knock_knock_on_wood'),(10450,11,0,0,'achievement_firefighter'),(10451,11,0,0,'achievement_smell_saronite'),(10452,18,0,0,''),(10457,11,0,0,'achievement_lose_your_illusion'),(10458,18,0,0,''),(10459,11,0,0,'achievement_knock_on_wood'),(10460,11,0,0,'achievement_knock_knock_on_wood'),(10461,11,0,0,'achievement_knock_knock_knock_on_wood'),(10462,11,0,0,'achievement_smell_saronite'),(10463,11,0,0,'achievement_firefighter'),(10543,11,0,0,'achievement_setup_mine'),(10544,11,0,0,'achievement_setup_rocket'),(10545,11,0,0,'achievement_setup_boom'),(10546,11,0,0,'achievement_setup_mine'),(10547,11,0,0,'achievement_setup_rocket'),(10548,11,0,0,'achievement_setup_boom'),(10619,1,33142,0,''),(10620,1,33142,0,''),(10678,18,0,0,''),(10718,23,157,0,''),(10758,11,0,0,'achievement_getting_back_to_nature'),(10878,23,157,0,''),(10879,23,157,0,''),(10881,23,157,0,''),(10898,23,167,0,''),(10918,0,0,0,''),(10919,0,0,0,''),(11086,16,404,0,''),(11088,16,404,0,''),(11089,16,404,0,''),(11090,16,404,0,''),(11118,16,404,0,''),(11119,16,404,0,''),(11120,16,404,0,''),(11121,16,404,0,''),(11122,16,404,0,''),(11123,16,404,0,''),(11124,16,404,0,''),(11125,16,404,0,''),(11126,16,404,0,''),(11127,16,404,0,''),(11128,16,404,0,''),(11134,5,66303,0,''),(11134,16,404,0,''),(11135,5,66303,0,''),(11135,16,404,0,''),(11136,5,66303,0,''),(11136,16,404,0,''),(11137,5,66303,0,''),(11137,16,404,0,''),(11138,5,66303,0,''),(11138,16,404,0,''),(11139,5,66303,0,''),(11139,16,404,0,''),(11140,5,66303,0,''),(11140,16,404,0,''),(11141,5,66303,0,''),(11141,16,404,0,''),(11142,5,66303,0,''),(11142,16,404,0,''),(11158,2,4,1,''),(11158,16,404,0,''),(11159,2,4,4,''),(11159,16,404,0,''),(11160,2,4,2,''),(11160,16,404,0,''),(11161,2,4,8,''),(11161,16,404,0,''),(11162,2,4,5,''),(11162,16,404,0,''),(11163,2,4,10,''),(11163,16,404,0,''),(11164,2,4,3,''),(11164,16,404,0,''),(11165,2,4,7,''),(11165,16,404,0,''),(11167,16,404,0,''),(11168,16,404,0,''),(11178,16,404,0,''),(11179,16,404,0,''),(11180,16,404,0,''),(11181,16,404,0,''),(11358,0,0,0,''),(11402,23,169,0,''),(11418,0,0,0,''),(11419,0,0,0,''),(11487,0,0,0,''),(11488,0,0,0,''),(11492,0,0,0,''),(11493,0,0,0,''),(11494,0,0,0,''),(11495,0,0,0,''),(11497,1,35273,0,''),(11498,1,34775,0,''),(11500,1,34793,0,''),(11501,1,35069,0,''),(11718,0,0,0,''),(11719,0,0,0,''),(11779,18,0,0,''),(11780,18,0,0,''),(11789,18,0,0,''),(11801,18,0,0,''),(11802,18,0,0,''),(11838,18,0,0,''),(11839,18,0,0,''),(11861,18,0,0,''),(11862,18,0,0,''),(11902,0,0,0,''),(11903,0,0,0,''),(11959,0,0,0,''),(12018,18,0,0,''),(12019,18,0,0,''),(12059,0,0,0,''),(12060,11,0,0,'achievement_bg_ic_resource_glut'),(12061,11,0,0,'achievement_bg_ic_resource_glut'),(12062,11,0,0,'achievement_bg_control_all_nodes'),(12063,11,0,0,'achievement_bg_control_all_nodes'),(12064,11,0,0,'achievement_bg_control_all_nodes'),(12065,11,0,0,'achievement_bg_control_all_nodes'),(12066,0,5,0,''),(12067,0,5,0,''),(12068,11,0,0,'achievement_bg_ic_mowed_down'),(12114,0,0,0,''),(12163,0,0,0,''),(12178,1,34802,0,''),(12179,1,34775,0,''),(12181,1,34793,0,''),(12182,1,34776,0,''),(12183,11,0,0,'achievement_bg_ic_glaive_grave'),(12199,0,0,0,''),(12200,0,0,0,''),(12202,0,0,0,''),(12203,0,0,0,''),(12204,0,0,0,''),(12205,0,0,0,''),(12207,0,0,0,''),(12208,0,0,0,''),(12228,0,0,0,''),(12229,0,0,0,''),(12230,0,0,0,''),(12231,0,0,0,''),(12232,0,0,0,''),(12233,0,0,0,''),(12234,0,0,0,''),(12235,0,0,0,''),(12240,0,0,0,''),(12241,0,0,0,''),(12242,0,0,0,''),(12243,0,0,0,''),(12244,0,0,0,''),(12245,0,0,0,''),(12246,0,0,0,''),(12247,0,0,0,''),(12350,18,0,0,''),(12358,18,0,0,''),(12359,18,0,0,''),(12398,5,58818,0,''),(12398,16,201,0,''),(12562,24,66,0,''),(12565,18,0,0,''),(12566,18,0,0,''),(12568,18,0,0,''),(12569,18,0,0,''),(12578,0,0,0,''),(12658,1,4949,0,''),(12660,1,16802,0,''),(12661,1,10181,0,''),(12662,1,3057,0,''),(12663,1,29611,0,''),(12664,1,2784,0,''),(12666,1,7999,0,''),(12667,1,17468,0,''),(12678,0,0,0,''),(12679,0,0,0,''),(12680,0,0,0,''),(12682,0,0,0,''),(12683,0,0,0,''),(12684,0,0,0,''),(12685,0,0,0,''),(12686,0,0,0,''),(12687,0,0,0,''),(12688,0,0,0,''),(12689,0,0,0,''),(12740,0,0,0,''),(12741,0,0,0,''),(12742,0,0,0,''),(12744,0,0,0,''),(12747,0,0,0,''),(12748,0,0,0,''),(12749,0,0,0,''),(12750,0,0,0,''),(12752,11,0,0,'achievement_bronjahm_soul_power'),(12775,18,0,0,''),(12777,11,0,0,'achievement_im_on_a_boat'),(12778,11,0,0,'achievement_ive_gone_and_made_a_mess'),(12780,11,0,0,'achievement_once_bitten_twice_shy_n'),(12798,0,0,0,''),(12799,0,0,0,''),(12800,0,0,0,''),(12802,0,0,0,''),(12803,0,0,0,''),(12804,0,0,0,''),(12805,0,0,0,''),(12806,0,0,0,''),(12807,0,0,0,''),(12808,0,0,0,''),(12809,0,0,0,''),(12822,11,0,0,'achievement_all_you_can_eat'),(12823,11,0,0,'achievement_neck_deep_in_vile'),(12846,16,423,0,''),(12859,1,9099,0,''),(12859,11,0,0,'achievement_flirt_with_disaster_perf_check'),(12859,15,3,0,''),(12912,23,177,0,''),(12962,18,0,0,''),(12966,18,0,0,''),(12967,11,0,0,'achievement_flu_shot_shortage'),(12968,18,0,0,''),(12969,18,0,0,''),(12971,11,0,0,'achievement_portal_jockey'),(12972,11,0,0,'achievement_all_you_can_eat'),(12976,11,0,0,'achievement_three_faced'),(12977,11,0,0,'achievement_flu_shot_shortage'),(12978,11,0,0,'achievement_portal_jockey'),(12979,11,0,0,'achievement_portal_jockey'),(12980,11,0,0,'achievement_portal_jockey'),(12981,18,0,0,''),(12982,11,0,0,'achievement_flu_shot_shortage'),(12983,18,0,0,''),(12984,18,0,0,''),(12985,18,0,0,''),(12986,11,0,0,'achievement_flu_shot_shortage'),(12987,18,0,0,''),(12988,18,0,0,''),(12989,11,0,0,'achievement_all_you_can_eat'),(12990,0,0,0,''),(12991,0,0,0,''),(12992,0,0,0,''),(12993,0,0,0,''),(12993,11,0,0,'achievement_doesnt_go_to_eleven'),(12996,11,0,0,'achievement_all_you_can_eat'),(12999,23,167,0,''),(13000,23,169,0,''),(13001,23,157,0,''),(13002,23,167,0,''),(13003,23,169,0,''),(13004,23,167,0,''),(13005,23,169,0,''),(13006,23,167,0,''),(13007,23,169,0,''),(13011,11,0,0,'achievement_once_bitten_twice_shy_v'),(13012,11,0,0,'achievement_once_bitten_twice_shy_n'),(13013,11,0,0,'achievement_once_bitten_twice_shy_v'),(13032,18,0,0,''),(13033,18,0,0,''),(13034,18,0,0,''),(13035,11,0,0,'achievement_ive_gone_and_made_a_mess'),(13036,11,0,0,'achievement_ive_gone_and_made_a_mess'),(13037,11,0,0,'achievement_ive_gone_and_made_a_mess'),(13079,11,0,0,'achievement_im_on_a_boat'),(13080,11,0,0,'achievement_im_on_a_boat'),(13081,11,0,0,'achievement_im_on_a_boat'),(13089,0,0,0,''),(13090,0,0,0,''),(13091,0,0,0,''),(13092,0,0,0,''),(13093,0,0,0,''),(13104,0,0,0,''),(13105,0,0,0,''),(13106,0,0,0,''),(13107,0,0,0,''),(13108,0,0,0,''),(13163,11,0,0,'achievement_neck_deep_in_vile'),(13164,11,0,0,'achievement_neck_deep_in_vile'),(13166,0,0,0,''),(13167,0,0,0,''),(13168,0,0,0,''),(13169,0,0,0,''),(13170,0,0,0,''),(13172,0,0,0,''),(13173,0,0,0,''),(13174,0,0,0,''),(13175,0,0,0,''),(13176,0,0,0,''),(13177,0,0,0,''),(13178,0,0,0,''),(13179,0,0,0,''),(13182,0,0,0,''),(13243,11,0,0,'achievement_neck_deep_in_vile'),(13244,11,0,0,'achievement_been_waiting_long_time'),(13245,11,0,0,'achievement_been_waiting_long_time'),(13246,11,0,0,'achievement_been_waiting_long_time'),(13247,11,0,0,'achievement_been_waiting_long_time'),(13340,18,0,0,''),(13360,18,0,0,''),(13393,18,0,0,''),(13394,18,0,0,''),(14173,21,7,0,''),(14175,21,11,0,''),(14176,21,2,0,''),(14177,21,5,0,''),(14178,21,9,0,''),(14179,21,3,0,''),(14180,21,6,0,''),(14181,21,8,0,''),(14182,21,1,0,''),(14183,21,4,0,''),(15943,11,0,0,'achievement_crushing_bones_and_cracking_skulls'),(15944,11,0,0,'achievement_arrested_development'),(15947,11,0,0,'achievement_too_hot_to_handle'),(15989,11,0,0,'achievement_straw_broke_camels_back'),(16082,24,67,0,''),(16848,11,0,0,'achievement_ohganot_so_fast'),(17577,11,0,0,'achievement_share_the_pain'),(17804,16,467,0,''),(18114,23,279,0,''),(18115,23,279,0,''),(18116,23,279,0,''),(18117,23,279,0,''),(18144,23,279,0,''),(18488,23,280,0,''),(18500,16,484,0,''),(18842,23,280,0,''),(18843,23,280,0,''),(18844,23,281,0,''),(18845,23,280,0,''),(18846,23,281,0,''),(18847,23,280,0,''),(18848,23,281,0,''),(18849,23,281,0,'');
/*!40000 ALTER TABLE `achievement_criteria_data` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-07-15 10:33:49
