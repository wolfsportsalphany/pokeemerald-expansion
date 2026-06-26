BirthIsland_Exterior_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, BirthIsland_Exterior_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, BirthIsland_Exterior_OnResume
	.byte 0

@ SAG: New Island. No puzzle -- Armored Mewtwo waits at the top of the island and is
@ confronted directly. The old Deoxys rock triangle is permanently hidden.
BirthIsland_Exterior_OnTransition:
	setflag FLAG_HIDE_BIRTH_ISLAND_DEOXYS_TRIANGLE
	call_if_set FLAG_DEFEATED_DEOXYS, BirthIsland_Exterior_EventScript_HideMewtwo
	call_if_unset FLAG_DEFEATED_DEOXYS, BirthIsland_Exterior_EventScript_ShowMewtwo
	end

BirthIsland_Exterior_EventScript_HideMewtwo::
	setflag FLAG_HIDE_DEOXYS
	return

BirthIsland_Exterior_EventScript_ShowMewtwo::
	clearflag FLAG_HIDE_DEOXYS
	return

BirthIsland_Exterior_OnResume:
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, BirthIsland_Exterior_EventScript_TryRemoveMewtwo
	end

BirthIsland_Exterior_EventScript_TryRemoveMewtwo::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject LOCALID_BIRTH_ISLAND_DEOXYS
	return

BirthIsland_Exterior_EventScript_Mewtwo::
	lock
	faceplayer
	msgbox BirthIsland_Exterior_Text_MewtwoConfront, MSGBOX_DEFAULT
	waitse
	playmoncry SPECIES_MEWTWO, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_LAST_TALKED, LOCALID_BIRTH_ISLAND_DEOXYS
	seteventmon SPECIES_MEWTWO, 70, ITEM_MEWTWONITE_X @ SAG: caught Mewtwo holds its Mega Stone (Armored Mewtwo = Mega X)
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, BirthIsland_Exterior_EventScript_DefeatedMewtwo
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, BirthIsland_Exterior_EventScript_RanFromMewtwo
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, BirthIsland_Exterior_EventScript_RanFromMewtwo
	setflag FLAG_BATTLED_DEOXYS    @ SAG: marks CAUGHT (Montauk gift reads this)
	setflag FLAG_DEFEATED_DEOXYS   @ SAG: also hide Mewtwo on re-entry -- no infinite re-catches
	release
	end

BirthIsland_Exterior_EventScript_DefeatedMewtwo::
	setflag FLAG_DEFEATED_DEOXYS
	setvar VAR_0x8004, SPECIES_MEWTWO
	goto Common_EventScript_LegendaryFlewAway
	end

BirthIsland_Exterior_EventScript_RanFromMewtwo::
	setvar VAR_0x8004, SPECIES_MEWTWO
	goto Common_EventScript_LegendaryFlewAway
	end

BirthIsland_Exterior_Text_MewtwoConfront:
	.string "…\p"
	.string "I am the one they engineered. The\n"
	.string "strongest in this city -- in any city.\p"
	.string "You came all this way to test that?\n"
	.string "Then come. Show me your resolve!$"

BirthIsland_Exterior_EventScript_Grunt::
	msgbox BirthIsland_Exterior_Text_Grunt, MSGBOX_NPC
	end

BirthIsland_Exterior_Text_Grunt:
	.string "Heh… so the courier's invitation\n"
	.string "reached someone worth a damn.\p"
	.string "TEAM ROCKET owns NEW ISLAND now.\n"
	.string "Our masterwork is up at the summit --\l"
	.string "ARMORED MEWTWO.\p"
	.string "Go on. Go amuse it.$"
