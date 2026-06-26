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
	.string "That castle? SIDLEY AUSTIN --\n"
	.string "ALTORELLI's firm. Gym 1, Midtown.\p"
	.string "Dark types, sharp suits, no mercy.\n"
	.string "Good luck, kid.$"

OldaleTown_EventScript_SAGElmo::
	msgbox OldaleTown_Text_SAGElmo, MSGBOX_NPC
	end
OldaleTown_Text_SAGElmo:
	.string "Costumed Elmo wants\ntwenty bucks for a photo.$"

OldaleTown_EventScript_SAGTkts::
	msgbox OldaleTown_Text_SAGTkts, MSGBOX_NPC
	end
OldaleTown_Text_SAGTkts:
	.string "TKTS booth: half-price\nshows, two-hour line.$"

OldaleTown_EventScript_CentralParkSign::
	msgbox OldaleTown_Text_CentralParkSign, MSGBOX_SIGN
	end

OldaleTown_Text_CentralParkSign:
	.string "CENTRAL PARK lies to the WEST.\p"
	.string "Wild battles wait in the green.$"
