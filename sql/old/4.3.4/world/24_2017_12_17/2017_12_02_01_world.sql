-- Warlock
DELETE FROM `spell_bonus_data` WHERE `entry` IN (172, 686, 6229, 980, 603, 6789, 689, 1120, 5676, 6353, 17877, 348, 1949, 5857, 42223, 30108, 27243, 27285, 30283, 29722, 47897, 48181, 50796, 50590, 54786, 3110, 54049, 7814, 86121, 71521, 77799);
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(172, 0.176, 0, 0, 0, 'Walock - Corruption'),
(686, 0.754, 0, 0, 0, 'Walock - Shadow Bolt'),
(6229, 0.807, 0, 0, 0, 'Walock - Shadow Ward'),
(980, 0, 0.133, 0, 0, 'Walock - Bane of Agony'),
(603, 0, 2.024, 0, 0, 'Walock - Bane of Doom'),
(6789, 0.188, 0, 0, 0, 'Walock - Death Coil'),
(689, 0, 0.129, 0, 0, 'Walock - Drain Life'),
(1120, 0, 0.378, 0, 0, 'Walock - Drain Soul'),
(5676, 0.378, 0, 0, 0, 'Walock - Searing Pain'),
(6353, 0.726, 0, 0, 0, 'Walock - Soul Fire'),
(17877, 1.056, 0, 0, 0, 'Walock - Shadowburn'),
(348, 0.22, 0.176, 0, 0, 'Walock - Immolate'),
(1949, 0, 0.095, 0, 0, 'Walock - Hellfire'),
(5857, 0.095, 0, 0, 0, 'Walock - Hellfire Triggered'),
(42223, 0.238, 0, 0, 0, 'Walock - Rain of Fire'),
(30108, 0, 0.2, 0, 0, 'Walock - Unstable Afflcition'),
(27243, 0, 0.2292, 0, 0, 'Walock - Seed of Corruption'),
(27285, 0.3, 0, 0, 0, 'Walock - Seed of Corruption Triggered'),
(30283, 0.214, 0, 0, 0, 'Walock - Shadowfury'),
(29722, 0.539, 0, 0, 0, 'Walock - Incinerate'),
(47897, 0.102, 0, 0, 0, 'Walock - Shadowflame'),
(48181, 0.5577, 0, 0, 0, 'Walock - Haunt'),
(50796, 0.628, 0, 0, 0, 'Walock - Chaos Bolt'),
(50590, 0.1, 0, 0, 0, 'Walock - Immolation Aura'),
(54786, 0.214, 0, 0, 0,  'Walock - Demon Leap'),
(3110, 0.657, 0, 0, 0, 'Walock - Imp - Firebolt'),
(54049, 1.228, 0, 0, 0, 'Walock - Shadow Bite'),
(7814, 0.612, 0, 0, 0, 'Walock - Lash of Pain'),
(86121, 0.2, 0, 0, 0, 'Walock - Soul Swap'),
(71521, 0.968, 0, 0, 0, 'Walock - Hand of Gul''Dan'),
(77799, 0.302, 0, 0, 0, 'Walock - Fel Flame');

