--
-- Table structure for table `rbac_permissions`
--

DROP TABLE IF EXISTS `rbac_permissions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `rbac_permissions` (
  `id` int unsigned NOT NULL DEFAULT '0' COMMENT 'Permission id',
  `name` varchar(100) COLLATE utf8mb4_unicode_ci NOT NULL COMMENT 'Permission name',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='Permission List';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `rbac_permissions`
--

LOCK TABLES `rbac_permissions` WRITE;
/*!40000 ALTER TABLE `rbac_permissions` DISABLE KEYS */;
INSERT INTO `rbac_permissions` VALUES
(1,'Instant logout'),
(2,'Skip Queue'),
(3,'Join Normal Battleground'),
(4,'Join Random Battleground'),
(5,'Join Arenas'),
(6,'Join Dungeon Finder'),
(7,'Skip idle connection check'),
(11,'Log GM trades'),
(13,'Skip Instance required bosses check'),
(14,'Skip character creation team mask check'),
(15,'Skip character creation class mask check'),
(16,'Skip character creation race mask check'),
(17,'Skip character creation reserved name check'),
(18,'Skip character creation heroic min level check'),
(19,'Skip needed requirements to use channel check'),
(20,'Skip disable map check'),
(21,'Skip reset talents when used more than allowed check'),
(22,'Skip spam chat check'),
(23,'Skip over-speed ping check'),
(24,'Two side faction characters on the same account'),
(25,'Allow say chat between factions'),
(26,'Allow channel chat between factions'),
(27,'Two side mail interaction'),
(28,'See two side who list'),
(29,'Add friends of other faction'),
(30,'Save character without delay with .save command'),
(31,'Use params with .unstuck command'),
(32,'Can be assigned tickets with .assign ticket command'),
(33,'Notify if a command was not found'),
(34,'Check if should appear in list using .gm ingame command'),
(35,'See all security levels with who command'),
(36,'Filter whispers'),
(37,'Use staff badge in chat'),
(38,'Resurrect with full Health Points'),
(39,'Restore saved gm setting states'),
(40,'Allows to add a gm to friend list'),
(41,'Use Config option START_GM_LEVEL to assign new character level'),
(42,'Allows to use CMSG_WORLD_TELEPORT opcode'),
(43,'Allows to use CMSG_WHOIS opcode'),
(44,'Receive global GM messages/texts'),
(45,'Join channels without announce'),
(46,'Change channel settings without being channel moderator'),
(47,'Enables lower security than target check'),
(48,'Enable IP, Last Login and EMail output in pinfo'),
(49,'Forces to enter the email for confirmation on password change'),
(50,'Allow user to check his own email with .account'),
(192,'Role: Sec Level Administrator'),
(193,'Role: Sec Level Gamemaster'),
(194,'Role: Sec Level Moderator'),
(195,'Role: Sec Level Player'),
(196,'Role: Administrator Commands'),
(197,'Role: Gamemaster Commands'),
(198,'Role: Moderator Commands'),
(199,'Role: Player Commands'),
(200,'Command: rbac'),
(201,'Command: rbac account'),
(202,'Command: rbac account list'),
(203,'Command: rbac account grant'),
(204,'Command: rbac account deny'),
(205,'Command: rbac account revoke'),
(206,'Command: rbac list'),
(217,'Command: account'),
(218,'Command: account addon'),
(219,'Command: account create'),
(220,'Command: account delete'),
(221,'Command: account lock'),
(222,'Command: account lock country'),
(223,'Command: account lock ip'),
(224,'Command: account onlinelist'),
(225,'Command: account password'),
(226,'Command: account set'),
(227,'Command: account set addon'),
(228,'Command: account set gmlevel'),
(229,'Command: account set password'),
(230,'Command: achievement'),
(231,'Command: achievement add'),
(232,'Command: arena'),
(233,'Command: arena captain'),
(234,'Command: arena create'),
(235,'Command: arena disband'),
(236,'Command: arena info'),
(237,'Command: arena lookup'),
(238,'Command: arena rename'),
(239,'Command: ban'),
(240,'Command: ban account'),
(241,'Command: ban character'),
(242,'Command: ban ip'),
(243,'Command: ban playeraccount'),
(244,'Command: baninfo'),
(245,'Command: baninfo account'),
(246,'Command: baninfo character'),
(247,'Command: baninfo ip'),
(248,'Command: banlist'),
(249,'Command: banlist account'),
(250,'Command: banlist character'),
(251,'Command: banlist ip'),
(252,'Command: unban'),
(253,'Command: unban account'),
(254,'Command: unban character'),
(255,'Command: unban ip'),
(256,'Command: unban playeraccount'),
(257,'Command: bf'),
(258,'Command: bf start'),
(259,'Command: bf stop'),
(260,'Command: bf switch'),
(261,'Command: bf timer'),
(262,'Command: bf enable'),
(263,'Command: account email'),
(264,'Command: account set sec'),
(265,'Command: account set sec email'),
(266,'Command: account set sec regmail'),
(267,'Command: cast'),
(268,'Command: cast back'),
(269,'Command: cast dist'),
(270,'Command: cast self'),
(271,'Command: cast target'),
(272,'Command: cast dest'),
(273,'Command: character'),
(274,'Command: character customize'),
(275,'Command: character changefaction'),
(276,'Command: character changerace'),
(277,'Command: character deleted'),
(279,'Command: character deleted list'),
(280,'Command: character deleted restore'),
(283,'Command: character level'),
(284,'Command: character rename'),
(285,'Command: character reputation'),
(286,'Command: character titles'),
(287,'Command: levelup'),
(288,'Command: pdump'),
(289,'Command: pdump load'),
(290,'Command: pdump write'),
(291,'Command: cheat'),
(292,'Command: cheat casttime'),
(293,'Command: cheat cooldown'),
(294,'Command: cheat explore'),
(295,'Command: cheat god'),
(296,'Command: cheat power'),
(297,'Command: cheat status'),
(298,'Command: cheat taxi'),
(299,'Command: cheat waterwalk'),
(300,'Command: debug'),
(301,'Command: debug anim'),
(302,'Command: debug areatriggers'),
(303,'Command: debug arena'),
(304,'Command: debug bg'),
(305,'Command: debug entervehicle'),
(306,'Command: debug getitemstate'),
(307,'Command: debug getitemvalue'),
(308,'Command: debug getvalue'),
(309,'Command: debug hostil'),
(310,'Command: debug itemexpire'),
(311,'Command: debug lootrecipient'),
(312,'Command: debug los'),
(313,'Command: debug mod32value'),
(314,'Command: debug moveflags'),
(315,'Command: debug play'),
(316,'Command: debug play cinematics'),
(317,'Command: debug play movie'),
(318,'Command: debug play sound'),
(319,'Command: debug send'),
(320,'Command: debug send buyerror'),
(321,'Command: debug send channelnotify'),
(322,'Command: debug send chatmessage'),
(323,'Command: debug send equiperror'),
(324,'Command: debug send largepacket'),
(325,'Command: debug send opcode'),
(326,'Command: debug send qinvalidmsg'),
(327,'Command: debug send qpartymsg'),
(328,'Command: debug send sellerror'),
(329,'Command: debug send setphaseshift'),
(330,'Command: debug send spellfail'),
(331,'Command: debug setaurastate'),
(332,'Command: debug setbit'),
(333,'Command: debug setitemvalue'),
(334,'Command: debug setvalue'),
(335,'Command: debug setvid'),
(336,'Command: debug spawnvehicle'),
(337,'Command: debug combat'),
(338,'Command: debug update'),
(339,'Command: debug worldstate'),
(340,'Command: wpgps'),
(341,'Command: deserter'),
(342,'Command: deserter bg'),
(343,'Command: deserter bg add'),
(344,'Command: deserter bg remove'),
(345,'Command: deserter instance'),
(346,'Command: deserter instance add'),
(347,'Command: deserter instance remove'),
(348,'Command: disable'),
(349,'Command: disable add'),
(350,'Command: disable add achievement_criteria'),
(351,'Command: disable add battleground'),
(352,'Command: disable add map'),
(353,'Command: disable add mmap'),
(354,'Command: disable add outdoorpvp'),
(355,'Command: disable add quest'),
(356,'Command: disable add spell'),
(357,'Command: disable add vmap'),
(358,'Command: disable remove'),
(359,'Command: disable remove achievement_criteria'),
(360,'Command: disable remove battleground'),
(361,'Command: disable remove map'),
(362,'Command: disable remove mmap'),
(363,'Command: disable remove outdoorpvp'),
(364,'Command: disable remove quest'),
(365,'Command: disable remove spell'),
(366,'Command: disable remove vmap'),
(367,'Command: event'),
(368,'Command: event activelist'),
(369,'Command: event start'),
(370,'Command: event stop'),
(371,'Command: gm'),
(372,'Command: gm chat'),
(373,'Command: gm fly'),
(374,'Command: gm ingame'),
(375,'Command: gm list'),
(376,'Command: gm visible'),
(377,'Command: go'),
(387,'Command: gobject'),
(388,'Command: gobject activate'),
(389,'Command: gobject add'),
(390,'Command: gobject add temp'),
(391,'Command: gobject delete'),
(392,'Command: gobject info'),
(393,'Command: gobject move'),
(394,'Command: gobject near'),
(395,'Command: gobject set'),
(396,'Command: gobject set phase'),
(397,'Command: gobject set state'),
(398,'Command: gobject target'),
(399,'Command: gobject turn'),
(400,'Command: debug transport'),
(401,'Command: guild'),
(402,'Command: guild create'),
(403,'Command: guild delete'),
(404,'Command: guild invite'),
(405,'Command: guild uninvite'),
(406,'Command: guild rank'),
(407,'Command: guild rename'),
(408,'Command: honor'),
(409,'Command: honor add'),
(410,'Command: honor add kill'),
(411,'Command: honor update'),
(412,'Command: instance'),
(413,'Command: instance listbinds'),
(414,'Command: instance unbind'),
(415,'Command: instance stats'),
(416,'Command: instance savedata'),
(417,'Command: learn'),
(418,'Command: learn all'),
(419,'Command: learn all my'),
(420,'Command: learn all my class'),
(421,'Command: learn all my pettalents'),
(422,'Command: learn all my spells'),
(423,'Command: learn all my talents'),
(424,'Command: learn all gm'),
(425,'Command: learn all crafts'),
(426,'Command: learn all default'),
(427,'Command: learn all lang'),
(428,'Command: learn all recipes'),
(429,'Command: unlearn'),
(430,'Command: lfg'),
(431,'Command: lfg player'),
(432,'Command: lfg group'),
(433,'Command: lfg queue'),
(434,'Command: lfg clean'),
(435,'Command: lfg options'),
(436,'Command: list'),
(437,'Command: list creature'),
(438,'Command: list item'),
(439,'Command: list object'),
(440,'Command: list auras'),
(441,'Command: list mail'),
(442,'Command: lookup'),
(443,'Command: lookup area'),
(444,'Command: lookup creature'),
(445,'Command: lookup event'),
(446,'Command: lookup faction'),
(447,'Command: lookup item'),
(448,'Command: lookup itemset'),
(449,'Command: lookup object'),
(450,'Command: lookup quest'),
(451,'Command: lookup player'),
(452,'Command: lookup player ip'),
(453,'Command: lookup player account'),
(454,'Command: lookup player email'),
(455,'Command: lookup skill'),
(456,'Command: lookup spell'),
(457,'Command: lookup spell id'),
(458,'Command: lookup taxinode'),
(459,'Command: lookup tele'),
(460,'Command: lookup title'),
(461,'Command: lookup map'),
(462,'Command: announce'),
(463,'Command: channel'),
(464,'Command: channel set'),
(465,'Command: channel set ownership'),
(466,'Command: gmannounce'),
(467,'Command: gmnameannounce'),
(468,'Command: gmnotify'),
(469,'Command: nameannounce'),
(470,'Command: notify'),
(471,'Command: whispers'),
(472,'Command: group'),
(473,'Command: group leader'),
(474,'Command: group disband'),
(475,'Command: group remove'),
(476,'Command: group join'),
(477,'Command: group list'),
(478,'Command: group summon'),
(479,'Command: pet'),
(480,'Command: pet create'),
(481,'Command: pet learn'),
(482,'Command: pet unlearn'),
(483,'Command: send'),
(484,'Command: send items'),
(485,'Command: send mail'),
(486,'Command: send message'),
(487,'Command: send money'),
(488,'Command: additem'),
(489,'Command: additemset'),
(490,'Command: appear'),
(491,'Command: aura'),
(492,'Command: bank'),
(493,'Command: bindsight'),
(494,'Command: combatstop'),
(495,'Command: cometome'),
(496,'Command: commands'),
(497,'Command: cooldown'),
(498,'Command: damage'),
(499,'Command: dev'),
(500,'Command: die'),
(501,'Command: dismount'),
(502,'Command: distance'),
(503,'Command: flusharenapoints'),
(504,'Command: freeze'),
(505,'Command: gps'),
(506,'Command: guid'),
(507,'Command: help'),
(508,'Command: hidearea'),
(509,'Command: itemmove'),
(510,'Command: kick'),
(511,'Command: linkgrave'),
(512,'Command: listfreeze'),
(513,'Command: maxskill'),
(514,'Command: movegens'),
(515,'Command: mute'),
(516,'Command: neargrave'),
(517,'Command: pinfo'),
(518,'Command: playall'),
(519,'Command: possess'),
(520,'Command: recall'),
(521,'Command: repairitems'),
(522,'Command: respawn'),
(523,'Command: revive'),
(524,'Command: saveall'),
(525,'Command: save'),
(526,'Command: setskill'),
(527,'Command: showarea'),
(528,'Command: summon'),
(529,'Command: unaura'),
(530,'Command: unbindsight'),
(531,'Command: unfreeze'),
(532,'Command: unmute'),
(533,'Command: unpossess'),
(534,'Command: unstuck'),
(535,'Command: wchange'),
(536,'Command: mmap'),
(537,'Command: mmap loadedtiles'),
(538,'Command: mmap loc'),
(539,'Command: mmap path'),
(540,'Command: mmap stats'),
(541,'Command: mmap testarea'),
(542,'Command: morph'),
(543,'Command: demorph'),
(544,'Command: modify'),
(545,'Command: modify arenapoints'),
(546,'Command: modify bit'),
(547,'Command: modify drunk'),
(548,'Command: modify energy'),
(549,'Command: modify faction'),
(550,'Command: modify gender'),
(551,'Command: modify honor'),
(552,'Command: modify hp'),
(553,'Command: modify mana'),
(554,'Command: modify money'),
(555,'Command: modify mount'),
(556,'Command: modify phase'),
(557,'Command: modify rage'),
(558,'Command: modify reputation'),
(559,'Command: modify runicpower'),
(560,'Command: modify scale'),
(561,'Command: modify speed'),
(562,'Command: modify speed all'),
(563,'Command: modify speed backwalk'),
(564,'Command: modify speed fly'),
(565,'Command: modify speed walk'),
(566,'Command: modify speed swim'),
(567,'Command: modify spell'),
(568,'Command: modify standstate'),
(569,'Command: modify talentpoints'),
(570,'Command: npc'),
(571,'Command: npc add'),
(572,'Command: npc add formation'),
(573,'Command: npc add item'),
(574,'Command: npc add move'),
(575,'Command: npc add temp'),
(576,'Command: npc add delete'),
(577,'Command: npc add delete item'),
(578,'Command: npc add follow'),
(579,'Command: npc add follow stop'),
(580,'Command: npc set'),
(581,'Command: npc set allowmove'),
(582,'Command: npc set entry'),
(583,'Command: npc set factionid'),
(584,'Command: npc set flag'),
(585,'Command: npc set level'),
(586,'Command: npc set link'),
(587,'Command: npc set model'),
(588,'Command: npc set movetype'),
(589,'Command: npc set phase'),
(590,'Command: npc set spawndist'),
(591,'Command: npc set spawntime'),
(592,'Command: npc set data'),
(593,'Command: npc info'),
(594,'Command: npc near'),
(595,'Command: npc move'),
(596,'Command: npc playemote'),
(597,'Command: npc say'),
(598,'Command: npc textemote'),
(599,'Command: npc whisper'),
(600,'Command: npc yell'),
(601,'Command: npc tame'),
(602,'Command: quest'),
(603,'Command: quest add'),
(604,'Command: quest complete'),
(605,'Command: quest remove'),
(606,'Command: quest reward'),
(607,'Command: reload'),
(608,'Command: reload access_requirement'),
(609,'Command: reload achievement_criteria_data'),
(610,'Command: reload achievement_reward'),
(611,'Command: reload all'),
(612,'Command: reload all achievement'),
(613,'Command: reload all area'),
(614,'Command: broadcast_text'),
(615,'Command: reload all gossips'),
(616,'Command: reload all item'),
(617,'Command: reload all locales'),
(618,'Command: reload all loot'),
(619,'Command: reload all npc'),
(620,'Command: reload all quest'),
(621,'Command: reload all scripts'),
(622,'Command: reload all spell'),
(623,'Command: reload areatrigger_involvedrelation'),
(624,'Command: reload areatrigger_tavern'),
(625,'Command: reload areatrigger_teleport'),
(626,'Command: reload auctions'),
(627,'Command: reload autobroadcast'),
(628,'Command: reload command'),
(629,'Command: reload conditions'),
(630,'Command: reload config'),
(631,'Command: reload battleground_template'),
(632,'Command: .mutehistory'),
(633,'Command: reload creature_linked_respawn'),
(634,'Command: reload creature_loot_template'),
(635,'Command: reload creature_onkill_reputation'),
(636,'Command: reload creature_questender'),
(637,'Command: reload creature_queststarter'),
(638,'Command: reload creature_summon_groups'),
(639,'Command: reload creature_template'),
(640,'Command: reload creature_text'),
(641,'Command: reload disables'),
(642,'Command: reload disenchant_loot_template'),
(643,'Command: reload event_scripts'),
(644,'Command: reload fishing_loot_template'),
(645,'Command: reload game_graveyard_zone'),
(646,'Command: reload game_tele'),
(647,'Command: reload gameobject_questender'),
(648,'Command: reload gameobject_loot_template'),
(649,'Command: reload gameobject_queststarter'),
(650,'Command: reload gm_tickets'),
(651,'Command: reload gossip_menu'),
(652,'Command: reload gossip_menu_option'),
(653,'Command: reload item_enchantment_template'),
(654,'Command: reload item_loot_template'),
(655,'Command: reload item_set_names'),
(656,'Command: reload lfg_dungeon_rewards'),
(657,'Command: reload locales_achievement_reward'),
(658,'Command: reload locales_creature'),
(659,'Command: reload locales_creature_text'),
(660,'Command: reload locales_gameobject'),
(661,'Command: reload locales_gossip_menu_option'),
(662,'Command: reload locales_item'),
(663,'Command: reload locales_item_set_name'),
(664,'Command: reload locales_npc_text'),
(665,'Command: reload locales_page_text'),
(666,'Command: reload locales_points_of_interest'),
(667,'Command: reload locales_quest'),
(668,'Command: reload mail_level_reward'),
(669,'Command: reload mail_loot_template'),
(670,'Command: reload milling_loot_template'),
(671,'Command: reload npc_spellclick_spells'),
(672,'Command: reload npc_trainer'),
(673,'Command: reload npc_vendor'),
(674,'Command: reload page_text'),
(675,'Command: reload pickpocketing_loot_template'),
(676,'Command: reload points_of_interest'),
(677,'Command: reload prospecting_loot_template'),
(678,'Command: reload quest_poi'),
(679,'Command: reload quest_template'),
(680,'Command: reload rbac'),
(681,'Command: reload reference_loot_template'),
(682,'Command: reload reserved_name'),
(683,'Command: reload reputation_reward_rate'),
(684,'Command: reload reputation_spillover_template'),
(685,'Command: reload skill_discovery_template'),
(686,'Command: reload skill_extra_item_template'),
(687,'Command: reload skill_fishing_base_level'),
(688,'Command: reload skinning_loot_template'),
(689,'Command: reload smart_scripts'),
(690,'Command: reload spell_required'),
(691,'Command: reload spell_area'),
(692,'Command: reload spell_bonus_data'),
(693,'Command: reload spell_group'),
(694,'Command: reload spell_learn_spell'),
(695,'Command: reload spell_loot_template'),
(696,'Command: reload spell_linked_spell'),
(697,'Command: reload spell_pet_auras'),
(698,'Command: character changeaccount'),
(699,'Command: reload spell_proc'),
(700,'Command: reload spell_scripts'),
(701,'Command: reload spell_target_position'),
(702,'Command: reload spell_threats'),
(703,'Command: reload spell_group_stack_rules'),
(704,'Command: reload trinity_string'),
(705,'Command: reload warden_action'),
(706,'Command: reload waypoint_scripts'),
(707,'Command: reload waypoint_data'),
(708,'Command: reload vehicle_accessory'),
(709,'Command: reload vehicle_template_accessory'),
(710,'Command: reset'),
(711,'Command: reset achievements'),
(712,'Command: reset honor'),
(713,'Command: reset level'),
(714,'Command: reset spells'),
(715,'Command: reset stats'),
(716,'Command: reset talents'),
(717,'Command: reset all'),
(718,'Command: server'),
(719,'Command: server corpses'),
(720,'Command: server exit'),
(721,'Command: server idlerestart'),
(722,'Command: server idlerestart cancel'),
(723,'Command: server idleshutdown'),
(724,'Command: server idleshutdown cancel'),
(725,'Command: server info'),
(726,'Command: server plimit'),
(727,'Command: server restart'),
(728,'Command: server restart cancel'),
(729,'Command: server set'),
(730,'Command: server set closed'),
(731,'Command: server set difftime'),
(732,'Command: server set loglevel'),
(733,'Command: server set motd'),
(734,'Command: server shutdown'),
(735,'Command: server shutdown cancel'),
(736,'Command: server motd'),
(737,'Command: tele'),
(738,'Command: tele add'),
(739,'Command: tele del'),
(740,'Command: tele name'),
(741,'Command: tele group'),
(742,'Command: ticket'),
(743,'Command: ticket assign'),
(744,'Command: ticket close'),
(745,'Command: ticket closedlist'),
(746,'Command: ticket comment'),
(747,'Command: ticket complete'),
(748,'Command: ticket delete'),
(749,'Command: ticket escalate'),
(750,'Command: ticket escalatedlist'),
(751,'Command: ticket list'),
(752,'Command: ticket onlinelist'),
(753,'Command: ticket reset'),
(754,'Command: ticket response'),
(755,'Command: ticket response append'),
(756,'Command: ticket response appendln'),
(757,'Command: ticket togglesystem'),
(758,'Command: ticket unassign'),
(759,'Command: ticket viewid'),
(760,'Command: ticket viewname'),
(761,'Command: titles'),
(762,'Command: titles add'),
(763,'Command: titles current'),
(764,'Command: titles remove'),
(765,'Command: titles set'),
(766,'Command: titles set mask'),
(767,'Command: wp'),
(768,'Command: wp add'),
(769,'Command: wp event'),
(770,'Command: wp load'),
(771,'Command: wp modify'),
(772,'Command: wp unload'),
(773,'Command: wp reload'),
(774,'Command: wp show'),
(775,'Command: modify currency'),
(776,'Command: debug phase'),
(777,'Command: mailbox'),
(778,'Command: ahbot'),
(779,'Command: ahbot items'),
(780,'Command: ahbot items gray'),
(781,'Command: ahbot items white'),
(782,'Command: ahbot items green'),
(783,'Command: ahbot items blue'),
(784,'Command: ahbot items purple'),
(785,'Command: ahbot items orange'),
(786,'Command: ahbot items yellow'),
(787,'Command: ahbot ratio'),
(788,'Command: ahbot ratio alliance'),
(789,'Command: ahbot ratio horde'),
(790,'Command: ahbot ratio neutral'),
(791,'Command: ahbot rebuild'),
(792,'Command: ahbot reload'),
(793,'Command: ahbot status'),
(794,'Command: guild info'),
(835,'Command: debug loadcells'),
(836,'Command: debug boundary'),
(837,'Command: npc evade'),
(838,'Command: pet level'),
(839,'Command: server shutdown force'),
(840,'Command: server restart force'),
(841,'Command: debug neargraveyard'),
(845,'Command: go offset'),
(855,'Command: debug play music'),
(856,'Command: npc spawngroup'),
(857,'Command: npc despawngroup'),
(858,'Command: gobject spawngroup'),
(859,'Command: gobject despawngroup'),
(860,'Command: list respawns'),
(861,'Command: group set'),
(862,'Command: group set assistant'),
(863,'Command: group set maintank'),
(864,'Command: group set mainassist'),
(865,'Command: npc showloot'),
(866,'Command: list spawnpoints'),
(867,'Command: reload quest_greeting_locale'),
(870,'Command: debug threatinfo'),
(872,'Command: server debug'),
(873,'Command: reload creature_movement_override');
/*!40000 ALTER TABLE `rbac_permissions` ENABLE KEYS */;
UNLOCK TABLES;