--
-- Table structure for table `account`
--
DROP TABLE IF EXISTS `account`;
CREATE TABLE IF NOT EXISTS `account` (
  `id` INT unsigned NOT NULL AUTO_INCREMENT COMMENT 'Identifier',
  `username` VARCHAR(32) NOT NULL DEFAULT '',
  `salt` binary(32) NOT NULL,
  `verifier` binary(32) NOT NULL,
  `session_key` binary(40) DEFAULT NULL,
  `totp_secret` varbinary(128) DEFAULT NULL,
  `email` VARCHAR(255) NOT NULL DEFAULT '',
  `reg_mail` VARCHAR(255) NOT NULL DEFAULT '',
  `joindate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `last_ip` VARCHAR(15) NOT NULL DEFAULT '127.0.0.1',
  `last_attempt_ip` VARCHAR(15) NOT NULL DEFAULT '127.0.0.1',
  `failed_logins` INT unsigned NOT NULL DEFAULT 0,
  `locked` TINYINT unsigned NOT NULL DEFAULT 0,
  `lock_country` VARCHAR(2) NOT NULL DEFAULT '00',
  `last_login` timestamp NULL DEFAULT NULL,
  `online` INT unsigned NOT NULL DEFAULT 0,
  `expansion` TINYINT unsigned NOT NULL DEFAULT '3',
  `mutetime` BIGINT NOT NULL DEFAULT 0,
  `mutereason` VARCHAR(255) NOT NULL DEFAULT '',
  `muteby` VARCHAR(50) NOT NULL DEFAULT '',
  `locale` TINYINT unsigned NOT NULL DEFAULT 0,
  `os` VARCHAR(3) NOT NULL DEFAULT '',
  `recruiter` INT unsigned NOT NULL DEFAULT 0,
  `totaltime` INT unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`),
  UNIQUE KEY `idx_username` (`username`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='Account System';
/*!40101 SET character_set_client = @saved_cs_client */;
