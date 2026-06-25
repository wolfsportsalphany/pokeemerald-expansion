LittlerootTown_MaysHouse_1F_MapScripts::
	.byte 0

LittlerootTown_MaysHouse_1F_EventScript_Elevator::
	lock
	faceplayer
	message LittlerootTown_MaysHouse_1F_Text_WhichFloor
	waitmessage
	multichoice 0, 0, MULTI_SAG_FLOORS, FALSE
	switch VAR_RESULT
	case 0, LittlerootTown_MaysHouse_1F_EventScript_ToFloor28
	case 1, LittlerootTown_MaysHouse_1F_EventScript_ToFloor11
	case 2, LittlerootTown_MaysHouse_1F_EventScript_StayLobby
	case MULTI_B_PRESSED, LittlerootTown_MaysHouse_1F_EventScript_StayLobby
	release
	end

LittlerootTown_MaysHouse_1F_EventScript_ToFloor28::
	closemessage
	warp MAP_OLDALE_TOWN_HOUSE1, 5, 6
	waitstate
	end

LittlerootTown_MaysHouse_1F_EventScript_ToFloor11::
	closemessage
	warp MAP_OLDALE_TOWN_HOUSE2, 5, 5
	waitstate
	end

LittlerootTown_MaysHouse_1F_EventScript_StayLobby::
	closemessage
	release
	end

LittlerootTown_MaysHouse_1F_EventScript_Doorman1::
	msgbox LittlerootTown_MaysHouse_1F_Text_Doorman1, MSGBOX_NPC
	end

LittlerootTown_MaysHouse_1F_EventScript_Doorman2::
	msgbox LittlerootTown_MaysHouse_1F_Text_Doorman2, MSGBOX_NPC
	end

LittlerootTown_MaysHouse_1F_Text_WhichFloor:
	.string "Which floor, Mr. Gaer?$"

LittlerootTown_MaysHouse_1F_Text_Doorman1:
	.string "Evening, Mr. Gaer.\n"
	.string "Your brother's up on 11.$"

LittlerootTown_MaysHouse_1F_Text_Doorman2:
	.string "Welcome home. Elevator's right\n"
	.string "there -- 28 is your floor.$"
