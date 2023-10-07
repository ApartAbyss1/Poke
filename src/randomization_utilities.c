#include "global.h"
#include "randomization_utilities.h"
#include "pokemon.h"
#include "random.h"
#include "constants/map_groups.h"
#include "constants/pokemon.h"
#include "constants/species.h"

#define NUM_ENCOUNTER_RANDOMIZATION_TRIES       50

#define MIN_ENCOUNTER_LVL_NON_EVOLVERS          22
#define MIN_ENCOUNTER_LVL_FRIENDSHIP_EVOLVERS   28
#define MIN_ENCOUNTER_LVL_TRADE_EVOLVERS        38
#define MIN_ENCOUNTER_LVL_ITEM_EVOLVERS         32

#define MAX_LEVEL_OVER_EVOLUTION_LEVEL          7

extern struct Evolution gEvolutionTable[][EVOS_PER_MON];

enum { // stolen from wild_encounter.c - find better solution than defining it here a 2nd time...
    WILD_AREA_LAND,
    WILD_AREA_WATER,
    WILD_AREA_ROCKS,
    WILD_AREA_FISHING,
};

// This is the least efficient of the tests and should therefore be done last.
static bool8 DoesSpeciesMatchLevel(u16 species, u8 level)
{
    u16 j;
    u8 i, k;

    for (i = 0; i < EVOS_PER_MON; i++)
    {
        switch (gEvolutionTable[species][i].method)
        {
        // case already evolved:
        case 0:
            if ((i == 0) && (level < MIN_ENCOUNTER_LVL_NON_EVOLVERS))
            {
                return FALSE;
            }
            break;
        
        // case evolves via level:
        case EVO_LEVEL:
        case EVO_LEVEL_ATK_GT_DEF:
        case EVO_LEVEL_ATK_EQ_DEF:
        case EVO_LEVEL_ATK_LT_DEF:
        case EVO_LEVEL_SILCOON:
        case EVO_LEVEL_CASCOON:
        case EVO_LEVEL_NINJASK:
        case EVO_LEVEL_SHEDINJA:
        case EVO_LEVEL_FEMALE:
        case EVO_LEVEL_MALE:
        case EVO_LEVEL_NIGHT:
        case EVO_LEVEL_DAY:
        case EVO_LEVEL_DUSK:
        case EVO_LEVEL_RAIN:
        case EVO_LEVEL_DARK_TYPE_MON_IN_PARTY:
        case EVO_LEVEL_NATURE_AMPED:
        case EVO_LEVEL_NATURE_LOW_KEY:
        case EVO_LEVEL_FOG:
            if (gEvolutionTable[species][i].param < level + MAX_LEVEL_OVER_EVOLUTION_LEVEL)
            {
                return FALSE;
            }
            break;
        }
    }

    // check if previous evolution aready evolved at this level:
    for (j=0; j<NUM_SPECIES; j++)
    {
        for (i=0; i<EVOS_PER_MON; i++)
        {
            if (gEvolutionTable[j][i].targetSpecies == species)
            {
                switch (gEvolutionTable[j][i].method)
                {
                // case evolves via level:
                case EVO_LEVEL:
                case EVO_LEVEL_ATK_GT_DEF:
                case EVO_LEVEL_ATK_EQ_DEF:
                case EVO_LEVEL_ATK_LT_DEF:
                case EVO_LEVEL_SILCOON:
                case EVO_LEVEL_CASCOON:
                case EVO_LEVEL_NINJASK:
                case EVO_LEVEL_SHEDINJA:
                case EVO_LEVEL_FEMALE:
                case EVO_LEVEL_MALE:
                case EVO_LEVEL_NIGHT:
                case EVO_LEVEL_DAY:
                case EVO_LEVEL_DUSK:
                case EVO_LEVEL_RAIN:
                case EVO_LEVEL_DARK_TYPE_MON_IN_PARTY:
                case EVO_LEVEL_NATURE_AMPED:
                case EVO_LEVEL_NATURE_LOW_KEY:
                case EVO_LEVEL_FOG:
                    if (gEvolutionTable[j][i].param <= level)
                    {
                        return TRUE;
                    }
                    return FALSE;
                
                // case evolves via friendship:
                case EVO_FRIENDSHIP:
                case EVO_FRIENDSHIP_DAY:
                case EVO_FRIENDSHIP_NIGHT:
                case EVO_FRIENDSHIP_MOVE_TYPE:
                    if (level >= MIN_ENCOUNTER_LVL_FRIENDSHIP_EVOLVERS)
                    {
                        return TRUE;
                    }
                    return FALSE;

                // case evolves via trade:
                case EVO_TRADE:
                case EVO_TRADE_ITEM:
                case EVO_TRADE_SPECIFIC_MON:
                    if (level >= MIN_ENCOUNTER_LVL_TRADE_EVOLVERS)
                    {
                        return TRUE;
                    }
                    return FALSE;


                // case evolves via item:
                case EVO_ITEM:
                case EVO_ITEM_HOLD_DAY:
                case EVO_ITEM_HOLD_NIGHT:
                case EVO_ITEM_MALE:
                case EVO_ITEM_FEMALE:
                case EVO_ITEM_NIGHT:
                case EVO_ITEM_DAY:
                case EVO_ITEM_HOLD:
                    if (level >= MIN_ENCOUNTER_LVL_ITEM_EVOLVERS)
                    {
                        return TRUE;
                    }
                    return FALSE;
                }
            }
        }
    }

    return TRUE;
}

static bool8 DoesSpeciesMatchArea(u16 species, u8 areaType, u16 currentMapId)
{
    u8 i;
    bool8 match;

    match = FALSE;

    switch (areaType)
    {
    case WILD_AREA_LAND:
        switch (currentMapId)
        {
        // general nature:
        case MAP_ROUTE101:
        case MAP_ROUTE102:
        case MAP_ROUTE116:
        case MAP_ROUTE117:
        case MAP_ROUTE120:
        case MAP_ROUTE121:
        case MAP_ROUTE123:
            for (i=0; i<2; i++)
            {
                switch (gSpeciesInfo[species].eggGroups[i])
                {
                case EGG_GROUP_NONE:
                case EGG_GROUP_WATER_2:
                case EGG_GROUP_WATER_3:
                case EGG_GROUP_DRAGON:
                case EGG_GROUP_UNDISCOVERED:
                    return FALSE;
                case EGG_GROUP_MONSTER:
                case EGG_GROUP_BUG:
                case EGG_GROUP_FLYING:
                case EGG_GROUP_FIELD:
                case EGG_GROUP_FAIRY:
                case EGG_GROUP_GRASS:
                case EGG_GROUP_DITTO:
                    match = TRUE;
                    break;
                }
            }
            return match;
        
        // forest near water:
        case MAP_ROUTE103:
        case MAP_ROUTE104:
        case MAP_ROUTE118:
        case MAP_MOSSDEEP_CITY:
            for (i=0; i<2; i++)
            {
                switch (gSpeciesInfo[species].eggGroups[i])
                {
                case EGG_GROUP_NONE:
                case EGG_GROUP_WATER_2:
                case EGG_GROUP_WATER_3:
                case EGG_GROUP_DRAGON:
                case EGG_GROUP_UNDISCOVERED:
                    return FALSE;
                case EGG_GROUP_MONSTER:
                case EGG_GROUP_BUG:
                case EGG_GROUP_FLYING:
                case EGG_GROUP_FIELD:
                case EGG_GROUP_FAIRY:
                case EGG_GROUP_GRASS:
                case EGG_GROUP_DITTO:
                case EGG_GROUP_WATER_1:
                    match = TRUE;
                    break;
                }
            }
            return match;

        // sea:
        case MAP_ROUTE105:
        case MAP_ROUTE106:
        case MAP_ROUTE107:
        case MAP_ROUTE108:
        case MAP_ROUTE109:
        case MAP_ROUTE110:
        case MAP_ROUTE122:
        case MAP_ROUTE124:
        case MAP_ROUTE125:
        case MAP_ROUTE126:
        case MAP_ROUTE127:
        case MAP_ROUTE128:
        case MAP_ROUTE129:
        case MAP_ROUTE130:
        case MAP_ROUTE131:
        case MAP_ROUTE132:
        case MAP_ROUTE133:
        case MAP_ROUTE134:
            for (i=0; i<2; i++)
            {
                switch (gSpeciesInfo[species].eggGroups[i])
                {
                case EGG_GROUP_NONE:
                case EGG_GROUP_WATER_2:
                case EGG_GROUP_WATER_3:
                case EGG_GROUP_DRAGON:
                case EGG_GROUP_UNDISCOVERED:
                    return FALSE;
                case EGG_GROUP_WATER_1:
                    match = TRUE;
                    break;
                }
            }
            if ((gSpeciesInfo[species].types[0] == TYPE_WATER)
                    || (gSpeciesInfo[species].types[1] == TYPE_WATER))
            {
                match = TRUE;
            }
            return match;
// #define EGG_GROUP_NONE          0 <- dont use
// #define EGG_GROUP_MONSTER       1 <- mountains, general nature
// #define EGG_GROUP_WATER_1       2 <- land near water
// #define EGG_GROUP_BUG           3 <- BUG
// #define EGG_GROUP_FLYING        4 <- flying types
// #define EGG_GROUP_FIELD         5 <- pretty much anywhere on land
// #define EGG_GROUP_FAIRY         6 <- general nature, maybe other nature too
// #define EGG_GROUP_GRASS         7 <- general nature, forests
// #define EGG_GROUP_HUMAN_LIKE    8 <- mountains and general nature
// #define EGG_GROUP_WATER_3       9 <- only in water
// #define EGG_GROUP_MINERAL       10 <- caves
// #define EGG_GROUP_AMORPHOUS     11 <- mountains and caves
// #define EGG_GROUP_WATER_2       12 <- ONLY in water
// #define EGG_GROUP_DITTO         13 <- ditto
// #define EGG_GROUP_DRAGON        14 <- mountains, if not combined with water(1 or 2) egg group
// #define EGG_GROUP_UNDISCOVERED  15 <- no random encounters
        }
        break;
    }
    // case WILD_AREA_WATER:
    // case WILD_AREA_ROCKS:
    // case WILD_AREA_FISHING:


    //     // deep forest: petalburg woods




    //     // mountain-like: route 111-112, 114-115, victory road, jagged pass

    //     // near volcano: route 113, mt chimney

    //     // rain forest: route 119

    //     // abandoned monument: sky pillar

    // TODO: return FALSE is only for debugging, will cause problems for undefined map segments
    return FALSE;
}

static bool8 IsSpeciesValidWildEncounter(u16 species)
{
    u16 flags;

    flags = gSpeciesInfo[species].flags;
    if ((flags & SPECIES_FLAG_LEGENDARY) 
            || (flags & SPECIES_FLAG_MYTHICAL)
            || (flags & SPECIES_FLAG_MEGA_EVOLUTION)
            || (flags & SPECIES_FLAG_PRIMAL_REVERSION)
            || (flags & SPECIES_FLAG_ULTRA_BEAST))
    {
        return FALSE;
    }

    // anything else?

    return TRUE;
}

u16 GetRandomizedSpecies_Land(u16 seedSpecies, u8 level, u8 areaType)
{
    u16 currentMapId;
    u16 randomizedSpecies;
    u8 i;

    currentMapId = ((gSaveBlock1Ptr->location.mapGroup) << 8 | gSaveBlock1Ptr->location.mapNum);

    // create temporary random seed
    SeedRng(areaType + seedSpecies + currentMapId);

    for (i=0; i<NUM_ENCOUNTER_RANDOMIZATION_TRIES; i++)
    {
        randomizedSpecies = Random() % NUM_SPECIES;
        if (IsSpeciesValidWildEncounter(randomizedSpecies)
                && DoesSpeciesMatchArea(randomizedSpecies, areaType, currentMapId)
                // check level last because it is least efficient check:
                && DoesSpeciesMatchLevel(randomizedSpecies, level)) 
        {
            return randomizedSpecies;
        }
    }

    // no match found
    return SPECIES_UMBREON;
}