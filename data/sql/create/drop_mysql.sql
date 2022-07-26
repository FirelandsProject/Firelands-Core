REVOKE ALL PRIVILEGES ON * . * FROM 'cataclysm'@'localhost';

REVOKE ALL PRIVILEGES ON `cataclysm_world` . * FROM 'cataclysm'@'localhost';

REVOKE GRANT OPTION ON `cataclysm_world` . * FROM 'cataclysm'@'localhost';

REVOKE ALL PRIVILEGES ON `cataclysm_characters` . * FROM 'cataclysm'@'localhost';

REVOKE GRANT OPTION ON `cataclysm_characters` . * FROM 'cataclysm'@'localhost';

REVOKE ALL PRIVILEGES ON `cataclysm_auth` . * FROM 'cataclysm'@'localhost';

REVOKE GRANT OPTION ON `cataclysm_auth` . * FROM 'cataclysm'@'localhost';

DROP USER 'cataclysm'@'localhost';

DROP DATABASE IF EXISTS `cataclysm_world`;

DROP DATABASE IF EXISTS `cataclysm_characters`;

DROP DATABASE IF EXISTS `cataclysm_auth`;
