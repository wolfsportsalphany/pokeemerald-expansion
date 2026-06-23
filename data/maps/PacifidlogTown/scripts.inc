PacifidlogTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PacifidlogTown_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, PacifidlogTown_OnResume
	.byte 0

PacifidlogTown_OnTransition:
	setflag FLAG_VISITED_PACIFIDLOG_TOWN
	end

PacifidlogTown_OnResume:
	setstepcallback STEP_CB_PACIFIDLOG_BRIDGE
	end

PacifidlogTown_EventScript_NinjaBoy::
	msgbox PacifidlogTown_Text_NeatHousesOnWater, MSGBOX_NPC
	end

PacifidlogTown_EventScript_Girl::
	msgbox PacifidlogTown_Text_FastRunningCurrent, MSGBOX_NPC
	end

PacifidlogTown_EventScript_Fisherman::
	msgbox PacifidlogTown_Text_SkyPillarTooScary, MSGBOX_NPC
	end

PacifidlogTown_EventScript_TownSign::
	msgbox PacifidlogTown_Text_TownSign, MSGBOX_SIGN
	end

PacifidlogTown_Text_FastRunningCurrent:
	.string "The sea between PACIFIDLOG and\n"
	.string "SLATEPORT has a fast-running tide.\p"
	.string "If you decide to SURF, you could end\n"
	.string "up swept away somewhere else.$"

PacifidlogTown_Text_NeatHousesOnWater:
	.string "See, isn't it neat?\n"
	.string "These houses are on water!\p"
	.string "I was born here!$"

PacifidlogTown_Text_SkyPillarTooScary:
	.string "The SKY PILLAR?\p"
	.string "…Oh, you must mean that tall, tall\n"
	.string "tower a little further out.\p"
	.string "If you asked me, I wouldn't climb it.\n"
	.string "It's too scary to get up that high.\p"
	.string "Life at sea level in PACIFIDLOG,\n"
	.string "that suits me fine.$"

PacifidlogTown_Text_TownSign:
	.string "PACIFIDLOG TOWN\p"
	.string "“Where the morning sun smiles upon\n"
	.string "the waters.”$"


PacifidlogTown_EventScript_SAGRoman::
	trainerbattle_single TRAINER_FLANNERY_2, PacifidlogTown_Text_SAGRomanIntro, PacifidlogTown_Text_SAGRomanDefeat
	msgbox PacifidlogTown_Text_SAGRomanPost, MSGBOX_AUTOCLOSE
	end

PacifidlogTown_Text_SAGRomanIntro:
	.string "Hot dog... or a beating? Gym 12.$"

PacifidlogTown_Text_SAGRomanDefeat:
	.string "Heh. Good game, champ.$"

PacifidlogTown_Text_SAGRomanPost:
	.string "Gym 12 down. Keep moving.$"

PacifidlogTown_EventScript_SAGRetiree::
	msgbox PacifidlogTown_Text_SAGRetiree, MSGBOX_NPC
	end
PacifidlogTown_Text_SAGRetiree:
	.string "The Hemispheres, FL.\nRoman, Gym 12.$"
