REVOKE ALL PRIVILEGES ON * . * FROM 'firelands'@'%';

REVOKE ALL PRIVILEGES ON `firelands_world` . * FROM 'firelands'@'%';

REVOKE GRANT OPTION ON `firelands_world` . * FROM 'firelands'@'%';

REVOKE ALL PRIVILEGES ON `firelands_characters` . * FROM 'firelands'@'%';

REVOKE GRANT OPTION ON `firelands_characters` . * FROM 'firelands'@'%';

REVOKE ALL PRIVILEGES ON `firelands_auth` . * FROM 'firelands'@'%';

REVOKE GRANT OPTION ON `firelands_auth` . * FROM 'firelands'@'%';

REVOKE ALL PRIVILEGES ON `firelands_hotfixes` . * FROM 'firelands'@'%';

REVOKE GRANT OPTION ON `firelands_hotfixes` . * FROM 'firelands'@'%';

DROP USER 'firelands'@'%';

DROP DATABASE IF EXISTS `firelands_world`;

DROP DATABASE IF EXISTS `firelands_characters`;

DROP DATABASE IF EXISTS `firelands_auth`;

DROP DATABASE IF EXISTS `firelands_hotfixes`;
