/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `instance_reset`;
CREATE TABLE IF NOT EXISTS `instance_reset` (
  `mapid` smallint unsigned NOT NULL DEFAULT '0',
  `difficulty` tinyint unsigned NOT NULL DEFAULT '0',
  `resettime` int unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`mapid`,`difficulty`),
  KEY `difficulty` (`difficulty`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;

DELETE FROM `instance_reset`;
/*!40000 ALTER TABLE `instance_reset` DISABLE KEYS */;
INSERT INTO `instance_reset` (`mapid`, `difficulty`, `resettime`) VALUES
	(33, 1, 1568779200),
	(36, 1, 1568779200),
	(249, 0, 1569124800),
	(249, 1, 1569124800),
	(269, 1, 1568779200),
	(409, 0, 1569124800),
	(469, 0, 1569124800),
	(509, 0, 1568865600),
	(531, 0, 1569124800),
	(532, 0, 1569124800),
	(533, 0, 1569124800),
	(533, 1, 1569124800),
	(534, 0, 1569124800),
	(540, 1, 1568779200),
	(542, 1, 1568779200),
	(543, 1, 1568779200),
	(544, 0, 1569124800),
	(545, 1, 1568779200),
	(546, 1, 1568779200),
	(547, 1, 1568779200),
	(548, 0, 1569124800),
	(550, 0, 1569124800),
	(552, 1, 1568779200),
	(553, 1, 1568779200),
	(554, 1, 1568779200),
	(555, 1, 1568779200),
	(556, 1, 1568779200),
	(557, 1, 1568779200),
	(558, 1, 1568779200),
	(560, 1, 1568779200),
	(564, 0, 1569124800),
	(565, 0, 1569124800),
	(568, 1, 1568779200),
	(574, 1, 1568779200),
	(575, 1, 1568779200),
	(576, 1, 1568779200),
	(578, 1, 1568779200),
	(580, 0, 1569124800),
	(585, 1, 1568779200),
	(595, 1, 1568779200),
	(598, 1, 1568779200),
	(599, 1, 1568779200),
	(600, 1, 1568779200),
	(601, 1, 1568779200),
	(602, 1, 1568779200),
	(603, 0, 1569124800),
	(603, 1, 1569124800),
	(604, 1, 1568779200),
	(608, 1, 1568779200),
	(615, 0, 1569124800),
	(615, 1, 1569124800),
	(616, 0, 1569124800),
	(616, 1, 1569124800),
	(619, 1, 1568779200),
	(624, 0, 1569124800),
	(624, 1, 1569124800),
	(631, 0, 1569124800),
	(631, 1, 1569124800),
	(631, 2, 1569124800),
	(631, 3, 1569124800),
	(632, 1, 1568779200),
	(643, 1, 1568779200),
	(644, 1, 1568779200),
	(645, 1, 1568779200),
	(649, 0, 1569124800),
	(649, 1, 1569124800),
	(649, 2, 1569124800),
	(649, 3, 1569124800),
	(650, 1, 1568779200),
	(657, 1, 1568779200),
	(658, 1, 1568779200),
	(668, 1, 1568779200),
	(669, 0, 1569124800),
	(669, 1, 1569124800),
	(669, 2, 1569124800),
	(669, 3, 1569124800),
	(670, 1, 1568779200),
	(671, 0, 1569124800),
	(671, 1, 1569124800),
	(671, 2, 1569124800),
	(671, 3, 1569124800),
	(720, 0, 1569124800),
	(720, 1, 1569124800),
	(720, 2, 1569124800),
	(720, 3, 1569124800),
	(724, 0, 1569124800),
	(724, 1, 1569124800),
	(724, 2, 1569124800),
	(724, 3, 1569124800),
	(725, 1, 1568779200),
	(754, 0, 1569124800),
	(754, 1, 1569124800),
	(754, 2, 1569124800),
	(754, 3, 1569124800),
	(755, 1, 1568779200),
	(757, 0, 1569124800),
	(757, 1, 1569124800),
	(859, 1, 1568779200),
	(938, 1, 1568779200),
	(939, 1, 1568779200),
	(940, 1, 1568779200),
	(967, 0, 1569124800),
	(967, 1, 1569124800),
	(967, 2, 1569124800),
	(967, 3, 1569124800);
/*!40000 ALTER TABLE `instance_reset` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
