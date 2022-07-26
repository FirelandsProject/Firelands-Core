/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `item_script_names`;
CREATE TABLE IF NOT EXISTS `item_script_names` (
  `Id` int unsigned NOT NULL,
  `ScriptName` varchar(64) NOT NULL,
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC;

DELETE FROM `item_script_names`;
/*!40000 ALTER TABLE `item_script_names` DISABLE KEYS */;
INSERT INTO `item_script_names` (`Id`, `ScriptName`) VALUES
	(24538, 'item_only_for_flight'),
	(30175, 'item_gor_dreks_ointment'),
	(31088, 'item_tainted_core'),
	(31742, 'item_nether_wraith_beacon'),
	(34475, 'item_only_for_flight'),
	(34489, 'item_only_for_flight'),
	(33098, 'item_petrov_cluster_bombs'),
	(35127, 'item_pile_fake_furs'),
	(35228, 'item_dehta_trap_smasher'),
	(35704, 'item_incendiary_explosives'),
	(35850, 'item_Trident_of_Nazjan'),
	(39878, 'item_mysterious_egg'),
	(44717, 'item_disgusting_jar'),
	(35205, 'internalitemhandler'),
	(35214, 'internalitemhandler'),
	(53510, 'item_captured_frog'),
	(49867, '423'),
	(5397, 'item_defias_gunpowder'),
	(45896, 'item_unbound_fragments_of_valanyr'),
	(74749, 'item_charged_decoder_ring'),
	(32971, 'item_water_bucket');
/*!40000 ALTER TABLE `item_script_names` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
