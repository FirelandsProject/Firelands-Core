/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `arena_reward`;
CREATE TABLE IF NOT EXISTS `arena_reward` (
  `seasonID` int NOT NULL,
  `achievement` int NOT NULL DEFAULT '0',
  `itemEntry` int NOT NULL DEFAULT '0',
  `rank` int NOT NULL,
  `title` int NOT NULL DEFAULT '0',
  PRIMARY KEY (`seasonID`,`rank`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

DELETE FROM `arena_reward`;
/*!40000 ALTER TABLE `arena_reward` DISABLE KEYS */;
INSERT INTO `arena_reward` (`seasonID`, `achievement`, `itemEntry`, `rank`, `title`) VALUES
	(1, 2091, 30609, 0, 42),
	(1, 2091, 0, 1, 42),
	(1, 2092, 0, 2, 43),
	(1, 2093, 0, 3, 44),
	(1, 2090, 0, 4, 45),
	(2, 418, 71954, 0, 62),
	(2, 2091, 0, 1, 42),
	(2, 2092, 0, 2, 43),
	(2, 2093, 0, 3, 44),
	(2, 2090, 0, 4, 45),
	(3, 419, 37676, 0, 71),
	(3, 2091, 0, 1, 42),
	(3, 2092, 0, 2, 43),
	(3, 2093, 0, 3, 44),
	(3, 2090, 0, 4, 45),
	(4, 420, 43516, 0, 80),
	(4, 2091, 0, 1, 42),
	(4, 2092, 0, 2, 43),
	(4, 2093, 0, 3, 44),
	(4, 2090, 0, 4, 45),
	(5, 3336, 46708, 0, 157),
	(5, 2091, 0, 1, 42),
	(5, 2092, 0, 2, 43),
	(5, 2093, 0, 3, 44),
	(5, 2090, 0, 4, 45),
	(6, 3436, 46171, 0, 167),
	(6, 2091, 0, 1, 42),
	(6, 2092, 0, 2, 43),
	(6, 2093, 0, 3, 44),
	(6, 2090, 0, 4, 45),
	(7, 3758, 47840, 0, 169),
	(7, 2091, 0, 1, 42),
	(7, 2092, 0, 2, 43),
	(7, 2093, 0, 3, 44),
	(7, 2090, 0, 4, 45),
	(8, 4599, 50435, 0, 177),
	(8, 2091, 0, 1, 42),
	(8, 2092, 0, 2, 43),
	(8, 2093, 0, 3, 44),
	(8, 2090, 0, 4, 45),
	(9, 6002, 71339, 0, 279),
	(9, 2091, 0, 1, 42),
	(9, 2092, 0, 2, 43),
	(9, 2093, 0, 3, 44),
	(9, 2090, 0, 4, 45),
	(10, 6124, 71954, 0, 280),
	(10, 2091, 0, 1, 42),
	(10, 2092, 0, 2, 43),
	(10, 2093, 0, 3, 44),
	(10, 2090, 0, 4, 45),
	(11, 6002, 71339, 0, 279),
	(11, 2091, 0, 1, 42),
	(11, 2092, 0, 2, 43),
	(11, 2093, 0, 3, 44),
	(11, 2090, 0, 4, 45);
/*!40000 ALTER TABLE `arena_reward` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
