UPDATE creature_template SET maxhealth = 133525, minhealth = 133525, maxmana = 51360, minmana = 51360 WHERE entry = 28670;
UPDATE creature_template SET maxhealth = 50000, minhealth = 50000 WHERE entry = 28094;
UPDATE creature_template SET maxhealth = 75000, minhealth = 75000 WHERE entry IN (28312,32627);
UPDATE creature_template SET maxhealth = 50000, minhealth = 50000 WHERE entry IN (28319,32629);

UPDATE creature_template SET minlevel = 80, maxlevel = 80 WHERE entry IN (28312,32627,28319,32629,28094,28670);

UPDATE creature_template SET speed = 2, InhabitType = 1 WHERE entry IN (28312,32627,28319,32629,28094,29929,28782);
UPDATE creature_template SET speed = 2.4, InhabitType = 4 WHERE entry IN (28670);
UPDATE creature_template SET mechanic_immune_mask = 652951551 WHERE entry IN (28670,28312,32627,28319,32629,28094,29929,28782);

DELETE FROM npc_spellclick_spells WHERE npc_entry in (28670, 28312, 32629, 28319, 32627, 28094);
INSERT INTO npc_spellclick_spells VALUES (28670, 53112, 0, 0, 0, 0);
INSERT INTO npc_spellclick_spells VALUES (28312, 60968, 0, 0, 0, 1);
INSERT INTO npc_spellclick_spells VALUES (32627, 60968, 0, 0, 0, 1);
INSERT INTO npc_spellclick_spells VALUES (28319, 60968, 0, 0, 0, 1);
INSERT INTO npc_spellclick_spells VALUES (32629, 60968, 0, 0, 0, 1);
INSERT INTO npc_spellclick_spells VALUES (28094, 60968, 0, 0, 0, 1);
INSERT INTO npc_spellclick_spells VALUES (29929, 58961, 0, 0, 0, 1);
INSERT INTO npc_spellclick_spells VALUES (28782, 52280, 12687, 1, 12687, 1);

INSERT IGNORE INTO creature_template_addon VALUES (28670, 0, 50331648, 1, 0, 1024, 156, NULL, NULL);
INSERT IGNORE INTO creature_template_addon VALUES (28312, 0, 0, 0, 0, 0, 117, '28319 7', NULL);
INSERT IGNORE INTO creature_template_addon VALUES (32627, 0, 0, 0, 0, 0, 117, '32629 7', NULL);
INSERT IGNORE INTO creature_template_addon VALUES (28319, 0, 0, 0, 0, 0, 116, NULL, NULL);
INSERT IGNORE INTO creature_template_addon VALUES (32629, 0, 0, 0, 0, 0, 116, NULL, NULL);
INSERT IGNORE INTO creature_template_addon VALUES (28094, 0, 0, 0, 0, 0, 106, NULL, NULL);
INSERT IGNORE INTO creature_template_addon VALUES (29929, 0, 0, 0, 0, 0, 210, NULL, NULL);
INSERT IGNORE INTO creature_template_addon VALUES (28782, 0, 0, 0, 0, 0, 200, NULL, NULL);

DELETE FROM vehicle_data WHERE entry in (106, 116, 117, 156, 200, 210);
INSERT INTO vehicle_data VALUES (106, 4, 50896, 0, 50652, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO vehicle_data VALUES (116, 5, 51362, 51421, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO vehicle_data VALUES (117, 4, 50652, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO vehicle_data VALUES (156, 24, 53114, 0, 53110, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO vehicle_data VALUES (200, 30, 52362, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO vehicle_data VALUES (210, 166, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

DELETE FROM vehicle_seat_data WHERE seat in (1643, 1648, 1649, 1650, 1652, 1554, 1556, 1557, 1986, 1987, 2144, 2181, 2182);
INSERT INTO vehicle_seat_data VALUES (1643, 3);
INSERT INTO vehicle_seat_data VALUES (1648, 3);
INSERT INTO vehicle_seat_data VALUES (1649, 2);
INSERT INTO vehicle_seat_data VALUES (1650, 2);
INSERT INTO vehicle_seat_data VALUES (1652, 4);
INSERT INTO vehicle_seat_data VALUES (1554, 3);
INSERT INTO vehicle_seat_data VALUES (1556, 6);
INSERT INTO vehicle_seat_data VALUES (1557, 6);
INSERT INTO vehicle_seat_data VALUES (1986, 3);
INSERT INTO vehicle_seat_data VALUES (1987, 8);
INSERT INTO vehicle_seat_data VALUES (2144, 1);
INSERT INTO vehicle_seat_data VALUES (2181, 1);
INSERT INTO vehicle_seat_data VALUES (2182, 0);