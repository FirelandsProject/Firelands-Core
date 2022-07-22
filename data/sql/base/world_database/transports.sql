/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `transports`;
CREATE TABLE IF NOT EXISTS `transports` (
  `entry` mediumint unsigned NOT NULL DEFAULT '0' COMMENT 'This references the Gameobject Template table''s unique ID.',
  `name` text COMMENT 'A name describing the transport.',
  `period` mediumint unsigned NOT NULL DEFAULT '0' COMMENT 'This is the amount of time that it take for the transport to make one full pass.',
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC COMMENT='Transports';

DELETE FROM `transports`;
/*!40000 ALTER TABLE `transports` DISABLE KEYS */;
INSERT INTO `transports` (`entry`, `name`, `period`) VALUES
	(20808, 'Ratchet and Booty Bay', 231236),
	(164871, 'Orgrimmar and Undercity', 255895),
	(175080, 'Grom\'Gol Base Camp and Orgrimmar', 244531),
	(176231, 'Menethil Harbor and Theramore Isle', 230016),
	(176310, 'Menethil Harbor and Auberdine', 235783),
	(176495, 'Grom\'Gol Base Camp and Undercity', 319210),
	(181646, 'Azuremyst and Auberdine', 238658),
	(181688, 'Valgarde and Menethil', 446334),
	(181689, 'Undercity and Vengeance Landing', 214579),
	(186238, 'Orgrimmar and Warsong Hold', 298829),
	(186371, 'Stolen Zeppelin', 484348),
	(187038, 'Pirate boat', 307953),
	(187568, 'Moa\'ki Harbor Turtle Boat', 445220),
	(188511, 'Unu\'pe Turtle Boat', 502354),
	(190536, 'Stormwind Harbor and Valiance Keep, Borean Tundra ("The Kraken")', 271979),
	(190549, 'Orgrimmar and Thunder Bluff', 599143),
	(192241, 'Horde gunship patrolling above Icecrown ("Orgrim\'s Hammer")', 1431158),
	(192242, 'Alliance gunship patrolling above Icecrown ("The Skybreaker")', 1051388),
	(195121, 'Alliance Gunship on Isle of Conquest BG', 118797),
	(195276, 'Horde Gunship on Isle of Conquest BG', 115661),
	(197195, 'Alliance Ship to Vashj\'ir', 317922),
	(201598, 'The Skybreaker', 23970),
	(201599, 'Orgrim\'s Hammer', 21134),
	(201811, 'Icecrown_Citadel_Alliance_10', 47740),
	(201812, 'Icecrown_Citadel_Horde_10', 82702),
	(203428, 'Orc Gunship in Gilneas (quest 26706)', 316236),
	(203466, 'Horde Ship to Vashj\'ir', 327895),
	(203620, 'Alliance Submarine to Leviathan Cave', 180413),
	(203621, 'Horde Submarine to Leviathan Cave', 209979),
	(203626, '<Need Name for entry: 203626>', 534650),
	(203730, 'Horde Submarine circling Abyssal Maw', 107129),
	(204018, 'Alliance Gunship patrolling in Deepholm', 178136),
	(206328, 'Krazzworks to Dragonmaw Port', 205674),
	(206329, 'Dragonmaw Port to Krazzworks', 205674),
	(207227, 'Krazzworks Attack Zeppelin (quest 28590)', 71606);
/*!40000 ALTER TABLE `transports` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
