DELETE FROM `smart_scripts` WHERE `entryorguid`=35077 AND `source_type`=0;

INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(35077, 0, 0, 0, 19, 0, 100, 0, 14154, 0, 0, 0, 11, 66914, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "When player accepts quest 14154 - Self: Cast spell 66914 on Action invoker"),
(35077, 0, 1, 0, 0, 0, 100, 0, 6000, 6000, 15000, 17000, 11, 67825, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "When in combat and timer at the begining between 6000 and 6000 ms (and later repeats every 15000 and 17000 ms) - Self: Cast spell 67825 on Victim");

DELETE FROM `creature_template_addon` WHERE `entry`=35077;
