-- Fix up formation from last commit
DELETE FROM `creature_formations` WHERE `leaderGUID`=57594;
INSERT INTO `creature_formations` (`leaderGUID`, `memberGUID`, `dist`, `angle`, `groupAI`) VALUES
(57594, 57594, 0, 0, 2),
(57594, 57595, 5, 0, 2),
(57594, 57596, 10, 0, 2),
(57594, 57597, 15, 0, 2);

DELETE FROM `creature_formations` WHERE `leaderGUID`=57965;
INSERT INTO `creature_formations` (`leaderGUID`, `memberGUID`, `dist`, `angle`, `groupAI`) VALUES
(57965, 57965, 0, 0, 2),
(57965, 57966, 5, 0, 2),
(57965, 57967, 10, 0, 2),
(57965, 57968, 15, 0, 2);

-- Pathing for Bonechewer Raider Entry: 16925 'TDB FORMAT'
SET @NPC := 58706;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2,`position_x`=-372.7111,`position_y`=2839.348,`position_z`=3.085857 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,17408,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_type`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,-372.7111,2839.348,3.085857,0,0,0,0,100,0),
(@PATH,2,-369.1973,2833.291,3.833567,0,0,0,0,100,0),
(@PATH,3,-366.1855,2828.1,4.404455,0,0,0,0,100,0),
(@PATH,4,-364.1777,2824.639,5.029455,0,0,0,0,100,0),
(@PATH,5,-363.1738,2822.908,5.654455,0,0,0,0,100,0),
(@PATH,6,-361.668,2820.313,6.279455,0,0,0,0,100,0),
(@PATH,7,-360.6641,2818.582,7.029455,0,0,0,0,100,0),
(@PATH,8,-357.6523,2813.391,7.904455,0,0,0,0,100,0),
(@PATH,9,-354.6406,2808.199,8.654455,0,0,0,0,100,0),
(@PATH,10,-352.1309,2803.873,9.279455,0,0,0,0,100,0);
-- 0x1C09E4424010874000002C0000212A8F .go -372.7111 2839.348 3.085857

-- Pathing for Bonechewer Raider Entry: 16925 'TDB FORMAT'
SET @NPC := 58710;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2,`position_x`=-173.5004,`position_y`=2340.303,`position_z`=60.07005 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,17408,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_type`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,-173.5004,2340.303,60.07005,0,0,0,0,100,0),
(@PATH,2,-167.5776,2345.781,59.66853,0,0,0,0,100,0),
(@PATH,3,-168.3658,2358.138,57.73066,0,0,0,0,100,0),
(@PATH,4,-170.6648,2371.729,55.37167,0,0,0,0,100,0),
(@PATH,5,-176.5131,2386.787,51.77451,0,0,0,0,100,0),
(@PATH,6,-179.7427,2391.649,50.90343,0,0,0,0,100,0),
(@PATH,7,-179.451,2411.232,48.48401,0,0,0,0,100,0),
(@PATH,8,-181.9889,2397.549,50.08649,0,0,0,0,100,0),
(@PATH,9,-179.687,2391.666,50.90021,0,0,0,0,100,0),
(@PATH,10,-172.3989,2376.937,54.03761,0,0,0,0,100,0),
(@PATH,11,-168.9601,2362.579,56.66465,0,0,0,0,100,0),
(@PATH,12,-167.4705,2349.302,59.07915,0,0,0,0,100,0);
-- 0x1C09E4424010874000002C000021381E .go -173.5004 2340.303 60.07005

-- Pathing for Bonechewer Raider Entry: 16925 'TDB FORMAT'
SET @NPC := 58709;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2,`position_x`=-280.5386,`position_y`=2385.487,`position_z`=49.5167 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,17408,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_type`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,-280.5386,2385.487,49.5167,0,0,0,0,100,0),
(@PATH,2,-274.4911,2399.897,49.77866,0,0,0,0,100,0),
(@PATH,3,-282.391,2415.943,47.12805,0,0,0,0,100,0),
(@PATH,4,-292.0588,2418.96,45.69984,0,0,0,0,100,0),
(@PATH,5,-296.9233,2425.813,44.45638,0,0,0,0,100,0),
(@PATH,6,-288.1144,2443.262,43.60903,0,0,0,0,100,0),
(@PATH,7,-293.5082,2451.608,42.39658,0,0,0,0,100,0),
(@PATH,8,-297.1542,2466.072,41.27671,0,0,0,0,100,0),
(@PATH,9,-296.9443,2470.941,40.92609,0,0,0,0,100,0),
(@PATH,10,-284.6176,2477.495,40.50471,0,0,0,0,100,0),
(@PATH,11,-290.6595,2483.031,40.95429,0,0,0,0,100,0),
(@PATH,12,-284.6285,2477.351,40.70044,0,0,0,0,100,0),
(@PATH,13,-297.5089,2457.768,41.62237,0,0,0,0,100,0),
(@PATH,14,-289.9194,2446.963,43.04158,0,0,0,0,100,0),
(@PATH,15,-287.7819,2443.668,43.38817,0,0,0,0,100,0),
(@PATH,16,-296.9543,2424.329,44.54842,0,0,0,0,100,0),
(@PATH,17,-284.9567,2418.073,46.32718,0,0,0,0,100,0),
(@PATH,18,-276.5181,2404.314,49.4864,0,0,0,0,100,0),
(@PATH,19,-280.4977,2385.4,49.48282,0,0,0,0,100,0);
-- 0x1C09E4424010874000002C0000213910 .go -280.5386 2385.487 49.5167

-- Pathing for Bonechewer Raider Entry: 16925 'TDB FORMAT'
SET @NPC := 58708;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2,`position_x`=-314.1599,`position_y`=2611.343,`position_z`=41.12054 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,17408,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_type`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,-314.1599,2611.343,41.12054,0,0,0,0,100,0),
(@PATH,2,-302.6028,2572.273,41.00031,0,0,0,0,100,0),
(@PATH,3,-297.84,2550.173,41.36596,0,0,0,0,100,0),
(@PATH,4,-303.8246,2525.236,42.58739,0,0,0,0,100,0),
(@PATH,5,-297.9362,2545.895,41.70477,0,0,0,0,100,0),
(@PATH,6,-300.882,2566.604,41.32217,0,0,0,0,100,0),
(@PATH,7,-306.6208,2578.347,41.52526,0,0,0,0,100,0),
(@PATH,8,-312.7142,2588.381,41.36144,0,0,0,0,100,0);
-- 0x1C09E4424010874000002C00002139B5 .go -314.1599 2611.343 41.12054

-- Add missing Bonechewer Raider
DELETE FROM `creature` WHERE `guid`=29980;
INSERT INTO `creature` (`guid`, `id`, `map`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `curhealth`, `MovementType`) VALUES
(29980, 16925, 530, 1, -354.3998, 2663.006, 41.72623, 0, 300, 3989, 2);

-- Pathing for Bonechewer Raider Entry: 16925 'TDB FORMAT'
SET @NPC := 29980;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2,`position_x`=-354.3998,`position_y`=2663.006,`position_z`=41.72623 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,17408,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_type`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,-354.3998,2663.006,41.72623,0,0,0,0,100,0),
(@PATH,2,-346.7007,2672.114,38.15508,0,0,0,0,100,0),
(@PATH,3,-332.8381,2681.229,34.02082,0,0,0,0,100,0),
(@PATH,4,-319.6889,2687.015,31.10067,0,0,0,0,100,0),
(@PATH,5,-318.0042,2694.25,29.72155,0,0,0,0,100,0),
(@PATH,6,-324.0378,2716.413,25.33266,0,0,0,0,100,0),
(@PATH,7,-319.7849,2729.284,23.22117,0,0,0,0,100,0),
(@PATH,8,-322.0183,2746.254,18.96142,0,0,0,0,100,0),
(@PATH,9,-322.4724,2747.062,18.86184,0,0,0,0,100,0),
(@PATH,10,-322.3822,2746.857,18.3417,0,0,0,0,100,0),
(@PATH,11,-319.7568,2733.282,22.47406,0,0,0,0,100,0),
(@PATH,12,-323.8112,2718.555,24.77036,0,0,0,0,100,0),
(@PATH,13,-317.9898,2696.302,28.92368,0,0,0,0,100,0),
(@PATH,14,-318.5087,2690.603,30.36932,0,0,0,0,100,0),
(@PATH,15,-329.6577,2682.807,32.7053,0,0,0,0,100,0),
(@PATH,16,-344.2849,2674.339,36.96185,0,0,0,0,100,0);
-- 0x1C09E4424010874000002C0000213896 .go -354.3998 2663.006 41.72623

-- Only one Tagar Spinebreaker should be spawned
DELETE FROM `creature` WHERE `guid`=85990;

-- Pathing for Tagar Spinebreaker Entry: 19443 'TDB FORMAT'
SET @NPC := 85987;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2,`position_x`=-179.7917,`position_y`=2839.993,`position_z`=23.64687 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,0,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_type`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,-179.7917,2839.993,23.64687,0,0,0,0,100,0),
(@PATH,2,-168.0662,2837.727,26.36559,0,0,0,0,100,0),
(@PATH,3,-149.3422,2826.91,33.67762,0,0,0,0,100,0),
(@PATH,4,-149.3403,2813.561,35.25396,0,0,0,0,100,0),
(@PATH,5,-160.4427,2807.621,32.6923,0,0,0,0,100,0),
(@PATH,6,-172.6706,2803.528,29.62396,0,0,0,0,100,0),
(@PATH,7,-185.5681,2808.792,25.53679,0,0,0,0,100,0),
(@PATH,8,-186.3388,2797.655,26.33011,0,0,0,0,100,0),
(@PATH,9,-153.882,2788.701,36.85746,0,0,0,0,100,0),
(@PATH,10,-134.5498,2778.104,41.05849,0,0,0,0,100,0),
(@PATH,11,-120.4556,2753.937,49.37396,0,0,0,0,100,0),
(@PATH,12,-110.122,2737.952,52.16914,0,0,0,0,100,0),
(@PATH,13,-108.798,2709.016,51.00445,0,0,0,0,100,0),
(@PATH,14,-125.7525,2695.987,46.92542,0,0,0,0,100,0),
(@PATH,15,-165.7749,2686.863,43.39313,0,0,0,0,100,0),
(@PATH,16,-166.924,2686.502,43.27299,0,0,0,0,100,0),
(@PATH,17,-170.6192,2661.56,42.24799,0,0,0,0,100,0),
(@PATH,18,-166.9141,2633.801,41.55241,0,0,0,0,100,0),
(@PATH,19,-165.2749,2618.521,40.12231,0,0,0,0,100,0),
(@PATH,20,-164.5241,2612.235,40.08208,0,0,0,0,100,0),
(@PATH,21,-162.7764,2603.097,40.06981,0,0,0,0,100,0),
(@PATH,22,-165.0483,2590.722,39.90811,0,0,0,0,100,0),
(@PATH,23,-144.7979,2569.513,41.10892,0,0,0,0,100,0),
(@PATH,24,-146.1405,2544.73,40.98074,0,0,0,0,100,0),
(@PATH,25,-156.8518,2539.49,42.95174,0,0,0,0,100,0),
(@PATH,26,-170.4912,2527.817,41.62189,0,0,0,0,100,0),
(@PATH,27,-177.3363,2516.895,41.11181,0,0,0,0,100,0),
(@PATH,28,-176.3846,2510.593,41.94983,0,0,0,0,100,0),
(@PATH,29,-168.8438,2501.025,43.79488,0,0,0,0,100,0),
(@PATH,30,-145.8467,2499.76,45.61186,0,0,0,0,100,0),
(@PATH,31,-131.4405,2495.996,46.55548,0,0,0,0,100,0),
(@PATH,32,-124.8685,2492.812,46.93701,0,0,0,0,100,0),
(@PATH,33,-118.3926,2470.393,46.95731,0,0,0,0,100,0),
(@PATH,34,-134.5914,2450.625,46.18983,0,0,0,0,100,0),
(@PATH,35,-139.636,2439.596,48.65514,0,0,0,0,100,0),
(@PATH,36,-153.0726,2420.765,44.33873,0,0,0,0,100,0),
(@PATH,37,-164.3153,2415.917,48.36614,0,0,0,0,100,0),
(@PATH,38,-176.0278,2408.138,48.95763,0,0,0,0,100,0),
(@PATH,39,-181.3092,2397.231,50.16575,0,0,0,0,100,0),
(@PATH,40,-174.2651,2386.537,51.88837,0,0,0,0,100,0),
(@PATH,41,-171.3092,2382.064,53.20797,0,0,0,0,100,0),
(@PATH,42,-168.7256,2366.751,56.15277,0,0,0,0,100,0),
(@PATH,43,-168.0793,2352.929,58.49963,0,0,0,0,100,0),
(@PATH,44,-168.3409,2341.554,60.14597,0,0,0,0,100,0),
(@PATH,45,-175.721,2327.79,62.15151,0,0,0,0,100,0),
(@PATH,46,-193.0925,2317.189,56.10988,0,0,0,0,100,0),
(@PATH,47,-211.1901,2312.889,49.67965,0,0,0,0,100,0),
(@PATH,48,-243.0014,2311.802,51.36759,0,0,0,0,100,0),
(@PATH,49,-257.705,2324.297,56.98617,0,0,0,0,100,0),
(@PATH,50,-267.8463,2334.503,54.92202,0,0,0,0,100,0),
(@PATH,51,-284.0244,2347.267,51.85397,0,0,0,0,100,0),
(@PATH,52,-285.5917,2348.854,51.30281,0,0,0,0,100,0),
(@PATH,53,-277.6388,2384.371,49.04704,0,0,0,0,100,0),
(@PATH,54,-277.4207,2389,49.55646,0,0,0,0,100,0),
(@PATH,55,-282.4726,2415.647,47.11025,0,0,0,0,100,0),
(@PATH,56,-290.6383,2417.861,45.76169,0,0,0,0,100,0),
(@PATH,57,-296.9912,2425.083,44.68782,0,0,0,0,100,0),
(@PATH,58,-297.4091,2429.13,44.28957,0,0,0,0,100,0),
(@PATH,59,-292.8183,2446.393,42.99865,0,0,0,0,100,0),
(@PATH,60,-297.4739,2458.016,41.8229,0,0,0,0,100,0),
(@PATH,61,-299.0847,2461.294,41.30234,0,0,0,0,100,0),
(@PATH,62,-291.5309,2472.148,40.82636,0,0,0,0,100,0),
(@PATH,63,-315.7325,2479.059,38.88892,0,0,0,0,100,0),
(@PATH,64,-333.1553,2479.061,30.43475,0,0,0,0,100,0),
(@PATH,65,-362.027,2481.302,26.76814,0,0,0,0,100,0),
(@PATH,66,-381.832,2489.02,35.44767,0,0,0,0,100,0),
(@PATH,67,-383.683,2499.09,43.07095,0,0,0,0,100,0),
(@PATH,68,-373.0249,2518.307,44.52934,0,0,0,0,100,0),
(@PATH,69,-356.4537,2524.72,43.92796,0,0,0,0,100,0),
(@PATH,70,-341.8646,2517.716,42.69872,0,0,0,0,100,0),
(@PATH,71,-325.4874,2514.352,39.74596,0,0,0,0,100,0),
(@PATH,72,-317.6911,2517.743,42.27646,0,0,0,0,100,0),
(@PATH,73,-307.9288,2522.574,42.47154,0,0,0,0,100,0),
(@PATH,74,-303.3547,2545.351,42.88755,0,0,0,0,100,0),
(@PATH,75,-313.6268,2562.334,44.01721,0,0,0,0,100,0),
(@PATH,76,-319.8146,2564.861,44.6563,0,0,0,0,100,0),
(@PATH,77,-315.3898,2592.442,41.60535,0,0,0,0,100,0),
(@PATH,78,-315.032,2593.062,41.37146,0,0,0,0,100,0),
(@PATH,79,-333.1518,2616.684,42.40425,0,0,0,0,100,0),
(@PATH,80,-352.777,2633.742,40.45049,0,0,0,0,100,0),
(@PATH,81,-360.1994,2656.165,43.72129,0,0,0,0,100,0),
(@PATH,82,-344.7037,2680.905,36.1579,0,0,0,0,100,0),
(@PATH,83,-334.6349,2700.036,30.34241,0,0,0,0,100,0),
(@PATH,84,-324.9058,2712.125,26.43651,0,0,0,0,100,0),
(@PATH,85,-322.9779,2724.364,23.96236,0,0,0,0,100,0),
(@PATH,86,-326.3518,2734.545,22.44383,0,0,0,0,100,0),
(@PATH,87,-342.5912,2742.288,22.4451,0,0,0,0,100,0),
(@PATH,88,-368.7488,2739.919,27.63747,0,0,0,0,100,0),
(@PATH,89,-392.2097,2734.005,34.55191,0,0,0,0,100,0),
(@PATH,90,-414.4307,2741.14,39.55831,0,0,0,0,100,0),
(@PATH,91,-436.1942,2742.233,45.3562,0,0,0,0,100,0),
(@PATH,92,-462.9823,2756.808,49.85812,0,0,0,0,100,0),
(@PATH,93,-480.5942,2778.588,50.47923,0,0,0,0,100,0),
(@PATH,94,-476.0007,2799.573,45.95654,0,0,0,0,100,0),
(@PATH,95,-458.507,2835.351,35.33127,0,0,0,0,100,0),
(@PATH,96,-431.757,2863.309,23.74757,0,0,0,0,100,0),
(@PATH,97,-413.4223,2892.728,17.20714,0,0,0,0,100,0),
(@PATH,98,-419.2291,2905.467,19.51181,0,0,0,0,100,0),
(@PATH,99,-443.1462,2933.887,17.85199,0,0,0,0,100,0),
(@PATH,100,-438.8401,2966.167,8.723536,0,0,0,0,100,0),
(@PATH,101,-444.0433,2936.331,17.40722,0,0,0,0,100,0),
(@PATH,102,-432.5814,2918.56,19.31524,0,0,0,0,100,0),
(@PATH,103,-411.3571,2898.61,17.23361,0,0,0,0,100,0),
(@PATH,104,-429.0982,2867.209,22.665,0,0,0,0,100,0),
(@PATH,105,-457.4272,2836.473,34.75057,0,0,0,0,100,0),
(@PATH,106,-474.1913,2804.114,44.81591,0,0,0,0,100,0),
(@PATH,107,-481.0764,2783.36,50.09081,0,0,0,0,100,0),
(@PATH,108,-465.5521,2759.48,50.03743,0,0,0,0,100,0),
(@PATH,109,-440.5409,2744.546,46.12499,0,0,0,0,100,0),
(@PATH,110,-420.0836,2742.421,41.1034,0,0,0,0,100,0),
(@PATH,111,-397.9436,2734.918,35.90202,0,0,0,0,100,0),
(@PATH,112,-372.9858,2738.609,28.64789,0,0,0,0,100,0),
(@PATH,113,-348.4457,2742.212,23.42384,0,0,0,0,100,0),
(@PATH,114,-333.7679,2740.035,21.81408,0,0,0,0,100,0),
(@PATH,115,-324.9037,2731.125,22.83686,0,0,0,0,100,0),
(@PATH,116,-323.9969,2714.816,25.20854,0,0,0,0,100,0),
(@PATH,117,-332.7059,2702.867,29.4334,0,0,0,0,100,0),
(@PATH,118,-343.4227,2683.021,35.1899,0,0,0,0,100,0),
(@PATH,119,-359.6221,2658.229,43.55175,0,0,0,0,100,0),
(@PATH,120,-353.887,2635.825,39.48611,0,0,0,0,100,0),
(@PATH,121,-344.0572,2624.184,43.10011,0,0,0,0,100,0),
(@PATH,122,-325.4768,2613.256,41.99747,0,0,0,0,100,0),
(@PATH,123,-315.0096,2608.486,41.19632,0,0,0,0,100,0),
(@PATH,124,-325.0719,2578.15,44.60706,0,0,0,0,100,0),
(@PATH,125,-325.6835,2576.813,44.85286,0,0,0,0,100,0),
(@PATH,126,-313.8424,2562.751,43.86629,0,0,0,0,100,0),
(@PATH,127,-306.6296,2559.59,43.31482,0,0,0,0,100,0),
(@PATH,128,-303.079,2544.062,42.30245,0,0,0,0,100,0),
(@PATH,129,-324.1274,2514.437,40.29177,0,0,0,0,100,0),
(@PATH,130,-333.2019,2515.876,41.902,0,0,0,0,100,0),
(@PATH,131,-354.4488,2524.19,43.33042,0,0,0,0,100,0),
(@PATH,132,-359.7796,2523.489,44.53107,0,0,0,0,100,0),
(@PATH,133,-378.1317,2510.007,43.7447,0,0,0,0,100,0),
(@PATH,134,-382.583,2491,37.3352,0,0,0,0,100,0),
(@PATH,135,-368.7838,2483.057,28.09278,0,0,0,0,100,0),
(@PATH,136,-337.7688,2479.346,28.95182,0,0,0,0,100,0),
(@PATH,137,-318.8761,2478.915,37.70119,0,0,0,0,100,0),
(@PATH,138,-301.9458,2480.981,40.5145,0,0,0,0,100,0),
(@PATH,139,-300.0336,2481.129,40.53691,0,0,0,0,100,0),
(@PATH,140,-291.6542,2471.877,40.91128,0,0,0,0,100,0),
(@PATH,141,-295.2729,2454.313,42.07068,0,0,0,0,100,0),
(@PATH,142,-291.9165,2442.606,43.58534,0,0,0,0,100,0),
(@PATH,143,-290.83,2436.461,44.0534,0,0,0,0,100,0),
(@PATH,144,-296.628,2422.13,44.75555,0,0,0,0,100,0),
(@PATH,145,-284.9365,2416.829,46.52535,0,0,0,0,100,0),
(@PATH,146,-277.901,2404.396,49.46965,0,0,0,0,100,0),
(@PATH,147,-276.7875,2401.742,49.57491,0,0,0,0,100,0),
(@PATH,148,-275.867,2368.15,50.02176,0,0,0,0,100,0),
(@PATH,149,-280.12,2358.433,51.42045,0,0,0,0,100,0),
(@PATH,150,-271.6628,2337.664,53.90419,0,0,0,0,100,0),
(@PATH,151,-260.8519,2327.604,56.16253,0,0,0,0,100,0),
(@PATH,152,-246.7255,2313.879,53.0292,0,0,0,0,100,0),
(@PATH,153,-233.4331,2311.107,50.28912,0,0,0,0,100,0),
(@PATH,154,-194.8462,2316.223,54.13938,0,0,0,0,100,0),
(@PATH,155,-184.3219,2321.592,61.90179,0,0,0,0,100,0),
(@PATH,156,-169.6792,2336.901,61.27917,0,0,0,0,100,0),
(@PATH,157,-167.8382,2348.53,59.15134,0,0,0,0,100,0),
(@PATH,158,-168.2972,2362.321,57.01959,0,0,0,0,100,0),
(@PATH,159,-169.3826,2376.633,54.4596,0,0,0,0,100,0),
(@PATH,160,-171.8145,2382.753,52.97548,0,0,0,0,100,0),
(@PATH,161,-180.183,2392.34,51.01487,0,0,0,0,100,0),
(@PATH,162,-181.2741,2398.439,49.99238,0,0,0,0,100,0),
(@PATH,163,-175.461,2408.534,48.88574,0,0,0,0,100,0),
(@PATH,164,-157.5141,2417.718,45.03862,0,0,0,0,100,0),
(@PATH,165,-146.3332,2426.921,48.30313,0,0,0,0,100,0),
(@PATH,166,-137.9598,2445.376,46.12595,0,0,0,0,100,0),
(@PATH,167,-118.3926,2470.393,46.95731,0,0,0,0,100,0),
(@PATH,168,-124.8685,2492.812,46.93701,0,0,0,0,100,0),
(@PATH,169,-131.4405,2495.996,46.55548,0,0,0,0,100,0),
(@PATH,170,-145.8467,2499.76,45.61186,0,0,0,0,100,0),
(@PATH,171,-168.8438,2501.025,43.79488,0,0,0,0,100,0),
(@PATH,172,-176.3846,2510.593,41.94983,0,0,0,0,100,0),
(@PATH,173,-177.3363,2516.895,41.11181,0,0,0,0,100,0),
(@PATH,174,-170.4912,2527.817,41.62189,0,0,0,0,100,0),
(@PATH,175,-156.8518,2539.49,42.95174,0,0,0,0,100,0),
(@PATH,176,-146.1405,2544.73,40.98074,0,0,0,0,100,0),
(@PATH,177,-144.7979,2569.513,41.10892,0,0,0,0,100,0),
(@PATH,178,-165.0483,2590.722,39.90811,0,0,0,0,100,0),
(@PATH,179,-162.7764,2603.097,40.06981,0,0,0,0,100,0),
(@PATH,180,-164.5241,2612.235,40.08208,0,0,0,0,100,0),
(@PATH,181,-165.2749,2618.521,40.12231,0,0,0,0,100,0),
(@PATH,182,-166.9141,2633.801,41.55241,0,0,0,0,100,0),
(@PATH,183,-170.6192,2661.56,42.24799,0,0,0,0,100,0),
(@PATH,184,-166.924,2686.502,43.27299,0,0,0,0,100,0),
(@PATH,185,-165.7749,2686.863,43.39313,0,0,0,0,100,0),
(@PATH,186,-125.7525,2695.987,46.92542,0,0,0,0,100,0),
(@PATH,187,-108.798,2709.016,51.00445,0,0,0,0,100,0),
(@PATH,188,-110.122,2737.952,52.16914,0,0,0,0,100,0),
(@PATH,189,-120.4556,2753.937,49.37396,0,0,0,0,100,0),
(@PATH,190,-134.5498,2778.104,41.05849,0,0,0,0,100,0),
(@PATH,191,-153.882,2788.701,36.85746,0,0,0,0,100,0),
(@PATH,192,-186.3388,2797.655,26.33011,0,0,0,0,100,0),
(@PATH,193,-185.5681,2808.792,25.53679,0,0,0,0,100,0),
(@PATH,194,-172.6706,2803.528,29.62396,0,0,0,0,100,0),
(@PATH,195,-160.4427,2807.621,32.6923,0,0,0,0,100,0),
(@PATH,196,-149.3403,2813.561,35.25396,0,0,0,0,100,0),
(@PATH,197,-149.3422,2826.91,33.67762,0,0,0,0,100,0),
(@PATH,198,-168.0662,2837.727,26.36559,0,0,0,0,100,0),
(@PATH,199,-179.7917,2839.993,23.64687,0,0,0,0,100,0);
-- 0x1C09E4424012FCC000002C00005FBF34 .go -179.7917 2839.993 23.64687

-- Pathing for Bonechewer Raider Entry: 16925 'TDB FORMAT'
SET @NPC := 58704;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2,`position_x`=-404.7873,`position_y`=2895.921,`position_z`=13.85265 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,17408,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_type`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,-404.7873,2895.921,13.85265,0,0,0,0,100,0),
(@PATH,2,-404.2322,2900.299,14.15039,0,0,0,0,100,0),
(@PATH,3,-428.3772,2914.106,19.38362,0,0,0,0,100,0),
(@PATH,4,-439.7838,2928.416,18.55647,0,0,0,0,100,0),
(@PATH,5,-445.6255,2946.441,15.33809,0,0,0,0,100,0),
(@PATH,6,-439.2244,2964.096,9.26938,0,0,0,0,100,0),
(@PATH,7,-454.467,2971.035,9.009753,0,0,0,0,100,0),
(@PATH,8,-467.3622,2966.943,12.50773,0,0,0,0,100,0),
(@PATH,9,-482.1766,2976.099,11.32469,0,0,0,0,100,0),
(@PATH,10,-471.1085,2966.682,13.06236,0,0,0,0,100,0),
(@PATH,11,-456.4288,2970.18,9.417983,0,0,0,0,100,0),
(@PATH,12,-450.4304,2969.706,9.393501,0,0,0,0,100,0),
(@PATH,13,-446.038,2951.119,14.24401,0,0,0,0,100,0),
(@PATH,14,-442.8287,2934.824,17.44841,0,0,0,0,100,0),
(@PATH,15,-435.4904,2921.99,19.82334,0,0,0,0,100,0),
(@PATH,16,-404.6763,2913.004,15.59055,0,0,0,0,100,0);
-- 0x1C09E4424010874000002C00005FACA1 .go -404.7873 2895.921 13.85265

-- Pathing for Bonechewer Raider Entry: 16925 'TDB FORMAT'
SET @NPC := 58705;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2,`position_x`=-394.2339,`position_y`=2891.354,`position_z`=8.341909 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,17408,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_type`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,-394.2339,2891.354,8.341909,0,0,0,0,100,0),
(@PATH,2,-404.354,2907.934,14.82262,0,0,0,0,100,0),
(@PATH,3,-420.1548,2911.418,19.02849,0,0,0,0,100,0),
(@PATH,4,-434.6948,2920.658,19.59609,0,0,0,0,100,0),
(@PATH,5,-443.507,2937.645,16.52073,0,0,0,0,100,0),
(@PATH,6,-438.1014,2952.321,11.34356,0,0,0,0,100,0),
(@PATH,7,-445.5783,2967.97,9.331216,0,0,0,0,100,0),
(@PATH,8,-438.8247,2958.072,10.36737,0,0,0,0,100,0),
(@PATH,9,-442.9203,2943.862,15.03016,0,0,0,0,100,0),
(@PATH,10,-440.1529,2925.814,19.82526,0,0,0,0,100,0),
(@PATH,11,-430.9389,2917.94,18.84661,0,0,0,0,100,0),
(@PATH,12,-407.8214,2910.193,16.49719,0,0,0,0,100,0);
-- 0x1C09E4424010874000002C00005FACD2 .go -394.2339 2891.354 8.341909

-- Pathing for Bonechewer Raider Entry: 16925 'TDB FORMAT'
SET @NPC := 58703;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2,`position_x`=-173.0258,`position_y`=2784.179,`position_z`=31.73058 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,17408,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_type`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,-184.2477,2841.474,22.17058,0,0,0,0,100,0),
(@PATH,2,-186.5912,2837.849,21.70995,0,0,0,0,100,0),
(@PATH,3,-186.0354,2821.816,23.10514,0,0,0,0,100,0),
(@PATH,4,-190.4859,2804.101,24.46144,0,0,0,0,100,0),
(@PATH,5,-190.1222,2800.265,25.05125,0,0,0,0,100,0),
(@PATH,6,-185.8087,2784.323,28.49104,0,0,0,0,100,0),
(@PATH,7,-173.0258,2784.179,31.73058,0,0,0,0,100,0),
(@PATH,8,-153.8195,2788.142,37.07464,0,0,0,0,100,0),
(@PATH,9,-146.762,2781.243,39.44977,0,0,0,0,100,0),
(@PATH,10,-150.9473,2773.002,39.19555,0,0,0,0,100,0),
(@PATH,11,-165.231,2763.583,36.20363,0,0,0,0,100,0),
(@PATH,12,-181.1507,2755.388,30.1041,0,0,0,0,100,0),
(@PATH,13,-168.6309,2761.67,34.94637,0,0,0,0,100,0),
(@PATH,14,-157.0087,2768.504,38.33034,0,0,0,0,100,0),
(@PATH,15,-147.9143,2776.101,39.64468,0,0,0,0,100,0),
(@PATH,16,-148.772,2785.843,38.60817,0,0,0,0,100,0),
(@PATH,17,-167.8991,2784.874,33.46256,0,0,0,0,100,0),
(@PATH,18,-185.8087,2784.323,28.49104,0,0,0,0,100,0),
(@PATH,19,-190.1222,2800.265,25.05125,0,0,0,0,100,0),
(@PATH,20,-190.4859,2804.101,24.46144,0,0,0,0,100,0),
(@PATH,21,-186.0354,2821.816,23.10514,0,0,0,0,100,0),
(@PATH,22,-186.5912,2837.849,21.70995,0,0,0,0,100,0),
(@PATH,23,-184.2477,2841.474,22.17058,0,0,0,0,100,0);
-- 0x1C09E4424010874000002C00005FBE47 .go -173.0258 2784.179 31.73058

-- Pathing for Bonechewer Raider Entry: 16925 'TDB FORMAT'
SET @NPC := 58707;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2,`position_x`=-180.3116,`position_y`=2686.223,`position_z`=40.81258 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,17408,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_type`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,-180.3116,2686.223,40.81258,0,0,0,0,100,0),
(@PATH,2,-175.7397,2679.259,42.18403,0,0,0,0,100,0),
(@PATH,3,-171.9714,2660.608,42.08878,0,0,0,0,100,0),
(@PATH,4,-170.7084,2658.794,42.01125,0,0,0,0,100,0),
(@PATH,5,-171.3484,2640.479,41.27383,0,0,0,0,100,0),
(@PATH,6,-171.948,2638.051,41.02221,0,0,0,0,100,0),
(@PATH,7,-165.7785,2619.644,40.29411,0,0,0,0,100,0),
(@PATH,8,-166.9228,2612.098,39.69428,0,0,0,0,100,0),
(@PATH,9,-169.5662,2594.979,39.33304,0,0,0,0,100,0),
(@PATH,10,-170.6965,2594.06,39.20753,0,0,0,0,100,0),
(@PATH,11,-170.8916,2594.003,38.98936,0,0,0,0,100,0),
(@PATH,12,-166.2133,2597.965,39.64554,0,0,0,0,100,0),
(@PATH,13,-163.8443,2600.274,39.8479,0,0,0,0,100,0),
(@PATH,14,-165.4025,2621.016,40.56765,0,0,0,0,100,0),
(@PATH,15,-164.8262,2624.799,41.17817,0,0,0,0,100,0),
(@PATH,16,-170.1738,2643.713,41.53741,0,0,0,0,100,0),
(@PATH,17,-169.1673,2647.221,41.99223,0,0,0,0,100,0),
(@PATH,18,-171.7151,2670.958,42.49662,0,0,0,0,100,0);
-- 0x1C09E4424010874000002C00005FBFA9 .go -180.3116 2686.223 40.81258
