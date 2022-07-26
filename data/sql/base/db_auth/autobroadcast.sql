/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `autobroadcast`;
CREATE TABLE IF NOT EXISTS `autobroadcast` (
  `id` int DEFAULT NULL,
  `weight` int DEFAULT NULL,
  `text` longtext,
  `text_1` longtext,
  `text_2` longtext,
  `text_3` longtext,
  `text_4` longtext,
  `text_5` longtext,
  `text_6` longtext,
  `text_7` longtext,
  `RealmID` int DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

DELETE FROM `autobroadcast`;
/*!40000 ALTER TABLE `autobroadcast` DISABLE KEYS */;
/*!40000 ALTER TABLE `autobroadcast` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
