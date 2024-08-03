Howdy! I made an Infinite Repel Item I thought the pokeemerald-expansion community would appreciate, and here's a guide on how to do it!

First, lets head to **_include/config/Overworld.h_** & have **_include/constants/flags.h_** open as well.

In **_Overworld.h_**, find:

```
#define OW_FLAG_NO_ENCOUNTER        0  // If this flag is set, wild encounters will be disabled.
```

Change **_0_** to an unused **_flag from flags.h._** For me, I changed it to:

```
#define OW_FLAG_NO_ENCOUNTER        0x20  // If this flag is set, wild encounters will be disabled.
```

Next, head to **_include/item_use.h_** and add the following:
```diff
void ItemUseOutOfBattle_CoinCase(u8);
+void ItemUseOutOfBattle_InfiniteRepel(u8);
void ItemUseOutOfBattle_PowderJar(u8);
```
In **_include/constants/items.h_,** add:
```diff
#define ITEM_PEAT_BLOCK 797
#define ITEM_BERSERK_GENE 798
+#define ITEM_INFINITE_REPEL 799
```
Then, change the following:
```diff
#define ITEM_INFINITE_REPEL 799

-#define ITEMS_COUNT 799
+#define ITEMS_COUNT 800
#define ITEM_FIELD_ARROW ITEMS_COUNT
```
In **_include/strings.h_,** add:
```diff
extern const u8 gText_CoinCase[];
+extern const u8 gText_InfiniteRepelOn[];
+extern const u8 gText_InfiniteRepelOff[];
extern const u8 gText_PowderQty[];
```
In **_src/strings.c_,** add:
```diff
const u8 gText_BasePointsResetToZero[] = _("{STR_VAR_1}'s base points\nwere all reset to zero!{PAUSE_UNTIL_PRESS}");
+const u8 gText_InfiniteRepelOn[] = _("The Infinite Repel is now ON!");
+const u8 gText_InfiniteRepelOff[] = _("The Infinite Repel is now OFF!");
```
In **_src/data/item_icon_table.h,_** add:
```diff
    [ITEM_BERSERK_GENE] = {gItemIcon_BerserkGene, gItemIconPalette_BerserkGene},
+    [ITEM_INFINITE_REPEL] = {gItemIcon_Repel, gItemIconPalette_MaxRepel},
    // Return to field arrow
    [ITEMS_COUNT] = {gItemIcon_ReturnToFieldArrow, gItemIconPalette_ReturnToFieldArrow},
```
In **_src/data/items.h_,** add at the bottom:
```diff
[ITEM_BERSERK_GENE] =
    {
        .name = _("Berserk Gene"),
        .price = 20,
        .holdEffect = HOLD_EFFECT_BERSERK_GENE,
        .description = sBerserkGene,
        .pocket = POCKET_ITEMS,
        .type = ITEM_USE_BAG_MENU,
        .fieldUseFunc = ItemUseOutOfBattle_CannotUse,
        .flingPower = 10,
    },

+    [ITEM_INFINITE_REPEL] =
+    {
+        .name = _("InfiniteRepel"),
+        .price = 0,
+        .description = sInfiniteRepelDesc,
+        .importance = 1,
+        .pocket = POCKET_KEY_ITEMS,
+        .type = ITEM_USE_BAG_MENU,
+        .fieldUseFunc = ItemUseOutOfBattle_InfiniteRepel,
+    },
+};
```
In **_src/data/text/item_descriptions.h_,** add:
```diff
static const u8 sCoinCaseDesc[] = _(
    "A case that holds\n"
    "up to 9,999 Coins.");

+static const u8 sInfiniteRepelDesc[] = _(
+    "Repels all wild\n"
+    "Pokémon for any\n"
+    "number of steps.");

static const u8 sPowderJarDesc[] = _(
    "Stores Berry\n"
    "Powder made using\n"
    "a Berry Crusher.");
```

Final step! go to **_src/item_use.c_,** and add:
```diff
void ItemUseOutOfBattle_CoinCase(u8 taskId)
{
    ConvertIntToDecimalStringN(gStringVar1, GetCoins(), STR_CONV_MODE_LEFT_ALIGN, 4);
    StringExpandPlaceholders(gStringVar4, gText_CoinCase);

    if (!gTasks[taskId].tUsingRegisteredKeyItem)
    {
        DisplayItemMessage(taskId, FONT_NORMAL, gStringVar4, CloseItemMessage);
    }
    else
    {
        DisplayItemMessageOnField(taskId, gStringVar4, Task_CloseCantUseKeyItemMessage);
    }
}

+void ItemUseOutOfBattle_InfiniteRepel(u8 taskId)
+{
+    bool8 infiniteRepelOn = FlagGet(OW_FLAG_NO_ENCOUNTER);
+    if (!infiniteRepelOn)
+    {
+        FlagToggle(OW_FLAG_NO_ENCOUNTER);
+        PlaySE(SE_REPEL);
+        if(gTasks[taskId].tUsingRegisteredKeyItem){
+            DisplayItemMessageOnField(taskId, gText_InfiniteRepelOn, Task_CloseCantUseKeyItemMessage);
+        }
+        else{
+            DisplayItemMessage(taskId, 1, gText_InfiniteRepelOn, CloseItemMessage);
+        }
+    }
+    else
+    {
+        FlagToggle(OW_FLAG_NO_ENCOUNTER);
+        PlaySE(SE_PC_OFF);
+        if (gTasks[taskId].tUsingRegisteredKeyItem){
+            DisplayItemMessageOnField(taskId, gText_InfiniteRepelOff, Task_CloseCantUseKeyItemMessage);
+        }
+        else{
+            DisplayItemMessage(taskId, 1, gText_InfiniteRepelOn, CloseItemMessage);
+        }
+    }
+}

void ItemUseOutOfBattle_PowderJar(u8 taskId)
```
Now, you can add the item through scripting or via debug menu. Works through Bag => Use or Register => Select!
