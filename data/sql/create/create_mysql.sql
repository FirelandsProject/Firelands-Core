CREATE USER 'cataclysm'@'localhost' IDENTIFIED BY 'cataclysm' WITH MAX_QUERIES_PER_HOUR 0 MAX_CONNECTIONS_PER_HOUR 0 MAX_UPDATES_PER_HOUR 0;

CREATE DATABASE `cataclysm_world` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;

CREATE DATABASE `cataclysm_characters` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;

CREATE DATABASE `cataclysm_auth` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;

GRANT ALL PRIVILEGES ON `cataclysm_world` . * TO 'cataclysm'@'localhost' WITH GRANT OPTION;

GRANT ALL PRIVILEGES ON `cataclysm_characters` . * TO 'cataclysm'@'localhost' WITH GRANT OPTION;

GRANT ALL PRIVILEGES ON `cataclysm_auth` . * TO 'cataclysm'@'localhost' WITH GRANT OPTION;
