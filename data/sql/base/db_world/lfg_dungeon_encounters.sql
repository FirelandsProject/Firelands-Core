/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `lfg_dungeon_encounters`;
CREATE TABLE IF NOT EXISTS `lfg_dungeon_encounters` (
  `achievementId` int unsigned NOT NULL DEFAULT '0' COMMENT 'Achievement marking final boss completion',
  `dungeonId` int unsigned NOT NULL DEFAULT '0' COMMENT 'Dungeon entry from dbc',
  PRIMARY KEY (`achievementId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 COMMENT='ArkDB LFG';

DELETE FROM `lfg_dungeon_encounters`;
/*!40000 ALTER TABLE `lfg_dungeon_encounters` DISABLE KEYS */;
INSERT INTO `lfg_dungeon_encounters` (`achievementId`, `dungeonId`) VALUES
	(1092, 8),
	(1091, 6),
	(1094, 24),
	(1095, 276),
	(1096, 30),
	(1093, 164),
	(1097, 40),
	(1071, 140),
	(1069, 148),
	(1068, 137),
	(1072, 146),
	(1070, 170),
	(1074, 150),
	(1075, 151),
	(1076, 171),
	(1077, 147),
	(1078, 138),
	(1079, 172),
	(1080, 173),
	(1081, 174),
	(1082, 198),
	(1073, 149),
	(1504, 242),
	(1505, 226),
	(1515, 210),
	(1506, 241),
	(1507, 219),
	(1508, 215),
	(1509, 221),
	(1510, 217),
	(1511, 213),
	(1512, 212),
	(1513, 211),
	(1514, 205),
	(4027, 249),
	(4716, 252),
	(4721, 254),
	(4727, 256),
	(1242, 202),
	(1231, 225),
	(1241, 210),
	(1232, 204),
	(1233, 218),
	(1234, 214),
	(1235, 220),
	(1236, 216),
	(1237, 208),
	(1238, 207),
	(1239, 206),
	(1240, 203),
	(4026, 245),
	(4715, 251),
	(4720, 253),
	(4726, 255),
	(5739, 209),
	(5773, 3805),
	(5730, 5035),
	(5731, 5035),
	(5729, 5088),
	(5728, 5088),
	(5726, 5004),
	(5727, 5004),
	(5724, 4926),
	(5725, 4926),
	(5732, 4950),
	(5733, 4950),
	(5738, 1581),
	(5736, 5396),
	(5737, 5396),
	(5734, 4945),
	(5735, 4945);
/*!40000 ALTER TABLE `lfg_dungeon_encounters` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
