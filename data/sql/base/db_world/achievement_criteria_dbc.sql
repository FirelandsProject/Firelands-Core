/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `achievement_criteria_dbc`;
CREATE TABLE IF NOT EXISTS `achievement_criteria_dbc` (
  `ID` int NOT NULL DEFAULT '0',
  `achievement` int NOT NULL DEFAULT '0',
  `type` int NOT NULL DEFAULT '0',
  `value1` int NOT NULL DEFAULT '0',
  `value2` int NOT NULL DEFAULT '0',
  `additionalReqType1` int NOT NULL DEFAULT '0',
  `additionalReqValue1` int NOT NULL DEFAULT '0',
  `additionalReqType2` int NOT NULL DEFAULT '0',
  `additionalReqValue2` int NOT NULL DEFAULT '0',
  `completionFlag` int NOT NULL DEFAULT '0',
  `timedCriteriaStartType` int NOT NULL DEFAULT '0',
  `timeCriteriaMiscId` int NOT NULL DEFAULT '0',
  `timeLimit` int NOT NULL DEFAULT '0',
  `showOrder` int NOT NULL DEFAULT '0',
  `additionalConditionType1` int NOT NULL DEFAULT '0',
  `additionalConditionType2` int NOT NULL DEFAULT '0',
  `additionalConditionType3` int NOT NULL DEFAULT '0',
  `additionalConditionValue1` int NOT NULL DEFAULT '0',
  `additionalConditionValue2` int NOT NULL DEFAULT '0',
  `additionalConditionValue3` int NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

DELETE FROM `achievement_criteria_dbc`;
/*!40000 ALTER TABLE `achievement_criteria_dbc` DISABLE KEYS */;
INSERT INTO `achievement_criteria_dbc` (`ID`, `achievement`, `type`, `value1`, `value2`, `additionalReqType1`, `additionalReqValue1`, `additionalReqType2`, `additionalReqValue2`, `completionFlag`, `timedCriteriaStartType`, `timeCriteriaMiscId`, `timeLimit`, `showOrder`, `additionalConditionType1`, `additionalConditionType2`, `additionalConditionType3`, `additionalConditionValue1`, `additionalConditionValue2`, `additionalConditionValue3`) VALUES
	(17003, 5779, 28, 87649, 91, 0, 0, 0, 0, 39967, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(16241, 5415, 28, 89796, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(479, 273, 0, 15664, 1, 0, 0, 0, 0, 3274, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(7493, 2116, 57, 22999, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(7382, 2079, 57, 28788, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(5812, 1636, 57, 36941, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(15624, 5442, 27, 27370, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0),
	(17428, 5442, 28, 98914, 1, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0),
	(15649, 5454, 28, 98915, 1, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0),
	(6642, 1786, 30, 122, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(5275, 150, 109, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 17, 0, 0, 1519, 0),
	(3383, 1161, 39, 3, 2500, 0, 0, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0),
	(11137, 3580, 69, 65403, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 8, 17, 0, 66303, 85),
	(1282, 1282, 27, 11023, 1, 0, 0, 0, 0, 2, 0, 0, 0, 4, 0, 8, 0, 0, 62061, 0),
	(12992, 4624, 27, 25485, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(6626, 1785, 41, 34753, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 41, 0, 0, 3358, 0),
	(11085, 3557, 69, 61849, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 17, 0, 0, 85, 0),
	(7590, 2154, 28, 151004, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(3684, 233, 56, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
/*!40000 ALTER TABLE `achievement_criteria_dbc` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
