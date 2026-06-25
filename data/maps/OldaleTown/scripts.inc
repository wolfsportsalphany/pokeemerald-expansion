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
	.string "Those GYM doors lead to\n"
	.string "SIDLEY AUSTIN -- ALTORELLI's firm.\p"
	.string "Gym 1, over in Midtown.\n"
	.string "Dark types, sharp suits, no mercy.$"

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
