/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `dungeonfinder_item_rewards`;
CREATE TABLE IF NOT EXISTS `dungeonfinder_item_rewards` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `min_level` smallint unsigned NOT NULL COMMENT 'dbc value',
  `max_level` smallint unsigned NOT NULL COMMENT 'dbc value',
  `item_reward` mediumint unsigned NOT NULL,
  `item_amount` mediumint unsigned NOT NULL,
  `dungeon_type` smallint unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3;

DELETE FROM `dungeonfinder_item_rewards`;
/*!40000 ALTER TABLE `dungeonfinder_item_rewards` DISABLE KEYS */;
INSERT INTO `dungeonfinder_item_rewards` (`id`, `min_level`, `max_level`, `item_reward`, `item_amount`, `dungeon_type`) VALUES
	(1, 15, 25, 51999, 1, 0),
	(2, 26, 35, 52000, 1, 0),
	(3, 36, 45, 52001, 1, 0),
	(4, 46, 55, 52002, 1, 0),
	(5, 56, 60, 52003, 1, 0),
	(6, 61, 64, 52004, 1, 1),
	(7, 65, 68, 52005, 1, 1),
	(8, 69, 80, 29434, 12, 3),
	(9, 80, 82, 49426, 2, 4),
	(10, 70, 75, 0, 0, 2);
/*!40000 ALTER TABLE `dungeonfinder_item_rewards` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
