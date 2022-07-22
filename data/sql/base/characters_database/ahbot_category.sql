/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `ahbot_category`;
CREATE TABLE IF NOT EXISTS `ahbot_category` (
  `id` bigint NOT NULL AUTO_INCREMENT COMMENT 'Unique identifier of the category',
  `category` varchar(45) DEFAULT NULL COMMENT 'An Item category Id',
  `multiplier` decimal(20,2) NOT NULL COMMENT 'The cost multiplier',
  `max_auction_count` bigint NOT NULL COMMENT 'The max number of items of this category in the AH. ',
  `expire_time` bigint NOT NULL COMMENT 'The UNIX time when the item will expire.',
  PRIMARY KEY (`id`),
  KEY `helper` (`category`,`multiplier`,`expire_time`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;

DELETE FROM `ahbot_category`;
/*!40000 ALTER TABLE `ahbot_category` DISABLE KEYS */;
/*!40000 ALTER TABLE `ahbot_category` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
