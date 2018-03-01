#include "global.h"
#include "battle.h"
#include "battle_controllers.h"
#include "battle_message.h"
#include "battle_interface.h"
#include "battle_anim.h"
#include "constants/battle_anim.h"
#include "battle_ai_script_commands.h"
#include "battle_tv.h"
#include "pokemon.h"
#include "link.h"
#include "util.h"
#include "main.h"
#include "constants/songs.h"
#include "constants/trainers.h"
#include "sound.h"
#include "window.h"
#include "m4a.h"
#include "palette.h"
#include "task.h"
#include "text.h"
#include "string_util.h"
#include "bg.h"
#include "reshow_battle_screen.h"
#include "pokeball.h"
#include "data2.h"
#include "battle_setup.h"

extern u16 gBattle_BG0_X;
extern u16 gBattle_BG0_Y;
extern struct MusicPlayerInfo gMPlayInfo_BGM;
extern struct UnusedControllerStruct gUnknown_02022D0C;

extern const struct CompressedSpritePalette gTrainerFrontPicPaletteTable[];

extern void sub_8172EF0(u8 bank, struct Pokemon *mon);
extern void sub_806A068(u16, u8);
extern void sub_81851A8(u8 *);
extern u16 sub_8068B48(void);
extern u8 GetFrontierTrainerFrontSpriteId(u16 trainerId);

// this file's functions
static void LinkOpponentHandleGetMonData(void);
static void LinkOpponentHandleGetRawMonData(void);
static void LinkOpponentHandleSetMonData(void);
static void LinkOpponentHandleSetRawMonData(void);
static void LinkOpponentHandleLoadMonSprite(void);
static void LinkOpponentHandleSwitchInAnim(void);
static void LinkOpponentHandleReturnMonToBall(void);
static void LinkOpponentHandleDrawTrainerPic(void);
static void LinkOpponentHandleTrainerSlide(void);
static void LinkOpponentHandleTrainerSlideBack(void);
static void LinkOpponentHandleFaintAnimation(void);
static void LinkOpponentHandlePaletteFade(void);
static void LinkOpponentHandleSuccessBallThrowAnim(void);
static void LinkOpponentHandleBallThrowAnim(void);
static void LinkOpponentHandlePause(void);
static void LinkOpponentHandleMoveAnimation(void);
static void LinkOpponentHandlePrintString(void);
static void LinkOpponentHandlePrintSelectionString(void);
static void LinkOpponentHandleChooseAction(void);
static void LinkOpponentHandleUnknownYesNoBox(void);
static void LinkOpponentHandleChooseMove(void);
static void LinkOpponentHandleChooseItem(void);
static void LinkOpponentHandleChoosePokemon(void);
static void LinkOpponentHandleCmd23(void);
static void LinkOpponentHandleHealthBarUpdate(void);
static void LinkOpponentHandleExpUpdate(void);
static void LinkOpponentHandleStatusIconUpdate(void);
static void LinkOpponentHandleStatusAnimation(void);
static void LinkOpponentHandleStatusXor(void);
static void LinkOpponentHandleDataTransfer(void);
static void LinkOpponentHandleDMA3Transfer(void);
static void LinkOpponentHandlePlayBGM(void);
static void LinkOpponentHandleCmd32(void);
static void LinkOpponentHandleTwoReturnValues(void);
static void LinkOpponentHandleChosenMonReturnValue(void);
static void LinkOpponentHandleOneReturnValue(void);
static void LinkOpponentHandleOneReturnValue_Duplicate(void);
static void LinkOpponentHandleCmd37(void);
static void LinkOpponentHandleCmd38(void);
static void LinkOpponentHandleCmd39(void);
static void LinkOpponentHandleCmd40(void);
static void LinkOpponentHandleHitAnimation(void);
static void LinkOpponentHandleCmd42(void);
static void LinkOpponentHandlePlaySE(void);
static void LinkOpponentHandlePlayFanfareOrBGM(void);
static void LinkOpponentHandleFaintingCry(void);
static void LinkOpponentHandleIntroSlide(void);
static void LinkOpponentHandleIntroTrainerBallThrow(void);
static void LinkOpponentHandleDrawPartyStatusSummary(void);
static void LinkOpponentHandleCmd49(void);
static void LinkOpponentHandleCmd50(void);
static void LinkOpponentHandleSpriteInvisibility(void);
static void LinkOpponentHandleBattleAnimation(void);
static void LinkOpponentHandleLinkStandbyMsg(void);
static void LinkOpponentHandleResetActionMoveSelection(void);
static void LinkOpponentHandleCmd55(void);
static void nullsub_92(void);

static void LinkOpponentBufferRunCommand(void);
static void LinkOpponentBufferExecCompleted(void);
static void sub_8064DD0(void);
static u32 CopyLinkOpponentMonData(u8 monId, u8 *dst);
static void SetLinkOpponentMonData(u8 monId);
static void sub_8066494(u8 bank, bool8 dontClearSubstituteBit);
static void DoSwitchOutAnimation(void);
static void LinkOpponentDoMoveAnimation(void);
static void sub_8067618(u8 taskId);
static void sub_80676FC(struct Sprite *sprite);
static void sub_806782C(void);

static void (*const sLinkOpponentBufferCommands[CONTROLLER_CMDS_COUNT])(void) =
{
    LinkOpponentHandleGetMonData,
    LinkOpponentHandleGetRawMonData,
    LinkOpponentHandleSetMonData,
    LinkOpponentHandleSetRawMonData,
    LinkOpponentHandleLoadMonSprite,
    LinkOpponentHandleSwitchInAnim,
    LinkOpponentHandleReturnMonToBall,
    LinkOpponentHandleDrawTrainerPic,
    LinkOpponentHandleTrainerSlide,
    LinkOpponentHandleTrainerSlideBack,
    LinkOpponentHandleFaintAnimation,
    LinkOpponentHandlePaletteFade,
    LinkOpponentHandleSuccessBallThrowAnim,
    LinkOpponentHandleBallThrowAnim,
    LinkOpponentHandlePause,
    LinkOpponentHandleMoveAnimation,
    LinkOpponentHandlePrintString,
    LinkOpponentHandlePrintSelectionString,
    LinkOpponentHandleChooseAction,
    LinkOpponentHandleUnknownYesNoBox,
    LinkOpponentHandleChooseMove,
    LinkOpponentHandleChooseItem,
    LinkOpponentHandleChoosePokemon,
    LinkOpponentHandleCmd23,
    LinkOpponentHandleHealthBarUpdate,
    LinkOpponentHandleExpUpdate,
    LinkOpponentHandleStatusIconUpdate,
    LinkOpponentHandleStatusAnimation,
    LinkOpponentHandleStatusXor,
    LinkOpponentHandleDataTransfer,
    LinkOpponentHandleDMA3Transfer,
    LinkOpponentHandlePlayBGM,
    LinkOpponentHandleCmd32,
    LinkOpponentHandleTwoReturnValues,
    LinkOpponentHandleChosenMonReturnValue,
    LinkOpponentHandleOneReturnValue,
    LinkOpponentHandleOneReturnValue_Duplicate,
    LinkOpponentHandleCmd37,
    LinkOpponentHandleCmd38,
    LinkOpponentHandleCmd39,
    LinkOpponentHandleCmd40,
    LinkOpponentHandleHitAnimation,
    LinkOpponentHandleCmd42,
    LinkOpponentHandlePlaySE,
    LinkOpponentHandlePlayFanfareOrBGM,
    LinkOpponentHandleFaintingCry,
    LinkOpponentHandleIntroSlide,
    LinkOpponentHandleIntroTrainerBallThrow,
    LinkOpponentHandleDrawPartyStatusSummary,
    LinkOpponentHandleCmd49,
    LinkOpponentHandleCmd50,
    LinkOpponentHandleSpriteInvisibility,
    LinkOpponentHandleBattleAnimation,
    LinkOpponentHandleLinkStandbyMsg,
    LinkOpponentHandleResetActionMoveSelection,
    LinkOpponentHandleCmd55,
    nullsub_92
};

static void nullsub_28(void)
{
}

void SetControllerToLinkOpponent(void)
{
    gBattlerControllerFuncs[gActiveBattler] = LinkOpponentBufferRunCommand;
}

static void LinkOpponentBufferRunCommand(void)
{
    if (gBattleControllerExecFlags & gBitTable[gActiveBattler])
    {
        if (gBattleBufferA[gActiveBattler][0] < ARRAY_COUNT(sLinkOpponentBufferCommands))
            sLinkOpponentBufferCommands[gBattleBufferA[gActiveBattler][0]]();
        else
            LinkOpponentBufferExecCompleted();
    }
}

static void CompleteOnBankSpriteCallbackDummy(void)
{
    if (gSprites[gBattlerSpriteIds[gActiveBattler]].callback == SpriteCallbackDummy)
        LinkOpponentBufferExecCompleted();
}

static void CompleteOnBankSpriteCallbackDummy2(void)
{
    if (gSprites[gBattlerSpriteIds[gActiveBattler]].callback == SpriteCallbackDummy)
        LinkOpponentBufferExecCompleted();
}

static void sub_8064470(void)
{
    if (gSprites[gBattlerSpriteIds[gActiveBattler]].callback == SpriteCallbackDummy)
    {
        FreeTrainerFrontPicPalette(gSprites[gBattlerSpriteIds[gActiveBattler]].oam.affineParam);
        FreeSpriteOamMatrix(&gSprites[gBattlerSpriteIds[gActiveBattler]]);
        DestroySprite(&gSprites[gBattlerSpriteIds[gActiveBattler]]);
        LinkOpponentBufferExecCompleted();
    }
}

static void sub_80644D8(void)
{
    if (--gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_9 == 0xFF)
    {
        gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_9 = 0;
        LinkOpponentBufferExecCompleted();
    }
}

static void sub_8064520(void)
{
    bool32 r8 = FALSE;
    bool32 r4 = FALSE;

    if (!IsDoubleBattle() || (IsDoubleBattle() && (gBattleTypeFlags & BATTLE_TYPE_MULTI)))
    {
        if (gSprites[gHealthboxSpriteIds[gActiveBattler]].callback == SpriteCallbackDummy)
            r8 = TRUE;
    }
    else
    {
        if (gSprites[gHealthboxSpriteIds[gActiveBattler]].callback == SpriteCallbackDummy
         && gSprites[gHealthboxSpriteIds[BATTLE_PARTNER(gActiveBattler)]].callback == SpriteCallbackDummy)
        {
            r8 = TRUE;
        }
        r4 = TRUE;
    }

    if (r8)
    {
        if (r4 || !IsBattlerSpriteVisible(BATTLE_PARTNER(gActiveBattler)))
        {
            if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_1_x1)
                return;
            if (!gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(gActiveBattler)].field_1_x1)
                return;

            gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].flag_x80 = 0;
            gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_1_x1 = 0;

            gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(gActiveBattler)].flag_x80 = 0;
            gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(gActiveBattler)].field_1_x1 = 0;

            FreeSpriteTilesByTag(0x27F9);
            FreeSpritePaletteByTag(0x27F9);
        }
        else
        {
            if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_1_x1)
                return;

            gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].flag_x80 = 0;
            gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_1_x1 = 0;

            if (GetBattlerPosition(gActiveBattler) == B_POSITION_OPPONENT_RIGHT)
            {
                FreeSpriteTilesByTag(0x27F9);
                FreeSpritePaletteByTag(0x27F9);
            }
        }

        gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_9 = 3;
        gBattlerControllerFuncs[gActiveBattler] = sub_80644D8;
    }
}

static void sub_8064734(void)
{
    bool32 r10 = FALSE;

    if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].ballAnimActive
        && !gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].flag_x80
        && !gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_1_x1)
    {
        sub_8172EF0(gActiveBattler, &gEnemyParty[gBattlerPartyIndexes[gActiveBattler]]);
    }
    if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI)
        && !gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(gActiveBattler)].ballAnimActive
        && !gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(gActiveBattler)].flag_x80
        && !gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(gActiveBattler)].field_1_x1)
    {
        sub_8172EF0(BATTLE_PARTNER(gActiveBattler), &gEnemyParty[gBattlerPartyIndexes[BATTLE_PARTNER(gActiveBattler)]]);
    }


    if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].ballAnimActive && !gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(gActiveBattler)].ballAnimActive)
    {
        if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_1_x80)
        {
            if (IsDoubleBattle() && !(gBattleTypeFlags & BATTLE_TYPE_MULTI))
            {
                UpdateHealthboxAttribute(gHealthboxSpriteIds[BATTLE_PARTNER(gActiveBattler)], &gEnemyParty[gBattlerPartyIndexes[BATTLE_PARTNER(gActiveBattler)]], HEALTHBOX_ALL);
                sub_8076918(BATTLE_PARTNER(gActiveBattler));
                SetHealthboxSpriteVisible(gHealthboxSpriteIds[BATTLE_PARTNER(gActiveBattler)]);
            }
            UpdateHealthboxAttribute(gHealthboxSpriteIds[gActiveBattler], &gEnemyParty[gBattlerPartyIndexes[gActiveBattler]], HEALTHBOX_ALL);
            sub_8076918(gActiveBattler);
            SetHealthboxSpriteVisible(gHealthboxSpriteIds[gActiveBattler]);
        }
        gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_1_x80 = 1;
    }

    if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_1_x40
        && gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_1_x80
        && !gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(gActiveBattler)].field_1_x40
        && !IsCryPlayingOrClearCrySongs())
    {
        if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_1_x20)
        {
            if (gBattleTypeFlags & BATTLE_TYPE_MULTI && gBattleTypeFlags & BATTLE_TYPE_LINK)
            {
                if (GetBattlerPosition(gActiveBattler) == B_POSITION_OPPONENT_LEFT)
                    m4aMPlayContinue(&gMPlayInfo_BGM);
            }
            else
            {
                m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 0x100);
            }
        }

        gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_1_x20 = 1;
        r10 = TRUE;
    }

    if (r10)
    {
        if (gSprites[gUnknown_03005D7C[gActiveBattler]].callback == SpriteCallbackDummy
            && gSprites[gBattlerSpriteIds[gActiveBattler]].callback == SpriteCallbackDummy)
        {
            if (gBattleTypeFlags & BATTLE_TYPE_MULTI && GetBattlerPosition(gActiveBattler) == B_POSITION_OPPONENT_RIGHT)
            {
                if (++gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_9 == 1)
                    return;
                gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_9 = 0;
            }

            if (IsDoubleBattle() && !(gBattleTypeFlags & BATTLE_TYPE_MULTI))
            {
                DestroySprite(&gSprites[gUnknown_03005D7C[BATTLE_PARTNER(gActiveBattler)]]);
                SetBattlerShadowSpriteCallback(BATTLE_PARTNER(gActiveBattler), GetMonData(&gEnemyParty[gBattlerPartyIndexes[BATTLE_PARTNER(gActiveBattler)]], MON_DATA_SPECIES));
            }


            DestroySprite(&gSprites[gUnknown_03005D7C[gActiveBattler]]);
            SetBattlerShadowSpriteCallback(gActiveBattler, GetMonData(&gEnemyParty[gBattlerPartyIndexes[gActiveBattler]], MON_DATA_SPECIES));

            gBattleSpritesDataPtr->animationData->field_9_x1 = 0;
            gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_1_x20 = 0;
            gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_1_x80 = 0;

            gBattlerControllerFuncs[gActiveBattler] = sub_8064520;
        }
    }
}

static void sub_8064B04(void)
{
    if (gSprites[gBattlerSpriteIds[gActiveBattler]].callback == SpriteCallbackDummy
     && gSprites[gBattlerSpriteIds[gActiveBattler]].pos2.x == 0)
    {
        if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].flag_x80)
        {
            sub_8172EF0(gActiveBattler, &gEnemyParty[gBattlerPartyIndexes[gActiveBattler]]);
        }
        else
        {
            if (gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_1_x1)
            {
                gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].flag_x80 = 0;
                gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_1_x1 = 0;
                FreeSpriteTilesByTag(0x27F9);
                FreeSpritePaletteByTag(0x27F9);
                LinkOpponentBufferExecCompleted();
            }
        }
    }
}

static void CompleteOnHealthbarDone(void)
{
    s16 hpValue = sub_8074AA0(gActiveBattler, gHealthboxSpriteIds[gActiveBattler], HEALTH_BAR, 0);

    SetHealthboxSpriteVisible(gHealthboxSpriteIds[gActiveBattler]);

    if (hpValue != -1)
    {
        UpdateHpTextInHealthbox(gHealthboxSpriteIds[gActiveBattler], hpValue, HP_CURRENT);
    }
    else
    {
        LinkOpponentBufferExecCompleted();
    }
}

static void sub_8064C14(void)
{
    if (!gSprites[gBattlerSpriteIds[gActiveBattler]].inUse)
    {
        SetHealthboxSpriteInvisible(gHealthboxSpriteIds[gActiveBattler]);
        LinkOpponentBufferExecCompleted();
    }
}

static void sub_8064C58(void)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].specialAnimActive)
    {
        FreeSpriteOamMatrix(&gSprites[gBattlerSpriteIds[gActiveBattler]]);
        DestroySprite(&gSprites[gBattlerSpriteIds[gActiveBattler]]);
        HideBattlerShadowSprite(gActiveBattler);
        SetHealthboxSpriteInvisible(gHealthboxSpriteIds[gActiveBattler]);
        LinkOpponentBufferExecCompleted();
    }
}

static void CompleteOnInactiveTextPrinter(void)
{
    if (!IsTextPrinterActive(0))
        LinkOpponentBufferExecCompleted();
}

static void DoHitAnimBlinkSpriteEffect(void)
{
    u8 spriteId = gBattlerSpriteIds[gActiveBattler];

    if (gSprites[spriteId].data[1] == 32)
    {
        gSprites[spriteId].data[1] = 0;
        gSprites[spriteId].invisible = 0;
        gDoingBattleAnim = FALSE;
        LinkOpponentBufferExecCompleted();
    }
    else
    {
        if ((gSprites[spriteId].data[1] % 4) == 0)
            gSprites[spriteId].invisible ^= 1;
        gSprites[spriteId].data[1]++;
    }
}

static void sub_8064D60(void)
{
    if (gSprites[gHealthboxSpriteIds[gActiveBattler]].callback == SpriteCallbackDummy)
    {
        if (gBattleSpritesDataPtr->battlerData[gActiveBattler].behindSubstitute)
            InitAndLaunchSpecialAnimation(gActiveBattler, gActiveBattler, gActiveBattler, B_ANIM_MON_TO_SUBSTITUTE);

        gBattlerControllerFuncs[gActiveBattler] = sub_8064DD0;
    }
}

static void sub_8064DD0(void)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].specialAnimActive && !IsCryPlayingOrClearCrySongs())
    {
        if (gSprites[gBattlerSpriteIds[gActiveBattler]].callback == SpriteCallbackDummy
            || gSprites[gBattlerSpriteIds[gActiveBattler]].callback == SpriteCallbackDummy_2)
        {
            m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 0x100);
            LinkOpponentBufferExecCompleted();
        }
    }
}

static void sub_8064E50(void)
{
    if (gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_1_x1
     && gSprites[gBattlerSpriteIds[gActiveBattler]].callback == SpriteCallbackDummy)
    {
        gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].flag_x80 = 0;
        gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_1_x1 = 0;

        FreeSpriteTilesByTag(0x27F9);
        FreeSpritePaletteByTag(0x27F9);

        StartSpriteAnim(&gSprites[gBattlerSpriteIds[gActiveBattler]], 0);

        UpdateHealthboxAttribute(gHealthboxSpriteIds[gActiveBattler], &gEnemyParty[gBattlerPartyIndexes[gActiveBattler]], HEALTHBOX_ALL);
        sub_8076918(gActiveBattler);
        SetHealthboxSpriteVisible(gHealthboxSpriteIds[gActiveBattler]);
        CopyBattleSpriteInvisibility(gActiveBattler);
        gBattlerControllerFuncs[gActiveBattler] = sub_8064D60;
    }
}

static void sub_8064F40(void)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].ballAnimActive
        && !gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].flag_x80)
    {
        sub_8172EF0(gActiveBattler, &gEnemyParty[gBattlerPartyIndexes[gActiveBattler]]);
    }

    if (gSprites[gUnknown_03005D7C[gActiveBattler]].callback == SpriteCallbackDummy
        && !gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].ballAnimActive)
    {
        DestroySprite(&gSprites[gUnknown_03005D7C[gActiveBattler]]);
        SetBattlerShadowSpriteCallback(gActiveBattler, GetMonData(&gEnemyParty[gBattlerPartyIndexes[gActiveBattler]], MON_DATA_SPECIES));
        gBattlerControllerFuncs[gActiveBattler] = sub_8064E50;
    }
}

static void CompleteOnFinishedStatusAnimation(void)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].statusAnimActive)
        LinkOpponentBufferExecCompleted();
}

static void CompleteOnFinishedBattleAnimation(void)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].animFromTableActive)
        LinkOpponentBufferExecCompleted();
}

static void LinkOpponentBufferExecCompleted(void)
{
    gBattlerControllerFuncs[gActiveBattler] = LinkOpponentBufferRunCommand;
    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
    {
        u8 playerId = GetMultiplayerId();

        PrepareBufferDataTransferLink(2, 4, &playerId);
        gBattleBufferA[gActiveBattler][0] = CONTROLLER_TERMINATOR_NOP;
    }
    else
    {
        gBattleControllerExecFlags &= ~gBitTable[gActiveBattler];
    }
}

static void LinkOpponentHandleGetMonData(void)
{
    u8 monData[sizeof(struct Pokemon) * 2 + 56]; // this allows to get full data of two pokemon, trying to get more will result in overwriting data
    u32 size = 0;
    u8 monToCheck;
    s32 i;

    if (gBattleBufferA[gActiveBattler][2] == 0)
    {
        size += CopyLinkOpponentMonData(gBattlerPartyIndexes[gActiveBattler], monData);
    }
    else
    {
        monToCheck = gBattleBufferA[gActiveBattler][2];
        for (i = 0; i < 6; i++)
        {
            if (monToCheck & 1)
                size += CopyLinkOpponentMonData(i, monData + size);
            monToCheck >>= 1;
        }
    }
    BtlController_EmitDataTransfer(1, size, monData);
    LinkOpponentBufferExecCompleted();
}

static u32 CopyLinkOpponentMonData(u8 monId, u8 *dst)
{
    struct BattlePokemon battleMon;
    struct MovePpInfo moveData;
    u8 nickname[20];
    u8 *src;
    s16 data16;
    u32 data32;
    s32 size = 0;

    switch (gBattleBufferA[gActiveBattler][1])
    {
    case REQUEST_ALL_BATTLE:
        battleMon.species = GetMonData(&gEnemyParty[monId], MON_DATA_SPECIES);
        battleMon.item = GetMonData(&gEnemyParty[monId], MON_DATA_HELD_ITEM);
        for (size = 0; size < 4; size++)
        {
            battleMon.moves[size] = GetMonData(&gEnemyParty[monId], MON_DATA_MOVE1 + size);
            battleMon.pp[size] = GetMonData(&gEnemyParty[monId], MON_DATA_PP1 + size);
        }
        battleMon.ppBonuses = GetMonData(&gEnemyParty[monId], MON_DATA_PP_BONUSES);
        battleMon.friendship = GetMonData(&gEnemyParty[monId], MON_DATA_FRIENDSHIP);
        battleMon.experience = GetMonData(&gEnemyParty[monId], MON_DATA_EXP);
        battleMon.hpIV = GetMonData(&gEnemyParty[monId], MON_DATA_HP_IV);
        battleMon.attackIV = GetMonData(&gEnemyParty[monId], MON_DATA_ATK_IV);
        battleMon.defenseIV = GetMonData(&gEnemyParty[monId], MON_DATA_DEF_IV);
        battleMon.speedIV = GetMonData(&gEnemyParty[monId], MON_DATA_SPEED_IV);
        battleMon.spAttackIV = GetMonData(&gEnemyParty[monId], MON_DATA_SPATK_IV);
        battleMon.spDefenseIV = GetMonData(&gEnemyParty[monId], MON_DATA_SPDEF_IV);
        battleMon.personality = GetMonData(&gEnemyParty[monId], MON_DATA_PERSONALITY);
        battleMon.status1 = GetMonData(&gEnemyParty[monId], MON_DATA_STATUS);
        battleMon.level = GetMonData(&gEnemyParty[monId], MON_DATA_LEVEL);
        battleMon.hp = GetMonData(&gEnemyParty[monId], MON_DATA_HP);
        battleMon.maxHP = GetMonData(&gEnemyParty[monId], MON_DATA_MAX_HP);
        battleMon.attack = GetMonData(&gEnemyParty[monId], MON_DATA_ATK);
        battleMon.defense = GetMonData(&gEnemyParty[monId], MON_DATA_DEF);
        battleMon.speed = GetMonData(&gEnemyParty[monId], MON_DATA_SPEED);
        battleMon.spAttack = GetMonData(&gEnemyParty[monId], MON_DATA_SPATK);
        battleMon.spDefense = GetMonData(&gEnemyParty[monId], MON_DATA_SPDEF);
        battleMon.isEgg = GetMonData(&gEnemyParty[monId], MON_DATA_IS_EGG);
        battleMon.altAbility = GetMonData(&gEnemyParty[monId], MON_DATA_ALT_ABILITY);
        battleMon.otId = GetMonData(&gEnemyParty[monId], MON_DATA_OT_ID);
        GetMonData(&gEnemyParty[monId], MON_DATA_NICKNAME, nickname);
        StringCopy10(battleMon.nickname, nickname);
        GetMonData(&gEnemyParty[monId], MON_DATA_OT_NAME, battleMon.otName);
        src = (u8 *)&battleMon;
        for (size = 0; size < sizeof(battleMon); size++)
            dst[size] = src[size];
        break;
    case REQUEST_SPECIES_BATTLE:
        data16 = GetMonData(&gEnemyParty[monId], MON_DATA_SPECIES);
        dst[0] = data16;
        dst[1] = data16 >> 8;
        size = 2;
        break;
    case REQUEST_HELDITEM_BATTLE:
        data16 = GetMonData(&gEnemyParty[monId], MON_DATA_HELD_ITEM);
        dst[0] = data16;
        dst[1] = data16 >> 8;
        size = 2;
        break;
    case REQUEST_MOVES_PP_BATTLE:
        for (size = 0; size < 4; size++)
        {
            moveData.moves[size] = GetMonData(&gEnemyParty[monId], MON_DATA_MOVE1 + size);
            moveData.pp[size] = GetMonData(&gEnemyParty[monId], MON_DATA_PP1 + size);
        }
        moveData.ppBonuses = GetMonData(&gEnemyParty[monId], MON_DATA_PP_BONUSES);
        src = (u8*)(&moveData);
        for (size = 0; size < sizeof(moveData); size++)
            dst[size] = src[size];
        break;
    case REQUEST_MOVE1_BATTLE:
    case REQUEST_MOVE2_BATTLE:
    case REQUEST_MOVE3_BATTLE:
    case REQUEST_MOVE4_BATTLE:
        data16 = GetMonData(&gEnemyParty[monId], MON_DATA_MOVE1 + gBattleBufferA[gActiveBattler][1] - REQUEST_MOVE1_BATTLE);
        dst[0] = data16;
        dst[1] = data16 >> 8;
        size = 2;
        break;
    case REQUEST_PP_DATA_BATTLE:
        for (size = 0; size < 4; size++)
            dst[size] = GetMonData(&gEnemyParty[monId], MON_DATA_PP1 + size);
        dst[size] = GetMonData(&gEnemyParty[monId], MON_DATA_PP_BONUSES);
        size++;
        break;
    case REQUEST_PPMOVE1_BATTLE:
    case REQUEST_PPMOVE2_BATTLE:
    case REQUEST_PPMOVE3_BATTLE:
    case REQUEST_PPMOVE4_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_PP1 + gBattleBufferA[gActiveBattler][1] - REQUEST_PPMOVE1_BATTLE);
        size = 1;
        break;
    case REQUEST_OTID_BATTLE:
        data32 = GetMonData(&gEnemyParty[monId], MON_DATA_OT_ID);
        dst[0] = (data32 & 0x000000FF);
        dst[1] = (data32 & 0x0000FF00) >> 8;
        dst[2] = (data32 & 0x00FF0000) >> 16;
        size = 3;
        break;
    case REQUEST_EXP_BATTLE:
        data32 = GetMonData(&gEnemyParty[monId], MON_DATA_EXP);
        dst[0] = (data32 & 0x000000FF);
        dst[1] = (data32 & 0x0000FF00) >> 8;
        dst[2] = (data32 & 0x00FF0000) >> 16;
        size = 3;
        break;
    case REQUEST_HP_EV_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_HP_EV);
        size = 1;
        break;
    case REQUEST_ATK_EV_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_ATK_EV);
        size = 1;
        break;
    case REQUEST_DEF_EV_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_DEF_EV);
        size = 1;
        break;
    case REQUEST_SPEED_EV_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_SPEED_EV);
        size = 1;
        break;
    case REQUEST_SPATK_EV_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_SPATK_EV);
        size = 1;
        break;
    case REQUEST_SPDEF_EV_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_SPDEF_EV);
        size = 1;
        break;
    case REQUEST_FRIENDSHIP_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_FRIENDSHIP);
        size = 1;
        break;
    case REQUEST_POKERUS_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_POKERUS);
        size = 1;
        break;
    case REQUEST_MET_LOCATION_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_MET_LOCATION);
        size = 1;
        break;
    case REQUEST_MET_LEVEL_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_MET_LEVEL);
        size = 1;
        break;
    case REQUEST_MET_GAME_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_MET_GAME);
        size = 1;
        break;
    case REQUEST_POKEBALL_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_POKEBALL);
        size = 1;
        break;
    case REQUEST_ALL_IVS_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_HP_IV);
        dst[1] = GetMonData(&gEnemyParty[monId], MON_DATA_ATK_IV);
        dst[2] = GetMonData(&gEnemyParty[monId], MON_DATA_DEF_IV);
        dst[3] = GetMonData(&gEnemyParty[monId], MON_DATA_SPEED_IV);
        dst[4] = GetMonData(&gEnemyParty[monId], MON_DATA_SPATK_IV);
        dst[5] = GetMonData(&gEnemyParty[monId], MON_DATA_SPDEF_IV);
        size = 6;
        break;
    case REQUEST_HP_IV_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_HP_IV);
        size = 1;
        break;
    case REQUEST_ATK_IV_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_ATK_IV);
        size = 1;
        break;
    case REQUEST_DEF_IV_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_DEF_IV);
        size = 1;
        break;
    case REQUEST_SPEED_IV_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_SPEED_IV);
        size = 1;
        break;
    case REQUEST_SPATK_IV_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_SPATK_IV);
        size = 1;
        break;
    case REQUEST_SPDEF_IV_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_SPDEF_IV);
        size = 1;
        break;
    case REQUEST_PERSONALITY_BATTLE:
        data32 = GetMonData(&gEnemyParty[monId], MON_DATA_PERSONALITY);
        dst[0] = (data32 & 0x000000FF);
        dst[1] = (data32 & 0x0000FF00) >> 8;
        dst[2] = (data32 & 0x00FF0000) >> 16;
        dst[3] = (data32 & 0xFF000000) >> 24;
        size = 4;
        break;
    case REQUEST_CHECKSUM_BATTLE:
        data16 = GetMonData(&gEnemyParty[monId], MON_DATA_CHECKSUM);
        dst[0] = data16;
        dst[1] = data16 >> 8;
        size = 2;
        break;
    case REQUEST_STATUS_BATTLE:
        data32 = GetMonData(&gEnemyParty[monId], MON_DATA_STATUS);
        dst[0] = (data32 & 0x000000FF);
        dst[1] = (data32 & 0x0000FF00) >> 8;
        dst[2] = (data32 & 0x00FF0000) >> 16;
        dst[3] = (data32 & 0xFF000000) >> 24;
        size = 4;
        break;
    case REQUEST_LEVEL_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_LEVEL);
        size = 1;
        break;
    case REQUEST_HP_BATTLE:
        data16 = GetMonData(&gEnemyParty[monId], MON_DATA_HP);
        dst[0] = data16;
        dst[1] = data16 >> 8;
        size = 2;
        break;
    case REQUEST_MAX_HP_BATTLE:
        data16 = GetMonData(&gEnemyParty[monId], MON_DATA_MAX_HP);
        dst[0] = data16;
        dst[1] = data16 >> 8;
        size = 2;
        break;
    case REQUEST_ATK_BATTLE:
        data16 = GetMonData(&gEnemyParty[monId], MON_DATA_ATK);
        dst[0] = data16;
        dst[1] = data16 >> 8;
        size = 2;
        break;
    case REQUEST_DEF_BATTLE:
        data16 = GetMonData(&gEnemyParty[monId], MON_DATA_DEF);
        dst[0] = data16;
        dst[1] = data16 >> 8;
        size = 2;
        break;
    case REQUEST_SPEED_BATTLE:
        data16 = GetMonData(&gEnemyParty[monId], MON_DATA_SPEED);
        dst[0] = data16;
        dst[1] = data16 >> 8;
        size = 2;
        break;
    case REQUEST_SPATK_BATTLE:
        data16 = GetMonData(&gEnemyParty[monId], MON_DATA_SPATK);
        dst[0] = data16;
        dst[1] = data16 >> 8;
        size = 2;
        break;
    case REQUEST_SPDEF_BATTLE:
        data16 = GetMonData(&gEnemyParty[monId], MON_DATA_SPDEF);
        dst[0] = data16;
        dst[1] = data16 >> 8;
        size = 2;
        break;
    case REQUEST_COOL_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_COOL);
        size = 1;
        break;
    case REQUEST_BEAUTY_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_BEAUTY);
        size = 1;
        break;
    case REQUEST_CUTE_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_CUTE);
        size = 1;
        break;
    case REQUEST_SMART_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_SMART);
        size = 1;
        break;
    case REQUEST_TOUGH_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_TOUGH);
        size = 1;
        break;
    case REQUEST_SHEEN_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_SHEEN);
        size = 1;
        break;
    case REQUEST_COOL_RIBBON_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_COOL_RIBBON);
        size = 1;
        break;
    case REQUEST_BEAUTY_RIBBON_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_BEAUTY_RIBBON);
        size = 1;
        break;
    case REQUEST_CUTE_RIBBON_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_CUTE_RIBBON);
        size = 1;
        break;
    case REQUEST_SMART_RIBBON_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_SMART_RIBBON);
        size = 1;
        break;
    case REQUEST_TOUGH_RIBBON_BATTLE:
        dst[0] = GetMonData(&gEnemyParty[monId], MON_DATA_TOUGH_RIBBON);
        size = 1;
        break;
    }

    return size;
}

static void LinkOpponentHandleGetRawMonData(void)
{
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleSetMonData(void)
{
    u8 monToCheck;
    u8 i;

    if (gBattleBufferA[gActiveBattler][2] == 0)
    {
        SetLinkOpponentMonData(gBattlerPartyIndexes[gActiveBattler]);
    }
    else
    {
        monToCheck = gBattleBufferA[gActiveBattler][2];
        for (i = 0; i < 6; i++)
        {
            if (monToCheck & 1)
                SetLinkOpponentMonData(i);
            monToCheck >>= 1;
        }
    }
    LinkOpponentBufferExecCompleted();
}

static void SetLinkOpponentMonData(u8 monId)
{
    struct BattlePokemon *battlePokemon = (struct BattlePokemon *)&gBattleBufferA[gActiveBattler][3];
    struct MovePpInfo *moveData = (struct MovePpInfo *)&gBattleBufferA[gActiveBattler][3];
    s32 i;

    switch (gBattleBufferA[gActiveBattler][1])
    {
    case REQUEST_ALL_BATTLE:
        {
            u8 iv;

            SetMonData(&gEnemyParty[monId], MON_DATA_SPECIES, &battlePokemon->species);
            SetMonData(&gEnemyParty[monId], MON_DATA_HELD_ITEM, &battlePokemon->item);
            for (i = 0; i < 4; i++)
            {
                SetMonData(&gEnemyParty[monId], MON_DATA_MOVE1 + i, &battlePokemon->moves[i]);
                SetMonData(&gEnemyParty[monId], MON_DATA_PP1 + i, &battlePokemon->pp[i]);
            }
            SetMonData(&gEnemyParty[monId], MON_DATA_PP_BONUSES, &battlePokemon->ppBonuses);
            SetMonData(&gEnemyParty[monId], MON_DATA_FRIENDSHIP, &battlePokemon->friendship);
            SetMonData(&gEnemyParty[monId], MON_DATA_EXP, &battlePokemon->experience);
            iv = battlePokemon->hpIV;
            SetMonData(&gEnemyParty[monId], MON_DATA_HP_IV, &iv);
            iv = battlePokemon->attackIV;
            SetMonData(&gEnemyParty[monId], MON_DATA_ATK_IV, &iv);
            iv = battlePokemon->defenseIV;
            SetMonData(&gEnemyParty[monId], MON_DATA_DEF_IV, &iv);
            iv = battlePokemon->speedIV;
            SetMonData(&gEnemyParty[monId], MON_DATA_SPEED_IV, &iv);
            iv = battlePokemon->spAttackIV;
            SetMonData(&gEnemyParty[monId], MON_DATA_SPATK_IV, &iv);
            iv = battlePokemon->spDefenseIV;
            SetMonData(&gEnemyParty[monId], MON_DATA_SPDEF_IV, &iv);
            SetMonData(&gEnemyParty[monId], MON_DATA_PERSONALITY, &battlePokemon->personality);
            SetMonData(&gEnemyParty[monId], MON_DATA_STATUS, &battlePokemon->status1);
            SetMonData(&gEnemyParty[monId], MON_DATA_LEVEL, &battlePokemon->level);
            SetMonData(&gEnemyParty[monId], MON_DATA_HP, &battlePokemon->hp);
            SetMonData(&gEnemyParty[monId], MON_DATA_MAX_HP, &battlePokemon->maxHP);
            SetMonData(&gEnemyParty[monId], MON_DATA_ATK, &battlePokemon->attack);
            SetMonData(&gEnemyParty[monId], MON_DATA_DEF, &battlePokemon->defense);
            SetMonData(&gEnemyParty[monId], MON_DATA_SPEED, &battlePokemon->speed);
            SetMonData(&gEnemyParty[monId], MON_DATA_SPATK, &battlePokemon->spAttack);
            SetMonData(&gEnemyParty[monId], MON_DATA_SPDEF, &battlePokemon->spDefense);
        }
        break;
    case REQUEST_SPECIES_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_SPECIES, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_HELDITEM_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_HELD_ITEM, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_MOVES_PP_BATTLE:
        for (i = 0; i < 4; i++)
        {
            SetMonData(&gEnemyParty[monId], MON_DATA_MOVE1 + i, &moveData->moves[i]);
            SetMonData(&gEnemyParty[monId], MON_DATA_PP1 + i, &moveData->pp[i]);
        }
        SetMonData(&gEnemyParty[monId], MON_DATA_PP_BONUSES, &moveData->ppBonuses);
        break;
    case REQUEST_MOVE1_BATTLE:
    case REQUEST_MOVE2_BATTLE:
    case REQUEST_MOVE3_BATTLE:
    case REQUEST_MOVE4_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_MOVE1 + gBattleBufferA[gActiveBattler][1] - REQUEST_MOVE1_BATTLE, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_PP_DATA_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_PP1, &gBattleBufferA[gActiveBattler][3]);
        SetMonData(&gEnemyParty[monId], MON_DATA_PP2, &gBattleBufferA[gActiveBattler][4]);
        SetMonData(&gEnemyParty[monId], MON_DATA_PP3, &gBattleBufferA[gActiveBattler][5]);
        SetMonData(&gEnemyParty[monId], MON_DATA_PP4, &gBattleBufferA[gActiveBattler][6]);
        SetMonData(&gEnemyParty[monId], MON_DATA_PP_BONUSES, &gBattleBufferA[gActiveBattler][7]);
        break;
    case REQUEST_PPMOVE1_BATTLE:
    case REQUEST_PPMOVE2_BATTLE:
    case REQUEST_PPMOVE3_BATTLE:
    case REQUEST_PPMOVE4_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_PP1 + gBattleBufferA[gActiveBattler][1] - REQUEST_PPMOVE1_BATTLE, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_OTID_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_OT_ID, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_EXP_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_EXP, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_HP_EV_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_HP_EV, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_ATK_EV_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_ATK_EV, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_DEF_EV_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_DEF_EV, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_SPEED_EV_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_SPEED_EV, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_SPATK_EV_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_SPATK_EV, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_SPDEF_EV_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_SPDEF_EV, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_FRIENDSHIP_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_FRIENDSHIP, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_POKERUS_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_POKERUS, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_MET_LOCATION_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_MET_LOCATION, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_MET_LEVEL_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_MET_LEVEL, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_MET_GAME_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_MET_GAME, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_POKEBALL_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_POKEBALL, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_ALL_IVS_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_HP_IV, &gBattleBufferA[gActiveBattler][3]);
        SetMonData(&gEnemyParty[monId], MON_DATA_ATK_IV, &gBattleBufferA[gActiveBattler][4]);
        SetMonData(&gEnemyParty[monId], MON_DATA_DEF_IV, &gBattleBufferA[gActiveBattler][5]);
        SetMonData(&gEnemyParty[monId], MON_DATA_SPEED_IV, &gBattleBufferA[gActiveBattler][6]);
        SetMonData(&gEnemyParty[monId], MON_DATA_SPATK_IV, &gBattleBufferA[gActiveBattler][7]);
        SetMonData(&gEnemyParty[monId], MON_DATA_SPDEF_IV, &gBattleBufferA[gActiveBattler][8]);
        break;
    case REQUEST_HP_IV_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_HP_IV, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_ATK_IV_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_ATK_IV, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_DEF_IV_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_DEF_IV, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_SPEED_IV_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_SPEED_IV, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_SPATK_IV_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_SPATK_IV, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_SPDEF_IV_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_SPDEF_IV, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_PERSONALITY_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_PERSONALITY, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_CHECKSUM_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_CHECKSUM, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_STATUS_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_STATUS, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_LEVEL_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_LEVEL, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_HP_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_HP, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_MAX_HP_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_MAX_HP, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_ATK_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_ATK, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_DEF_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_DEF, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_SPEED_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_SPEED, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_SPATK_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_SPATK, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_SPDEF_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_SPDEF, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_COOL_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_COOL, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_BEAUTY_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_BEAUTY, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_CUTE_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_CUTE, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_SMART_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_SMART, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_TOUGH_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_TOUGH, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_SHEEN_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_SHEEN, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_COOL_RIBBON_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_COOL_RIBBON, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_BEAUTY_RIBBON_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_BEAUTY_RIBBON, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_CUTE_RIBBON_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_CUTE_RIBBON, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_SMART_RIBBON_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_SMART_RIBBON, &gBattleBufferA[gActiveBattler][3]);
        break;
    case REQUEST_TOUGH_RIBBON_BATTLE:
        SetMonData(&gEnemyParty[monId], MON_DATA_TOUGH_RIBBON, &gBattleBufferA[gActiveBattler][3]);
        break;
    }
}

static void LinkOpponentHandleSetRawMonData(void)
{
    u8 *dst = (u8 *)&gEnemyParty[gBattlerPartyIndexes[gActiveBattler]] + gBattleBufferA[gActiveBattler][1];
    u8 i;

    for (i = 0; i < gBattleBufferA[gActiveBattler][2]; i++)
        dst[i] = gBattleBufferA[gActiveBattler][3 + i];

    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleLoadMonSprite(void)
{
    u16 species = GetMonData(&gEnemyParty[gBattlerPartyIndexes[gActiveBattler]], MON_DATA_SPECIES);

    BattleLoadOpponentMonSpriteGfx(&gEnemyParty[gBattlerPartyIndexes[gActiveBattler]], gActiveBattler);
    sub_806A068(species, GetBattlerPosition(gActiveBattler));

    gBattlerSpriteIds[gActiveBattler] = CreateSprite(&gUnknown_0202499C,
                                               GetBattlerSpriteCoord(gActiveBattler, 2),
                                               GetBattlerSpriteDefault_Y(gActiveBattler),
                                               sub_80A82E4(gActiveBattler));

    gSprites[gBattlerSpriteIds[gActiveBattler]].pos2.x = -240;
    gSprites[gBattlerSpriteIds[gActiveBattler]].data[0] = gActiveBattler;
    gSprites[gBattlerSpriteIds[gActiveBattler]].oam.paletteNum = gActiveBattler;
    StartSpriteAnim(&gSprites[gBattlerSpriteIds[gActiveBattler]], gBattleMonForms[gActiveBattler]);

    SetBattlerShadowSpriteCallback(gActiveBattler, GetMonData(&gEnemyParty[gBattlerPartyIndexes[gActiveBattler]], MON_DATA_SPECIES));

    gBattlerControllerFuncs[gActiveBattler] = sub_8064B04;
}

static void LinkOpponentHandleSwitchInAnim(void)
{
    gBattlerPartyIndexes[gActiveBattler] = gBattleBufferA[gActiveBattler][1];
    sub_8066494(gActiveBattler, gBattleBufferA[gActiveBattler][2]);
    gBattlerControllerFuncs[gActiveBattler] = sub_8064F40;
}

static void sub_8066494(u8 bank, bool8 dontClearSubstituteBit)
{
    u16 species;

    ClearTemporarySpeciesSpriteData(bank, dontClearSubstituteBit);
    gBattlerPartyIndexes[bank] = gBattleBufferA[bank][1];
    species = GetMonData(&gEnemyParty[gBattlerPartyIndexes[bank]], MON_DATA_SPECIES);
    gUnknown_03005D7C[bank] = CreateInvisibleSpriteWithCallback(sub_805D714);
    BattleLoadOpponentMonSpriteGfx(&gEnemyParty[gBattlerPartyIndexes[bank]], bank);
    sub_806A068(species, GetBattlerPosition(bank));

    gBattlerSpriteIds[bank] = CreateSprite(
      &gUnknown_0202499C,
      GetBattlerSpriteCoord(bank, 2),
      GetBattlerSpriteDefault_Y(bank),
      sub_80A82E4(bank));

    gSprites[gUnknown_03005D7C[bank]].data[1] = gBattlerSpriteIds[bank];
    gSprites[gUnknown_03005D7C[bank]].data[2] = bank;

    gSprites[gBattlerSpriteIds[bank]].data[0] = bank;
    gSprites[gBattlerSpriteIds[bank]].data[2] = species;
    gSprites[gBattlerSpriteIds[bank]].oam.paletteNum = bank;

    StartSpriteAnim(&gSprites[gBattlerSpriteIds[bank]], gBattleMonForms[bank]);

    gSprites[gBattlerSpriteIds[bank]].invisible = TRUE;
    gSprites[gBattlerSpriteIds[bank]].callback = SpriteCallbackDummy;

    gSprites[gUnknown_03005D7C[bank]].data[0] = DoPokeballSendOutAnimation(0, POKEBALL_OPPONENT_SENDOUT);
}

static void LinkOpponentHandleReturnMonToBall(void)
{
    if (gBattleBufferA[gActiveBattler][1] == 0)
    {
        gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].animationState = 0;
        gBattlerControllerFuncs[gActiveBattler] = DoSwitchOutAnimation;
    }
    else
    {
        FreeSpriteOamMatrix(&gSprites[gBattlerSpriteIds[gActiveBattler]]);
        DestroySprite(&gSprites[gBattlerSpriteIds[gActiveBattler]]);
        HideBattlerShadowSprite(gActiveBattler);
        SetHealthboxSpriteInvisible(gHealthboxSpriteIds[gActiveBattler]);
        LinkOpponentBufferExecCompleted();
    }
}

static void DoSwitchOutAnimation(void)
{
    switch (gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].animationState)
    {
    case 0:
        if (gBattleSpritesDataPtr->battlerData[gActiveBattler].behindSubstitute)
            InitAndLaunchSpecialAnimation(gActiveBattler, gActiveBattler, gActiveBattler, B_ANIM_SUBSTITUTE_TO_MON);

        gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].animationState = 1;
        break;
    case 1:
        if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].specialAnimActive)
        {
            gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].animationState = 0;
            InitAndLaunchSpecialAnimation(gActiveBattler, gActiveBattler, gActiveBattler, B_ANIM_SWITCH_OUT_OPPONENT_MON);
            gBattlerControllerFuncs[gActiveBattler] = sub_8064C58;
        }
        break;
    }
}

static void LinkOpponentHandleDrawTrainerPic(void)
{
    s16 xPos;
    u32 trainerPicId;

    if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
    {
        if ((GetBattlerPosition(gActiveBattler) & BIT_FLANK) != 0) // second mon
            xPos = 152;
        else // first mon
            xPos = 200;

        if (gBattleTypeFlags & BATTLE_TYPE_BATTLE_TOWER)
        {
            if (gActiveBattler == B_POSITION_OPPONENT_LEFT)
                trainerPicId = GetFrontierTrainerFrontSpriteId(gTrainerBattleOpponent_A);
            else
                trainerPicId = GetFrontierTrainerFrontSpriteId(gTrainerBattleOpponent_B);
        }
        else
        {
            if ((gLinkPlayers[GetBattlerMultiplayerId(gActiveBattler)].version & 0xFF) == VERSION_FIRE_RED
            || (gLinkPlayers[GetBattlerMultiplayerId(gActiveBattler)].version & 0xFF) == VERSION_LEAF_GREEN)
            {
                if (gLinkPlayers[GetBattlerMultiplayerId(gActiveBattler)].gender != MALE)
                    trainerPicId = gFacilityClassToPicIndex[FACILITY_CLASS_PKMN_TRAINER_LEAF];
                else
                    trainerPicId = gFacilityClassToPicIndex[FACILITY_CLASS_PKMN_TRAINER_RED];
            }
            else if ((gLinkPlayers[GetBattlerMultiplayerId(gActiveBattler)].version & 0xFF) == VERSION_RUBY
                     || (gLinkPlayers[GetBattlerMultiplayerId(gActiveBattler)].version & 0xFF) == VERSION_SAPPHIRE)
            {
                if (gLinkPlayers[GetBattlerMultiplayerId(gActiveBattler)].gender != MALE)
                    trainerPicId = gFacilityClassToPicIndex[FACILITY_CLASS_PKMN_TRAINER_RS_MAY];
                else
                    trainerPicId = gFacilityClassToPicIndex[FACILITY_CLASS_PKMN_TRAINER_RS_BRENDAN];
            }
            else
            {
                trainerPicId = PlayerGenderToFrontTrainerPicId(gLinkPlayers[GetBattlerMultiplayerId(gActiveBattler)].gender);
            }
        }
    }
    else
    {
        xPos = 176;
        if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
        {
            trainerPicId = sub_8068B48();
        }
        else if ((gLinkPlayers[GetMultiplayerId() ^ BIT_SIDE].version & 0xFF) == VERSION_FIRE_RED
                 || (gLinkPlayers[GetMultiplayerId() ^ BIT_SIDE].version & 0xFF) == VERSION_LEAF_GREEN)
        {
            if (gLinkPlayers[GetMultiplayerId() ^ BIT_SIDE].gender != 0)
                trainerPicId = gFacilityClassToPicIndex[FACILITY_CLASS_PKMN_TRAINER_LEAF];
            else
                trainerPicId = gFacilityClassToPicIndex[FACILITY_CLASS_PKMN_TRAINER_RED];
        }
        else if ((gLinkPlayers[GetMultiplayerId() ^ BIT_SIDE].version & 0xFF) == VERSION_RUBY
                 || (gLinkPlayers[GetMultiplayerId() ^ BIT_SIDE].version & 0xFF) == VERSION_SAPPHIRE)
        {
            if (gLinkPlayers[GetMultiplayerId() ^ BIT_SIDE].gender != 0)
                trainerPicId = gFacilityClassToPicIndex[FACILITY_CLASS_PKMN_TRAINER_RS_MAY];
            else
                trainerPicId = gFacilityClassToPicIndex[FACILITY_CLASS_PKMN_TRAINER_RS_BRENDAN];
        }
        else
        {
            trainerPicId = PlayerGenderToFrontTrainerPicId(gLinkPlayers[GetMultiplayerId() ^ BIT_SIDE].gender);
        }
    }

    DecompressTrainerFrontPic(trainerPicId, gActiveBattler);
    sub_806A12C(trainerPicId, GetBattlerPosition(gActiveBattler));
    gBattlerSpriteIds[gActiveBattler] = CreateSprite(&gUnknown_0202499C,
                                               xPos,
                                               (8 - gTrainerFrontPicCoords[trainerPicId].coords) * 4 + 40,
                                               sub_80A82E4(gActiveBattler));

    gSprites[gBattlerSpriteIds[gActiveBattler]].pos2.x = -240;
    gSprites[gBattlerSpriteIds[gActiveBattler]].data[0] = 2;
    gSprites[gBattlerSpriteIds[gActiveBattler]].oam.paletteNum = IndexOfSpritePaletteTag(gTrainerFrontPicPaletteTable[trainerPicId].tag);
    gSprites[gBattlerSpriteIds[gActiveBattler]].oam.affineParam = trainerPicId;
    gSprites[gBattlerSpriteIds[gActiveBattler]].callback = sub_805D7AC;

    gBattlerControllerFuncs[gActiveBattler] = CompleteOnBankSpriteCallbackDummy;
}

static void LinkOpponentHandleTrainerSlide(void)
{
    u32 trainerPicId;

    if (gActiveBattler == 1)
        trainerPicId = GetFrontierTrainerFrontSpriteId(gTrainerBattleOpponent_A);
    else
        trainerPicId = GetFrontierTrainerFrontSpriteId(gTrainerBattleOpponent_B);

    DecompressTrainerFrontPic(trainerPicId, gActiveBattler);
    sub_806A12C(trainerPicId, GetBattlerPosition(gActiveBattler));
    gBattlerSpriteIds[gActiveBattler] = CreateSprite(&gUnknown_0202499C, 176, (8 - gTrainerFrontPicCoords[trainerPicId].coords) * 4 + 40, 0x1E);

    gSprites[gBattlerSpriteIds[gActiveBattler]].pos2.x = 96;
    gSprites[gBattlerSpriteIds[gActiveBattler]].pos1.x += 32;
    gSprites[gBattlerSpriteIds[gActiveBattler]].data[0] = -2;
    gSprites[gBattlerSpriteIds[gActiveBattler]].oam.paletteNum = IndexOfSpritePaletteTag(gTrainerFrontPicPaletteTable[trainerPicId].tag);
    gSprites[gBattlerSpriteIds[gActiveBattler]].oam.affineParam = trainerPicId;
    gSprites[gBattlerSpriteIds[gActiveBattler]].callback = sub_805D7AC;

    gBattlerControllerFuncs[gActiveBattler] = CompleteOnBankSpriteCallbackDummy2; // this line is redundant, because LinkOpponentBufferExecCompleted changes the battle bank function
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleTrainerSlideBack(void)
{
    oamt_add_pos2_onto_pos1(&gSprites[gBattlerSpriteIds[gActiveBattler]]);
    gSprites[gBattlerSpriteIds[gActiveBattler]].data[0] = 35;
    gSprites[gBattlerSpriteIds[gActiveBattler]].data[2] = 280;
    gSprites[gBattlerSpriteIds[gActiveBattler]].data[4] = gSprites[gBattlerSpriteIds[gActiveBattler]].pos1.y;
    gSprites[gBattlerSpriteIds[gActiveBattler]].callback = sub_80A6EEC;
    StoreSpriteCallbackInData6(&gSprites[gBattlerSpriteIds[gActiveBattler]], SpriteCallbackDummy);
    gBattlerControllerFuncs[gActiveBattler] = sub_8064470;
}

static void LinkOpponentHandleFaintAnimation(void)
{
    if (gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].animationState == 0)
    {
        if (gBattleSpritesDataPtr->battlerData[gActiveBattler].behindSubstitute)
            InitAndLaunchSpecialAnimation(gActiveBattler, gActiveBattler, gActiveBattler, B_ANIM_SUBSTITUTE_TO_MON);
        gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].animationState++;
    }
    else
    {
        if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].specialAnimActive)
        {
            gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].animationState = 0;
            PlaySE12WithPanning(SE_POKE_DEAD, PAN_SIDE_OPPONENT);
            gSprites[gBattlerSpriteIds[gActiveBattler]].callback = sub_8039934;
            gBattlerControllerFuncs[gActiveBattler] = sub_8064C14;
        }
    }
}

static void LinkOpponentHandlePaletteFade(void)
{
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleSuccessBallThrowAnim(void)
{
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleBallThrowAnim(void)
{
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandlePause(void)
{
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleMoveAnimation(void)
{
    if (!mplay_80342A4(gActiveBattler))
    {
        u16 move = gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8);

        gAnimMoveTurn = gBattleBufferA[gActiveBattler][3];
        gAnimMovePower = gBattleBufferA[gActiveBattler][4] | (gBattleBufferA[gActiveBattler][5] << 8);
        gAnimMoveDmg = gBattleBufferA[gActiveBattler][6] | (gBattleBufferA[gActiveBattler][7] << 8) | (gBattleBufferA[gActiveBattler][8] << 16) | (gBattleBufferA[gActiveBattler][9] << 24);
        gAnimFriendship = gBattleBufferA[gActiveBattler][10];
        gWeatherMoveAnim = gBattleBufferA[gActiveBattler][12] | (gBattleBufferA[gActiveBattler][13] << 8);
        gAnimDisableStructPtr = (struct DisableStruct *)&gBattleBufferA[gActiveBattler][16];
        gTransformedPersonalities[gActiveBattler] = gAnimDisableStructPtr->transformedMonPersonality;
        if (IsMoveWithoutAnimation(move, gAnimMoveTurn)) // always returns FALSE
        {
            LinkOpponentBufferExecCompleted();
        }
        else
        {
            gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].animationState = 0;
            gBattlerControllerFuncs[gActiveBattler] = LinkOpponentDoMoveAnimation;
            BattleTv_SetDataBasedOnMove(move, gWeatherMoveAnim, gAnimDisableStructPtr);
        }
    }
}

static void LinkOpponentDoMoveAnimation(void)
{
    u16 move = gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8);
    u8 multihit = gBattleBufferA[gActiveBattler][11];

    switch (gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].animationState)
    {
    case 0:
        if (gBattleSpritesDataPtr->battlerData[gActiveBattler].behindSubstitute
            && !gBattleSpritesDataPtr->battlerData[gActiveBattler].flag_x8)
        {
            gBattleSpritesDataPtr->battlerData[gActiveBattler].flag_x8 = 1;
            InitAndLaunchSpecialAnimation(gActiveBattler, gActiveBattler, gActiveBattler, B_ANIM_SUBSTITUTE_TO_MON);
        }
        gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].animationState = 1;
        break;
    case 1:
        if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].specialAnimActive)
        {
            sub_805EB9C(0);
            DoMoveAnim(move);
            gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].animationState = 2;
        }
        break;
    case 2:
        gAnimScriptCallback();
        if (!gAnimScriptActive)
        {
            sub_805EB9C(1);
            if (gBattleSpritesDataPtr->battlerData[gActiveBattler].behindSubstitute && multihit < 2)
            {
                InitAndLaunchSpecialAnimation(gActiveBattler, gActiveBattler, gActiveBattler, B_ANIM_MON_TO_SUBSTITUTE);
                gBattleSpritesDataPtr->battlerData[gActiveBattler].flag_x8 = 0;
            }
            gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].animationState = 3;
        }
        break;
    case 3:
        if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].specialAnimActive)
        {
            CopyAllBattleSpritesInvisibilities();
            TrySetBehindSubstituteSpriteBit(gActiveBattler, gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8));
            gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].animationState = 0;
            LinkOpponentBufferExecCompleted();
        }
        break;
    }
}

static void LinkOpponentHandlePrintString(void)
{
    u16 *stringId;

    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    stringId = (u16*)(&gBattleBufferA[gActiveBattler][2]);
    BufferStringBattle(*stringId);
    BattleHandleAddTextPrinter(gDisplayedStringBattle, 0);
    gBattlerControllerFuncs[gActiveBattler] = CompleteOnInactiveTextPrinter;
    BattleTv_SetDataBasedOnString(*stringId);
}

static void LinkOpponentHandlePrintSelectionString(void)
{
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleChooseAction(void)
{
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleUnknownYesNoBox(void)
{
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleChooseMove(void)
{
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleChooseItem(void)
{
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleChoosePokemon(void)
{
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleCmd23(void)
{
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleHealthBarUpdate(void)
{
    s16 hpVal;

    LoadBattleBarGfx(0);
    hpVal = gBattleBufferA[gActiveBattler][2] | (gBattleBufferA[gActiveBattler][3] << 8);

    if (hpVal != INSTANT_HP_BAR_DROP)
    {
        u32 maxHP = GetMonData(&gEnemyParty[gBattlerPartyIndexes[gActiveBattler]], MON_DATA_MAX_HP);
        u32 curHP = GetMonData(&gEnemyParty[gBattlerPartyIndexes[gActiveBattler]], MON_DATA_HP);

        SetBattleBarStruct(gActiveBattler, gHealthboxSpriteIds[gActiveBattler], maxHP, curHP, hpVal);
    }
    else
    {
        u32 maxHP = GetMonData(&gEnemyParty[gBattlerPartyIndexes[gActiveBattler]], MON_DATA_MAX_HP);

        SetBattleBarStruct(gActiveBattler, gHealthboxSpriteIds[gActiveBattler], maxHP, 0, hpVal);
    }

    gBattlerControllerFuncs[gActiveBattler] = CompleteOnHealthbarDone;
}

static void LinkOpponentHandleExpUpdate(void)
{
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleStatusIconUpdate(void)
{
    if (!mplay_80342A4(gActiveBattler))
    {
        u8 bank;

        UpdateHealthboxAttribute(gHealthboxSpriteIds[gActiveBattler], &gEnemyParty[gBattlerPartyIndexes[gActiveBattler]], HEALTHBOX_STATUS_ICON);
        bank = gActiveBattler;
        gBattleSpritesDataPtr->healthBoxesData[bank].statusAnimActive = 0;
        gBattlerControllerFuncs[gActiveBattler] = CompleteOnFinishedStatusAnimation;
    }
}

static void LinkOpponentHandleStatusAnimation(void)
{
    if (!mplay_80342A4(gActiveBattler))
    {
        InitAndLaunchChosenStatusAnimation(gBattleBufferA[gActiveBattler][1],
                        gBattleBufferA[gActiveBattler][2] | (gBattleBufferA[gActiveBattler][3] << 8) | (gBattleBufferA[gActiveBattler][4] << 16) | (gBattleBufferA[gActiveBattler][5] << 24));
        gBattlerControllerFuncs[gActiveBattler] = CompleteOnFinishedStatusAnimation;
    }
}

static void LinkOpponentHandleStatusXor(void)
{
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleDataTransfer(void)
{
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleDMA3Transfer(void)
{
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandlePlayBGM(void)
{
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleCmd32(void)
{
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleTwoReturnValues(void)
{
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleChosenMonReturnValue(void)
{
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleOneReturnValue(void)
{
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleOneReturnValue_Duplicate(void)
{
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleCmd37(void)
{
    gUnknown_02022D0C.field_0 = 0;
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleCmd38(void)
{
    gUnknown_02022D0C.field_0 = gBattleBufferA[gActiveBattler][1];
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleCmd39(void)
{
    gUnknown_02022D0C.flag_x80 = 0;
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleCmd40(void)
{
    gUnknown_02022D0C.flag_x80 ^= 1;
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleHitAnimation(void)
{
    if (gSprites[gBattlerSpriteIds[gActiveBattler]].invisible == TRUE)
    {
        LinkOpponentBufferExecCompleted();
    }
    else
    {
        gDoingBattleAnim = TRUE;
        gSprites[gBattlerSpriteIds[gActiveBattler]].data[1] = 0;
        DoHitAnimHealthboxEffect(gActiveBattler);
        gBattlerControllerFuncs[gActiveBattler] = DoHitAnimBlinkSpriteEffect;
    }
}

static void LinkOpponentHandleCmd42(void)
{
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandlePlaySE(void)
{
    s8 pan;

    if (GetBattlerSide(gActiveBattler) == B_SIDE_PLAYER)
        pan = PAN_SIDE_PLAYER;
    else
        pan = PAN_SIDE_OPPONENT;

    PlaySE12WithPanning(gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8), pan);
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandlePlayFanfareOrBGM(void)
{
    if (gBattleBufferA[gActiveBattler][3])
    {
        BattleStopLowHpSound();
        PlayBGM(gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8));
    }
    else
    {
        PlayFanfare(gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8));
    }

    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleFaintingCry(void)
{
    u16 species = GetMonData(&gEnemyParty[gBattlerPartyIndexes[gActiveBattler]], MON_DATA_SPECIES);

    PlayCry3(species, 25, 5);
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleIntroSlide(void)
{
    HandleIntroSlide(gBattleBufferA[gActiveBattler][1]);
    gIntroSlideFlags |= 1;
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleIntroTrainerBallThrow(void)
{
    u8 paletteNum;
    u8 taskId;

    oamt_add_pos2_onto_pos1(&gSprites[gBattlerSpriteIds[gActiveBattler]]);

    gSprites[gBattlerSpriteIds[gActiveBattler]].data[0] = 35;
    gSprites[gBattlerSpriteIds[gActiveBattler]].data[2] = 280;
    gSprites[gBattlerSpriteIds[gActiveBattler]].data[4] = gSprites[gBattlerSpriteIds[gActiveBattler]].pos1.y;
    gSprites[gBattlerSpriteIds[gActiveBattler]].callback = sub_80A6EEC;

    StoreSpriteCallbackInData6(&gSprites[gBattlerSpriteIds[gActiveBattler]], sub_80676FC);

    taskId = CreateTask(sub_8067618, 5);
    gTasks[taskId].data[0] = gActiveBattler;

    if (gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].flag_x1)
        gTasks[gBattlerStatusSummaryTaskId[gActiveBattler]].func = sub_8073C30;

    gBattleSpritesDataPtr->animationData->field_9_x1 = 1;
    gBattlerControllerFuncs[gActiveBattler] = nullsub_28;
}

static void sub_8067618(u8 taskId)
{
    u8 savedActiveBank = gActiveBattler;

    gActiveBattler = gTasks[taskId].data[0];
    if (!IsDoubleBattle() || (gBattleTypeFlags & BATTLE_TYPE_MULTI))
    {
        gBattleBufferA[gActiveBattler][1] = gBattlerPartyIndexes[gActiveBattler];
        sub_8066494(gActiveBattler, FALSE);
    }
    else
    {
        gBattleBufferA[gActiveBattler][1] = gBattlerPartyIndexes[gActiveBattler];
        sub_8066494(gActiveBattler, FALSE);
        gActiveBattler = BATTLE_PARTNER(gActiveBattler);
        gBattleBufferA[gActiveBattler][1] = gBattlerPartyIndexes[gActiveBattler];
        sub_8066494(gActiveBattler, FALSE);
        gActiveBattler = BATTLE_PARTNER(gActiveBattler);
    }
    gBattlerControllerFuncs[gActiveBattler] = sub_8064734;
    gActiveBattler = savedActiveBank;
    DestroyTask(taskId);
}

static void sub_80676FC(struct Sprite *sprite)
{
    FreeTrainerFrontPicPalette(sprite->oam.affineParam);
    FreeSpriteOamMatrix(sprite);
    DestroySprite(sprite);
}

static void LinkOpponentHandleDrawPartyStatusSummary(void)
{
    if (gBattleBufferA[gActiveBattler][1] != 0 && GetBattlerSide(gActiveBattler) == B_SIDE_PLAYER)
    {
        LinkOpponentBufferExecCompleted();
    }
    else
    {
        gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].flag_x1 = 1;

        if (gBattleBufferA[gActiveBattler][2] != 0)
        {
            if (gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_1_x1E < 2)
            {
                gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_1_x1E++;
                return;
            }
            else
            {
                gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_1_x1E = 0;
            }
        }

        gBattlerStatusSummaryTaskId[gActiveBattler] = CreatePartyStatusSummarySprites(gActiveBattler, (struct HpAndStatus *)&gBattleBufferA[gActiveBattler][4], gBattleBufferA[gActiveBattler][1], gBattleBufferA[gActiveBattler][2]);
        gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_5 = 0;

        if (gBattleBufferA[gActiveBattler][2] != 0)
            gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_5 = 0x5D;

        gBattlerControllerFuncs[gActiveBattler] = sub_806782C;
    }
}

static void sub_806782C(void)
{
    if (gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_5++ > 0x5C)
    {
        gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].field_5 = 0;
        LinkOpponentBufferExecCompleted();
    }
}

static void LinkOpponentHandleCmd49(void)
{
    if (gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].flag_x1)
        gTasks[gBattlerStatusSummaryTaskId[gActiveBattler]].func = sub_8073C30;
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleCmd50(void)
{
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleSpriteInvisibility(void)
{
    if (IsBattlerSpritePresent(gActiveBattler))
    {
        gSprites[gBattlerSpriteIds[gActiveBattler]].invisible = gBattleBufferA[gActiveBattler][1];
        CopyBattleSpriteInvisibility(gActiveBattler);
    }
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleBattleAnimation(void)
{
    if (!mplay_80342A4(gActiveBattler))
    {
        u8 animationId = gBattleBufferA[gActiveBattler][1];
        u16 argument = gBattleBufferA[gActiveBattler][2] | (gBattleBufferA[gActiveBattler][3] << 8);

        if (TryHandleLaunchBattleTableAnimation(gActiveBattler, gActiveBattler, gActiveBattler, animationId, argument))
            LinkOpponentBufferExecCompleted();
        else
            gBattlerControllerFuncs[gActiveBattler] = CompleteOnFinishedBattleAnimation;

        BattleTv_SetDataBasedOnAnimation(animationId);
    }
}

static void LinkOpponentHandleLinkStandbyMsg(void)
{
    sub_81851A8(&gBattleBufferA[gActiveBattler][2]);
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleResetActionMoveSelection(void)
{
    LinkOpponentBufferExecCompleted();
}

static void LinkOpponentHandleCmd55(void)
{
    sub_81851A8(&gBattleBufferA[gActiveBattler][4]);

    if (gBattleBufferA[gActiveBattler][1] == B_OUTCOME_DREW)
        gBattleOutcome = gBattleBufferA[gActiveBattler][1];
    else
        gBattleOutcome = gBattleBufferA[gActiveBattler][1] ^ B_OUTCOME_DREW;

    gSaveBlock2Ptr->field_CA9_b = gBattleBufferA[gActiveBattler][2];
    FadeOutMapMusic(5);
    BeginFastPaletteFade(3);
    LinkOpponentBufferExecCompleted();
    gBattlerControllerFuncs[gActiveBattler] = sub_80587B0;
}

static void nullsub_92(void)
{
}
