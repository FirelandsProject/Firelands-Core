/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `ahbot_history`;
CREATE TABLE IF NOT EXISTS `ahbot_history` (
  `id` bigint NOT NULL AUTO_INCREMENT,
  `buytime` bigint NOT NULL,
  `item` bigint NOT NULL,
  `bid` bigint NOT NULL,
  `buyout` bigint NOT NULL,
  `won` bigint NOT NULL,
  `category` varchar(45) DEFAULT NULL COMMENT 'An Item category Id from the list below:',
  `auction_house` bigint NOT NULL,
  PRIMARY KEY (`id`),
  KEY `won` (`won`),
  KEY `category` (`category`),
  KEY `auction_house` (`auction_house`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;

DELETE FROM `ahbot_history`;
/*!40000 ALTER TABLE `ahbot_history` DISABLE KEYS */;
/*!40000 ALTER TABLE `ahbot_history` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
