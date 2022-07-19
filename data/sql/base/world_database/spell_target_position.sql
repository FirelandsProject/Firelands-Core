CREATE DATABASE  IF NOT EXISTS `world` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `world`;
-- MySQL dump 10.13  Distrib 8.0.29, for macos12 (x86_64)
--
-- Host: 127.0.0.1    Database: world
-- ------------------------------------------------------
-- Server version	5.6.51

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
-- Table structure for table `spell_target_position`
--

DROP TABLE IF EXISTS `spell_target_position`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `spell_target_position` (
  `id` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'The spell identifier. The value has to match with a defined spell identifier.',
  `target_map` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT 'The target map''s identifier.',
  `target_position_x` float NOT NULL DEFAULT '0' COMMENT 'The X position on the target map.',
  `target_position_y` float NOT NULL DEFAULT '0' COMMENT 'The Y position on the target map.',
  `target_position_z` float NOT NULL DEFAULT '0' COMMENT 'The Z position on the target map.',
  `target_orientation` float NOT NULL DEFAULT '0' COMMENT 'The orientation for the character on the target map.',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Spell System';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_target_position`
--

LOCK TABLES `spell_target_position` WRITE;
/*!40000 ALTER TABLE `spell_target_position` DISABLE KEYS */;
INSERT INTO `spell_target_position` VALUES (31,0,-9464,62,56,0),(33,0,-10643,1052,34,0),(34,0,-10368,-422,66,0),(35,0,-9104,-70,83,0),(427,189,1688.99,1053.48,18.6775,0.00117),(428,0,-11020,1436,44,0),(442,129,2592.55,1107.5,51.29,4.74),(443,0,16229,16265,14,4.74),(444,1,-4658.12,-2526.35,82.9671,0),(445,0,-10566,-1189,28,0),(446,109,-319.24,99.9,-131.85,3.19),(447,0,16229,16265,14,3.19),(518,531,-8028.5,1050.9,-54,0),(720,531,-8043.01,1254.21,-84.19,0),(731,531,-8003,1222.9,-82.1,0),(1121,531,-8022.68,1150.08,-89.33,0),(1936,0,16229,16265,14,0),(3561,0,-9003.46,870.031,29.6206,5.28),(3562,0,-4613.62,-915.38,501.062,3.88),(3563,0,1773.47,61.121,-46.3207,0.54),(3565,1,9664.14,2526.36,1334.27,1.84454),(3566,1,-964.98,283.433,111.187,3.02),(3567,1,1469.85,-4221.52,58.9939,5.98),(3721,0,16229,16265,14,3.19),(4801,531,-8340.78,2083.81,125.649,0),(4996,1,1552.5,-4420.66,8.94802,0),(4997,0,-14457,496.45,39.1392,0),(4998,0,-12415,207.618,31.5017,0.124875),(4999,1,-998.359,-3827.52,5.44507,4.16654),(6348,0,-3752.81,-851.558,10.1153,0),(6349,1,6581.05,767.5,5.78428,6.01616),(6483,1,5483.9,-749.881,334.621,0),(6714,1,-4884.49,-1596.2,101.2,3.17),(6719,1,-3615.49,-4467.34,24.3141,0),(6766,1,-2354.03,-1902.07,95.78,4.6),(7136,33,-105.654,2154.98,156.43,1.24782),(7586,33,-84.99,2151.01,155.62,1.11623),(7587,33,-103.46,2122.1,155.655,4.4224),(8136,43,116.35,239.8,-95.08,3.1),(8195,531,-8341.55,2118.5,133.058,0),(8606,0,325.8,-1481.47,42.68,0.47),(8735,1,4139.14,883.43,-0.87,4.53),(8995,0,-2646.62,-2455.57,80.87,0),(8996,0,-2645.25,-2449.5,82.65,0),(8997,0,-2648.88,-2442.54,80.71,0),(9002,0,-2625,-2423,79,0),(9003,0,-2607,-2434,80,0),(9004,0,-2634,-2411,81,0),(9055,0,-1350.83,-2740.16,62.74,2.95),(9268,1,9664.01,2526.15,1332.16,0),(11012,1,161,299.05,-51.36,0.37),(11362,0,-5095,757,261,4.42),(11409,0,-14462,460,16,3.49),(11511,90,-530.754,670.571,-313.784,0),(11795,90,-530.754,670.571,-313.784,0),(12158,90,-478.448,-89.3074,-146.602,0),(12159,90,-562.7,-121.674,-150.862,0),(12241,1,-3254.76,2022.67,250.716,4.15),(12510,1,3778,-4612,228,4.23263),(12520,1,3641,-4702,121,6.25),(12885,0,-11238.4,-2831.97,157.93,5.11),(13044,0,-11224.8,-2835.35,158.85,3.73),(13142,0,-11238.4,-2831.97,157.93,5.11),(13461,0,-11234.8,-2842.52,157.92,1.47),(13912,230,1380.58,-819.48,-92.72,4.69),(14208,1,-6025.75,-1021.17,-216,0),(15737,230,611.073,-222.201,-52.6412,0),(15739,230,600.752,-225.146,-52.6315,0),(15740,230,630.355,-174.342,-52.6395,0),(15741,230,633.182,-184.641,-52.6346,0),(16572,0,-6046.85,-207.329,418.304,0),(16767,0,-6046.73,-207.159,417.267,0),(16768,0,-6051.39,-206.749,418.159,0),(16772,0,-6046.84,-204.535,416.897,0),(16775,0,-6044.47,-205.783,417.267,0),(16776,0,-6044.54,-205.562,417.663,0),(16777,0,-6044.53,-202.912,416.756,0),(16778,0,-6042.18,-204.949,416.336,0),(16779,0,-6042.02,-204.134,416.894,0),(16780,0,-6044.79,-196.959,419.19,0),(16786,0,-6078.3,-211.89,424.197,0),(16787,229,-20.469,-379.33,48.981,6.058),(17086,249,25,-216.025,-84.874,3.142),(17087,249,15,-216.025,-85.8052,3.142),(17088,249,5,-216.025,-86.0683,3.142),(17089,249,-5,-216.025,-86.9675,3.142),(17090,249,-15,-216.025,-88.3522,3.142),(17091,249,-25,-216.025,-89.2463,3.142),(17092,249,-35,-216.025,-87.4351,3.142),(17093,249,-45,-216.025,-86.4653,3.142),(17094,249,-55,-216.025,-85.3717,3.142),(17095,249,-65,-216.025,-84.3052,3.142),(17097,249,-75,-216.025,-83.395,3.142),(17159,1,6106.4,-4191.1,849.74,0.0201),(17160,1,5904.2,-4045.9,596.43,0.0083),(17237,329,3838,-3500,195,0),(17239,329,3847,-3748,195,0),(17240,329,4057,-3665,182,0),(17278,329,3533.95,-2965.1,125,0),(17334,0,-9003.46,870.031,29.6206,2.28),(17475,329,4012.92,-3365.7,116.251,5.48),(17476,329,4009.1,-3352.31,116.712,0.13),(17477,329,4013.96,-3338.65,116.242,0.72),(17478,329,4051.75,-3339.05,116.241,3.62),(17479,329,4055.96,-3351.46,116.586,3.06),(17480,329,4053.11,-3364.98,116.402,2.45),(17607,0,-4613.62,-915.38,501.062,3.88),(17608,1,9660.81,2513.64,1331.66,3.06),(17609,1,1469.85,-4221.52,58.9939,5.98),(17610,1,-964.98,283.433,111.187,3.02),(17611,0,1773.47,61.121,-46.3207,0.54),(17863,289,274.877,1.33366,85.3117,3.22886),(17939,289,182.246,-95.4371,85.3117,1.5694),(17943,289,86.6634,-1.96039,85.3117,0.017453),(17944,289,265.956,0.903429,75.3334,3.07178),(17946,289,178.724,-91.0232,70.8568,1.64061),(17948,289,103.305,-1.67752,75.2183,6.17846),(18351,249,-68.8342,-215.036,-84.0189,6.28),(18352,249,-61.8343,-215.052,-84.6734,6.28),(18353,249,-53.3433,-215.071,-85.5972,6.28),(18354,249,-42.6193,-215.095,-86.6636,6.28),(18355,249,-35.8993,-215.11,-87.1965,6.28),(18356,249,-28.2483,-215.127,-89.1917,6.28),(18357,249,-20.3244,-215.145,-88.964,6.28),(18358,249,-11.1894,-215.166,-87.8171,6.28),(18359,249,-2.04741,-215.186,-86.2797,6.28),(18360,249,7.47957,-215.208,-86.0755,6.28),(18361,249,20.7305,-215.238,-85.2544,6.28),(18564,249,-56.5597,-241.224,-85.4236,0.666),(18565,249,-51.0675,-228.91,-85.7656,0.666),(18566,249,-41.7621,-221.897,-86.1141,0.666),(18567,249,-35.4399,-217.26,-87.3363,0.666),(18568,249,-29.4959,-213.014,-88.9104,0.666),(18569,249,-23.6503,-221.969,-89.1727,0.666),(18570,249,-15.6021,-216.894,-88.4032,0.666),(18571,249,-14.3212,-199.462,-87.9225,0.666),(18572,249,-8.4493,-193.958,-87.565,0.666),(18573,249,-2.52965,-188.69,-87.1729,0.666),(18574,249,3.86022,-183.227,-86.3754,0.666),(18575,249,6.01671,-181.306,-85.6546,0.666),(18576,249,-37.7439,-243.668,-88.2177,1.416),(18578,249,-35.8053,-232.029,-87.7492,1.416),(18579,249,-34.0457,-224.715,-85.5295,1.416),(18580,249,-32.0816,-214.917,-88.3274,1.416),(18581,249,-36.6117,-202.685,-85.6538,1.416),(18582,249,-37.0673,-195.759,-87.7458,1.416),(18583,249,-37.7285,-188.617,-88.0749,1.416),(18584,249,6.01671,-181.306,-85.6546,3.776),(18585,249,3.86022,-183.227,-86.3754,3.776),(18586,249,-2.52965,-188.69,-87.1729,3.776),(18587,249,-8.4493,-193.958,-87.565,3.776),(18588,249,-14.3212,-199.462,-87.9225,3.776),(18589,249,-15.6021,-216.894,-88.4032,3.776),(18590,249,-23.6503,-221.969,-89.1727,3.776),(18591,249,-29.4959,-213.014,-88.9104,3.776),(18592,249,-35.4399,-217.26,-87.3363,3.776),(18593,249,-41.7621,-221.897,-86.1141,3.776),(18594,249,-51.0675,-228.91,-85.7656,3.776),(18595,249,-56.5597,-241.224,-85.4236,3.776),(18596,249,-58.2509,-189.02,-85.2923,5.587),(18597,249,-52.0063,-193.797,-85.8088,5.587),(18598,249,-46.1355,-198.549,-85.9018,5.587),(18599,249,-40.5002,-203.001,-85.5551,5.587),(18600,249,-30.9076,-211.058,-88.5921,5.587),(18601,249,-20.0981,-218.681,-88.9371,5.587),(18602,249,-12.2232,-224.666,-87.8563,5.587),(18603,249,-6.4753,-229.099,-87.0764,5.587),(18604,249,-2.01026,-232.542,-86.9951,5.587),(18605,249,2.7363,-236.202,-86.7904,5.587),(18606,249,7.19778,-239.643,-86.3073,5.587),(18607,249,12.1209,-243.439,-85.8743,5.587),(18609,249,-37.7285,-188.617,-88.0749,4.526),(18611,249,-37.0673,-195.759,-87.7458,4.526),(18612,249,-36.6117,-202.685,-85.6538,4.526),(18613,249,-32.0816,-214.917,-88.3274,4.526),(18614,249,-34.0457,-224.715,-85.5295,4.526),(18615,249,-35.8053,-232.029,-87.7492,4.526),(18616,249,-37.7439,-243.668,-88.2177,4.526),(18617,249,12.1209,-243.439,-85.8743,2.428),(18618,249,-58.2509,-189.02,-85.2923,2.428),(18619,249,7.19778,-239.643,-86.3073,2.428),(18620,249,2.7363,-236.202,-86.7904,2.428),(18621,249,-2.01026,-232.542,-86.9951,2.428),(18622,249,-6.4753,-229.099,-87.0764,2.428),(18623,249,-12.2232,-224.666,-87.8563,2.428),(18624,249,-20.0981,-218.681,-88.9371,2.428),(18625,249,-30.9076,-211.058,-88.5921,2.428),(18626,249,-40.5002,-203.001,-85.5551,2.428),(18627,249,-46.1355,-198.549,-85.9018,2.428),(18628,249,-52.0063,-193.797,-85.8088,2.428),(18634,0,-6076,-215,424,0),(18960,1,7991.88,-2679.84,512.1,0.034183),(20449,531,-8318.82,2058.23,133.058,0),(20534,409,736.88,-1175.69,-119.094,1),(20618,409,736.88,-1175.69,-119.094,1),(20682,1,-4018.1,-4525.24,12,6.13),(21128,349,386.27,33.4144,-130.934,0),(21132,249,-120.471,-214.335,-71.9481,3.142),(21133,249,-142.719,-213.206,-68.8335,3.142),(21135,249,-162.585,-205.683,-68.7282,3.142),(21136,249,-194.92,-206.546,-68.7282,3.142),(21137,249,-173.573,-186.406,-66.1293,3.142),(21138,249,-164.067,-164.104,-59.5405,3.142),(21139,249,-144.485,-149.742,-53.8462,3.142),(22191,249,-42.4794,-270.487,-92.11,0),(22192,249,-59.0983,-269.976,-94.3079,0),(22193,249,-72.5971,-264.395,-94.267,0),(22194,249,-88.7517,-259.617,-93.7426,0),(22195,249,-104.072,-244.79,-94.0876,0),(22196,249,-106.875,-223.247,-94.6765,0),(22197,249,-107.076,-197.489,-94.1113,0),(22198,249,-102.751,-181.577,-94.06,0),(22199,249,-89.9793,-171.261,-93.8249,0),(22200,249,-72.6705,-164.756,-94.2678,0),(22201,249,-50.8834,-159.992,-93.6519,0),(22202,249,-40.259,-158.961,-91.75,0),(22267,249,-85,-216.025,-82.5516,3.142),(22268,249,-96.8227,-214.425,-82.2065,3.142),(22563,30,-1345.82,-289.861,90.8835,5.19479),(22564,30,665.02,-27.7919,50.6199,3.36559),(22651,429,-25.98,-448.24,-36.09,3.13),(22668,469,-7581.11,-1216.19,476.8,0),(22669,469,-7542.47,-1191.92,476.355,0),(22670,469,-7506.58,-1165.26,476.796,0),(22671,469,-7561.54,-1244.01,476.8,0),(22674,469,-7538.63,-1273.64,476.8,0),(22675,469,-7500.7,-1249.89,476.798,0),(22676,469,-7469.93,-1227.93,476.777,0),(22950,429,-37.5643,813.433,-7.43823,4.77),(22951,1,-3760.87,1086.28,131.96,1.53),(22972,469,-7538.63,-1277.56,476.799,0),(22975,469,-7557.95,-1249.87,476.799,0),(22976,469,-7579.59,-1218.76,476.799,0),(22977,469,-7599.83,-1190.94,475.472,0),(22978,469,-7497.51,-1249.17,476.798,0),(22979,469,-7519.9,-1218.85,476.799,0),(22980,469,-7540.82,-1190.7,476.355,0),(22981,469,-7461.41,-1226.29,476.781,0),(22982,469,-7483.16,-1195.39,476.799,0),(22983,469,-7504.77,-1164.43,476.797,0),(22984,469,-7524.15,-1138.16,473.348,0),(22985,469,-7538.63,-1277.56,476.799,0),(23441,1,-7109.1,-3825.21,10.151,2.8331),(23442,1,6755.33,-4658.09,724.8,3.4049),(23446,1,-7303,-3545,137,2.8331),(23460,469,-7672.46,-1107.19,396.65,0.59),(24228,309,-11516.1,-1605.31,41.3,0),(24325,309,-11726.3,-1777.38,10.46,0.16),(24593,309,-11726.3,-1777.38,10.46,0.16),(24730,1,-1327.66,85.9815,134.169,0),(25004,1,7738.52,-2314.8,452.679,0),(25139,409,1096,-467,-104.6,3.64),(25182,1,-8124.41,1525.13,16.368,3.05),(25725,509,-9717.2,1517.81,27.6,5.4),(25831,531,-8158.03,1139.3,-83.95,0),(25832,531,-8029.25,1237.78,-85.2285,0),(25865,531,-8023.59,964.772,-41.229,5.376),(25866,531,-8042.13,911.263,-42.841,6.2),(25867,531,-7999.36,860.525,-47.206,1.417),(25868,531,-7971.3,862.581,-48.099,2.148),(25869,531,-7943.21,903.804,-48.473,3.078),(25870,531,-7954.38,958.562,-41.609,3.962),(25871,531,-7997.19,979.192,-41.653,4.896),(25872,531,-8037.89,929.679,-43.416,6.024),(25873,531,-8015.41,867.734,-45.607,1.103),(25874,531,-7982.8,857.172,-48.212,1.5),(25875,531,-7952.12,883.183,-48.194,2.43),(25876,531,-7947.22,939.122,-44.014,3.718),(25877,531,-7975.77,974.82,-41.584,4.417),(25878,531,-8032.75,948.274,-41.919,5.595),(25879,531,-8037.08,887.268,-43.581,0.675),(25880,531,-7992.21,857.5,-47.762,1.664),(25881,531,-7960.71,872.483,-48.759,2.36),(25882,531,-7942.98,918.616,-46.401,3.306),(25883,531,-7964.32,967.879,-42.112,4.087),(25884,531,-8015.24,976.553,-41.647,4.947),(26220,531,-8563.56,2040.69,-97,4.9426),(26448,1,7571.95,-2202.04,474.86,5.4358),(26450,1,10150.1,2602,1330.83,4.80894),(26452,0,-4663,-955.663,500.377,0.722862),(26453,1,1982.9,-4255.62,31.6692,3.31984),(26454,0,-8748.48,1073.61,90.4898,5.03789),(26455,1,-1031.45,-230.867,160.14,3.28358),(26456,0,1642.01,239.002,62.5918,3.02316),(28025,533,2692,-3399.27,267.686,4.70454),(28026,533,2692,-3321.86,267.686,2.19126),(28147,0,-11123.3,-2014.44,47.0927,0.675977),(28280,533,3125.44,-3309.74,293.251,3.14929),(28444,533,3005.74,-3434.27,304.196,0),(29181,531,-8971.81,1321.47,-104.249,0),(29190,531,-8632.84,2055.87,108.86,0),(29216,533,2633.49,-3529.56,274.111,0),(29217,533,2646.37,-3466.23,263.098,6.17666),(29224,533,2704.86,-3458.79,264.067,5.43839),(29225,533,2725.71,-3463.45,263.415,5.45017),(29226,533,2724.76,-3514.27,263.067,2.87018),(29227,533,2663.92,-3464.7,262.574,5.15958),(29231,533,2685.06,-3502.37,261.315,0),(29237,533,2648.83,-3467.3,262.399,5.46297),(29238,533,2684.95,-3457.55,262.578,4.62159),(29239,533,2706.78,-3461.59,262.517,4.00358),(29247,533,2646.37,-3466.23,263.098,5.46297),(29248,533,2704.86,-3458.79,264.067,4.31236),(29249,533,2724.76,-3514.27,263.067,3.09084),(29255,533,2704.22,-3461.86,262.546,4.37027),(29256,533,2663.7,-3461.9,263.763,5.07774),(29257,533,2704.86,-3458.79,264.067,3.85584),(29258,533,2722.03,-3514.3,262.354,2.84269),(29262,533,2727.58,-3535.23,263.012,2.60314),(29267,533,2711.72,-3458.56,262.384,4.02864),(29268,533,2646.93,-3470.95,262.427,5.51363),(29273,533,3498.28,-5349.9,144.968,1.31324),(29318,533,3493.45,-5375.38,138.168,0),(30211,533,2794.61,-3707.68,276.552,2.32);
/*!40000 ALTER TABLE `spell_target_position` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-07-19  0:45:46
