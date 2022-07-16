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
-- Table structure for table `npc_spellclick_spells`
--

DROP TABLE IF EXISTS `npc_spellclick_spells`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `npc_spellclick_spells` (
  `npc_entry` int unsigned NOT NULL COMMENT 'reference to creature_template',
  `spell_id` int unsigned NOT NULL COMMENT 'spell which should be casted ',
  `cast_flags` tinyint unsigned NOT NULL COMMENT 'first bit defines caster: 1=player, 0=creature; second bit defines target, same mapping as caster bit',
  `user_type` smallint unsigned NOT NULL DEFAULT '0' COMMENT 'relation with summoner: 0-no 1-friendly 2-raid 3-party player can click',
  PRIMARY KEY (`npc_entry`,`spell_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `npc_spellclick_spells`
--

LOCK TABLES `npc_spellclick_spells` WRITE;
/*!40000 ALTER TABLE `npc_spellclick_spells` DISABLE KEYS */;
INSERT INTO `npc_spellclick_spells` VALUES (14864,104517,1,0),(24083,55074,1,0),(24418,43768,1,0),(24750,46598,1,0),(24992,45008,0,0),(25334,46598,1,0),(25460,46598,1,0),(25596,45875,0,0),(25743,46260,1,0),(25762,43671,1,0),(25765,43671,1,0),(25968,46598,1,0),(26048,46407,0,0),(26191,46978,2,0),(26200,39996,1,0),(26200,61286,2,0),(26421,47575,1,0),(26477,61832,1,0),(26503,47121,0,0),(26523,48296,1,0),(26572,46598,1,0),(26809,47416,0,0),(26813,47424,1,0),(27061,47920,1,0),(27131,48754,1,0),(27213,43671,0,0),(27241,46598,1,0),(27258,48365,1,0),(27354,43671,1,0),(27409,48678,1,0),(27496,48881,1,0),(27587,46598,1,1),(27587,49078,1,0),(27593,49177,1,0),(27626,49138,1,0),(27661,48365,1,0),(27714,49584,1,0),(27761,43671,1,0),(27881,60968,1,0),(27894,60682,1,0),(27923,46598,1,1),(27924,50007,1,0),(27996,50343,1,0),(28018,46598,1,0),(28054,50556,1,0),(28061,50557,1,0),(28094,60968,1,0),(28161,51037,0,0),(28162,51026,1,0),(28192,46598,1,0),(28202,50926,1,0),(28203,50918,1,0),(28222,52082,1,0),(28312,46598,1,0),(28312,60968,1,0),(28319,60968,1,0),(28366,60962,1,0),(28379,51658,0,0),(28389,51592,1,0),(28389,51593,3,0),(28468,46598,1,0),(28605,52263,1,0),(28606,52263,1,0),(28607,52263,1,0),(28614,46598,1,0),(28669,46598,0,1),(28670,53173,1,0),(28710,46598,1,1),(28781,60683,1,0),(28782,52280,1,0),(28817,52462,1,0),(28833,52447,1,0),(28851,52600,1,0),(28864,52589,1,0),(29005,60944,1,0),(29152,83580,1,0),(29351,46598,0,0),(29358,46598,0,0),(29403,49641,1,0),(29414,18277,1,0),(29433,47020,1,0),(29460,46598,1,0),(29488,54568,3,0),(29500,46598,1,0),(29501,54575,3,0),(29555,47020,1,0),(29563,56795,2,0),(29579,46598,1,1),(29598,54768,1,0),(29602,54908,1,0),(29625,46598,1,0),(29679,54952,1,0),(29698,46598,1,0),(29708,55028,1,0),(29709,55029,1,0),(29736,43671,1,0),(29838,46598,1,0),(29856,55364,1,0),(29857,55460,3,0),(29863,46598,0,1),(29884,55430,1,0),(29912,55479,3,0),(29918,54301,1,0),(29929,58961,1,0),(29931,46598,1,0),(30013,43671,1,0),(30021,46598,1,0),(30021,55785,3,0),(30066,57053,1,0),(30108,46598,1,0),(30123,55957,3,0),(30134,43671,1,1),(30134,56603,1,0),(30174,46598,1,0),(30204,46598,1,0),(30234,61421,0,0),(30236,57573,1,0),(30248,61421,0,0),(30272,57401,1,0),(30330,46598,1,0),(30337,43671,1,0),(30388,46598,1,0),(30393,46598,1,0),(30403,56699,1,0),(30468,56795,1,0),(30470,56839,1,0),(30500,56922,2,0),(30560,57347,1,1),(30564,57401,1,0),(30585,57418,1,0),(30645,57612,1,0),(30841,57897,2,1),(30850,57897,2,1),(30852,57897,2,1),(30895,46598,1,0),(30935,46598,1,0),(31050,46598,1,0),(31110,46598,1,0),(31137,46598,1,0),(31157,46598,1,0),(31262,46598,1,0),(31269,46598,1,0),(31406,46598,1,0),(31407,46598,1,0),(31408,46598,1,0),(31409,46598,1,0),(31432,59091,1,0),(31583,46598,1,0),(31736,59592,1,0),(31748,56378,1,0),(31749,61421,1,0),(31770,59654,1,0),(31784,46598,1,0),(31784,59593,1,0),(31785,46598,1,0),(31785,59656,1,0),(31830,46598,1,0),(31840,46598,1,0),(31856,46598,1,0),(31861,61466,1,0),(31862,61466,1,0),(31881,46598,1,0),(31883,60123,2,2),(31884,46598,1,0),(31893,60123,2,2),(31894,60123,2,2),(31895,60123,2,2),(31896,60123,2,2),(31897,60123,2,2),(32152,46598,1,0),(32189,46598,1,0),(32225,46598,1,0),(32227,46598,1,0),(32286,61666,1,0),(32292,46598,1,0),(32344,46598,1,0),(32370,59724,1,0),(32490,46598,1,0),(32513,46598,1,0),(32535,61245,1,0),(32627,46598,1,0),(32627,60968,1,0),(32629,60968,1,0),(32633,61424,1,0),(32640,61424,1,0),(32788,57539,1,1),(32790,57654,1,1),(32795,60682,1,0),(32796,60683,1,0),(32823,46598,1,0),(32830,46598,1,0),(32840,46598,1,0),(32930,65343,0,0),(33060,65031,1,0),(33062,65030,1,0),(33067,65031,1,0),(33109,62309,1,0),(33113,46598,1,0),(33114,46598,1,0),(33139,46598,1,0),(33143,62399,0,0),(33167,46598,1,0),(33214,46598,1,0),(33217,63151,1,0),(33293,63852,1,0),(33316,63151,1,0),(33317,63151,1,0),(33318,63151,1,0),(33319,63151,1,0),(33320,63151,1,0),(33321,63151,1,0),(33322,63151,1,0),(33323,63151,1,0),(33324,63151,1,0),(33357,75648,1,0),(33432,46598,1,1),(33498,63126,2,0),(33519,63163,1,0),(33651,46598,1,1),(33669,46598,1,0),(33687,46598,1,0),(33778,43671,1,0),(33782,63151,1,0),(33790,62781,1,0),(33791,62786,1,0),(33792,62785,1,0),(33793,62780,1,0),(33794,62782,1,0),(33795,62779,1,0),(33796,62784,1,0),(33798,62787,1,0),(33799,62783,1,0),(33800,62774,1,0),(33842,63791,1,0),(33843,63792,1,0),(33844,63151,1,0),(33845,63151,1,0),(33870,63663,1,0),(34045,65030,1,0),(34072,51347,3,0),(34111,46598,1,0),(34120,55089,1,0),(34125,63215,1,0),(34146,43671,1,0),(34150,43671,1,0),(34151,43671,1,0),(34161,46598,1,0),(34162,46598,1,0),(34282,46598,0,0),(34567,65684,1,0),(34568,65686,1,0),(34658,46598,0,0),(34775,66245,1,0),(34776,46598,1,0),(34776,66245,1,0),(34777,66245,1,0),(34778,66245,1,0),(34793,66245,1,0),(34796,46598,0,0),(34802,68503,1,0),(34812,65403,1,0),(34819,65403,1,0),(34822,65403,1,0),(34823,65403,1,0),(34824,65403,1,0),(34830,66306,1,0),(34929,43671,1,0),(34935,43671,1,0),(34944,68458,1,0),(35069,46598,1,0),(35069,66245,1,0),(35111,66778,1,0),(35231,46598,1,0),(35273,68503,1,0),(35299,46598,1,0),(35317,43671,1,0),(35410,43671,1,0),(35413,66245,1,0),(35415,66245,1,0),(35417,66245,1,0),(35419,68503,1,0),(35421,68503,1,0),(35427,43671,1,0),(35429,68458,1,0),(35431,46598,1,0),(35431,66245,1,0),(35433,46598,1,0),(35433,66245,1,0),(35436,66245,1,0),(35491,46598,0,0),(35633,46598,0,0),(35634,46598,0,0),(35635,46598,0,0),(35636,46598,0,0),(35637,46598,0,0),(35638,46598,0,0),(35640,46598,0,0),(35641,46598,0,0),(35644,67830,1,0),(35768,46598,0,0),(35995,46598,1,0),(36059,68291,3,0),(36059,68293,1,0),(36283,46598,0,1),(36283,69434,1,0),(36287,68597,1,0),(36288,68598,1,0),(36289,68596,1,0),(36355,66245,1,0),(36356,66245,1,0),(36357,66245,1,0),(36358,66245,1,0),(36437,68726,1,0),(36440,68735,1,0),(36459,68743,1,0),(36476,46598,1,0),(36540,94654,1,0),(36558,67830,1,0),(36661,46598,1,0),(36678,46598,1,0),(36794,46598,1,0),(36812,69342,1,0),(36838,70510,1,0),(36839,70510,1,0),(36891,46598,1,0),(36896,46598,1,0),(36917,81432,1,0),(36972,46598,0,0),(37139,46598,1,0),(37198,70036,0,0),(37227,70104,3,0),(37598,46598,1,0),(37807,69434,1,0),(37945,70766,3,0),(37966,69342,1,0),(37968,46598,1,0),(37980,69342,1,0),(38111,71170,1,0),(38150,71238,1,0),(38186,70766,3,0),(38248,71462,1,2),(38377,69434,1,0),(38412,71916,1,0),(38424,43671,1,0),(38429,70766,3,0),(38430,70766,3,0),(38500,46598,1,0),(38526,72240,1,0),(38615,72472,1,0),(38755,72767,1,0),(38918,46598,1,0),(38929,46598,1,0),(38933,73123,1,0),(38936,46598,0,0),(39039,46598,1,0),(39128,76649,1,0),(39140,73518,1,0),(39294,80343,1,1),(39443,89397,1,0),(39456,73947,1,0),(39592,73991,1,0),(39619,90102,1,0),(39714,74205,1,0),(39789,46598,1,0),(39833,46598,1,0),(39839,46598,1,0),(39867,94656,1,0),(40081,46598,0,1),(40121,46598,1,0),(40176,74904,1,0),(40176,74905,3,0),(40190,46598,1,0),(40240,75140,3,0),(40250,97400,1,0),(40305,46598,1,0),(40470,46598,0,1),(40471,46598,0,1),(40472,46598,0,1),(40573,46598,1,0),(40650,46598,1,0),(40725,75953,1,0),(40825,69063,1,0),(41327,59099,1,0),(41442,46598,1,1),(41445,77709,0,1),(41570,77901,1,0),(41620,41020,1,0),(41776,86358,1,0),(41789,41020,1,0),(41848,43767,1,0),(42175,43767,1,0),(42204,46598,0,0),(42205,63313,0,0),(42246,63313,0,0),(42247,63313,0,0),(42255,46598,0,0),(42260,46598,1,0),(42268,46598,0,0),(42314,46598,0,0),(42339,78974,1,0),(42399,46598,0,0),(42400,46598,0,0),(42500,46598,1,0),(42548,79276,1,0),(42548,79277,2,0),(42571,94350,1,0),(42576,46598,1,0),(42601,46598,1,0),(42603,79418,1,0),(42603,90102,1,0),(42608,94350,1,0),(42644,79936,1,0),(42646,80017,1,0),(42654,46598,0,0),(42671,79568,1,0),(42673,79545,1,0),(42716,80690,1,0),(42801,79796,1,0),(42801,80633,1,0),(42839,46598,0,0),(42947,77709,0,1),(42949,77709,0,1),(42951,77709,0,1),(42954,77709,0,1),(42956,77709,0,1),(42958,77709,0,1),(42960,77709,0,1),(43032,80393,1,0),(43044,80436,1,0),(43046,80436,1,0),(43048,80446,1,0),(43115,56685,1,0),(43182,84178,1,0),(43195,46598,0,0),(43209,46598,0,0),(43259,46598,0,0),(43747,81877,1,0),(43952,82261,1,0),(43984,82359,1,0),(44049,82456,1,0),(44126,82535,1,0),(44427,67001,3,0),(44428,46598,3,0),(44429,82992,3,0),(44578,71916,1,0),(44579,71916,1,0),(44579,90102,1,0),(44580,71916,1,0),(44648,46598,1,1),(44653,83447,1,1),(44731,86807,1,0),(44764,86807,1,0),(44776,94120,3,0),(44798,46598,1,0),(44826,83591,1,1),(44828,83589,1,1),(44829,83590,1,1),(44928,72767,1,0),(45004,46598,1,0),(45005,84071,1,0),(45024,46598,1,0),(45030,46598,1,1),(45191,84499,1,0),(45344,84754,1,0),(45435,91271,1,0),(45527,46598,1,0),(45642,46598,1,0),(45648,46598,1,0),(45649,46598,1,0),(45651,56685,1,0),(45682,56685,1,0),(45881,46598,1,0),(45942,46598,1,0),(46085,85875,1,0),(46088,46598,1,0),(46100,85897,1,0),(46107,46598,1,0),(46165,90102,1,0),(46185,86106,0,0),(46208,46598,0,0),(46301,86293,3,0),(46301,86296,0,0),(46411,59907,1,0),(46449,46598,0,0),(46485,46598,1,0),(46706,46598,1,0),(46892,93799,1,0),(46895,93799,1,0),(46962,46598,1,1),(47034,46598,1,1),(47038,93799,1,0),(47066,87742,1,0),(47186,46598,1,0),(47201,87961,1,0),(47207,87975,1,0),(47210,46598,1,0),(47213,87975,1,0),(47216,87975,1,0),(47219,87975,1,0),(47225,46598,1,0),(47241,46598,1,0),(47241,88083,1,0),(47297,46598,1,0),(47315,46598,1,0),(47316,46598,1,0),(47316,88258,1,0),(47403,90102,1,0),(47404,90102,1,0),(47482,46598,1,0),(47747,89960,1,1),(47814,90102,1,0),(47820,90102,1,0),(47821,90102,1,0),(47822,90102,1,0),(47973,46598,1,0),(48006,89267,3,0),(48092,93970,1,0),(48116,89480,1,0),(48165,89524,1,1),(48197,46598,1,0),(48256,46598,1,0),(48266,90102,1,0),(48276,89732,3,0),(48293,89732,3,0),(48294,89267,3,0),(48295,89732,3,0),(48296,89267,3,0),(48297,89267,3,0),(48298,89732,3,0),(48299,89732,3,0),(48300,89267,3,0),(48301,89267,3,0),(48302,89732,3,0),(48327,46598,1,0),(48338,46598,1,0),(48341,89908,1,0),(48342,89908,1,0),(48343,89908,1,0),(48351,46598,1,0),(48411,46598,1,0),(48457,90102,1,1),(48476,46598,1,0),(48520,90287,1,0),(48639,90644,1,0),(48719,46598,1,0),(48721,46598,1,0),(48739,46598,1,0),(48790,46598,1,0),(48794,46598,1,0),(48796,46598,1,0),(48803,46598,1,0),(48804,46598,1,0),(48805,46598,1,0),(48806,46598,1,0),(48820,46598,1,0),(48821,46598,1,0),(48852,46598,1,1),(48909,46598,1,0),(48913,46598,1,0),(48939,46598,1,0),(48974,46598,1,1),(48975,47020,1,1),(49039,47020,1,1),(49040,47020,1,1),(49072,69063,1,0),(49101,46598,1,1),(49104,46598,1,1),(49135,57346,1,0),(49143,91586,1,0),(49194,91687,1,0),(49208,43671,1,0),(49457,92399,0,0),(49520,46598,1,0),(49550,89731,1,0),(49583,46598,1,1),(49584,46598,1,1),(49585,46598,1,1),(49604,92434,1,0),(49680,92793,1,0),(49683,46598,1,0),(49708,46598,1,1),(49841,46598,1,0),(49848,46598,1,0),(49910,92932,1,0),(50047,93072,1,0),(50089,61791,1,0),(50262,93320,1,0),(50266,46598,1,0),(50268,93323,1,0),(50409,93473,1,0),(50426,46598,1,0),(50426,93523,1,0),(50473,46598,1,0),(50523,46598,1,1),(50612,46598,1,0),(50635,68742,1,0),(50635,94271,1,0),(50636,68742,1,0),(50636,94272,1,0),(50638,68742,1,0),(50638,94275,1,0),(50643,68742,1,0),(50643,94269,1,0),(51033,46598,0,0),(51101,77901,1,0),(51102,77901,1,0),(51103,77901,1,0),(51141,46598,1,0),(51148,46598,1,0),(51156,46598,1,0),(51157,68742,1,0),(51173,46598,1,0),(51340,84965,1,1),(51391,95284,1,0),(51395,95285,1,0),(51396,95284,1,0),(51397,95285,1,0),(51551,86296,1,0),(51552,46598,1,0),(52351,46598,1,1),(52353,46598,1,1),(52354,46598,1,1),(52358,46598,0,0),(52359,46598,0,0),(52363,46598,1,0),(52529,96328,3,0),(52585,90102,1,0),(52682,97400,1,0),(52809,46598,0,0),(52812,46598,0,0),(52884,97773,1,0),(52884,97885,1,0),(52885,97773,1,0),(52885,97885,1,0),(52886,97773,1,0),(52886,97885,1,0),(52887,97773,1,0),(52887,97885,1,0),(52888,97773,1,0),(52888,97885,1,0),(52889,97773,1,0),(52889,97885,1,0),(52890,97773,1,0),(52890,97885,1,0),(53088,98208,1,0),(53089,99933,1,0),(53131,76387,1,0),(53275,98725,1,0),(53493,101376,1,0),(53517,99325,1,0),(53517,99328,2,0),(53526,99356,1,0),(53526,99357,2,0),(53561,99471,1,0),(53561,99473,2,0),(53786,98509,1,0),(53789,93970,1,0),(53791,98509,1,0),(53792,93970,1,0),(53887,97773,1,0),(53887,97885,1,0),(54247,90102,1,0),(54348,101316,3,0),(54367,101403,3,0),(55305,46598,0,0),(55306,46598,0,0),(55549,90102,1,0),(55549,103989,1,0),(55715,104515,1,0),(55967,46598,1,0),(56097,75648,1,0),(56103,46598,1,0),(56129,75648,1,0),(56130,75648,1,0),(56131,75648,1,0),(56173,46598,1,0),(57287,90102,1,0),(57288,90102,1,0),(57289,90102,1,0),(57328,49213,1,0),(57377,106094,1,0),(57378,90102,1,0),(57379,109835,1,0),(57443,90102,1,0),(57882,90102,1,0);
/*!40000 ALTER TABLE `npc_spellclick_spells` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-07-15 10:34:25
