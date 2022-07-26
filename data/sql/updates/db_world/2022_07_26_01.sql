DELETE FROM `smart_scripts` WHERE `entryorguid`=35456 AND `source_type`=0;

INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(35456, 0, 0, 0, 2, 0, 100, 1, 1, 20, 100, 200, 11, 8599, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Worgen Alpha 2 - If health below 20%, Cast Enrage on Self (from any phase)'),
(35456, 0, 1, 0, 0, 0, 70, 0, 5000, 7000, 5000, 7000, 11, 75355, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Worgen Alpha 2 - In Combat, every 5 - 7 seconds, 70% chance to cast Horrifying Howl on victim (from any phase)'),
(35456, 0, 2, 0, 60, 0, 100, 1, 0, 0, 0, 0, 53, 1, 3516700, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Worgen Alpha 2 - On spawn, Run to the edge of the rooftop'),
(35456, 0, 3, 0, 60, 0, 100, 1, 2500, 2500, 0, 0, 97, 20, 20, 0, 0, 0, 0, 8, 0, 0, 0, -1681.94, 1453.67, 52.28, 0, 'Worgen Alpha 2 - At the edge of the roof, cast jump to invisible stalker'),
(35456, 0, 4, 0, 60, 0, 100, 1, 5000, 5000, 0, 0, 49, 0, 0, 0, 0, 0, 0, 19, 35124, 100, 0, 0, 0, 0, 0, 'Worgen Alpha 2 - Attack Closest Player');

