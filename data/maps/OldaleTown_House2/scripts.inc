OldaleTown_House2_MapScripts::
	.byte 0

OldaleTown_House2_EventScript_Elevator::
	lock
	faceplayer
	message OldaleTown_House2_Text_WhichFloor
	waitmessage
	multichoice 0, 0, MULTI_SAG_FLOORS, FALSE
	switch VAR_RESULT
	case 0, OldaleTown_House2_EventScript_ToFloor28
	case 1, OldaleTown_House2_EventScript_Stay
	case 2, OldaleTown_House2_EventScript_ToLobby
	case MULTI_B_PRESSED, OldaleTown_House2_EventScript_Stay
	release
	end

OldaleTown_House2_EventScript_ToFloor28::
	closemessage
	warp MAP_OLDALE_TOWN_HOUSE1, 5, 6
	waitstate
	end

OldaleTown_House2_EventScript_ToLobby::
	closemessage
	warp MAP_LITTLEROOT_TOWN_MAYS_HOUSE_1F, 7, 5
	waitstate
	end

OldaleTown_House2_EventScript_Stay::
	closemessage
	release
	end

OldaleTown_House2_Text_WhichFloor:
	.string "Which floor, Mr. Gaer?$"
