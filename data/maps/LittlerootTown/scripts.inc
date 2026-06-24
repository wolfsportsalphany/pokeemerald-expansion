LittlerootTown_MapScripts::
	.byte 0

LittlerootTown_EventScript_SAGJared::
	msgbox LittlerootTown_Text_SAGJared, MSGBOX_NPC
	end

LittlerootTown_Text_SAGJared:
	.string "Shane! It's your brother.\n"
	.string "Apt 11I, same as always.\p"
	.string "Took you long enough to wake up.\n"
	.string "That Eevee I gave you? Don't waste it.\p"
	.string "Sixteen gyms between you and me.\n"
	.string "I'll be waiting at the top. Go.$"

LittlerootTown_EventScript_SAGDoorman::
	msgbox LittlerootTown_Text_SAGDoorman, MSGBOX_NPC
	end

LittlerootTown_Text_SAGDoorman:
	.string "Mornin', Mr. Gaer.\n"
	.string "Welcome to Hudson Yards.\p"
	.string "Head south for Times Square when\n"
	.string "you're ready. Whole city's waiting.$"

LittlerootTown_EventScript_SAGLapras::
	lock
	faceplayer
	goto_if_set FLAG_UNUSED_0x022, LittlerootTown_EventScript_SAGLaprasDoneScr
	msgbox LittlerootTown_Text_SAGLaprasGive, MSGBOX_DEFAULT
	givemon SPECIES_LAPRAS, 30
	setflag FLAG_UNUSED_0x022
	msgbox LittlerootTown_Text_SAGLaprasGot, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_SAGLaprasDoneScr::
	lock
	faceplayer
	msgbox LittlerootTown_Text_SAGLaprasDone, MSGBOX_DEFAULT
	release
	end

LittlerootTown_Text_SAGLaprasGive:
	.string "Found this LAPRAS off the\nWest Side, down in the Hudson.\nIt took a likin' to you.\nGo on, take it.$"

LittlerootTown_Text_SAGLaprasGot:
	.string "Shane received LAPRAS!$"

LittlerootTown_Text_SAGLaprasDone:
	.string "Mind the currents out there.$"

LittlerootTown_EventScript_SAGDogs::
	msgbox LittlerootTown_Text_SAGDogs, MSGBOX_NPC
	end
LittlerootTown_Text_SAGDogs:
	.string "Five-dollar dirty water\ndogs! Best in Hudson Yards.$"

LittlerootTown_EventScript_SAGHardhat::
	msgbox LittlerootTown_Text_SAGHardhat, MSGBOX_NPC
	end
LittlerootTown_Text_SAGHardhat:
	.string "They built this whole\nneighborhood in a week.$"
