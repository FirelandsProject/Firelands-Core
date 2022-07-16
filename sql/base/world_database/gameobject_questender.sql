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
-- Table structure for table `gameobject_questender`
--

DROP TABLE IF EXISTS `gameobject_questender`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `gameobject_questender` (
  `id` int unsigned NOT NULL DEFAULT '0',
  `quest` int unsigned NOT NULL DEFAULT '0' COMMENT 'Quest Identifier',
  PRIMARY KEY (`id`,`quest`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `gameobject_questender`
--

LOCK TABLES `gameobject_questender` WRITE;
/*!40000 ALTER TABLE `gameobject_questender` DISABLE KEYS */;
INSERT INTO `gameobject_questender` VALUES (33,26356),(34,26355),(35,26353),(36,26354),(55,37),(56,45),(61,26797),(259,25804),(261,25802),(2059,26854),(2076,26300),(2076,26301),(2076,26325),(2076,26330),(2076,26332),(2083,26609),(2688,26042),(2701,26039),(2702,26041),(2908,749),(4141,29021),(4141,29022),(4141,29023),(7510,2399),(7923,941),(24776,264),(32569,1393),(35251,1454),(112948,25818),(131474,2278),(138492,26049),(138492,26341),(138492,26909),(138492,26912),(142151,25803),(142195,25365),(142487,2945),(142487,2951),(142487,2952),(142487,2953),(152097,3525),(152097,27022),(164869,4083),(164886,3363),(164888,4443),(164955,24724),(164955,24725),(164956,24728),(164956,24729),(164957,24726),(164957,24727),(175084,4601),(175084,4603),(175084,4605),(175085,4602),(175085,4604),(175085,4606),(176392,27053),(177544,27391),(179485,27118),(179517,27111),(179880,7761),(180025,7937),(180055,7944),(180056,7945),(180503,8307),(180715,8763),(180715,8799),(180743,8744),(180746,8767),(180746,8788),(180747,8768),(180748,8769),(180793,8803),(181150,9161),(181698,9529),(181748,9565),(181756,9550),(181758,9561),(182947,29643),(183770,10243),(184300,10316),(184825,10555),(185126,10793),(185165,10819),(185165,10820),(186585,11253),(187236,28878),(187236,29385),(187559,11580),(187564,11581),(187565,11605),(187565,11607),(187565,11609),(187565,11610),(187914,11732),(187916,11734),(187917,11735),(187919,11736),(187920,11737),(187921,11738),(187922,11739),(187923,11740),(187924,11741),(187925,11742),(187926,11743),(187927,11744),(187928,11745),(187929,11746),(187930,11747),(187931,11748),(187932,11749),(187933,11750),(187934,11751),(187935,11752),(187936,11753),(187937,11754),(187938,11755),(187939,11756),(187940,11757),(187941,11758),(187942,11759),(187943,11760),(187944,11761),(187945,11762),(187946,11763),(187947,11764),(187948,11765),(187949,11799),(187950,11800),(187951,11801),(187952,11802),(187953,11803),(187954,11766),(187955,11767),(187956,11768),(187957,11769),(187958,11770),(187959,11771),(187960,11772),(187961,11773),(187962,11774),(187963,11775),(187964,11776),(187965,11777),(187966,11778),(187967,11779),(187968,11780),(187969,11781),(187970,11782),(187971,11783),(187972,11784),(187973,11785),(187974,11786),(187975,11787),(188085,11901),(188365,12042),(188419,12030),(188419,12031),(189303,12286),(190034,12331),(190035,12345),(190036,12332),(190037,12333),(190038,12334),(190039,12335),(190040,12336),(190041,12337),(190043,12339),(190044,12343),(190045,12341),(190046,12342),(190047,12340),(190048,12344),(190050,12347),(190051,12348),(190052,12349),(190053,12350),(190054,12351),(190055,12352),(190056,12353),(190057,12354),(190058,12355),(190059,12356),(190060,12357),(190061,12358),(190062,12359),(190063,12360),(190064,12361),(190065,12362),(190066,12363),(190067,12364),(190068,12365),(190069,12366),(190070,12367),(190071,12368),(190072,12369),(190073,12370),(190074,12371),(190075,12373),(190076,12374),(190077,12375),(190078,12376),(190079,12377),(190080,12378),(190081,12379),(190082,12380),(190083,12381),(190084,12382),(190085,12383),(190086,12384),(190087,12385),(190088,12386),(190089,12387),(190090,12388),(190091,12389),(190096,12390),(190097,12391),(190098,12392),(190099,12393),(190100,12394),(190101,12395),(190102,12396),(190103,12397),(190104,12398),(190105,12399),(190106,12400),(190107,12401),(190108,12402),(190109,12403),(190110,12404),(190111,12404),(190112,12406),(190113,12407),(190114,12408),(190115,12409),(190116,12409),(190535,12565),(190535,12567),(190602,12615),(190602,12618),(190657,12655),(190657,12656),(190768,12691),(190777,12581),(190917,12711),(190936,12717),(190936,12718),(191760,13415),(191760,13416),(191761,12889),(191761,13843),(191766,12902),(191878,12940),(191879,12941),(191880,12946),(191881,12947),(191882,12944),(191883,12945),(192018,12950),(192060,12922),(192071,12981),(192078,12977),(192079,13003),(192080,13006),(192524,13046),(193195,13263),(193195,13389),(193400,13262),(193400,13388),(194032,13440),(194033,13441),(194034,13450),(194035,13442),(194036,13443),(194037,13451),(194038,13444),(194039,13453),(194040,13445),(194042,13454),(194043,13455),(194044,13446),(194045,13447),(194046,13457),(194048,13458),(194049,13449),(194056,13433),(194057,13434),(194058,13435),(194059,13436),(194060,13437),(194061,13438),(194062,13439),(194063,13448),(194064,13473),(194065,13452),(194066,13456),(194067,13459),(194068,13460),(194069,13461),(194070,13462),(194071,13463),(194072,13472),(194073,13464),(194074,13465),(194075,13466),(194076,13467),(194077,13468),(194078,13469),(194079,13470),(194080,13471),(194081,13474),(194084,13501),(194105,13521),(194119,13548),(194122,13528),(194555,13604),(194555,13614),(194555,13622),(194555,13629),(194555,13817),(194555,13818),(195431,14190),(195433,14189),(195433,14191),(195433,14193),(195433,14360),(195435,14192),(195445,14195),(195445,14196),(195497,14213),(195497,14217),(195497,14358),(195497,14359),(195517,14219),(195517,14357),(195600,14247),(195642,14267),(195642,14270),(195676,14308),(196394,14320),(196832,14428),(196833,14429),(201742,24545),(202135,24702),(202264,24734),(202335,24941),(202474,25069),(202474,25070),(202474,25566),(202598,25183),(202613,25204),(202697,25300),(202701,25296),(202701,25308),(202701,25314),(202706,25297),(202712,25301),(202712,25303),(202714,27393),(202759,25377),(202859,25467),(202916,25503),(202975,25524),(202975,25526),(203134,25643),(203140,27394),(203301,25883),(203305,25862),(203395,27668),(204050,26335),(204351,26519),(204450,26650),(204450,26662),(204450,26663),(204450,26664),(204578,26678),(204817,26725),(204824,26753),(204825,26722),(204959,26869),(205134,26260),(205143,27039),(205198,27092),(205207,27101),(205258,27170),(205350,27231),(205874,27602),(205874,27623),(205875,27460),(206293,27760),(206293,27761),(206293,27777),(206335,27796),(206335,27883),(206336,27797),(206336,27884),(206374,27912),(206374,27913),(206504,27930),(206569,27947),(206569,27951),(206569,28241),(206569,28242),(206585,27989),(206585,27994),(206585,28100),(207104,28335),(207104,28385),(207125,28322),(207125,28456),(207179,28464),(207291,28630),(207406,28798),(207407,28799),(207408,28800),(207409,28801),(207410,28802),(207411,28803),(207412,28804),(207982,28910),(207983,28911),(207984,28912),(207985,28913),(207986,28914),(207987,28915),(207988,28916),(207989,28917),(207990,28918),(207991,28919),(207992,28920),(207993,28921),(208089,28943),(208090,28944),(208093,28947),(208094,28948),(208115,28951),(208116,28952),(208117,28953),(208118,28958),(208119,28989),(208121,28956),(208122,28960),(208123,28961),(208124,28963),(208125,28964),(208126,28968),(208127,28970),(208128,28977),(208129,28980),(208130,28978),(208131,28979),(208132,28985),(208133,28983),(208134,28988),(208135,28991),(208136,28990),(208137,28955),(208138,28965),(208139,28967),(208140,28992),(208141,28981),(208142,28982),(208143,28957),(208144,28959),(208145,28962),(208146,28966),(208147,28969),(208148,28971),(208149,28972),(208150,28973),(208151,28974),(208152,28975),(208153,28976),(208154,28984),(208155,28986),(208156,28987),(208157,28993),(208158,28994),(208159,28995),(208160,28996),(208161,28998),(208162,28999),(208163,29000),(208164,29001),(208165,29002),(208166,29003),(208167,29004),(208168,29005),(208169,29006),(208170,29007),(208171,29008),(208172,29009),(208173,29010),(208174,29011),(208175,29012),(208176,29013),(208177,29014),(208178,29016),(208179,29017),(208180,29018),(208181,29019),(208183,29020),(208184,29030),(208187,29031),(208188,29036),(208316,29071),(208317,29073),(208420,29155),(208420,29253),(208549,29261),(208550,29262),(208825,29314),(209072,29403),(209076,29413),(209094,29427),(209095,29429);
/*!40000 ALTER TABLE `gameobject_questender` ENABLE KEYS */;
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