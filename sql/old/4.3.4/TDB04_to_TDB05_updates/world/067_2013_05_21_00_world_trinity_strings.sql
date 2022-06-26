DELETE FROM `firelands_string` WHERE entry IN (453, 548, 549, 550, 714, 716, 749, 752, 843, 844, 845, 846, 847, 848, 849, 850, 851, 852, 853, 854, 855, 856); 
INSERT INTO `firelands_string` (entry, content_default) VALUES
(453,'│Player %s %s (guid: %u)'),
(548,'│ GM Mode active, Phase: -1'),
(549,'├─ Banned: (Type: %s, Reason: %s, Time: %s, By: %s)'),
(550,'├─ Muted: (Reason: %s, Time: %s, By: %s)'),
(714,'│ Account: %s (ID: %u), GMLevel: %u'),
(716,'│ Last Login: %s (Failed Logins: %u)'),
(749,'│ OS: %s - Latency: %u ms - Mail: %s'),
(752,'│ Last IP: %s (Locked: %s)'),
(843,'│ Level: %u (%u/%u XP (%u XP left))'),
(844,'│ Race: %s %s, %s'),
(845,'│ Alive ?: %s'),
(846,'│ Phase: %u'),
(847,'│ Money: %ug%us%uc'),
(848,'│ Map: %s, Area: %s, Zone: %s'),
(849,'│ Guild: %s (ID: %u)'),
(850,'├─ Rank: %s'),
(851,'├─ Note: %s'),
(852,'├─ O. Note: %s'),
(853,'│ Played time: %s'),
(854,'└ Mails: %d Read/%I64u Total'),
(855,'Male'),
(856,'Female');
