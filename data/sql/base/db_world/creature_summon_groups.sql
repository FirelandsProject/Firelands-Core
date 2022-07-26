/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `creature_summon_groups`;
CREATE TABLE IF NOT EXISTS `creature_summon_groups` (
  `summonerId` mediumint unsigned NOT NULL DEFAULT '0',
  `summonerType` tinyint unsigned NOT NULL DEFAULT '0',
  `groupId` tinyint unsigned NOT NULL DEFAULT '0',
  `entry` mediumint unsigned NOT NULL DEFAULT '0',
  `position_x` float NOT NULL DEFAULT '0',
  `position_y` float NOT NULL DEFAULT '0',
  `position_z` float NOT NULL DEFAULT '0',
  `orientation` float NOT NULL DEFAULT '0',
  `summonType` tinyint unsigned NOT NULL DEFAULT '0',
  `summonTime` int unsigned NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3;

DELETE FROM `creature_summon_groups`;
/*!40000 ALTER TABLE `creature_summon_groups` DISABLE KEYS */;
INSERT INTO `creature_summon_groups` (`summonerId`, `summonerType`, `groupId`, `entry`, `position_x`, `position_y`, `position_z`, `orientation`, `summonType`, `summonTime`) VALUES
	(55624, 0, 1, 55654, 3199.29, -4943.93, 189.524, 1.904, 3, 60000),
	(55624, 0, 1, 55656, 3194.09, -4949.69, 189.525, 1.585, 3, 60000),
	(55624, 0, 1, 55656, 3206.93, -4947.36, 189.525, 2.545, 3, 60000),
	(55624, 0, 1, 55503, 3180.4, -4941.18, 189.525, 6.153, 8, 0),
	(55085, 0, 1, 54500, 3456.42, -5074.84, 213.596, 2.132, 8, 0),
	(55085, 0, 1, 54500, 3450.35, -5078.85, 213.596, 2.132, 8, 0),
	(55085, 0, 2, 54500, 3331.25, -4889.83, 181.171, 3.8027, 8, 0),
	(55085, 0, 2, 54500, 3335.57, -4896, 181.171, 3.699, 8, 0),
	(55085, 0, 3, 54500, 3342.19, -4891.45, 181.171, 0.55497, 8, 0),
	(55085, 0, 3, 54500, 3338.59, -4885.65, 181.171, 0.55497, 8, 0),
	(54548, 0, 3, 55563, 4884.14, 147.92, 103.94, 2.35, 8, 0),
	(54548, 0, 3, 55563, 4883.64, 138.91, 103.94, 2.26, 8, 0),
	(54548, 0, 3, 55563, 4880.54, 147.6, 102.58, 2.78, 8, 0),
	(54548, 0, 3, 55563, 4877.97, 144.42, 101.73, 1.33, 8, 0),
	(54548, 0, 3, 55563, 4878.85, 140.05, 102.4, 1.67, 8, 0),
	(54548, 0, 3, 55559, 4880.37, 145.55, 102.48, 2.06, 8, 0),
	(54548, 0, 2, 54560, 4888.48, 198.455, 99.992, 0.994838, 8, 0),
	(54548, 0, 2, 55385, 4905.2, 230.731, 99.1528, 4.5204, 8, 0),
	(54548, 0, 2, 55384, 4911.98, 226.653, 99.1152, 3.57792, 8, 0),
	(54548, 0, 1, 55389, 4937.31, 267.479, 97.7318, 1.45, 8, 0),
	(54548, 0, 1, 55389, 4921.63, 267.392, 97.3737, 1.45, 8, 0);
/*!40000 ALTER TABLE `creature_summon_groups` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
