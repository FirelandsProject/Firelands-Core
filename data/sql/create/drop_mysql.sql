REVOKE ALL PRIVILEGES ON * . * FROM 'firelands'@'localhost';

REVOKE ALL PRIVILEGES ON `firelands_world` . * FROM 'firelands'@'localhost';

REVOKE GRANT OPTION ON `firelands_world` . * FROM 'firelands'@'localhost';

REVOKE ALL PRIVILEGES ON `firelands_characters` . * FROM 'firelands'@'localhost';

REVOKE GRANT OPTION ON `firelands_characters` . * FROM 'firelands'@'localhost';

REVOKE ALL PRIVILEGES ON `firelands_auth` . * FROM 'firelands'@'localhost';

REVOKE GRANT OPTION ON `firelands_auth` . * FROM 'firelands'@'localhost';

DROP USER 'firelands'@'localhost';

DROP DATABASE IF EXISTS `firelands_world`;

DROP DATABASE IF EXISTS `firelands_characters`;

DROP DATABASE IF EXISTS `firelands_auth`;
