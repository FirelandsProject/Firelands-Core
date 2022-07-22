CREATE DATABASE  IF NOT EXISTS `world` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `world`;
--
-- Table structure for table `pool_creature`
--

DROP TABLE IF EXISTS `pool_creature`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pool_creature` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Creature GUID (See creature.guid).',
  `pool_entry` mediumint(8) unsigned NOT NULL DEFAULT 0 COMMENT 'Pool ID (See pool_template.entry).',
  `chance` float unsigned NOT NULL DEFAULT 0 COMMENT 'Chance in %.',
  `description` varchar(255) NOT NULL COMMENT 'Description.',
  PRIMARY KEY (`guid`),
  KEY `pool_idx` (`pool_entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
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
-- Dumping data for table `pool_creature`
--

LOCK TABLES `pool_creature` WRITE;
/*!40000 ALTER TABLE `pool_creature` DISABLE KEYS */;
INSERT INTO `pool_creature` (`guid`, `pool_entry`, `chance`, `description`) VALUES 
(58,1341,0,'NPC 14490'),
(181,1300,0,'NPC 10077'),
(1173,312,0,'NPC 8298'),
(1343,326,0,''),
(1356,313,0,'NPC 8300'),
(1504,1247,0,'NPC 534'),
(1634,325,6,'16181 Rokad the Ravager'),
(2186,1245,0,'NPC 462'),
(2892,1300,0,'NPC 10077'),
(2915,1300,0,'NPC 10077'),
(3419,1249,0,'NPC 573'),
(3481,170,0,'null'),
(3570,1246,0,'NPC 521'),
(3695,1248,0,'NPC 572'),
(4838,1326,0,'NPC 14269'),
(5401,1244,0,'NPC 61'),
(5462,1250,0,'NPC 947'),
(5765,1328,0,'NPC 14272'),
(6297,301,0,''),
(6485,148,0,'null'),
(6536,224,12,'10119 Volchan'),
(6568,224,0,'null'),
(6575,148,12,'8976 Hematos'),
(6669,1301,0,'NPC 10078'),
(6677,315,0,'NPC 8978'),
(6785,316,0,'NPC 8979'),
(6969,1259,0,'NPC 2753'),
(7058,103,12,'2754 Anathemus'),
(7064,199,12,'2749 Siege Golem'),
(7074,199,0,'null'),
(7102,103,0,'null'),
(7128,228,4,'2931 Zaricotl'),
(7136,1291,0,'NPC 8283'),
(7141,228,0,'null'),
(7249,1291,0,'NPC 8283'),
(7265,1289,0,'NPC 8279'),
(7350,152,0,'null'),
(7353,1260,0,'NPC 2850'),
(7403,1291,0,'NPC 8283'),
(7426,1288,0,'NPC 8277'),
(7783,124,0,'null'),
(8652,1251,0,'NPC 1137'),
(9181,1295,0,'NPC 8503'),
(9460,129,0,'null'),
(9950,1268,0,'NPC 5352'),
(11128,189,0,'null'),
(11134,189,12,'2602 Ruul Onestone'),
(11189,134,0,'null'),
(11195,134,12,'2601 Foulbelly'),
(11225,119,12,'2598 Darbel Montrose'),
(11554,153,3,'32495 Hildana Deathstealer'),
(11838,1329,0,'NPC 14279'),
(12316,1316,0,'NPC 12432'),
(12402,1316,0,'NPC 12432'),
(12498,1316,0,'NPC 12432'),
(12501,1316,0,'NPC 12432'),
(12518,1316,0,'NPC 12432'),
(12537,174,0,'null'),
(12700,1316,0,'NPC 12432'),
(12912,1285,0,'NPC 8213'),
(13109,121,12,'3872 Deathsworn Captain'),
(13151,1330,0,'NPC 14280'),
(13352,142,0,'null'),
(13388,142,12,'8215 Grimungous'),
(13454,1331,0,'NPC 14281'),
(13520,1316,0,'NPC 12432'),
(13819,1317,0,'NPC 14222'),
(13884,1316,0,'NPC 12432'),
(13895,311,0,'NPC 8210'),
(13990,177,0,'null'),
(14065,308,0,'NPC 2452'),
(14069,1316,0,'NPC 12432'),
(14073,1316,0,'NPC 12432'),
(14116,1284,0,'NPC 8212'),
(14436,309,0,'NPC 2453'),
(14660,177,12,'2447 Narillasanz'),
(15146,183,12,'1850 Putridius'),
(15154,183,0,'null'),
(15174,153,3,'32495 Hildana Deathstealer'),
(15189,153,3,'32495 Hildana Deathstealer'),
(15217,153,3,'32495 Hildana Deathstealer'),
(15262,166,2,'32398 King Ping'),
(15277,166,2,'32398 King Ping'),
(15280,166,2,'32398 King Ping'),
(15306,1316,0,'NPC 12432'),
(15359,166,2,'32398 King Ping'),
(15598,149,6,'10828 High General Abbendis'),
(15636,218,1,'32491 Time-Lost Proto Drake'),
(15693,218,1,'32491 Time-Lost Proto Drake'),
(15832,149,0,'null'),
(16335,1309,0,'NPC 10821'),
(17112,228,4,'2931 Zaricotl'),
(17163,228,4,'2931 Zaricotl'),
(17307,202,6,'5930 Sister Riven'),
(17514,149,6,'10828 High General Abbendis'),
(17518,152,6,'8282 Highlord Mastrogonde'),
(17626,307,0,'NPC 1531'),
(17708,115,4,'5824 Captain Flat Tusk'),
(17748,1311,0,'NPC 10825'),
(18077,105,4,'38453 Arcturis'),
(18090,1313,0,'NPC 10827'),
(18174,1311,0,'NPC 10825'),
(18439,105,4,'38453 Arcturis'),
(18506,1312,0,'NPC 10826'),
(18568,132,0,'null'),
(18578,132,12,'1843 Foreman Jerris'),
(18612,307,0,'NPC 1531'),
(18632,105,4,'38453 Arcturis'),
(18634,146,6,'10558 Hearthsinger Forresten'),
(18737,209,6,'10809 Stonespine'),
(18993,178,0,'null'),
(18994,1310,0,'NPC 10824'),
(19579,1281,0,'NPC 8202'),
(21433,1275,0,'NPC 6581'),
(21447,144,0,'null'),
(21746,1282,0,'NPC 8207'),
(21828,1282,0,'NPC 8207'),
(21934,230,0,'null'),
(21958,1282,0,'NPC 8207'),
(22140,1282,0,'NPC 8207'),
(22186,1282,0,'NPC 8207'),
(22555,1275,0,'NPC 6581'),
(22844,1337,0,'NPC 14472'),
(22880,161,0,'null'),
(22930,161,12,'8200 Jin\'Zallah the Sandbringer'),
(23003,1275,0,'NPC 6581'),
(23129,1275,0,'NPC 6581'),
(23168,1275,0,'NPC 6581'),
(23259,1275,0,'NPC 6581'),
(23595,321,0,'NPC 14479'),
(23741,227,0,'null'),
(23756,198,0,'null'),
(23796,168,0,'null'),
(23859,198,12,'14471 Setis'),
(24178,165,0,'null'),
(24188,165,12,'6584 King Krush'),
(24477,187,12,'14475 Rex Ashil'),
(24498,187,0,'null'),
(24627,1266,0,'NPC 4132'),
(24634,1266,0,'NPC 4132'),
(24643,1266,0,'NPC 4132'),
(24858,1266,0,'NPC 4132'),
(25198,223,0,'null'),
(25201,158,0,'null'),
(25625,1335,0,'NPC 14427'),
(25885,147,12,'5934 Heartrazor'),
(25959,147,0,'null'),
(26458,110,0,'null'),
(27004,306,12,'5851 Captain Gerogg Hammertoe'),
(27018,306,0,'null'),
(27516,123,6,'5912 Deviate Faerie Dragon'),
(27546,145,12,'5859 Hagg Taurenbane'),
(27550,145,0,'null'),
(29197,320,0,'NPC 14477'),
(29636,1322,0,'NPC 14230'),
(29784,111,12,'5827 Brontus'),
(29808,111,0,'null'),
(32343,159,6,'6489 Ironspine'),
(32398,211,0,'null'),
(32399,211,12,'5864 Swinegart Spearhide'),
(32447,108,6,'9596 Bannok Grimaxe'),
(32816,101,100,'null'),
(32859,100,25,'5799 Hannah Bladeleaf'),
(32868,100,25,'5797 Aean Swiftriver'),
(32875,100,25,'5800 Marcus Bel'),
(32876,100,25,'5798 Thora Feathermoon'),
(33146,188,12,'5841 Rocklance'),
(33233,188,0,'null'),
(33541,200,4,'5785 Sister Hatelash'),
(33564,200,0,'null'),
(33692,114,4,'10263 Burning Felguard'),
(33719,137,12,'3398 Gesharahan'),
(33729,137,0,'null'),
(33860,200,4,'5785 Sister Hatelash'),
(33893,1319,0,'NPC 14227'),
(34338,1261,0,'NPC 3056'),
(34973,210,0,'null'),
(35088,200,4,'5785 Sister Hatelash'),
(35216,154,0,'null'),
(35262,154,12,'5828 Humar the Pridelord'),
(36474,1271,0,'NPC 5823'),
(36520,115,0,'null'),
(36654,1273,0,'NPC 5837'),
(36681,128,12,'3270 Earthcaller Halmgar'),
(36702,128,0,'null'),
(36705,1271,0,'NPC 5823'),
(36905,310,0,'NPC 5826'),
(36929,310,0,'NPC 5826'),
(36958,310,0,'NPC 5826'),
(36989,310,0,'NPC 5826'),
(37171,1271,0,'NPC 5823'),
(37865,310,0,'NPC 5826'),
(37930,310,0,'NPC 5826'),
(38094,202,0,'null'),
(38197,201,12,'5830 Sister Rathtalon'),
(38265,201,0,'null'),
(38327,1265,0,'NPC 4030'),
(38518,1265,0,'NPC 4030'),
(39140,213,0,'null'),
(39195,213,12,'5842 Takk the Leaper'),
(39498,1264,0,'NPC 4015'),
(39551,1264,0,'NPC 4015'),
(39591,1264,0,'NPC 4015'),
(39646,131,0,'null'),
(39649,215,0,'null'),
(39652,133,12,'5931 Foreman Rigger'),
(39660,133,0,'null'),
(39715,131,12,'5822 Felweaver Scornn'),
(39749,1264,0,'NPC 4015'),
(39781,1264,0,'NPC 4015'),
(39872,1264,0,'NPC 4015'),
(39892,1264,0,'NPC 4015'),
(39925,1264,0,'NPC 4015'),
(40225,1314,0,'NPC 12037'),
(40498,197,12,'5916 Sentinel Amarassan'),
(40529,197,0,'null'),
(40680,112,0,'null'),
(41737,191,0,'null'),
(42179,1305,0,'NPC 10559'),
(42642,167,0,'null'),
(42654,167,12,'10201 Lady Hederine'),
(43044,229,0,'null'),
(43269,163,12,'10198 Kashoch the Reaver'),
(43326,163,0,'null'),
(43506,157,0,'null'),
(43544,157,12,'7137 Immolatus'),
(43776,1254,0,'NPC 2186'),
(43925,136,12,'10196 General Colbatann'),
(43931,136,0,'null'),
(44094,122,12,'7104 Dessecus'),
(44103,122,0,'null'),
(44939,319,0,'NPC 14343'),
(46114,1336,0,'NPC 14430'),
(47495,118,6,'10376 Crystal Fang'),
(47613,207,6,'9219 Spirestone Butcher'),
(47644,172,0,'null'),
(47709,301,0,''),
(47714,303,0,''),
(47716,303,0,''),
(47723,303,0,''),
(47738,303,0,''),
(47910,172,12,'12237 Meshlok the Harvester'),
(47927,146,6,'10558 Hearthsinger Forresten'),
(47940,205,12,'10393 Skul'),
(47954,209,0,'null'),
(47963,209,6,'10809 Stonespine'),
(47993,205,0,'null'),
(48034,146,0,'null'),
(48849,181,0,'null'),
(48858,185,0,'null'),
(48979,226,0,'null'),
(49002,221,0,'null'),
(49007,181,12,'8923 Panzor the Invincible'),
(49066,114,4,'10263 Burning Felguard'),
(49067,206,6,'9218 Spirestone Battle Lord'),
(49070,118,6,'10376 Crystal Fang'),
(49086,138,0,'null'),
(49090,138,12,'9718 Ghok Bashguud'),
(49095,108,0,'null'),
(49111,118,0,'null'),
(49112,108,6,'9596 Bannok Grimaxe'),
(49130,208,0,'null'),
(49147,114,0,'null'),
(49155,160,0,'null'),
(49157,207,0,'null'),
(49165,207,6,'9219 Spirestone Butcher'),
(49185,114,4,'10263 Burning Felguard'),
(49191,206,0,'null'),
(49220,160,12,'10509 Jed Runewatcher'),
(49275,190,0,'null'),
(49406,1309,0,'NPC 10821'),
(49469,106,0,'null'),
(49470,106,12,'6490 Azshir the Sleepless'),
(49550,159,0,'6489 Ironspine'),
(49559,130,0,'null'),
(49588,130,12,'6488 Fallen Champion'),
(49598,159,6,'6489 Ironspine'),
(49973,120,0,'null'),
(50295,120,12,'6228 Dark Iron Ambassador'),
(50696,123,0,'null'),
(50926,173,0,'null'),
(50928,173,12,'3586 Miner Johnson'),
(51077,208,6,'9217 Spirestone Lord Magus'),
(51081,206,6,'9218 Spirestone Battle Lord'),
(51122,113,0,'null'),
(51168,113,12,'1720 Bruegal Ironknuckle'),
(51228,121,0,'null'),
(51469,171,12,'14506 Lord Hel\'nurath'),
(51482,219,0,'null'),
(51510,229,12,'10082 Zerillis'),
(51644,171,0,'null'),
(51705,219,12,'11467 Tsu\'zee'),
(51772,218,1,'32491 Time-Lost Proto Drake'),
(52232,1300,0,'NPC 10077'),
(52259,1270,0,'NPC 5786'),
(52389,141,2,'32471 Griegen'),
(52499,1263,0,'NPC 3581'),
(52500,1263,0,'NPC 3581'),
(52637,1302,0,'NPC 10197'),
(52638,1303,0,'NPC 10200'),
(52641,1304,0,'NPC 10356'),
(52670,318,0,'NPC 10817'),
(52709,176,0,'null'),
(52713,203,0,'null'),
(52717,217,0,'null'),
(52769,126,0,'null'),
(52813,1315,0,'NPC 12431'),
(52881,191,12,'13896 Scalebeard'),
(52884,1318,0,'NPC 14224'),
(52886,1323,0,'NPC 14233'),
(52888,1324,0,'NPC 14237'),
(52889,129,12,'14267 Emogg the Crusher'),
(52890,1325,0,'NPC 14268'),
(52891,1327,0,'NPC 14271'),
(52892,214,12,'14275 Tamra Stormpike'),
(52896,1332,0,'NPC 14339'),
(52898,1334,0,'NPC 14344'),
(52905,170,12,'14445 Lord Captain Wyrmak'),
(52922,168,12,'14473 Lapress'),
(52923,230,12,'14474 Zora'),
(52924,1338,0,'NPC 14476'),
(52982,109,6,'4425 Blind Hunter'),
(52998,185,12,'9024 Pyromancer Loregrain'),
(52999,226,12,'9041 Warder Stilgiss'),
(53012,221,12,'9042 Verek'),
(53044,1253,0,'NPC 1944'),
(53045,1253,0,'NPC 1944'),
(53087,1257,0,'NPC 2751'),
(53088,1258,0,'NPC 2752'),
(53108,1262,0,'NPC 3295'),
(53109,1262,0,'NPC 3295'),
(53150,110,12,'4339 Brimgore'),
(53182,1267,0,'NPC 5345'),
(53199,115,4,'5824 Captain Flat Tusk'),
(53200,115,4,'5824 Captain Flat Tusk'),
(53202,210,12,'5831 Swiftmane'),
(53205,1244,0,'NPC 61'),
(53208,123,6,'5912 Deviate Faerie Dragon'),
(53209,202,6,'5930 Sister Riven'),
(53216,215,12,'5932 Taskmaster Whipfang'),
(53217,223,12,'5937 Vile Sting'),
(53225,1274,0,'NPC 6118'),
(53226,1274,0,'NPC 6118'),
(53228,1274,0,'NPC 6118'),
(53229,1274,0,'NPC 6118'),
(53317,144,6,'6583 Gruff'),
(53318,144,6,'6583 Gruff'),
(53320,141,2,'32471 Griegen'),
(53321,1276,0,'NPC 6648'),
(53322,1276,0,'NPC 6648'),
(53323,1276,0,'NPC 6648'),
(53324,1276,0,'NPC 6648'),
(53325,1277,0,'NPC 6652'),
(53331,1278,0,'NPC 7015'),
(53332,1279,0,'NPC 7016'),
(53333,1279,0,'NPC 7016'),
(53354,124,12,'7057 Digmaster Shovelphlange'),
(53446,227,12,'8199 Warleader Krazzilak'),
(53447,1280,0,'NPC 8201'),
(53448,1280,0,'NPC 8201'),
(53452,1286,0,'NPC 8214'),
(53453,1287,0,'NPC 8219'),
(53454,1290,0,'NPC 8281'),
(53455,152,6,'8282 Highlord Mastrogonde'),
(53459,1294,0,'NPC 8302'),
(53483,314,0,'NPC 8660'),
(53484,314,0,'NPC 8660'),
(53485,314,0,'NPC 8660'),
(53490,1299,0,'NPC 8981'),
(53558,1303,0,'NPC 10200'),
(53676,158,12,'5935 Ironeye the Invincible'),
(53954,325,6,'16179 Hyakiss the Lurker'),
(54462,9868,0,'32838'),
(54485,317,0,'NPC 10644'),
(54486,317,0,'NPC 10644'),
(54527,9868,0,'32838'),
(54528,9868,0,'32838'),
(54531,9868,0,'32838'),
(54532,9868,0,'32838'),
(54548,218,1,'32491 Time-Lost Proto Drake'),
(54549,218,1,'32491 Time-Lost Proto Drake'),
(54550,218,1,'32491 Time-Lost Proto Drake'),
(54551,218,1,'32491 Time-Lost Proto Drake'),
(54553,9868,0,'32838'),
(55089,141,2,'32471 Griegen'),
(56533,214,0,'null'),
(56918,1249,0,'NPC 573'),
(57255,1249,0,'NPC 573'),
(57605,179,0,'null'),
(58224,1249,0,'NPC 573'),
(58229,1249,0,'NPC 573'),
(59219,1249,0,'NPC 573'),
(60699,1247,0,'NPC 534'),
(61615,325,0,'null'),
(61877,1328,0,'NPC 14272'),
(61878,1326,0,'NPC 14269'),
(61880,1249,0,'NPC 573'),
(61881,1326,0,'NPC 14269'),
(61882,312,0,'NPC 8298'),
(61885,1249,0,'NPC 573'),
(61896,312,0,'NPC 8298'),
(61900,1249,0,'NPC 573'),
(61909,1249,0,'NPC 573'),
(61912,1328,0,'NPC 14272'),
(61913,1341,0,'NPC 14490'),
(61915,1249,0,'NPC 573'),
(61916,1249,0,'NPC 573'),
(61937,1249,0,'NPC 573'),
(61938,1249,0,'NPC 573'),
(63232,180,3,'32357 Old Crystalbark'),
(63242,220,2,'32400 Tukemuth'),
(63245,135,12,'32358 Fumblub Gearwind'),
(63861,180,3,'32357 Old Crystalbark'),
(63882,180,3,'32357 Old Crystalbark'),
(63913,180,3,'32357 Old Crystalbark'),
(64529,194,3,'32417 Scarlet Highlord Daion'),
(64539,182,4,'32377 Perobas the Bloodthirster'),
(64551,220,2,'32400 Tukemuth'),
(64554,182,4,'32377 Perobas the Bloodthirster'),
(64555,143,2,'32422 Grocklar'),
(64560,220,2,'32400 Tukemuth'),
(64565,143,2,'32422 Grocklar'),
(64566,194,3,'32417 Scarlet Highlord Daion'),
(64572,220,2,'32400 Tukemuth'),
(64576,143,2,'32422 Grocklar'),
(64578,194,3,'32417 Scarlet Highlord Daion'),
(64581,212,2,'32438 Syreian the Bonecarver'),
(64582,212,2,'32438 Syreian the Bonecarver'),
(64595,143,2,'32422 Grocklar'),
(64598,143,2,'32422 Grocklar'),
(64600,212,2,'32438 Syreian the Bonecarver'),
(64606,182,4,'32377 Perobas the Bloodthirster'),
(64608,141,2,'32471 Griegen'),
(64611,231,3,'32447 Zul\'drak Sentinel'),
(64613,220,2,'32400 Tukemuth'),
(64618,220,2,'32400 Tukemuth'),
(64628,220,2,'32400 Tukemuth'),
(64632,212,2,'32438 Syreian the Bonecarver'),
(64638,212,2,'32438 Syreian the Bonecarver'),
(64642,143,2,'32422 Grocklar'),
(64645,212,2,'32438 Syreian the Bonecarver'),
(64647,141,2,'32471 Griegen'),
(64650,220,2,'32400 Tukemuth'),
(66140,1362,0,'NPC 22060'),
(67330,1347,0,'NPC 18680'),
(68415,1362,0,'NPC 22060'),
(69002,316,0,'NPC 8979'),
(71730,119,0,'null'),
(73561,332,0,''),
(73654,216,3,'32475 Terror Spinner'),
(74349,333,0,''),
(74373,1244,0,'NPC 61'),
(74375,316,0,'NPC 8979'),
(74377,1244,0,'NPC 61'),
(74922,1270,0,'NPC 5786'),
(75887,1249,0,'NPC 573'),
(75888,1249,0,'NPC 573'),
(75973,216,0,'null'),
(76723,323,0,'NPC 18682'),
(76724,323,0,'NPC 18682'),
(76725,323,0,'NPC 18682'),
(77174,231,3,'32447 Zul\'drak Sentinel'),
(77175,231,3,'32447 Zul\'drak Sentinel'),
(77176,231,3,'32447 Zul\'drak Sentinel'),
(77239,104,2,'32481 Aotona'),
(77240,104,2,'32481 Aotona'),
(77241,104,2,'32481 Aotona'),
(77283,104,2,'32481 Aotona'),
(77284,104,2,'32481 Aotona'),
(77285,104,2,'32481 Aotona'),
(77286,222,2,'32386 Vigdis the War Maiden'),
(77287,222,2,'32386 Vigdis the War Maiden'),
(77288,222,2,'32386 Vigdis the War Maiden'),
(77289,222,2,'32386 Vigdis the War Maiden'),
(77290,222,2,'32386 Vigdis the War Maiden'),
(77291,222,2,'32386 Vigdis the War Maiden'),
(77292,156,4,'32361 Icehorn'),
(77293,156,4,'32361 Icehorn'),
(77294,156,4,'32361 Icehorn'),
(77295,151,4,'32501 High Thane Jorfus'),
(77717,141,2,'32471 Griegen'),
(77741,179,4,'20932 Nuramoc'),
(77854,216,3,'32475 Terror Spinner'),
(77855,216,3,'32475 Terror Spinner'),
(78936,15,0,''),
(78937,14,0,''),
(78938,32,0,''),
(78950,5,0,''),
(79642,1318,0,'NPC 14224'),
(79643,1318,0,'NPC 14224'),
(79644,1290,0,'NPC 8281'),
(79901,1,0,''),
(79902,4,0,''),
(79903,3,0,''),
(79904,2,0,''),
(79905,38,0,''),
(79906,6,0,''),
(79907,38,0,''),
(79908,38,0,''),
(79909,6,0,''),
(79910,39,0,''),
(79911,7,0,''),
(79912,39,0,''),
(79913,39,0,''),
(79914,7,0,''),
(79915,41,0,''),
(79916,41,0,''),
(79917,8,0,''),
(79918,41,0,''),
(79919,8,0,''),
(79920,42,0,''),
(79921,9,0,''),
(79922,42,0,''),
(79923,9,0,''),
(79924,42,0,''),
(79925,44,0,''),
(79926,44,0,''),
(79927,11,0,''),
(79928,44,0,''),
(79929,11,0,''),
(79930,43,0,''),
(79931,43,0,''),
(79932,10,0,''),
(79933,43,0,''),
(79934,10,0,''),
(79935,45,0,''),
(79936,12,0,''),
(79937,45,0,''),
(79938,12,0,''),
(79939,45,0,''),
(79940,46,0,''),
(79941,13,0,''),
(79942,46,0,''),
(79943,13,0,''),
(79944,46,0,''),
(79947,47,0,''),
(79948,14,0,''),
(79949,47,0,''),
(79950,14,0,''),
(79951,47,0,''),
(79952,48,0,''),
(79953,15,0,''),
(79954,48,0,''),
(79955,48,0,''),
(79956,15,0,''),
(79957,49,0,''),
(79958,49,0,''),
(79959,16,0,''),
(79960,49,0,''),
(79961,16,0,''),
(79962,50,0,''),
(79963,50,0,''),
(79964,17,0,''),
(79965,50,0,''),
(79966,17,0,''),
(79967,51,0,''),
(79968,18,0,''),
(79969,51,0,''),
(79970,51,0,''),
(79971,18,0,''),
(79972,37,0,''),
(79973,37,0,''),
(79974,36,0,''),
(79975,36,0,''),
(79976,35,0,''),
(79977,35,0,''),
(79978,34,0,''),
(79979,34,0,''),
(79980,33,0,''),
(79981,33,0,''),
(79982,32,0,''),
(79983,32,0,''),
(79984,31,0,''),
(79985,31,0,''),
(79986,27,0,''),
(79987,27,0,''),
(79988,27,0,''),
(79989,28,0,''),
(79990,28,0,''),
(79991,29,0,''),
(79992,29,0,''),
(79993,30,0,''),
(79994,30,0,''),
(79995,26,0,''),
(79996,26,0,''),
(79997,25,0,''),
(79998,25,0,''),
(79999,24,0,''),
(80000,24,0,''),
(80001,23,0,''),
(80002,23,0,''),
(80003,22,0,''),
(80004,22,0,''),
(80005,22,0,''),
(80006,21,0,''),
(80007,21,0,''),
(80008,20,0,''),
(80009,20,0,''),
(80010,19,0,''),
(80011,19,0,''),
(81378,178,4,'16184 Nerubian Overseer'),
(81379,178,4,'16184 Nerubian Overseer'),
(81380,178,4,'16184 Nerubian Overseer'),
(81471,139,2,'33776 Gondria'),
(81915,104,0,'null'),
(82536,1249,0,'NPC 573'),
(83050,1353,0,'NPC 18690'),
(83288,1347,0,'NPC 18680'),
(83337,326,40,''),
(85540,332,40,''),
(86265,204,4,'35189 Skoll'),
(86266,204,4,'35189 Skoll'),
(86267,204,4,'35189 Skoll'),
(86395,5683,0,'30877'),
(86396,5683,0,'30877'),
(86397,5683,0,'30877'),
(86398,5683,0,'30877'),
(86399,5683,0,'30877'),
(86400,5683,0,'30877'),
(86401,5683,0,'30877'),
(86402,5683,0,'30877'),
(86403,5683,0,'30877'),
(86404,5683,0,'30877'),
(86405,5683,0,'30877'),
(86406,5683,0,'30877'),
(86407,5682,0,'30877'),
(86408,5682,0,'30877'),
(86409,5682,0,'30877'),
(86410,5682,0,'30877'),
(86411,5682,0,'30877'),
(86412,5682,0,'30877'),
(86413,5682,0,'30877'),
(86414,5682,0,'30877'),
(86415,5682,0,'30877'),
(86416,5682,0,'30877'),
(86417,5682,0,'30877'),
(86418,5682,0,'30877'),
(86419,5682,0,'30877'),
(86420,5682,0,'30877'),
(86421,5682,0,'30877'),
(86422,5682,0,'30877'),
(86423,5682,0,'30877'),
(86424,5682,0,'30877'),
(86425,5681,0,'30877'),
(86426,5681,0,'30877'),
(86427,5681,0,'30877'),
(86428,5681,0,'30877'),
(86429,5681,0,'30877'),
(86430,5681,0,'30877'),
(86431,5681,0,'30877'),
(86432,5681,0,'30877'),
(86433,5681,0,'30877'),
(86434,5681,0,'30877'),
(86435,5681,0,'30877'),
(86436,5681,0,'30877'),
(86437,5681,0,'30877'),
(86438,5681,0,'30877'),
(86439,5681,0,'30877'),
(86440,5681,0,'30877'),
(86441,5681,0,'30877'),
(86442,5681,0,'30877'),
(86443,5680,0,'30877'),
(86444,5680,0,'30877'),
(86445,5680,0,'30877'),
(86446,5680,0,'30877'),
(86447,5680,0,'30877'),
(86448,5680,0,'30877'),
(86449,5680,0,'30877'),
(86450,5680,0,'30877'),
(86451,5680,0,'30877'),
(86452,5680,0,'30877'),
(86453,5680,0,'30877'),
(86454,5680,0,'30877'),
(86455,5680,0,'30877'),
(86456,5680,0,'30877'),
(86457,5680,0,'30877'),
(86458,5680,0,'30877'),
(86459,5680,0,'30877'),
(86460,5680,0,'30877'),
(86461,5679,0,'30877'),
(86462,5679,0,'30877'),
(86463,5679,0,'30877'),
(86464,5679,0,'30877'),
(86465,5679,0,'30877'),
(86466,5679,0,'30877'),
(86467,5679,0,'30877'),
(86468,5679,0,'30877'),
(86469,5679,0,'30877'),
(86470,5679,0,'30877'),
(86471,5679,0,'30877'),
(86472,5679,0,'30877'),
(86473,5679,0,'30877'),
(86474,5679,0,'30877'),
(86475,5679,0,'30877'),
(86476,5679,0,'30877'),
(86477,5679,0,'30877'),
(86478,5679,0,'30877'),
(86616,151,4,'32501 High Thane Jorfus'),
(86670,180,0,'null'),
(87053,1249,0,'NPC 573'),
(87278,305,0,'Dalaran guests'),
(87279,305,0,'Dalaran guests'),
(87284,305,0,'Dalaran guests'),
(87286,305,0,'Dalaran guests'),
(87288,305,0,'Dalaran guests'),
(87290,333,40,''),
(87296,326,0,''),
(88424,1249,0,'NPC 573'),
(88595,139,2,'33776 Gondria'),
(88680,151,4,'32501 High Thane Jorfus'),
(88681,125,3,'32500 Dirkee'),
(89915,196,4,'32429 Seething Hate'),
(89916,196,4,'32429 Seething Hate'),
(89917,196,4,'32429 Seething Hate'),
(89921,125,3,'32500 Dirkee'),
(89923,125,3,'32500 Dirkee'),
(89925,125,3,'32500 Dirkee'),
(90257,5678,0,'30872'),
(90260,5670,0,'30847'),
(90261,5670,0,'30847'),
(90262,5670,0,'30847'),
(90487,5683,0,'30877'),
(90488,5683,0,'30877'),
(90489,5683,0,'30877'),
(90490,5683,0,'30877'),
(90491,5683,0,'30877'),
(90492,5683,0,'30877'),
(90493,5684,0,'30877'),
(90494,5684,0,'30877'),
(90495,5684,0,'30877'),
(90496,5684,0,'30877'),
(90497,5684,0,'30877'),
(90498,5684,0,'30877'),
(90499,5684,0,'30877'),
(90500,5684,0,'30877'),
(90501,5684,0,'30877'),
(90502,5684,0,'30877'),
(90505,5684,0,'30877'),
(90506,5684,0,'30877'),
(90507,5684,0,'30877'),
(90508,5684,0,'30877'),
(90512,5684,0,'30877'),
(90513,5684,0,'30877'),
(90518,5684,0,'30877'),
(90519,5684,0,'30877'),
(90520,5685,0,'30877'),
(90521,5685,0,'30877'),
(90522,5685,0,'30877'),
(90523,5685,0,'30877'),
(90524,5685,0,'30877'),
(90525,5685,0,'30877'),
(90526,5685,0,'30877'),
(90527,5685,0,'30877'),
(90528,5685,0,'30877'),
(90529,5685,0,'30877'),
(90530,5685,0,'30877'),
(90531,5685,0,'30877'),
(90532,5685,0,'30877'),
(90533,5685,0,'30877'),
(90534,5685,0,'30877'),
(90535,5685,0,'30877'),
(90536,5685,0,'30877'),
(90537,5685,0,'30877'),
(90538,5686,0,'30877'),
(90539,5686,0,'30877'),
(90540,5686,0,'30877'),
(90541,5686,0,'30877'),
(90542,5686,0,'30877'),
(90543,5686,0,'30877'),
(90544,5686,0,'30877'),
(90545,5686,0,'30877'),
(90546,5686,0,'30877'),
(90547,5686,0,'30877'),
(90548,5686,0,'30877'),
(90549,5686,0,'30877'),
(90550,5686,0,'30877'),
(90551,5686,0,'30877'),
(90553,5686,0,'30877'),
(90555,5686,0,'30877'),
(90556,5686,0,'30877'),
(90557,5686,0,'30877'),
(90561,5687,0,'30877'),
(90562,5687,0,'30877'),
(90563,5687,0,'30877'),
(90564,5687,0,'30877'),
(90565,5687,0,'30877'),
(90566,5687,0,'30877'),
(90568,5687,0,'30877'),
(90569,5687,0,'30877'),
(90570,5687,0,'30877'),
(90571,5687,0,'30877'),
(90572,5687,0,'30877'),
(90573,5687,0,'30877'),
(90574,5687,0,'30877'),
(90575,5687,0,'30877'),
(90576,5687,0,'30877'),
(90577,5687,0,'30877'),
(90578,5687,0,'30877'),
(90583,5687,0,'30877'),
(90584,5688,0,'30877'),
(90585,5688,0,'30877'),
(90586,5688,0,'30877'),
(90587,5688,0,'30877'),
(90588,5688,0,'30877'),
(90589,5688,0,'30877'),
(90590,5688,0,'30877'),
(90591,5688,0,'30877'),
(90592,5688,0,'30877'),
(90593,5688,0,'30877'),
(90594,5688,0,'30877'),
(90595,5688,0,'30877'),
(90596,5688,0,'30877'),
(90597,5688,0,'30877'),
(90598,5688,0,'30877'),
(90599,5688,0,'30877'),
(90600,5688,0,'30877'),
(90601,5688,0,'30877'),
(90605,5688,0,'30877'),
(90606,5688,0,'30877'),
(90609,5688,0,'30877'),
(90610,5688,0,'30877'),
(90611,5688,0,'30877'),
(90612,5688,0,'30877'),
(90992,218,1,'32491 Time-Lost Proto Drake'),
(90993,218,1,'32491 Time-Lost Proto Drake'),
(90994,218,1,'32491 Time-Lost Proto Drake'),
(90995,218,1,'32491 Time-Lost Proto Drake'),
(90996,218,1,'32491 Time-Lost Proto Drake'),
(90997,218,1,'32491 Time-Lost Proto Drake'),
(90998,218,1,'32491 Time-Lost Proto Drake'),
(90999,218,1,'32491 Time-Lost Proto Drake'),
(91000,218,1,'32491 Time-Lost Proto Drake'),
(91001,218,1,'32491 Time-Lost Proto Drake'),
(91002,218,1,'32491 Time-Lost Proto Drake'),
(91035,169,2,'32517 Loque\'nahak'),
(91036,169,2,'32517 Loque\'nahak'),
(91037,169,2,'32517 Loque\'nahak'),
(91038,169,2,'32517 Loque\'nahak'),
(91039,169,2,'32517 Loque\'nahak'),
(91040,169,2,'32517 Loque\'nahak'),
(91041,169,2,'32517 Loque\'nahak'),
(91060,218,1,'32491 Time-Lost Proto Drake'),
(91061,218,1,'32491 Time-Lost Proto Drake'),
(91106,216,3,'32475 Terror Spinner'),
(91547,174,12,'8217 Mith\'rethis the Enchanter'),
(92202,1358,0,'NPC 18696'),
(92292,1268,0,'NPC 5352'),
(92317,332,0,''),
(92435,1351,0,'NPC 18685'),
(92458,1351,0,'NPC 18685'),
(92459,1351,0,'NPC 18685'),
(92460,1351,0,'NPC 18685'),
(92515,325,6,'16180 Shadikith the Glider'),
(92589,186,0,'null'),
(92649,127,0,'null'),
(92682,109,0,'null'),
(92856,164,6,'32485 King Krush'),
(92857,164,6,'32485 King Krush'),
(94941,333,0,''),
(94944,241,10,'NPC 33224'),
(94945,242,10,'NPC 33224'),
(94946,243,10,'NPC 33224'),
(94947,244,10,'NPC 33224'),
(94948,245,10,'NPC 33224'),
(94949,246,10,'NPC 33224'),
(94950,247,10,'NPC 33224'),
(95002,1325,0,'NPC 14268'),
(95003,1295,0,'NPC 8503'),
(95004,1325,0,'NPC 14268'),
(95005,1251,0,'NPC 1137'),
(95006,1258,0,'NPC 2752'),
(95007,1260,0,'NPC 2850'),
(95008,1260,0,'NPC 2850'),
(95009,1259,0,'NPC 2753'),
(95010,1318,0,'NPC 14224'),
(95011,1318,0,'NPC 14224'),
(95012,1318,0,'NPC 14224'),
(95014,1259,0,'NPC 2753'),
(95015,1259,0,'NPC 2753'),
(95016,1318,0,'NPC 14224'),
(95325,248,10,'NPC 33224'),
(95773,3,0,''),
(95786,19,0,''),
(95787,21,0,''),
(95789,23,0,''),
(95792,37,0,''),
(95797,20,0,''),
(95808,1,0,''),
(95809,4,0,''),
(95864,24,0,''),
(95865,34,0,''),
(95866,31,0,''),
(95867,36,0,''),
(95868,25,0,''),
(95869,26,0,''),
(95870,28,0,''),
(95871,29,0,''),
(95872,30,0,''),
(95873,35,0,''),
(95874,33,0,''),
(95890,6,0,''),
(95891,7,0,''),
(95892,18,0,''),
(95893,13,0,''),
(95894,12,0,''),
(95895,9,0,''),
(95898,8,0,''),
(95899,11,0,''),
(95900,17,0,''),
(95902,16,0,''),
(95903,10,0,''),
(96282,135,0,'null'),
(97049,169,0,'null'),
(97149,164,0,'null'),
(97158,140,0,'NPC 28083'),
(97159,140,0,'NPC 28083'),
(97160,140,0,'NPC 28083'),
(97253,1249,0,'NPC 573'),
(97255,1249,0,'NPC 573'),
(98440,218,1,'32491 Time-Lost Proto Drake'),
(99798,249,10,'NPC 33224'),
(103277,156,0,'null'),
(104346,225,12,'32630 Vyragosa'),
(104486,218,0,'null'),
(104783,225,0,'null'),
(104886,125,0,'null'),
(105367,204,0,'null'),
(107713,166,2,'32398 King Ping'),
(108075,105,0,'null'),
(109002,1364,0,'NPC 32435'),
(109551,153,0,'null'),
(110218,151,0,'null'),
(111375,184,12,'32487 Putridus the Ancient'),
(111448,140,0,'NPC 28083'),
(111776,184,0,'null'),
(112410,1364,0,'NPC 32435'),
(112775,194,3,'32417 Scarlet Highlord Daion'),
(113281,117,0,'null'),
(113405,194,0,'null'),
(114073,220,0,'null'),
(114089,240,0,'NPC 32704'),
(114090,240,0,'NPC 32704'),
(114091,240,0,'NPC 32704'),
(115093,5,0,''),
(115094,2,0,''),
(115593,303,0,''),
(115594,301,0,''),
(115916,243,0,'NPC 33211'),
(115917,244,0,'NPC 33211'),
(115918,245,0,'NPC 33211'),
(115919,246,0,'NPC 33211'),
(115920,247,0,'NPC 33211'),
(115921,248,0,'NPC 33211'),
(115922,249,0,'NPC 33211'),
(115923,250,0,'NPC 33211'),
(115924,251,0,'NPC 33211'),
(115925,252,0,'NPC 33211'),
(115926,253,0,'NPC 33211'),
(115927,254,0,'NPC 33211'),
(116611,143,0,'null'),
(116946,1245,0,'NPC 462'),
(116952,1245,0,'NPC 462'),
(117452,1245,0,'NPC 462'),
(117453,1245,0,'NPC 462'),
(117454,1245,0,'NPC 462'),
(117455,1245,0,'NPC 462'),
(117456,1245,0,'NPC 462'),
(117457,1245,0,'NPC 462'),
(117458,1245,0,'NPC 462'),
(118414,1327,0,'NPC 14271'),
(119158,139,0,'null'),
(119208,141,0,'null'),
(119802,5675,0,'30872'),
(119803,5675,0,'30872'),
(119804,5675,0,'30872'),
(119805,5675,0,'30872'),
(119806,5676,0,'30872'),
(119807,5676,0,'30872'),
(119808,5676,0,'30872'),
(119809,5676,0,'30872'),
(119810,5677,0,'30872'),
(119811,5677,0,'30872'),
(119812,5677,0,'30872'),
(119813,5677,0,'30872'),
(119814,5678,0,'30872'),
(119815,5678,0,'30872'),
(119816,5678,0,'30872'),
(119819,5671,0,'30873'),
(119820,5671,0,'30873'),
(119821,5671,0,'30873'),
(119822,5672,0,'30873'),
(119823,5672,0,'30873'),
(119824,5672,0,'30873'),
(119825,5673,0,'30873'),
(119826,5673,0,'30873'),
(119827,5673,0,'30873'),
(119828,5674,0,'30873'),
(119829,5674,0,'30873'),
(119830,5674,0,'30873'),
(119844,5670,0,'30847'),
(119845,5669,0,'30847'),
(119846,5669,0,'30847'),
(119847,5669,0,'30847'),
(119848,5669,0,'30847'),
(119849,5668,0,'30847'),
(119850,5668,0,'30847'),
(119851,5668,0,'30847'),
(119852,5668,0,'30847'),
(119853,5667,0,'30847'),
(119854,5667,0,'30847'),
(119855,5667,0,'30847'),
(119856,5667,0,'30847'),
(119857,5666,0,'30847'),
(119858,5666,0,'30847'),
(119859,5666,0,'30847'),
(119860,5666,0,'30847'),
(119861,5665,0,'30847'),
(119862,5665,0,'30847'),
(119863,5665,0,'30847'),
(119864,5665,0,'30847'),
(120226,117,12,'32409 Crazed Indu\'le Survivor'),
(120434,212,0,'null'),
(121196,222,0,'null'),
(121700,182,0,'32377 Perobas the Bloodthirster'),
(122281,166,0,'null'),
(122621,141,2,'32471 Griegen'),
(123395,301,0,''),
(123396,301,0,''),
(123509,139,2,'33776 Gondria'),
(123510,139,2,'33776 Gondria'),
(123648,1269,0,'NPC 5400'),
(123649,1269,0,'NPC 5400'),
(123650,1269,0,'NPC 5400'),
(123826,324,0,'NPC 22062'),
(123874,1359,0,'NPC 18697'),
(123899,190,12,'10080 Sandarr Dunereaver'),
(123900,126,12,'10081 Dustwraith'),
(124143,127,12,'4842 Earthcaller Halmgar'),
(124144,1306,0,'NPC 10640'),
(124164,217,12,'11497 The Razza'),
(124165,203,12,'11498 Skarr the Unbreakable'),
(124184,208,6,'9217 Spirestone Lord Magus'),
(124188,1344,0,'NPC 18677'),
(124189,1346,0,'NPC 18679'),
(124190,1348,0,'NPC 18681'),
(124191,1352,0,'NPC 18686'),
(124192,176,12,'11447 Mushgog'),
(124234,112,12,'5915 Brother Ravenoak'),
(124235,322,0,'NPC 18241'),
(124236,322,0,'NPC 18241'),
(124237,322,0,'NPC 18241'),
(124238,322,0,'NPC 18241'),
(124239,322,0,'NPC 18241'),
(124240,322,0,'NPC 18241'),
(124241,322,0,'NPC 18241'),
(124242,322,0,'NPC 18241'),
(124434,1320,0,'NPC 14228'),
(124626,1354,0,'NPC 18692'),
(124627,1354,0,'NPC 18692'),
(124851,1321,0,'NPC 14229'),
(126467,1356,0,'NPC 18694'),
(126670,1350,0,'NPC 18684'),
(126671,1350,0,'NPC 18684'),
(126672,1342,0,'NPC 17144'),
(126674,1355,0,'NPC 18693'),
(126675,1355,0,'NPC 18693'),
(126676,1345,0,'NPC 18678'),
(126680,1345,0,'NPC 18678'),
(126682,1342,0,'NPC 17144'),
(126683,1342,0,'NPC 17144'),
(126684,1342,0,'NPC 17144'),
(126689,1357,0,'NPC 18695'),
(126690,1357,0,'NPC 18695'),
(126693,1360,0,'NPC 18698'),
(126879,1248,0,'NPC 572'),
(126972,1245,0,'NPC 462'),
(126973,1245,0,'NPC 462'),
(126974,1245,0,'NPC 462'),
(126975,1245,0,'NPC 462'),
(126976,1245,0,'NPC 462'),
(126977,1245,0,'NPC 462'),
(127625,186,12,'4438 Razorfen Spearhide'),
(127678,250,10,'NPC 33224'),
(127679,251,10,'NPC 33224'),
(127680,252,10,'NPC 33224'),
(127681,253,10,'NPC 33224'),
(127682,254,10,'NPC 33224'),
(127764,139,2,'33776 Gondria'),
(127765,139,2,'33776 Gondria'),
(127900,231,0,'null'),
(128279,240,0,'NPC 32704'),
(129374,1288,0,'NPC 8277'),
(129400,1288,0,'NPC 8277'),
(129401,1289,0,'NPC 8279'),
(129402,1289,0,'NPC 8279'),
(129403,1288,0,'NPC 8277'),
(129431,1289,0,'NPC 8279'),
(129589,1246,0,'NPC 521'),
(129590,1250,0,'NPC 947'),
(129591,1257,0,'NPC 2751'),
(129592,1257,0,'NPC 2751'),
(129593,313,0,'NPC 8300'),
(129594,1294,0,'NPC 8302'),
(129662,1289,0,'NPC 8279'),
(129664,1299,0,'NPC 8981'),
(129747,1288,0,'NPC 8277'),
(129880,242,0,'NPC 33211'),
(129881,241,0,'NPC 33211'),
(130360,196,0,'null'),
(130864,1353,0,'NPC 18690'),
(130865,1355,0,'NPC 18693'),
(130866,1356,0,'NPC 18694'),
(130867,1320,0,'NPC 14228'),
(130868,1331,0,'NPC 14281'),
(131394,1254,0,'NPC 2186'),
(131395,1254,0,'NPC 2186'),
(131396,308,0,'NPC 2452'),
(131397,308,0,'NPC 2452'),
(131398,309,0,'NPC 2453'),
(131399,1261,0,'NPC 3056'),
(131400,1261,0,'NPC 3056'),
(131401,179,4,'20932 Nuramoc'),
(131402,179,4,'20932 Nuramoc'),
(131403,1281,0,'NPC 8202'),
(131404,1281,0,'NPC 8202'),
(131405,311,0,'NPC 8210'),
(131406,311,0,'NPC 8210'),
(131407,1284,0,'NPC 8212'),
(131408,1284,0,'NPC 8212'),
(131409,1336,0,'NPC 14430'),
(131410,1344,0,'NPC 18677'),
(131411,1346,0,'NPC 18679'),
(131412,1346,0,'NPC 18679'),
(131413,1346,0,'NPC 18679'),
(131414,1347,0,'NPC 18680'),
(131415,1347,0,'NPC 18680'),
(131416,1347,0,'NPC 18680'),
(131417,1347,0,'NPC 18680'),
(131418,1348,0,'NPC 18681'),
(131419,1348,0,'NPC 18681'),
(131420,1350,0,'NPC 18684'),
(131421,1350,0,'NPC 18684'),
(131422,1352,0,'NPC 18686'),
(131423,1352,0,'NPC 18686'),
(131424,1352,0,'NPC 18686'),
(131425,1352,0,'NPC 18686'),
(131426,1353,0,'NPC 18690'),
(131427,1355,0,'NPC 18693'),
(131428,1355,0,'NPC 18693'),
(131429,1356,0,'NPC 18694'),
(131430,1356,0,'NPC 18694'),
(131431,1357,0,'NPC 18695'),
(131432,1357,0,'NPC 18695'),
(131433,1357,0,'NPC 18695'),
(131434,1360,0,'NPC 18698'),
(131435,1360,0,'NPC 18698'),
(131436,1360,0,'NPC 18698'),
(131437,1267,0,'NPC 5345'),
(131438,1273,0,'NPC 5837'),
(131439,1277,0,'NPC 6652'),
(131440,1278,0,'NPC 7015'),
(131441,1278,0,'NPC 7015'),
(131442,1285,0,'NPC 8213'),
(131443,1285,0,'NPC 8213'),
(131444,1286,0,'NPC 8214'),
(131445,1286,0,'NPC 8214'),
(131446,1287,0,'NPC 8219'),
(131447,315,0,'NPC 8978'),
(131448,315,0,'NPC 8978'),
(131449,315,0,'NPC 8978'),
(131450,1301,0,'NPC 10078'),
(131451,1301,0,'NPC 10078'),
(131452,1301,0,'NPC 10078'),
(131453,1302,0,'NPC 10197'),
(131454,1302,0,'NPC 10197'),
(131455,1302,0,'NPC 10197'),
(131456,1304,0,'NPC 10356'),
(131457,1304,0,'NPC 10356'),
(131458,1304,0,'NPC 10356'),
(131459,1304,0,'NPC 10356'),
(131460,1305,0,'NPC 10559'),
(131461,1305,0,'NPC 10559'),
(131462,1305,0,'NPC 10559'),
(131463,1306,0,'NPC 10640'),
(131464,318,0,'NPC 10817'),
(131465,318,0,'NPC 10817'),
(131466,318,0,'NPC 10817'),
(131467,318,0,'NPC 10817'),
(131468,318,0,'NPC 10817'),
(131469,1309,0,'NPC 10821'),
(131470,1310,0,'NPC 10824'),
(131471,1312,0,'NPC 10826'),
(131472,1312,0,'NPC 10826'),
(131473,1312,0,'NPC 10826'),
(131474,1313,0,'NPC 10827'),
(131475,1313,0,'NPC 10827'),
(131476,1314,0,'NPC 12037'),
(131477,1314,0,'NPC 12037'),
(131478,1315,0,'NPC 12431'),
(131479,1315,0,'NPC 12431'),
(131480,1315,0,'NPC 12431'),
(131481,1317,0,'NPC 14222'),
(131482,1317,0,'NPC 14222'),
(131483,1317,0,'NPC 14222'),
(131484,1317,0,'NPC 14222'),
(131485,1319,0,'NPC 14227'),
(131486,1319,0,'NPC 14227'),
(131487,1319,0,'NPC 14227'),
(131488,1319,0,'NPC 14227'),
(131489,1319,0,'NPC 14227'),
(131490,1320,0,'NPC 14228'),
(131491,1320,0,'NPC 14228'),
(131492,1320,0,'NPC 14228'),
(131493,1320,0,'NPC 14228'),
(131494,1321,0,'NPC 14229'),
(131495,1321,0,'NPC 14229'),
(131496,1321,0,'NPC 14229'),
(131497,1322,0,'NPC 14230'),
(131498,1322,0,'NPC 14230'),
(131499,1322,0,'NPC 14230'),
(131500,1322,0,'NPC 14230'),
(131501,1323,0,'NPC 14233'),
(131502,1323,0,'NPC 14233'),
(131503,1323,0,'NPC 14233'),
(131504,1323,0,'NPC 14233'),
(131505,1324,0,'NPC 14237'),
(131506,1324,0,'NPC 14237'),
(131507,1329,0,'NPC 14279'),
(131508,1329,0,'NPC 14279'),
(131509,1329,0,'NPC 14279'),
(131510,1329,0,'NPC 14279'),
(131511,1330,0,'NPC 14280'),
(131512,1330,0,'NPC 14280'),
(131513,1330,0,'NPC 14280'),
(131514,1331,0,'NPC 14281'),
(131515,1331,0,'NPC 14281'),
(131516,1332,0,'NPC 14339'),
(131517,1332,0,'NPC 14339'),
(131518,319,0,'NPC 14343'),
(131519,1334,0,'NPC 14344'),
(131520,1334,0,'NPC 14344'),
(131521,1335,0,'NPC 14427'),
(131522,1335,0,'NPC 14427'),
(131523,1335,0,'NPC 14427'),
(131524,1337,0,'NPC 14472'),
(131525,1337,0,'NPC 14472'),
(131526,1337,0,'NPC 14472'),
(131527,1338,0,'NPC 14476'),
(131528,1338,0,'NPC 14476'),
(131529,1338,0,'NPC 14476'),
(131530,1338,0,'NPC 14476'),
(131531,320,0,'NPC 14477'),
(131532,320,0,'NPC 14477'),
(131533,320,0,'NPC 14477'),
(131534,321,0,'NPC 14479'),
(131535,321,0,'NPC 14479'),
(131536,1358,0,'NPC 18696'),
(131537,1358,0,'NPC 18696'),
(131538,1358,0,'NPC 18696'),
(131539,1358,0,'NPC 18696'),
(131540,1359,0,'NPC 18697'),
(131541,1359,0,'NPC 18697'),
(131542,324,0,'NPC 22062'),
(131543,324,0,'NPC 22062'),
(131544,324,0,'NPC 22062'),
(297671,30000,0,'Molten Front Rare4'),
(298204,30001,0,'Mount Hyjal Rare4'),
(299199,30001,0,'Mount Hyjal Rare4'),
(301868,30004,0,'Vashj\'ir Rare4'),
(302161,30000,0,'Molten Front Rare4'),
(302184,30000,50,'Molten Front Rare4'),
(302345,30006,0,'Molten Front - Ready for Raiding II'),
(302372,30006,0,'Molten Front - Ready for Raiding II'),
(304208,30006,0,'Molten Front - Ready for Raiding II'),
(304256,30001,0,'Mount Hyjal Rare4'),
(304306,30000,0,'Molten Front Rare4'),
(306439,30000,0,'Molten Front Rare4'),
(317090,30004,0,'Vashj\'ir Rare2'),
(318193,30001,0,'Mount Hyjal Rare4'),
(318795,30002,0,'Uldum Rare2'),
(358024,30000,0,'Molten Front Rare4'),
(358555,30000,0,'Molten Front Rare4'),
(358637,30006,0,'Molten Front - Ready for Raiding II'),
(371474,30002,0,'Uldum Rare4'),
(371475,30002,0,'Uldum Rare4'),
(371476,30002,0,'Uldum Rare4'),
(371477,30001,0,'Mount Hyjal Rare2'),
(371478,30001,0,'Mount Hyjal Rare4'),
(371479,30002,0,'Uldum Rare4'),
(371480,30002,0,'Uldum Rare4'),
(371481,30002,0,'Uldum Rare4'),
(371482,30002,0,'Uldum Rare4'),
(371483,30002,0,'Uldum Rare4'),
(371484,30002,0,'Uldum Rare4'),
(371485,30003,0,'Twilight Highlands Rare2'),
(371486,30003,0,'Twilight Highlands Rare4'),
(371487,30003,0,'Twilight Highlands Rare4'),
(371488,30003,0,'Twilight Highlands Rare4'),
(371489,30003,0,'Twilight Highlands Rare4'),
(371490,30003,0,'Twilight Highlands Rare4'),
(371491,30005,0,'Deepholm Rare2'),
(371492,30005,0,'Deepholm Rare4'),
(371493,30005,0,'Deepholm Rare4'),
(371494,30005,0,'Deepholm Rare4'),
(371495,30005,0,'Deepholm Rare4'),
(371496,30004,0,'Vashj\'ir Rare4'),
(371497,30004,0,'Vashj\'ir Rare4'),
(371498,30004,0,'Vashj\'ir Rare4'),
(371499,30004,0,'Vashj\'ir Rare4'),
(371500,30004,0,'Vashj\'ir Rare4'),
(371501,30004,0,'Vashj\'ir Rare4'),
(371502,30004,0,'Vashj\'ir Rare4'),
(371503,30004,0,'Vashj\'ir Rare4'),
(373801,30001,0,'Mount Hyjal Rare4'),
(375106,30006,0,'Molten Front - Ready for Raiding II');
/*!40000 ALTER TABLE `pool_creature` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-07-05 13:04:53
