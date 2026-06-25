LittlerootTown_MapScripts::
	.byte 0

LittlerootTown_EventScript_SAGJared::
	lock
	faceplayer
	goto_if_set FLAG_UNUSED_0x025, LittlerootTown_EventScript_SAGJaredDone
	msgbox LittlerootTown_Text_SAGJaredGive, MSGBOX_DEFAULT
	givemon SPECIES_EEVEE, 16
	setflag FLAG_UNUSED_0x025
	msgbox LittlerootTown_Text_SAGJaredGot, MSGBOX_DEFAULT
	msgbox LittlerootTown_Text_SAGJaredAfter, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_SAGJaredDone::
	msgbox LittlerootTown_Text_SAGJaredAfter, MSGBOX_DEFAULT
	release
	end

LittlerootTown_Text_SAGJaredGive:
	.string "Shane! There's the birthday boy.\p"
	.string "Took you long enough to come down\n"
	.string "from 28. Apt 11I, same as always.\p"
	.string "Here -- I've been holding onto this\n"
	.string "little guy for you. Go on, take him.$"

LittlerootTown_Text_SAGJaredGot:
	.string "Shane received the EEVEE\n"
	.string "from Jared!$"

LittlerootTown_Text_SAGJaredAfter:
	.string "Sixteen gyms between you and me,\n"
	.string "little brother.\p"
	.string "Don't waste that EEVEE. I'll be\n"
	.string "waiting at the top. Go.$"

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
	goto_if_unset FLAG_BADGE04_GET, LittlerootTown_EventScript_SAGLaprasNotYet
	msgbox LittlerootTown_Text_SAGLaprasGive, MSGBOX_DEFAULT
	givemon SPECIES_LAPRAS, 30
	setflag FLAG_UNUSED_0x022
	msgbox LittlerootTown_Text_SAGLaprasGot, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_SAGLaprasNotYet::
	msgbox LittlerootTown_Text_SAGLaprasNotYet, MSGBOX_DEFAULT
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

LittlerootTown_Text_SAGLaprasNotYet:
	.string "Whoa there. This LAPRAS is a\n"
	.string "handful, kid.\p"
	.string "Come back when you've earned a few\n"
	.string "badges and I'll see she's yours.$"

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
