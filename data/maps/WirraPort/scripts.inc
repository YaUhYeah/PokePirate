WirraPort_MapScripts::
	.byte 0


GiveMon::
	lockall
	showmonpic SPECIES_MUDKIP, 10, 3
	msgbox GiveMon_Text_0, MSGBOX_YESNO
	compare VAR_RESULT, YES
	goto_if_eq GiveMon_2
	hidemonpic
	msgbox GiveMon_Text_3
GiveMon_1:
	waitmessage
	trainerbattle_single TRAINER_SHADOW_TEST, GiveMon_Text_4, GiveMon_Text_5
	msgbox GiveMon_Text_6, MSGBOX_AUTOCLOSE
	releaseall
	end

GiveMon_2:
	givemon SPECIES_MUDKIP, 5
	compare VAR_RESULT, MON_GIVEN_TO_PARTY
	goto_if_eq GiveMon_5
	compare VAR_RESULT, MON_GIVEN_TO_PC
	goto_if_eq GiveMon_6
	hidemonpic
	msgbox GiveMon_Text_2
	goto GiveMon_1

GiveMon_5:
	playfanfare MUS_OBTAIN_ITEM
	msgbox GiveMon_Text_1
	removeobject 1
	setflag FLAG_UNUSED_0x020
	call NicknamePartyMon
	setflag FLAG_SYS_POKEMON_GET
	goto GiveMon_1

GiveMon_6:
	playfanfare MUS_OBTAIN_ITEM
	msgbox GiveMon_Text_1
	removeobject 1
	setflag FLAG_UNUSED_0x020
	call NicknameBoxMon
	setflag FLAG_SYS_POKEMON_GET
	goto GiveMon_1


GiveBalls::
	lock
	giveitem ITEM_POKE_BALL, 10
	setflag FLAG_UNUSED_0x021
	release
	end


NicknamePartyMon::
	msgbox NicknamePartyMon_Text_0, MSGBOX_YESNO
	hidemonpic
	compare VAR_RESULT, YES
	goto_if_eq NicknamePartyMon_1
	return

NicknamePartyMon_1:
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	return


NicknameBoxMon::
	msgbox NicknamePartyMon_Text_0, MSGBOX_YESNO
	hidemonpic
	compare VAR_RESULT, YES
	goto_if_eq NicknameBoxMon_2
NicknameBoxMon_1:
	call Common_EventScript_TransferredToPC
	return

NicknameBoxMon_2:
	call Common_EventScript_NameReceivedPartyMon
	goto NicknameBoxMon_1


GiveMon_Text_0:
	.string "Pick up mudkip?$"

GiveMon_Text_1:
	.string "Recieved Mudkip!$"

GiveMon_Text_2:
	.string "No more room.$"

GiveMon_Text_3:
	.string "damn fair ig$"

GiveMon_Text_4:
	.string "battle me now scrublord$"

GiveMon_Text_5:
	.string "imagine$"

GiveMon_Text_6:
	.string "damn i lost to a noob like pando$"

NicknamePartyMon_Text_0:
	.string "Give this pokemon a nickname?$"
