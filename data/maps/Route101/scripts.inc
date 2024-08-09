# 1 "data/maps/Route101/scripts.pory"
.set LOCALID_BIRCH, 2
# 2 "data/maps/Route101/scripts.pory"
.set LOCALID_ZIGZAGOON, 4
# 3 "data/maps/Route101/scripts.pory"

# 4 "data/maps/Route101/scripts.pory"
Route101_MapScripts::
# 5 "data/maps/Route101/scripts.pory"
	map_script MAP_SCRIPT_ON_TRANSITION, Route101_OnTransition
# 6 "data/maps/Route101/scripts.pory"
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Route101_OnFrame
# 7 "data/maps/Route101/scripts.pory"
	.byte 0
# 8 "data/maps/Route101/scripts.pory"

# 9 "data/maps/Route101/scripts.pory"
Route101_OnTransition:
# 10 "data/maps/Route101/scripts.pory"
	call ProfBirch_EventScript_UpdateLocation
# 11 "data/maps/Route101/scripts.pory"
	end
# 12 "data/maps/Route101/scripts.pory"

# 13 "data/maps/Route101/scripts.pory"
Route101_OnFrame:
# 14 "data/maps/Route101/scripts.pory"
	map_script_2 VAR_ROUTE101_STATE, 0, Route101_EventScript_HideMapNamePopup
# 15 "data/maps/Route101/scripts.pory"
	.2byte 0
# 16 "data/maps/Route101/scripts.pory"

# 17 "data/maps/Route101/scripts.pory"
Route101_EventScript_HideMapNamePopup::
# 18 "data/maps/Route101/scripts.pory"
	setflag FLAG_HIDE_MAP_NAME_POPUP
# 19 "data/maps/Route101/scripts.pory"
	setvar VAR_ROUTE101_STATE, 1
# 20 "data/maps/Route101/scripts.pory"
	end
# 21 "data/maps/Route101/scripts.pory"

# 22 "data/maps/Route101/scripts.pory"
Route101_EventScript_StartBirchRescue::
# 23 "data/maps/Route101/scripts.pory"
	lockall
# 24 "data/maps/Route101/scripts.pory"
	playbgm MUS_HELP, TRUE
# 25 "data/maps/Route101/scripts.pory"
	msgbox Route101_Text_HelpMe, MSGBOX_DEFAULT
# 26 "data/maps/Route101/scripts.pory"
	closemessage
# 27 "data/maps/Route101/scripts.pory"
	setobjectxy LOCALID_BIRCH, 0, 15
# 28 "data/maps/Route101/scripts.pory"
	setobjectxy LOCALID_ZIGZAGOON, 0, 16
# 29 "data/maps/Route101/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, Route101_Movement_EnterScene
# 30 "data/maps/Route101/scripts.pory"
	applymovement LOCALID_BIRCH, Route101_Movement_BirchRunAway1
# 31 "data/maps/Route101/scripts.pory"
	applymovement LOCALID_ZIGZAGOON, Route101_Movement_ZigzagoonChase1
# 32 "data/maps/Route101/scripts.pory"
	waitmovement 0
# 33 "data/maps/Route101/scripts.pory"
	applymovement LOCALID_ZIGZAGOON, Route101_Movement_ZigzagoonChaseInCircles
# 34 "data/maps/Route101/scripts.pory"
	applymovement LOCALID_BIRCH, Route101_Movement_BirchRunInCircles
# 35 "data/maps/Route101/scripts.pory"
	waitmovement 0
# 36 "data/maps/Route101/scripts.pory"
	applymovement LOCALID_BIRCH, Common_Movement_WalkInPlaceFasterRight
# 37 "data/maps/Route101/scripts.pory"
	waitmovement 0
# 38 "data/maps/Route101/scripts.pory"
	applymovement LOCALID_ZIGZAGOON, Route101_Movement_ZigzagoonFaceBirch
# 39 "data/maps/Route101/scripts.pory"
	applymovement LOCALID_BIRCH, Route101_Movement_BirchFaceZigzagoon
# 40 "data/maps/Route101/scripts.pory"
	waitmovement 0
# 41 "data/maps/Route101/scripts.pory"
	msgbox Route101_Text_PleaseHelp, MSGBOX_DEFAULT
# 42 "data/maps/Route101/scripts.pory"
	closemessage
# 43 "data/maps/Route101/scripts.pory"
	setvar VAR_ROUTE101_STATE, 2
# 44 "data/maps/Route101/scripts.pory"
	releaseall
# 45 "data/maps/Route101/scripts.pory"
	end
# 46 "data/maps/Route101/scripts.pory"

# 47 "data/maps/Route101/scripts.pory"
Route101_EventScript_PreventExitSouth::
# 48 "data/maps/Route101/scripts.pory"
	lockall
# 49 "data/maps/Route101/scripts.pory"
	msgbox Route101_Text_DontLeaveMe, MSGBOX_DEFAULT
# 50 "data/maps/Route101/scripts.pory"
	closemessage
# 51 "data/maps/Route101/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, Route101_Movement_PreventExitSouth
# 52 "data/maps/Route101/scripts.pory"
	waitmovement 0
# 53 "data/maps/Route101/scripts.pory"
	releaseall
# 54 "data/maps/Route101/scripts.pory"
	end
# 55 "data/maps/Route101/scripts.pory"

# 56 "data/maps/Route101/scripts.pory"
Route101_EventScript_PreventExitWest::
# 57 "data/maps/Route101/scripts.pory"
	lockall
# 58 "data/maps/Route101/scripts.pory"
	msgbox Route101_Text_DontLeaveMe, MSGBOX_DEFAULT
# 59 "data/maps/Route101/scripts.pory"
	closemessage
# 60 "data/maps/Route101/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, Route101_Movement_PreventExitWest
# 61 "data/maps/Route101/scripts.pory"
	waitmovement 0
# 62 "data/maps/Route101/scripts.pory"
	releaseall
# 63 "data/maps/Route101/scripts.pory"
	end
# 64 "data/maps/Route101/scripts.pory"

# 65 "data/maps/Route101/scripts.pory"
Route101_EventScript_PreventExitNorth::
# 66 "data/maps/Route101/scripts.pory"
	lockall
# 67 "data/maps/Route101/scripts.pory"
	msgbox Route101_Text_DontLeaveMe, MSGBOX_DEFAULT
# 68 "data/maps/Route101/scripts.pory"
	closemessage
# 69 "data/maps/Route101/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, Route101_Movement_PreventExitNorth
# 70 "data/maps/Route101/scripts.pory"
	waitmovement 0
# 71 "data/maps/Route101/scripts.pory"
	releaseall
# 72 "data/maps/Route101/scripts.pory"
	end
# 73 "data/maps/Route101/scripts.pory"

# 74 "data/maps/Route101/scripts.pory"
Route101_Movement_PreventExitSouth:
# 75 "data/maps/Route101/scripts.pory"
	walk_up
# 76 "data/maps/Route101/scripts.pory"
	step_end
# 77 "data/maps/Route101/scripts.pory"

# 78 "data/maps/Route101/scripts.pory"
Route101_Movement_PreventExitWest:
# 79 "data/maps/Route101/scripts.pory"
	walk_right
# 80 "data/maps/Route101/scripts.pory"
	step_end
# 81 "data/maps/Route101/scripts.pory"

# 82 "data/maps/Route101/scripts.pory"
Route101_Movement_PreventExitNorth:
# 83 "data/maps/Route101/scripts.pory"
	walk_down
# 84 "data/maps/Route101/scripts.pory"
	step_end
# 85 "data/maps/Route101/scripts.pory"

# 86 "data/maps/Route101/scripts.pory"
Route101_Movement_ZigzagoonChaseInCircles:
# 87 "data/maps/Route101/scripts.pory"
	walk_fast_up
# 88 "data/maps/Route101/scripts.pory"
	walk_fast_up
# 89 "data/maps/Route101/scripts.pory"
	walk_fast_up
# 90 "data/maps/Route101/scripts.pory"
	walk_fast_right
# 91 "data/maps/Route101/scripts.pory"
	walk_fast_right
# 92 "data/maps/Route101/scripts.pory"
	walk_fast_right
# 93 "data/maps/Route101/scripts.pory"
	walk_fast_down
# 94 "data/maps/Route101/scripts.pory"
	walk_fast_down
# 95 "data/maps/Route101/scripts.pory"
	walk_fast_left
# 96 "data/maps/Route101/scripts.pory"
	walk_fast_left
# 97 "data/maps/Route101/scripts.pory"
	walk_fast_left
# 98 "data/maps/Route101/scripts.pory"
	walk_fast_up
# 99 "data/maps/Route101/scripts.pory"
	walk_fast_up
# 100 "data/maps/Route101/scripts.pory"
	walk_fast_right
# 101 "data/maps/Route101/scripts.pory"
	walk_fast_right
# 102 "data/maps/Route101/scripts.pory"
	walk_fast_right
# 103 "data/maps/Route101/scripts.pory"
	walk_fast_down
# 104 "data/maps/Route101/scripts.pory"
	walk_fast_down
# 105 "data/maps/Route101/scripts.pory"
	walk_fast_left
# 106 "data/maps/Route101/scripts.pory"
	walk_fast_left
# 107 "data/maps/Route101/scripts.pory"
	walk_fast_left
# 108 "data/maps/Route101/scripts.pory"
	walk_fast_up
# 109 "data/maps/Route101/scripts.pory"
	walk_fast_up
# 110 "data/maps/Route101/scripts.pory"
	walk_fast_right
# 111 "data/maps/Route101/scripts.pory"
	walk_fast_right
# 112 "data/maps/Route101/scripts.pory"
	walk_fast_right
# 113 "data/maps/Route101/scripts.pory"
	walk_fast_down
# 114 "data/maps/Route101/scripts.pory"
	walk_fast_down
# 115 "data/maps/Route101/scripts.pory"
	walk_fast_left
# 116 "data/maps/Route101/scripts.pory"
	walk_fast_left
# 117 "data/maps/Route101/scripts.pory"
	step_end
# 118 "data/maps/Route101/scripts.pory"

# 119 "data/maps/Route101/scripts.pory"
Route101_Movement_ZigzagoonChase1:
# 120 "data/maps/Route101/scripts.pory"
	walk_fast_up
# 121 "data/maps/Route101/scripts.pory"
	walk_fast_right
# 122 "data/maps/Route101/scripts.pory"
	walk_fast_right
# 123 "data/maps/Route101/scripts.pory"
	walk_fast_right
# 124 "data/maps/Route101/scripts.pory"
	walk_fast_right
# 125 "data/maps/Route101/scripts.pory"
	walk_fast_up
# 126 "data/maps/Route101/scripts.pory"
	step_end
# 127 "data/maps/Route101/scripts.pory"

# 128 "data/maps/Route101/scripts.pory"
@ Leftover data? This command is unused.
# 129 "data/maps/Route101/scripts.pory"
	step_end
# 130 "data/maps/Route101/scripts.pory"

# 131 "data/maps/Route101/scripts.pory"
Route101_Movement_ZigzagoonFaceBirch:
# 132 "data/maps/Route101/scripts.pory"
	walk_in_place_fast_left
# 133 "data/maps/Route101/scripts.pory"
	walk_in_place_fast_left
# 134 "data/maps/Route101/scripts.pory"
	walk_in_place_fast_left
# 135 "data/maps/Route101/scripts.pory"
	walk_in_place_fast_left
# 136 "data/maps/Route101/scripts.pory"
	step_end
# 137 "data/maps/Route101/scripts.pory"

# 138 "data/maps/Route101/scripts.pory"
Route101_Movement_EnterScene:
# 139 "data/maps/Route101/scripts.pory"
	walk_fast_up
# 140 "data/maps/Route101/scripts.pory"
	walk_fast_up
# 141 "data/maps/Route101/scripts.pory"
	walk_fast_up
# 142 "data/maps/Route101/scripts.pory"
	walk_fast_up
# 143 "data/maps/Route101/scripts.pory"
	walk_in_place_faster_left
# 144 "data/maps/Route101/scripts.pory"
	step_end
# 145 "data/maps/Route101/scripts.pory"

# 146 "data/maps/Route101/scripts.pory"
Route101_Movement_BirchRunInCircles:
# 147 "data/maps/Route101/scripts.pory"
	walk_fast_up
# 148 "data/maps/Route101/scripts.pory"
	walk_fast_up
# 149 "data/maps/Route101/scripts.pory"
	walk_fast_right
# 150 "data/maps/Route101/scripts.pory"
	walk_fast_right
# 151 "data/maps/Route101/scripts.pory"
	walk_fast_right
# 152 "data/maps/Route101/scripts.pory"
	walk_fast_down
# 153 "data/maps/Route101/scripts.pory"
	walk_fast_down
# 154 "data/maps/Route101/scripts.pory"
	walk_fast_left
# 155 "data/maps/Route101/scripts.pory"
	walk_fast_left
# 156 "data/maps/Route101/scripts.pory"
	walk_fast_left
# 157 "data/maps/Route101/scripts.pory"
	walk_fast_up
# 158 "data/maps/Route101/scripts.pory"
	walk_fast_up
# 159 "data/maps/Route101/scripts.pory"
	walk_fast_right
# 160 "data/maps/Route101/scripts.pory"
	walk_fast_right
# 161 "data/maps/Route101/scripts.pory"
	walk_fast_right
# 162 "data/maps/Route101/scripts.pory"
	walk_fast_down
# 163 "data/maps/Route101/scripts.pory"
	walk_fast_down
# 164 "data/maps/Route101/scripts.pory"
	walk_fast_left
# 165 "data/maps/Route101/scripts.pory"
	walk_fast_left
# 166 "data/maps/Route101/scripts.pory"
	walk_fast_left
# 167 "data/maps/Route101/scripts.pory"
	walk_fast_up
# 168 "data/maps/Route101/scripts.pory"
	walk_fast_up
# 169 "data/maps/Route101/scripts.pory"
	walk_fast_right
# 170 "data/maps/Route101/scripts.pory"
	walk_fast_right
# 171 "data/maps/Route101/scripts.pory"
	walk_fast_right
# 172 "data/maps/Route101/scripts.pory"
	walk_fast_down
# 173 "data/maps/Route101/scripts.pory"
	walk_fast_down
# 174 "data/maps/Route101/scripts.pory"
	walk_fast_left
# 175 "data/maps/Route101/scripts.pory"
	walk_fast_left
# 176 "data/maps/Route101/scripts.pory"
	walk_fast_left
# 177 "data/maps/Route101/scripts.pory"
	step_end
# 178 "data/maps/Route101/scripts.pory"

# 179 "data/maps/Route101/scripts.pory"
Route101_Movement_BirchRunAway1:
# 180 "data/maps/Route101/scripts.pory"
	walk_fast_right
# 181 "data/maps/Route101/scripts.pory"
	walk_fast_right
# 182 "data/maps/Route101/scripts.pory"
	walk_fast_right
# 183 "data/maps/Route101/scripts.pory"
	walk_fast_right
# 184 "data/maps/Route101/scripts.pory"
	walk_fast_up
# 185 "data/maps/Route101/scripts.pory"
	walk_fast_up
# 186 "data/maps/Route101/scripts.pory"
	step_end
# 187 "data/maps/Route101/scripts.pory"

# 188 "data/maps/Route101/scripts.pory"
@ Leftover data? This command is unused.
# 189 "data/maps/Route101/scripts.pory"
	step_end
# 190 "data/maps/Route101/scripts.pory"

# 191 "data/maps/Route101/scripts.pory"
Route101_Movement_BirchFaceZigzagoon:
# 192 "data/maps/Route101/scripts.pory"
	walk_in_place_fast_right
# 193 "data/maps/Route101/scripts.pory"
	walk_in_place_fast_right
# 194 "data/maps/Route101/scripts.pory"
	walk_in_place_fast_right
# 195 "data/maps/Route101/scripts.pory"
	walk_in_place_fast_right
# 196 "data/maps/Route101/scripts.pory"
	step_end
# 197 "data/maps/Route101/scripts.pory"

# 198 "data/maps/Route101/scripts.pory"
Route101_Movement_Unused1:
# 199 "data/maps/Route101/scripts.pory"
	walk_up
# 200 "data/maps/Route101/scripts.pory"
	walk_up
# 201 "data/maps/Route101/scripts.pory"
	step_end
# 202 "data/maps/Route101/scripts.pory"

# 203 "data/maps/Route101/scripts.pory"
Route101_Movement_Unused2:
# 204 "data/maps/Route101/scripts.pory"
	walk_up
# 205 "data/maps/Route101/scripts.pory"
	walk_left
# 206 "data/maps/Route101/scripts.pory"
	walk_up
# 207 "data/maps/Route101/scripts.pory"
	step_end
# 208 "data/maps/Route101/scripts.pory"

# 209 "data/maps/Route101/scripts.pory"
Route101_EventScript_Youngster::
# 210 "data/maps/Route101/scripts.pory"
	msgbox Route101_Text_TakeTiredPokemonToPokeCenter, MSGBOX_NPC
# 211 "data/maps/Route101/scripts.pory"
	end
# 212 "data/maps/Route101/scripts.pory"

# 213 "data/maps/Route101/scripts.pory"
Route101_EventScript_Boy::
# 214 "data/maps/Route101/scripts.pory"
	msgbox Route101_Text_WildPokemonInTallGrass, MSGBOX_NPC
# 215 "data/maps/Route101/scripts.pory"
	end
# 216 "data/maps/Route101/scripts.pory"

# 217 "data/maps/Route101/scripts.pory"
Route101_EventScript_RouteSign::
# 218 "data/maps/Route101/scripts.pory"
	msgbox Route101_Text_RouteSign, MSGBOX_SIGN
# 219 "data/maps/Route101/scripts.pory"
	end
# 220 "data/maps/Route101/scripts.pory"

# 221 "data/maps/Route101/scripts.pory"
Route101_EventScript_BirchsBag::
# 222 "data/maps/Route101/scripts.pory"
	lock
# 223 "data/maps/Route101/scripts.pory"
	faceplayer
# 224 "data/maps/Route101/scripts.pory"
	setflag FLAG_SYS_POKEMON_GET
# 225 "data/maps/Route101/scripts.pory"
	setflag FLAG_RESCUED_BIRCH
# 226 "data/maps/Route101/scripts.pory"
	fadescreen FADE_TO_BLACK
# 227 "data/maps/Route101/scripts.pory"
	removeobject LOCALID_ZIGZAGOON
# 228 "data/maps/Route101/scripts.pory"
	setobjectxy OBJ_EVENT_ID_PLAYER, 6, 13
# 229 "data/maps/Route101/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
# 230 "data/maps/Route101/scripts.pory"
	waitmovement 0
# 231 "data/maps/Route101/scripts.pory"
    copyvar VAR_0x800A, REGION
# 232 "data/maps/Route101/scripts.pory"
	special ChooseStarter
# 233 "data/maps/Route101/scripts.pory"
	waitstate
# 234 "data/maps/Route101/scripts.pory"
	applymovement LOCALID_BIRCH, Route101_Movement_BirchApproachPlayer
# 235 "data/maps/Route101/scripts.pory"
	waitmovement 0
# 236 "data/maps/Route101/scripts.pory"
	msgbox Route101_Text_YouSavedMe, MSGBOX_DEFAULT
# 237 "data/maps/Route101/scripts.pory"
	special HealPlayerParty
# 238 "data/maps/Route101/scripts.pory"
	setflag FLAG_HIDE_ROUTE_101_BIRCH_ZIGZAGOON_BATTLE
# 239 "data/maps/Route101/scripts.pory"
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_BIRCHS_LAB_BIRCH
# 240 "data/maps/Route101/scripts.pory"
	setflag FLAG_HIDE_ROUTE_101_BIRCH_STARTERS_BAG
# 241 "data/maps/Route101/scripts.pory"
	setvar VAR_BIRCH_LAB_STATE, 2
# 242 "data/maps/Route101/scripts.pory"
	setvar VAR_ROUTE101_STATE, 3
# 243 "data/maps/Route101/scripts.pory"
	clearflag FLAG_HIDE_MAP_NAME_POPUP
# 244 "data/maps/Route101/scripts.pory"
	checkplayergender
# 245 "data/maps/Route101/scripts.pory"
	call_if_eq VAR_RESULT, MALE, Route101_EventScript_HideMayInBedroom
# 246 "data/maps/Route101/scripts.pory"
	call_if_eq VAR_RESULT, FEMALE, Route101_EventScript_HideBrendanInBedroom
# 247 "data/maps/Route101/scripts.pory"
	warp MAP_LITTLEROOT_TOWN_PROFESSOR_BIRCHS_LAB, 6, 5
# 248 "data/maps/Route101/scripts.pory"
	waitstate
# 249 "data/maps/Route101/scripts.pory"
	release
# 250 "data/maps/Route101/scripts.pory"
	end
# 251 "data/maps/Route101/scripts.pory"

# 252 "data/maps/Route101/scripts.pory"
Route101_EventScript_HideMayInBedroom::
# 253 "data/maps/Route101/scripts.pory"
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_RIVAL_BEDROOM
# 254 "data/maps/Route101/scripts.pory"
	return
# 255 "data/maps/Route101/scripts.pory"

# 256 "data/maps/Route101/scripts.pory"
Route101_EventScript_HideBrendanInBedroom::
# 257 "data/maps/Route101/scripts.pory"
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_RIVAL_BEDROOM
# 258 "data/maps/Route101/scripts.pory"
	return
# 259 "data/maps/Route101/scripts.pory"

# 260 "data/maps/Route101/scripts.pory"
Route101_Movement_BirchApproachPlayer:
# 261 "data/maps/Route101/scripts.pory"
	walk_right
# 262 "data/maps/Route101/scripts.pory"
	step_end
# 263 "data/maps/Route101/scripts.pory"

# 264 "data/maps/Route101/scripts.pory"
Route101_Text_HelpMe:
# 265 "data/maps/Route101/scripts.pory"
	.string "H-help me!$"
# 266 "data/maps/Route101/scripts.pory"

# 267 "data/maps/Route101/scripts.pory"
Route101_Text_PleaseHelp:
# 268 "data/maps/Route101/scripts.pory"
	.string "Hello! You over there!\n"
# 269 "data/maps/Route101/scripts.pory"
	.string "Please! Help!\p"
# 270 "data/maps/Route101/scripts.pory"
	.string "In my BAG!\n"
# 271 "data/maps/Route101/scripts.pory"
	.string "There's a POKé BALL!$"
# 272 "data/maps/Route101/scripts.pory"
    .string "The guy fishing lets\n"
# 273 "data/maps/Route101/scripts.pory"
    .string "you choose your region!$"
# 274 "data/maps/Route101/scripts.pory"

# 275 "data/maps/Route101/scripts.pory"
Route101_Text_DontLeaveMe:
# 276 "data/maps/Route101/scripts.pory"
	.string "Wh-Where are you going?!\n"
# 277 "data/maps/Route101/scripts.pory"
	.string "Don't leave me like this!$"
# 278 "data/maps/Route101/scripts.pory"

# 279 "data/maps/Route101/scripts.pory"
Route101_Text_YouSavedMe:
# 280 "data/maps/Route101/scripts.pory"
	.string "PROF. BIRCH: Whew…\p"
# 281 "data/maps/Route101/scripts.pory"
	.string "I was in the tall grass studying wild\n"
# 282 "data/maps/Route101/scripts.pory"
	.string "POKéMON when I was jumped.\p"
# 283 "data/maps/Route101/scripts.pory"
	.string "You saved me.\n"
# 284 "data/maps/Route101/scripts.pory"
	.string "Thanks a lot!\p"
# 285 "data/maps/Route101/scripts.pory"
	.string "Oh?\p"
# 286 "data/maps/Route101/scripts.pory"
	.string "Hi, you're {PLAYER}{KUN}!\p"
# 287 "data/maps/Route101/scripts.pory"
	.string "This is not the place to chat, so come\n"
# 288 "data/maps/Route101/scripts.pory"
	.string "by my POKéMON LAB later, okay?$"
# 289 "data/maps/Route101/scripts.pory"

# 290 "data/maps/Route101/scripts.pory"
Route101_Text_TakeTiredPokemonToPokeCenter:
# 291 "data/maps/Route101/scripts.pory"
	.string "If POKéMON get tired, take them to\n"
# 292 "data/maps/Route101/scripts.pory"
	.string "a POKéMON CENTER.\p"
# 293 "data/maps/Route101/scripts.pory"
	.string "There's a POKéMON CENTER in OLDALE\n"
# 294 "data/maps/Route101/scripts.pory"
	.string "TOWN right close by.$"
# 295 "data/maps/Route101/scripts.pory"

# 296 "data/maps/Route101/scripts.pory"
Route101_Text_WildPokemonInTallGrass:
# 297 "data/maps/Route101/scripts.pory"
	.string "Wild POKéMON will jump out at you in\n"
# 298 "data/maps/Route101/scripts.pory"
	.string "tall grass.\p"
# 299 "data/maps/Route101/scripts.pory"
	.string "If you want to catch POKéMON, you have\n"
# 300 "data/maps/Route101/scripts.pory"
	.string "to go into the tall grass and search.$"
# 301 "data/maps/Route101/scripts.pory"

# 302 "data/maps/Route101/scripts.pory"
Route101_Text_RouteSign:
# 303 "data/maps/Route101/scripts.pory"
	.string "ROUTE 101\n"
# 304 "data/maps/Route101/scripts.pory"
	.string "{UP_ARROW} OLDALE TOWN$"

Rout101_EventScript_RegionDefault::
# 307 "data/maps/Route101/scripts.pory"
	setvar REGION, 0
	return


Route101_EventScript_Region::
# 312 "data/maps/Route101/scripts.pory"
	goto_if_unset KANTO, Route101_EventScript_Region_2
	return

Route101_EventScript_Region_1:
# 313 "data/maps/Route101/scripts.pory"
	lock
# 314 "data/maps/Route101/scripts.pory"
	faceplayer
# 315 "data/maps/Route101/scripts.pory"
	multichoice 15, 31, MULTI_STARTER_CHOICE, 1
# 316 "data/maps/Route101/scripts.pory"
	closemessage
# 317 "data/maps/Route101/scripts.pory"
	compare VAR_RESULT, 0
	goto_if_eq Route101_EventScript_Region_20
Route101_EventScript_Region_19:
# 320 "data/maps/Route101/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq Route101_EventScript_Region_23
Route101_EventScript_Region_22:
# 323 "data/maps/Route101/scripts.pory"
	compare VAR_RESULT, 2
	goto_if_eq Route101_EventScript_Region_26
Route101_EventScript_Region_25:
# 326 "data/maps/Route101/scripts.pory"
	compare VAR_RESULT, 3
	goto_if_eq Route101_EventScript_Region_29
Route101_EventScript_Region_28:
# 329 "data/maps/Route101/scripts.pory"
	compare VAR_RESULT, 4
	goto_if_eq Route101_EventScript_Region_32
Route101_EventScript_Region_31:
# 332 "data/maps/Route101/scripts.pory"
	compare VAR_RESULT, 5
	goto_if_eq Route101_EventScript_Region_35
Route101_EventScript_Region_34:
# 335 "data/maps/Route101/scripts.pory"
	compare VAR_RESULT, 6
	goto_if_eq Route101_EventScript_Region_38
Route101_EventScript_Region_37:
# 338 "data/maps/Route101/scripts.pory"
	compare VAR_RESULT, 7
	goto_if_eq Route101_EventScript_Region_41
Route101_EventScript_Region_40:
# 341 "data/maps/Route101/scripts.pory"
	compare VAR_RESULT, 8
	goto_if_eq Route101_EventScript_Region_43
	return

Route101_EventScript_Region_2:
# 312 "data/maps/Route101/scripts.pory"
	goto_if_unset JOHTO, Route101_EventScript_Region_4
	return

Route101_EventScript_Region_4:
# 312 "data/maps/Route101/scripts.pory"
	goto_if_unset HOENN, Route101_EventScript_Region_6
	return

Route101_EventScript_Region_6:
# 312 "data/maps/Route101/scripts.pory"
	goto_if_unset SINNOH, Route101_EventScript_Region_8
	return

Route101_EventScript_Region_8:
# 312 "data/maps/Route101/scripts.pory"
	goto_if_unset UNOVA, Route101_EventScript_Region_10
	return

Route101_EventScript_Region_10:
# 312 "data/maps/Route101/scripts.pory"
	goto_if_unset KALOS, Route101_EventScript_Region_12
	return

Route101_EventScript_Region_12:
# 312 "data/maps/Route101/scripts.pory"
	goto_if_unset ALOLA, Route101_EventScript_Region_14
	return

Route101_EventScript_Region_14:
# 312 "data/maps/Route101/scripts.pory"
	goto_if_unset GALAR, Route101_EventScript_Region_16
	return

Route101_EventScript_Region_16:
# 312 "data/maps/Route101/scripts.pory"
	goto_if_unset PALDEA, Route101_EventScript_Region_1
	return

Route101_EventScript_Region_20:
# 318 "data/maps/Route101/scripts.pory"
	setvar REGION, 0
	goto Route101_EventScript_Region_19

Route101_EventScript_Region_23:
# 321 "data/maps/Route101/scripts.pory"
	setvar REGION, 1
	goto Route101_EventScript_Region_22

Route101_EventScript_Region_26:
# 324 "data/maps/Route101/scripts.pory"
	setvar REGION, 2
	goto Route101_EventScript_Region_25

Route101_EventScript_Region_29:
# 327 "data/maps/Route101/scripts.pory"
	setvar REGION, 3
	goto Route101_EventScript_Region_28

Route101_EventScript_Region_32:
# 330 "data/maps/Route101/scripts.pory"
	setvar REGION, 4
	goto Route101_EventScript_Region_31

Route101_EventScript_Region_35:
# 333 "data/maps/Route101/scripts.pory"
	setvar REGION, 5
	goto Route101_EventScript_Region_34

Route101_EventScript_Region_38:
# 336 "data/maps/Route101/scripts.pory"
	setvar REGION, 6
	goto Route101_EventScript_Region_37

Route101_EventScript_Region_41:
# 339 "data/maps/Route101/scripts.pory"
	setvar REGION, 7
	goto Route101_EventScript_Region_40

Route101_EventScript_Region_43:
# 342 "data/maps/Route101/scripts.pory"
	setvar REGION, 8
	return

