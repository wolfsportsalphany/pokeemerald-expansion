OldaleTown_MapScripts::
	.byte 0

OldaleTown_EventScript_SAGPizza::
	msgbox OldaleTown_Text_SAGPizza, MSGBOX_NPC
	end
OldaleTown_Text_SAGPizza:
	.string "Joe's Pizza! $2.75 a slice.\n"
	.string "Fold it, don't be a tourist.$"

OldaleTown_EventScript_SAGTourist::
	msgbox OldaleTown_Text_SAGTourist, MSGBOX_NPC
	end
OldaleTown_Text_SAGTourist:
	.string "TIMES SQUARE! So many lights!\n"
	.string "...and so many Rattata.$"

OldaleTown_EventScript_SAGGymInfo::
	msgbox OldaleTown_Text_SAGGymInfo, MSGBOX_NPC
	end
OldaleTown_Text_SAGGymInfo:
	.string "That big building? Gym 1.\n"
	.string "Altorelli runs it. Dark types,\p"
	.string "Russian goons, the whole deal.\n"
	.string "Good luck, kid.$"
