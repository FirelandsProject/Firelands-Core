DELETE FROM `firelands_string` WHERE `entry`=63; -- Existing entry 63 is not present in Language.h. It's safe to remove it.
INSERT INTO `firelands_string` (`entry`, `content_default`) VALUES
(63, 'You can''t do that right now.');
