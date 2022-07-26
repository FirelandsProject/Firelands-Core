/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP TABLE IF EXISTS `rbac_account_permissions`;
CREATE TABLE IF NOT EXISTS `rbac_account_permissions` (
  `accountId` int unsigned NOT NULL COMMENT 'Account id',
  `permissionId` int unsigned NOT NULL COMMENT 'Permission id',
  `granted` tinyint(1) NOT NULL DEFAULT '1' COMMENT 'Granted = 1, Denied = 0',
  `realmId` int NOT NULL DEFAULT '-1' COMMENT 'Realm Id, -1 means all',
  PRIMARY KEY (`accountId`,`permissionId`,`realmId`),
  KEY `fk__rbac_account_roles__rbac_permissions` (`permissionId`) USING BTREE
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 COMMENT='Account-Permission relation';

DELETE FROM `rbac_account_permissions`;
/*!40000 ALTER TABLE `rbac_account_permissions` DISABLE KEYS */;
/*!40000 ALTER TABLE `rbac_account_permissions` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
