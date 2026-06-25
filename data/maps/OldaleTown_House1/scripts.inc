OldaleTown_House1_MapScripts::
	.byte 0

OldaleTown_House1_EventScript_Elevator::
	lock
	faceplayer
	message OldaleTown_House1_Text_WhichFloor
	waitmessage
	multichoice 0, 0, MULTI_SAG_FLOORS, FALSE
	switch VAR_RESULT
	case 0, OldaleTown_House1_EventScript_Stay
	case 1, OldaleTown_House1_EventScript_ToFloor11
	case 2, OldaleTown_House1_EventScript_ToLobby
	case MULTI_B_PRESSED, OldaleTown_House1_EventScript_Stay
	release
	end

OldaleTown_House1_EventScript_ToFloor11::
	closemessage
	warp MAP_OLDALE_TOWN_HOUSE2, 5, 5
	waitstate
	end

OldaleTown_House1_EventScript_ToLobby::
	closemessage
	warp MAP_LITTLEROOT_TOWN_MAYS_HOUSE_1F, 5, 6
	waitstate
	end

OldaleTown_House1_EventScript_Stay::
	closemessage
	release
	end

OldaleTown_House1_Text_WhichFloor:
	.string "Which floor, Mr. Gaer?$"
