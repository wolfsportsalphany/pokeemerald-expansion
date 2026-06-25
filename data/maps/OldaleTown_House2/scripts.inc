OldaleTown_House2_MapScripts::
	.byte 0

OldaleTown_House2_EventScript_Elevator::
	lock
	faceplayer
	msgbox OldaleTown_House2_Text_Elevator, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, OldaleTown_House2_EventScript_ElevatorStay
	closemessage
	warp MAP_OLDALE_TOWN_HOUSE1, 4, 7
	waitstate
	end

OldaleTown_House2_EventScript_ElevatorStay::
	release
	end

OldaleTown_House2_Text_Elevator:
	.string "Floor, Mr. Gaer?\p"
	.string "Take the elevator up to your place\n"
	.string "on Floor 28?$"
