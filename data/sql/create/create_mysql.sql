CREATE USER 'firelands'@'%' IDENTIFIED BY 'uLsZo483XN8b8fqAPb8Z' WITH MAX_QUERIES_PER_HOUR 0 MAX_CONNECTIONS_PER_HOUR 0 MAX_UPDATES_PER_HOUR 0;

CREATE DATABASE `firelands_world` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;

CREATE DATABASE `firelands_characters` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;

CREATE DATABASE `firelands_auth` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;

CREATE DATABASE `firelands_hotfixes` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;

GRANT ALL PRIVILEGES ON `firelands_world` . * TO 'firelands'@'%' WITH GRANT OPTION;

GRANT ALL PRIVILEGES ON `firelands_characters` . * TO 'firelands'@'%' WITH GRANT OPTION;

GRANT ALL PRIVILEGES ON `firelands_auth` . * TO 'firelands'@'%' WITH GRANT OPTION;

GRANT ALL PRIVILEGES ON `firelands_hotfixes` . * TO 'firelands'@'%' WITH GRANT OPTION;
