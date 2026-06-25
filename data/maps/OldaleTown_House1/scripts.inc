OldaleTown_House1_MapScripts::
	.byte 0

OldaleTown_House1_EventScript_Elevator::
	lock
	faceplayer
	msgbox OldaleTown_House1_Text_Elevator, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, OldaleTown_House1_EventScript_ElevatorStay
	closemessage
	warp MAP_OLDALE_TOWN_HOUSE2, 4, 6
	waitstate
	end

OldaleTown_House1_EventScript_ElevatorStay::
	release
	end

OldaleTown_House1_Text_Elevator:
	.string "Floor, Mr. Gaer?\p"
	.string "Take the elevator down to Jared's\n"
	.string "place on Floor 11?$"
