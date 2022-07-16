REVOKE ALL PRIVILEGES ON * . * FROM 'firelands'@'%';

REVOKE ALL PRIVILEGES ON `world` . * FROM 'firelands'@'%';

REVOKE GRANT OPTION ON `world` . * FROM 'firelands'@'%';

REVOKE ALL PRIVILEGES ON `characters` . * FROM 'firelands'@'%';

REVOKE GRANT OPTION ON `characters` . * FROM 'firelands'@'%';

REVOKE ALL PRIVILEGES ON `auth` . * FROM 'firelands'@'%';

REVOKE GRANT OPTION ON `auth` . * FROM 'firelands'@'%';

REVOKE ALL PRIVILEGES ON `hotfixes` . * FROM 'firelands'@'%';

REVOKE GRANT OPTION ON `hotfixes` . * FROM 'firelands'@'%';

DROP USER 'firelands'@'%';

DROP DATABASE IF EXISTS `world`;

DROP DATABASE IF EXISTS `characters`;

DROP DATABASE IF EXISTS `auth`;

DROP DATABASE IF EXISTS `hotfixes`;
