#include "global.h"
#include "battle_setup.h"
#include "battle_pike.h"
#include "battle_pyramid.h"
#include "event_data.h"
#include "fieldmap.h"
#include "fishing.h"
#include "follower_npc.h"
#include "item.h"
#include "random.h"
#include "field_player_avatar.h"
#include "link.h"
#include "metatile_behavior.h"
#include "overworld.h"
#include "ow_abilities.h"
#include "pokeblock.h"
#include "pokemon.h"
#include "random.h"
#include "roamer.h"
#include "safari_zone.h"
#include "script.h"
#include "tv.h"
#include "wild_encounter.h"
#include "battle_debug.h"
#include "battle_pike.h"
#include "battle_pyramid.h"
#include "constants/abilities.h"
#include "constants/game_stat.h"
#include "constants/item.h"
#include "constants/items.h"
#include "constants/layouts.h"
#include "constants/weather.h"

extern const u8 EventScript_SprayWoreOff[];

#define MAX_ENCOUNTER_RATE 2880

#define NUM_FEEBAS_SPOTS 6

// Number of accessible fishing spots in each section of Route 119
// Each section is an area of the route between the y coordinates in sRoute119WaterTileData
#define NUM_FISHING_SPOTS_1 131
#define NUM_FISHING_SPOTS_2 167
#define NUM_FISHING_SPOTS_3 149
#define NUM_FISHING_SPOTS (NUM_FISHING_SPOTS_1 + NUM_FISHING_SPOTS_2 + NUM_FISHING_SPOTS_3)

static u16 FeebasRandom(void);
static void FeebasSeedRng(u16 seed);
static void ApplyFluteEncounterRateMod(u32 *encRate);
static void ApplyCleanseTagEncounterRateMod(u32 *encRate);
static u8 GetMaxLevelOfSpeciesInWildTable(const struct WildPokemon *wildMon, enum Species species, enum WildPokemonArea area);
#ifdef BUGFIX
static bool8 TryGetAbilityInfluencedWildMonIndex(const struct WildPokemon *wildMon, enum Type type, enum Ability ability, u8 *monIndex, u32 size);
#else
static bool8 TryGetAbilityInfluencedWildMonIndex(const struct WildPokemon *wildMon, enum Type type, enum Ability ability, u8 *monIndex);
#endif

EWRAM_DATA static u8 sWildEncountersDisabled = 0;
EWRAM_DATA static u32 sFeebasRngValue = 0;
EWRAM_DATA bool8 gIsFishingEncounter = 0;
EWRAM_DATA bool8 gIsSurfingEncounter = 0;
EWRAM_DATA u8 gChainFishingDexNavStreak = 0;

#include "data/wild_encounters.h"

const struct WildPokemon gWildFeebas = {20, 25, SPECIES_FEEBAS};

static const u16 sRoute119WaterTileData[] =
{
//yMin, yMax, numSpots in previous sections
     0,  45,  0,
    46,  91,  NUM_FISHING_SPOTS_1,
    92, 139,  NUM_FISHING_SPOTS_1 + NUM_FISHING_SPOTS_2,
};

void DisableWildEncounters(bool8 disabled)
{
    sWildEncountersDisabled = disabled;
}

// Each fishing spot on Route 119 is given a number between 1 and NUM_FISHING_SPOTS inclusive.
// The number is determined by counting the valid fishing spots left to right top to bottom.
// The map is divided into three sections, with each section having a pre-counted number of
// fishing spots to start from to avoid counting a large number of spots at the bottom of the map.
// Note that a spot is considered valid if it is surfable and not a waterfall. To exclude all
// of the inaccessible water metatiles (so that they can't be selected as a Feebas spot) they
// use a different metatile that isn't actually surfable because it has MB_NORMAL instead.
// This function is given the coordinates and section of a fishing spot and returns which number it is.
static u16 GetFeebasFishingSpotId(s16 targetX, s16 targetY, u8 section)
{
    u16 x, y;
    u16 yMin = sRoute119WaterTileData[section * 3 + 0];
    u16 yMax = sRoute119WaterTileData[section * 3 + 1];
    u16 spotId = sRoute119WaterTileData[section * 3 + 2];

    for (y = yMin; y <= yMax; y++)
    {
        for (x = 0; x < gMapHeader.mapLayout->width; x++)
        {
            u8 behavior = MapGridGetMetatileBehaviorAt(x + MAP_OFFSET, y + MAP_OFFSET);
            if (MetatileBehavior_IsSurfableAndNotWaterfall(behavior) == TRUE)
            {
                spotId++;
                if (targetX == x && targetY == y)
                    return spotId;
            }
        }
    }
    return spotId + 1;
}

bool8 CheckFeebasAtCoords(s16 x, s16 y)
{
    u8 i;
    u16 feebasSpots[NUM_FEEBAS_SPOTS];
    u8 route119Section = 0;
    u16 spotId;

    if (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_ROUTE119)
     && gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_ROUTE119))
    {
        x -= MAP_OFFSET;
        y -= MAP_OFFSET;

        // Get which third of the map the player is in
        if (y >= sRoute119WaterTileData[3 * 0 + 0] && y <= sRoute119WaterTileData[3 * 0 + 1])
            route119Section = 0;
        if (y >= sRoute119WaterTileData[3 * 1 + 0] && y <= sRoute119WaterTileData[3 * 1 + 1])
            route119Section = 1;
        if (y >= sRoute119WaterTileData[3 * 2 + 0] && y <= sRoute119WaterTileData[3 * 2 + 1])
            route119Section = 2;

        // 50% chance of encountering Feebas (assuming this is a Feebas spot)
        if (Random() % 100 > 49)
            return FALSE;

        FeebasSeedRng(gSaveBlock1Ptr->dewfordTrends[0].rand);

        // Assign each Feebas spot to a random fishing spot.
        // Randomness is fixed depending on the seed above.
        for (i = 0; i != NUM_FEEBAS_SPOTS;)
        {
            feebasSpots[i] = FeebasRandom() % NUM_FISHING_SPOTS;
            if (feebasSpots[i] == 0)
                feebasSpots[i] = NUM_FISHING_SPOTS;

            // < 1 below is a pointless check, it will never be TRUE.
            // >= 4 to skip fishing spots 1-3, because these are inaccessible
            // spots at the top of the map, at (9,7), (7,13), and (15,16).
            // The first accessible fishing spot is spot 4 at (18,18).
            if (feebasSpots[i] < 1 || feebasSpots[i] >= 4)
                i++;
        }

        // Check which fishing spot the player is at, and see if
        // it matches any of the Feebas spots.
        spotId = GetFeebasFishingSpotId(x, y, route119Section);
        for (i = 0; i < NUM_FEEBAS_SPOTS; i++)
        {
            if (spotId == feebasSpots[i])
                return TRUE;
        }
    }
    return FALSE;
}

static u16 FeebasRandom(void)
{
    sFeebasRngValue = ISO_RANDOMIZE2(sFeebasRngValue);
    return sFeebasRngValue >> 16;
}

static void FeebasSeedRng(u16 seed)
{
    sFeebasRngValue = seed;
}

// LAND_WILD_COUNT
u32 ChooseWildMonIndex_Land(void)
{
    u8 wildMonIndex = 0;
    bool8 swap = FALSE;
    u8 rand = Random() % ENCOUNTER_CHANCE_LAND_MONS_TOTAL;

    if (rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_0)
        wildMonIndex = 0;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_0 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_1)
        wildMonIndex = 1;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_1 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_2)
        wildMonIndex = 2;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_2 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_3)
        wildMonIndex = 3;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_3 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_4)
        wildMonIndex = 4;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_4 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_5)
        wildMonIndex = 5;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_5 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_6)
        wildMonIndex = 6;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_6 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_7)
        wildMonIndex = 7;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_7 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_8)
        wildMonIndex = 8;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_8 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_9)
        wildMonIndex = 9;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_9 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_10)
        wildMonIndex = 10;
    else
        wildMonIndex = 11;

    if (LURE_STEP_COUNT != 0 && (Random() % 10 < 2))
        swap = TRUE;

    if (swap)
        wildMonIndex = 11 - wildMonIndex;

    return wildMonIndex;
}

// WATER_WILD_COUNT
u32 ChooseWildMonIndex_Water(void)
{
    u32 wildMonIndex = 0;
    bool8 swap = FALSE;
    u8 rand = Random() % ENCOUNTER_CHANCE_WATER_MONS_TOTAL;

    if (rand < ENCOUNTER_CHANCE_WATER_MONS_SLOT_0)
        wildMonIndex = 0;
    else if (rand >= ENCOUNTER_CHANCE_WATER_MONS_SLOT_0 && rand < ENCOUNTER_CHANCE_WATER_MONS_SLOT_1)
        wildMonIndex = 1;
    else if (rand >= ENCOUNTER_CHANCE_WATER_MONS_SLOT_1 && rand < ENCOUNTER_CHANCE_WATER_MONS_SLOT_2)
        wildMonIndex = 2;
    else if (rand >= ENCOUNTER_CHANCE_WATER_MONS_SLOT_2 && rand < ENCOUNTER_CHANCE_WATER_MONS_SLOT_3)
        wildMonIndex = 3;
    else
        wildMonIndex = 4;

    if (LURE_STEP_COUNT != 0 && (Random() % 10 < 2))
        swap = TRUE;

    if (swap)
        wildMonIndex = 4 - wildMonIndex;

    return wildMonIndex;
}

// ROCK_WILD_COUNT
u32 ChooseWildMonIndex_Rocks(void)
{
    u32 wildMonIndex = 0;
    bool8 swap = FALSE;
    u8 rand = Random() % ENCOUNTER_CHANCE_ROCK_SMASH_MONS_TOTAL;

    if (rand < ENCOUNTER_CHANCE_ROCK_SMASH_MONS_SLOT_0)
        wildMonIndex = 0;
    else if (rand >= ENCOUNTER_CHANCE_ROCK_SMASH_MONS_SLOT_0 && rand < ENCOUNTER_CHANCE_ROCK_SMASH_MONS_SLOT_1)
        wildMonIndex = 1;
    else if (rand >= ENCOUNTER_CHANCE_ROCK_SMASH_MONS_SLOT_1 && rand < ENCOUNTER_CHANCE_ROCK_SMASH_MONS_SLOT_2)
        wildMonIndex = 2;
    else if (rand >= ENCOUNTER_CHANCE_ROCK_SMASH_MONS_SLOT_2 && rand < ENCOUNTER_CHANCE_ROCK_SMASH_MONS_SLOT_3)
        wildMonIndex = 3;
    else
        wildMonIndex = 4;

    if (LURE_STEP_COUNT != 0 && (Random() % 10 < 2))
        swap = TRUE;

    if (swap)
        wildMonIndex = 4 - wildMonIndex;

    return wildMonIndex;
}

// FISH_WILD_COUNT
static u32 ChooseWildMonIndex_Fishing(u8 rod)
{
    u8 wildMonIndex = 0;
    bool8 swap = FALSE;
    u8 rand = Random() % max(max(ENCOUNTER_CHANCE_FISHING_MONS_OLD_ROD_TOTAL, ENCOUNTER_CHANCE_FISHING_MONS_GOOD_ROD_TOTAL),
                             ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_TOTAL);

    if (LURE_STEP_COUNT != 0 && (Random() % 10 < 2))
        swap = TRUE;

    switch (rod)
    {
    case OLD_ROD:
        if (rand < ENCOUNTER_CHANCE_FISHING_MONS_OLD_ROD_SLOT_0)
            wildMonIndex = 0;
        else
            wildMonIndex = 1;

        if (swap)
            wildMonIndex = 1 - wildMonIndex;
        break;
    case GOOD_ROD:
        if (rand < ENCOUNTER_CHANCE_FISHING_MONS_GOOD_ROD_SLOT_2)
            wildMonIndex = 2;
        if (rand >= ENCOUNTER_CHANCE_FISHING_MONS_GOOD_ROD_SLOT_2 && rand < ENCOUNTER_CHANCE_FISHING_MONS_GOOD_ROD_SLOT_3)
            wildMonIndex = 3;
        if (rand >= ENCOUNTER_CHANCE_FISHING_MONS_GOOD_ROD_SLOT_3 && rand < ENCOUNTER_CHANCE_FISHING_MONS_GOOD_ROD_SLOT_4)
            wildMonIndex = 4;

        if (swap)
            wildMonIndex = 6 - wildMonIndex;
        break;
    case SUPER_ROD:
        if (rand < ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_5)
            wildMonIndex = 5;
        if (rand >= ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_5 && rand < ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_6)
            wildMonIndex = 6;
        if (rand >= ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_6 && rand < ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_7)
            wildMonIndex = 7;
        if (rand >= ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_7 && rand < ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_8)
            wildMonIndex = 8;
        if (rand >= ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_8 && rand < ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_9)
            wildMonIndex = 9;

        if (swap)
            wildMonIndex = 14 - wildMonIndex;
        break;
    }
    return wildMonIndex;
}

u8 ChooseWildMonLevel(const struct WildPokemon *wildPokemon, u8 wildMonIndex, enum WildPokemonArea area)
{
    u8 min;
    u8 max;
    u8 range;
    u8 rand;

    if (LURE_STEP_COUNT == 0)
    {
        // Make sure minimum level is less than maximum level
        if (wildPokemon[wildMonIndex].maxLevel >= wildPokemon[wildMonIndex].minLevel)
        {
            min = wildPokemon[wildMonIndex].minLevel;
            max = wildPokemon[wildMonIndex].maxLevel;
        }
        else
        {
            min = wildPokemon[wildMonIndex].maxLevel;
            max = wildPokemon[wildMonIndex].minLevel;
        }
        range = max - min + 1;
        rand = Random() % range;

        // check ability for max level mon
        if (!GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SANITY_IS_EGG))
        {
            enum Ability ability = GetMonAbility(&gParties[B_TRAINER_PLAYER][0]);
            if (ability == ABILITY_HUSTLE || ability == ABILITY_VITAL_SPIRIT || ability == ABILITY_PRESSURE)
            {
                if (Random() % 2 == 0)
                    return max;

                if (rand != 0)
                    rand--;
            }
        }
        return min + rand;
    }
    else
    {
        // Looks for the max level of all slots that share the same species as the selected slot.
        max = GetMaxLevelOfSpeciesInWildTable(wildPokemon, wildPokemon[wildMonIndex].species, area);
        if (max > 0)
            return max + 1;
        else // Failsafe
            return wildPokemon[wildMonIndex].maxLevel + 1;
    }
}

u16 GetCurrentMapWildMonHeaderId(void)
{
    u16 i;

    for (i = 0; ; i++)
    {
        const struct WildPokemonHeader *wildHeader = &gWildMonHeaders[i];
        if (wildHeader->mapGroup == MAP_GROUP(MAP_UNDEFINED))
            break;

        if (gWildMonHeaders[i].mapGroup == gSaveBlock1Ptr->location.mapGroup &&
            gWildMonHeaders[i].mapNum == gSaveBlock1Ptr->location.mapNum)
        {
            if (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_ALTERING_CAVE) &&
                gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_ALTERING_CAVE))
            {
                u16 alteringCaveId = VarGet(VAR_ALTERING_CAVE_WILD_SET);
                if (alteringCaveId >= NUM_ALTERING_CAVE_TABLES)
                    alteringCaveId = 0;

                i += alteringCaveId;
            }

            return i;
        }
    }

    return HEADER_NONE;
}

enum TimeOfDay GetTimeOfDayForEncounters(u32 headerId, enum WildPokemonArea area)
{
    const struct WildPokemonInfo *wildMonInfo;
    enum TimeOfDay timeOfDay = GetTimeOfDay();

    if (!OW_TIME_OF_DAY_ENCOUNTERS)
        return TIME_OF_DAY_DEFAULT;

    if (InBattlePike() || CurrentBattlePyramidLocation() != PYRAMID_LOCATION_NONE)
        return OW_TIME_OF_DAY_FALLBACK;

    switch (area)
    {
    default:
    case WILD_AREA_LAND:
        wildMonInfo = gWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo;
        break;
    case WILD_AREA_WATER:
        wildMonInfo = gWildMonHeaders[headerId].encounterTypes[timeOfDay].waterMonsInfo;
        break;
    case WILD_AREA_ROCKS:
        wildMonInfo = gWildMonHeaders[headerId].encounterTypes[timeOfDay].rockSmashMonsInfo;
        break;
    case WILD_AREA_FISHING:
        wildMonInfo = gWildMonHeaders[headerId].encounterTypes[timeOfDay].fishingMonsInfo;
        break;
    case WILD_AREA_HIDDEN:
        wildMonInfo = gWildMonHeaders[headerId].encounterTypes[timeOfDay].hiddenMonsInfo;
        break;
    }

    if (wildMonInfo == NULL && !OW_TIME_OF_DAY_DISABLE_FALLBACK)
        return OW_TIME_OF_DAY_FALLBACK;
    else
        return GenConfigTimeOfDay(timeOfDay);
}

static u8 PickWildMonNature(enum Species species)
{
    u8 i;
    struct Pokeblock *safariPokeblock;
    u8 natures[NUM_NATURES];

    if (GetSafariZoneFlag() == TRUE && Random() % 100 < 80)
    {
        safariPokeblock = SafariZoneGetActivePokeblock();
        if (safariPokeblock != NULL)
        {
            for (i = 0; i < NUM_NATURES; i++)
                natures[i] = i;
            Shuffle(natures, NUM_NATURES, sizeof(natures[0]));
            for (i = 0; i < NUM_NATURES; i++)
            {
                if (PokeblockGetGain(natures[i], safariPokeblock) > 0)
                    return natures[i];
            }
        }
    }

    // SAG: competitive nature -- Adamant for physical attackers, Modest for special
    if (gSpeciesInfo[species].baseAttack >= gSpeciesInfo[species].baseSpAttack)
        return NATURE_ADAMANT;
    return NATURE_MODEST;
}


// SAG: custom competitive movesets for wild Pokemon
static const struct { u16 species; u16 moves[MAX_MON_MOVES]; } sWildCompMoves[] =
{
    { SPECIES_AEGISLASH, { MOVE_SHADOW_BALL, MOVE_SHADOW_SNEAK, MOVE_FLASH_CANNON, MOVE_KINGS_SHIELD } },
    { SPECIES_AGGRON, { MOVE_HEAVY_SLAM, MOVE_EARTHQUAKE, MOVE_STONE_EDGE, MOVE_STEALTH_ROCK } },
    { SPECIES_ALAKAZAM, { MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_FOCUS_BLAST, MOVE_NASTY_PLOT } },
    { SPECIES_ARCANINE, { MOVE_FLARE_BLITZ, MOVE_CLOSE_COMBAT, MOVE_WILD_CHARGE, MOVE_EXTREME_SPEED } },
    { SPECIES_ARCHEOPS, { MOVE_STONE_EDGE, MOVE_DRAGON_CLAW, MOVE_EARTHQUAKE, MOVE_ACROBATICS } },
    { SPECIES_ARTICUNO, { MOVE_FREEZE_DRY, MOVE_HURRICANE, MOVE_ROOST, MOVE_CALM_MIND } },
    { SPECIES_BASTIODON, { MOVE_IRON_HEAD, MOVE_STONE_EDGE, MOVE_STEALTH_ROCK, MOVE_BODY_PRESS } },
    { SPECIES_BAXCALIBUR, { MOVE_GLAIVE_RUSH, MOVE_ICICLE_CRASH, MOVE_EARTHQUAKE, MOVE_DRAGON_DANCE } },
    { SPECIES_BEEDRILL, { MOVE_POISON_JAB, MOVE_X_SCISSOR, MOVE_DRILL_RUN, MOVE_KNOCK_OFF } },
    { SPECIES_BEWEAR, { MOVE_DOUBLE_EDGE, MOVE_CLOSE_COMBAT, MOVE_EARTHQUAKE, MOVE_SWORDS_DANCE } },
    { SPECIES_BLASTOISE, { MOVE_HYDRO_PUMP, MOVE_ICE_BEAM, MOVE_FLASH_CANNON, MOVE_RAPID_SPIN } },
    { SPECIES_BLAZIKEN, { MOVE_FLARE_BLITZ, MOVE_CLOSE_COMBAT, MOVE_THUNDER_PUNCH, MOVE_SWORDS_DANCE } },
    { SPECIES_BOLTUND, { MOVE_THUNDER_FANG, MOVE_CRUNCH, MOVE_PLAY_ROUGH, MOVE_VOLT_SWITCH } },
    { SPECIES_BOUFFALANT, { MOVE_HEAD_CHARGE, MOVE_EARTHQUAKE, MOVE_MEGAHORN, MOVE_SWORDS_DANCE } },
    { SPECIES_BRELOOM, { MOVE_SEED_BOMB, MOVE_MACH_PUNCH, MOVE_BULLET_SEED, MOVE_SPORE } },
    { SPECIES_BUTTERFREE, { MOVE_BUG_BUZZ, MOVE_AIR_SLASH, MOVE_QUIVER_DANCE, MOVE_SLEEP_POWDER } },
    { SPECIES_CAMERUPT, { MOVE_ERUPTION, MOVE_EARTH_POWER, MOVE_FLAMETHROWER, MOVE_STEALTH_ROCK } },
    { SPECIES_CETITAN, { MOVE_ICICLE_CRASH, MOVE_EARTHQUAKE, MOVE_LIQUIDATION, MOVE_BELLY_DRUM } },
    { SPECIES_CHANDELURE, { MOVE_FLAMETHROWER, MOVE_SHADOW_BALL, MOVE_ENERGY_BALL, MOVE_CALM_MIND } },
    { SPECIES_CHARIZARD, { MOVE_FLAMETHROWER, MOVE_AIR_SLASH, MOVE_DRAGON_PULSE, MOVE_ROOST } },
    { SPECIES_CLOYSTER, { MOVE_ICICLE_SPEAR, MOVE_ROCK_BLAST, MOVE_LIQUIDATION, MOVE_SHELL_SMASH } },
    { SPECIES_COALOSSAL, { MOVE_FLAMETHROWER, MOVE_STONE_EDGE, MOVE_EARTHQUAKE, MOVE_STEALTH_ROCK } },
    { SPECIES_CONKELDURR, { MOVE_DRAIN_PUNCH, MOVE_MACH_PUNCH, MOVE_KNOCK_OFF, MOVE_BULK_UP } },
    { SPECIES_CORVIKNIGHT, { MOVE_BRAVE_BIRD, MOVE_IRON_HEAD, MOVE_BODY_PRESS, MOVE_ROOST } },
    { SPECIES_CRAWDAUNT, { MOVE_CRABHAMMER, MOVE_KNOCK_OFF, MOVE_AQUA_JET, MOVE_SWORDS_DANCE } },
    { SPECIES_CROBAT, { MOVE_BRAVE_BIRD, MOVE_CROSS_POISON, MOVE_U_TURN, MOVE_ROOST } },
    { SPECIES_DARMANITAN, { MOVE_FLARE_BLITZ, MOVE_EARTHQUAKE, MOVE_ROCK_SLIDE, MOVE_U_TURN } },
    { SPECIES_DODRIO, { MOVE_BRAVE_BIRD, MOVE_DOUBLE_EDGE, MOVE_KNOCK_OFF, MOVE_SWORDS_DANCE } },
    { SPECIES_DONPHAN, { MOVE_EARTHQUAKE, MOVE_ICE_SHARD, MOVE_KNOCK_OFF, MOVE_RAPID_SPIN } },
    { SPECIES_DRAGAPULT, { MOVE_DRAGON_DARTS, MOVE_SHADOW_BALL, MOVE_FLAMETHROWER, MOVE_U_TURN } },
    { SPECIES_DRUDDIGON, { MOVE_DRAGON_CLAW, MOVE_EARTHQUAKE, MOVE_FIRE_PUNCH, MOVE_STEALTH_ROCK } },
    { SPECIES_DUSKNOIR, { MOVE_SHADOW_PUNCH, MOVE_EARTHQUAKE, MOVE_ICE_PUNCH, MOVE_PAIN_SPLIT } },
    { SPECIES_ELECTIVIRE, { MOVE_WILD_CHARGE, MOVE_ICE_PUNCH, MOVE_CROSS_CHOP, MOVE_EARTHQUAKE } },
    { SPECIES_ELECTRODE, { MOVE_THUNDERBOLT, MOVE_VOLT_SWITCH, MOVE_FOUL_PLAY, MOVE_TAUNT } },
    { SPECIES_EMPOLEON, { MOVE_HYDRO_PUMP, MOVE_FLASH_CANNON, MOVE_ICE_BEAM, MOVE_GRASS_KNOT } },
    { SPECIES_ESCAVALIER, { MOVE_IRON_HEAD, MOVE_MEGAHORN, MOVE_KNOCK_OFF, MOVE_SWORDS_DANCE } },
    { SPECIES_EXCADRILL, { MOVE_EARTHQUAKE, MOVE_IRON_HEAD, MOVE_ROCK_SLIDE, MOVE_RAPID_SPIN } },
    { SPECIES_EXPLOUD, { MOVE_BOOMBURST, MOVE_FLAMETHROWER, MOVE_ICE_BEAM, MOVE_SURF } },
    { SPECIES_FERALIGATR, { MOVE_WATERFALL, MOVE_ICE_PUNCH, MOVE_CRUNCH, MOVE_DRAGON_DANCE } },
    { SPECIES_FLYGON, { MOVE_EARTHQUAKE, MOVE_DRAGON_CLAW, MOVE_U_TURN, MOVE_FIRE_PUNCH } },
    { SPECIES_FROSLASS, { MOVE_ICE_BEAM, MOVE_SHADOW_BALL, MOVE_DESTINY_BOND, MOVE_SPIKES } },
    { SPECIES_GALLADE, { MOVE_PSYCHO_CUT, MOVE_CLOSE_COMBAT, MOVE_KNOCK_OFF, MOVE_SWORDS_DANCE } },
    { SPECIES_GALVANTULA, { MOVE_THUNDERBOLT, MOVE_BUG_BUZZ, MOVE_VOLT_SWITCH, MOVE_STICKY_WEB } },
    { SPECIES_GARCHOMP, { MOVE_EARTHQUAKE, MOVE_DRAGON_CLAW, MOVE_STONE_EDGE, MOVE_SWORDS_DANCE } },
    { SPECIES_GLISCOR, { MOVE_EARTHQUAKE, MOVE_KNOCK_OFF, MOVE_PROTECT, MOVE_SWORDS_DANCE } },
    { SPECIES_GOLEM, { MOVE_EARTHQUAKE, MOVE_STONE_EDGE, MOVE_EXPLOSION, MOVE_STEALTH_ROCK } },
    { SPECIES_GOLURK, { MOVE_EARTHQUAKE, MOVE_SHADOW_PUNCH, MOVE_ICE_PUNCH, MOVE_STEALTH_ROCK } },
    { SPECIES_GOODRA, { MOVE_DRACO_METEOR, MOVE_FIRE_BLAST, MOVE_THUNDERBOLT, MOVE_SLUDGE_BOMB } },
    { SPECIES_GRENINJA, { MOVE_HYDRO_PUMP, MOVE_DARK_PULSE, MOVE_ICE_BEAM, MOVE_U_TURN } },
    { SPECIES_GRIMMSNARL, { MOVE_SPIRIT_BREAK, MOVE_SUCKER_PUNCH, MOVE_DARKEST_LARIAT, MOVE_BULK_UP } },
    { SPECIES_GYARADOS, { MOVE_WATERFALL, MOVE_CRUNCH, MOVE_EARTHQUAKE, MOVE_DRAGON_DANCE } },
    { SPECIES_HARIYAMA, { MOVE_CLOSE_COMBAT, MOVE_KNOCK_OFF, MOVE_BULLET_PUNCH, MOVE_HEAVY_SLAM } },
    { SPECIES_HAXORUS, { MOVE_DRAGON_CLAW, MOVE_EARTHQUAKE, MOVE_POISON_JAB, MOVE_DRAGON_DANCE } },
    { SPECIES_HEATRAN, { MOVE_MAGMA_STORM, MOVE_EARTH_POWER, MOVE_FLASH_CANNON, MOVE_STEALTH_ROCK } },
    { SPECIES_HELIOLISK, { MOVE_THUNDERBOLT, MOVE_HYPER_VOICE, MOVE_VOLT_SWITCH, MOVE_SURF } },
    { SPECIES_HERACROSS, { MOVE_MEGAHORN, MOVE_CLOSE_COMBAT, MOVE_ROCK_BLAST, MOVE_SWORDS_DANCE } },
    { SPECIES_HIPPOWDON, { MOVE_EARTHQUAKE, MOVE_STONE_EDGE, MOVE_SLACK_OFF, MOVE_STEALTH_ROCK } },
    { SPECIES_HONCHKROW, { MOVE_BRAVE_BIRD, MOVE_SUCKER_PUNCH, MOVE_HEAT_WAVE, MOVE_ROOST } },
    { SPECIES_HYDREIGON, { MOVE_DARK_PULSE, MOVE_DRACO_METEOR, MOVE_FLASH_CANNON, MOVE_NASTY_PLOT } },
    { SPECIES_INFERNAPE, { MOVE_FLARE_BLITZ, MOVE_CLOSE_COMBAT, MOVE_U_TURN, MOVE_GRASS_KNOT } },
    { SPECIES_KANGASKHAN, { MOVE_DOUBLE_EDGE, MOVE_EARTHQUAKE, MOVE_SUCKER_PUNCH, MOVE_POWER_UP_PUNCH } },
    { SPECIES_KINGAMBIT, { MOVE_KOWTOW_CLEAVE, MOVE_IRON_HEAD, MOVE_SUCKER_PUNCH, MOVE_SWORDS_DANCE } },
    { SPECIES_KINGDRA, { MOVE_HYDRO_PUMP, MOVE_DRAGON_PULSE, MOVE_ICE_BEAM, MOVE_DRACO_METEOR } },
    { SPECIES_KINGLER, { MOVE_CRABHAMMER, MOVE_KNOCK_OFF, MOVE_X_SCISSOR, MOVE_AGILITY } },
    { SPECIES_KLINKLANG, { MOVE_GEAR_GRIND, MOVE_WILD_CHARGE, MOVE_SHIFT_GEAR, MOVE_SUBSTITUTE } },
    { SPECIES_KOMMO_O, { MOVE_CLANGOROUS_SOUL, MOVE_CLOSE_COMBAT, MOVE_DRAGON_CLAW, MOVE_POISON_JAB } },
    { SPECIES_KROOKODILE, { MOVE_EARTHQUAKE, MOVE_KNOCK_OFF, MOVE_STONE_EDGE, MOVE_DRAGON_DANCE } },
    { SPECIES_LEAVANNY, { MOVE_LEAF_BLADE, MOVE_X_SCISSOR, MOVE_KNOCK_OFF, MOVE_SWORDS_DANCE } },
    { SPECIES_LIEPARD, { MOVE_KNOCK_OFF, MOVE_SUCKER_PUNCH, MOVE_PLAY_ROUGH, MOVE_U_TURN } },
    { SPECIES_LILLIGANT, { MOVE_PETAL_DANCE, MOVE_QUIVER_DANCE, MOVE_SLEEP_POWDER, MOVE_GIGA_DRAIN } },
    { SPECIES_LUCARIO, { MOVE_AURA_SPHERE, MOVE_FLASH_CANNON, MOVE_NASTY_PLOT, MOVE_VACUUM_WAVE } },
    { SPECIES_LUDICOLO, { MOVE_SCALD, MOVE_GIGA_DRAIN, MOVE_ICE_BEAM, MOVE_RAIN_DANCE } },
    { SPECIES_LUXRAY, { MOVE_WILD_CHARGE, MOVE_CRUNCH, MOVE_ICE_FANG, MOVE_SUPERPOWER } },
    { SPECIES_LYCANROC, { MOVE_STONE_EDGE, MOVE_CLOSE_COMBAT, MOVE_CRUNCH, MOVE_SWORDS_DANCE } },
    { SPECIES_MACHAMP, { MOVE_DYNAMIC_PUNCH, MOVE_KNOCK_OFF, MOVE_STONE_EDGE, MOVE_BULLET_PUNCH } },
    { SPECIES_MAGMORTAR, { MOVE_FLAMETHROWER, MOVE_THUNDERBOLT, MOVE_FOCUS_BLAST, MOVE_NASTY_PLOT } },
    { SPECIES_MAGNEZONE, { MOVE_THUNDERBOLT, MOVE_FLASH_CANNON, MOVE_VOLT_SWITCH, MOVE_BODY_PRESS } },
    { SPECIES_MAMOSWINE, { MOVE_ICICLE_CRASH, MOVE_EARTHQUAKE, MOVE_ICE_SHARD, MOVE_KNOCK_OFF } },
    { SPECIES_MANECTRIC, { MOVE_THUNDERBOLT, MOVE_VOLT_SWITCH, MOVE_FLAMETHROWER, MOVE_OVERHEAT } },
    { SPECIES_MEDICHAM, { MOVE_HIGH_JUMP_KICK, MOVE_ZEN_HEADBUTT, MOVE_ICE_PUNCH, MOVE_BULLET_PUNCH } },
    { SPECIES_MEGANIUM, { MOVE_PETAL_BLIZZARD, MOVE_EARTHQUAKE, MOVE_BODY_PRESS, MOVE_SYNTHESIS } },
    { SPECIES_METAGROSS, { MOVE_METEOR_MASH, MOVE_EARTHQUAKE, MOVE_ZEN_HEADBUTT, MOVE_BULLET_PUNCH } },
    { SPECIES_MIGHTYENA, { MOVE_CRUNCH, MOVE_SUCKER_PUNCH, MOVE_PLAY_ROUGH, MOVE_FIRE_FANG } },
    { SPECIES_MILOTIC, { MOVE_SCALD, MOVE_ICE_BEAM, MOVE_RECOVER, MOVE_DRAGON_TAIL } },
    { SPECIES_MIMIKYU, { MOVE_PLAY_ROUGH, MOVE_SHADOW_SNEAK, MOVE_SWORDS_DANCE, MOVE_SHADOW_CLAW } },
    { SPECIES_MOLTRES, { MOVE_FLAMETHROWER, MOVE_HURRICANE, MOVE_ROOST, MOVE_WILL_O_WISP } },
    { SPECIES_MORPEKO, { MOVE_AURA_WHEEL, MOVE_THUNDERBOLT, MOVE_RAPID_SPIN, MOVE_PROTECT } },
    { SPECIES_NINETALES, { MOVE_FLAMETHROWER, MOVE_NASTY_PLOT, MOVE_SOLAR_BEAM, MOVE_WILL_O_WISP } },
    { SPECIES_NOIVERN, { MOVE_HURRICANE, MOVE_DRACO_METEOR, MOVE_FLAMETHROWER, MOVE_U_TURN } },
    { SPECIES_OBSTAGOON, { MOVE_KNOCK_OFF, MOVE_CLOSE_COMBAT, MOVE_GUNK_SHOT, MOVE_PARTING_SHOT } },
    { SPECIES_PANGORO, { MOVE_KNOCK_OFF, MOVE_CLOSE_COMBAT, MOVE_GUNK_SHOT, MOVE_BULLET_PUNCH } },
    { SPECIES_PAWMOT, { MOVE_WILD_CHARGE, MOVE_CLOSE_COMBAT, MOVE_DOUBLE_SHOCK, MOVE_NUZZLE } },
    { SPECIES_PELIPPER, { MOVE_HURRICANE, MOVE_HYDRO_PUMP, MOVE_SCALD, MOVE_U_TURN } },
    { SPECIES_PORYGON_Z, { MOVE_TRI_ATTACK, MOVE_DARK_PULSE, MOVE_THUNDERBOLT, MOVE_NASTY_PLOT } },
    { SPECIES_RAMPARDOS, { MOVE_HEAD_SMASH, MOVE_EARTHQUAKE, MOVE_ZEN_HEADBUTT, MOVE_ROCK_POLISH } },
    { SPECIES_ROSERADE, { MOVE_GIGA_DRAIN, MOVE_SLUDGE_BOMB, MOVE_LEAF_STORM, MOVE_SLEEP_POWDER } },
    { SPECIES_SALAMENCE, { MOVE_DRAGON_CLAW, MOVE_EARTHQUAKE, MOVE_DRAGON_DANCE, MOVE_DUAL_WINGBEAT } },
    { SPECIES_SCEPTILE, { MOVE_LEAF_BLADE, MOVE_DRAGON_PULSE, MOVE_FOCUS_BLAST, MOVE_SWORDS_DANCE } },
    { SPECIES_SCIZOR, { MOVE_BULLET_PUNCH, MOVE_U_TURN, MOVE_KNOCK_OFF, MOVE_SWORDS_DANCE } },
    { SPECIES_SCOLIPEDE, { MOVE_MEGAHORN, MOVE_EARTHQUAKE, MOVE_POISON_JAB, MOVE_SWORDS_DANCE } },
    { SPECIES_SCRAFTY, { MOVE_HIGH_JUMP_KICK, MOVE_KNOCK_OFF, MOVE_ICE_PUNCH, MOVE_DRAGON_DANCE } },
    { SPECIES_SHARPEDO, { MOVE_WATERFALL, MOVE_CRUNCH, MOVE_PSYCHIC_FANGS, MOVE_PROTECT } },
    { SPECIES_SHIFTRY, { MOVE_LEAF_BLADE, MOVE_KNOCK_OFF, MOVE_SUCKER_PUNCH, MOVE_SWORDS_DANCE } },
    { SPECIES_SLAKING, { MOVE_BODY_SLAM, MOVE_EARTHQUAKE, MOVE_KNOCK_OFF, MOVE_GIGA_IMPACT } },
    { SPECIES_SNORLAX, { MOVE_BODY_SLAM, MOVE_HIGH_HORSEPOWER, MOVE_CRUNCH, MOVE_CURSE } },
    { SPECIES_STARAPTOR, { MOVE_BRAVE_BIRD, MOVE_CLOSE_COMBAT, MOVE_U_TURN, MOVE_DOUBLE_EDGE } },
    { SPECIES_STARMIE, { MOVE_HYDRO_PUMP, MOVE_ICE_BEAM, MOVE_THUNDERBOLT, MOVE_RAPID_SPIN } },
    { SPECIES_STEELIX, { MOVE_EARTHQUAKE, MOVE_IRON_HEAD, MOVE_STONE_EDGE, MOVE_STEALTH_ROCK } },
    { SPECIES_SWAMPERT, { MOVE_WATERFALL, MOVE_EARTHQUAKE, MOVE_ICE_PUNCH, MOVE_STEALTH_ROCK } },
    { SPECIES_TALONFLAME, { MOVE_BRAVE_BIRD, MOVE_FLARE_BLITZ, MOVE_U_TURN, MOVE_ROOST } },
    { SPECIES_TANGROWTH, { MOVE_GIGA_DRAIN, MOVE_POWER_WHIP, MOVE_EARTHQUAKE, MOVE_SLEEP_POWDER } },
    { SPECIES_TAUROS, { MOVE_DOUBLE_EDGE, MOVE_EARTHQUAKE, MOVE_ROCK_SLIDE, MOVE_CLOSE_COMBAT } },
    { SPECIES_TENTACRUEL, { MOVE_SCALD, MOVE_SLUDGE_BOMB, MOVE_ICE_BEAM, MOVE_RAPID_SPIN } },
    { SPECIES_TOGEKISS, { MOVE_AIR_SLASH, MOVE_DAZZLING_GLEAM, MOVE_NASTY_PLOT, MOVE_ROOST } },
    { SPECIES_TORTERRA, { MOVE_WOOD_HAMMER, MOVE_EARTHQUAKE, MOVE_STONE_EDGE, MOVE_ROCK_POLISH } },
    { SPECIES_TOXICROAK, { MOVE_GUNK_SHOT, MOVE_DRAIN_PUNCH, MOVE_SUCKER_PUNCH, MOVE_SWORDS_DANCE } },
    { SPECIES_TOXTRICITY, { MOVE_OVERDRIVE, MOVE_SLUDGE_WAVE, MOVE_VOLT_SWITCH, MOVE_BOOMBURST } },
    { SPECIES_TYPHLOSION, { MOVE_ERUPTION, MOVE_FLAMETHROWER, MOVE_FOCUS_BLAST, MOVE_EXTRASENSORY } },
    { SPECIES_TYRANITAR, { MOVE_STONE_EDGE, MOVE_CRUNCH, MOVE_EARTHQUAKE, MOVE_DRAGON_DANCE } },
    { SPECIES_URSALUNA, { MOVE_HEADLONG_RUSH, MOVE_FACADE, MOVE_CRUNCH, MOVE_SWORDS_DANCE } },
    { SPECIES_VENUSAUR, { MOVE_GIGA_DRAIN, MOVE_SLUDGE_BOMB, MOVE_EARTH_POWER, MOVE_SLEEP_POWDER } },
    { SPECIES_VICTREEBEL, { MOVE_LEAF_BLADE, MOVE_SLUDGE_BOMB, MOVE_SLEEP_POWDER, MOVE_SWORDS_DANCE } },
    { SPECIES_VIKAVOLT, { MOVE_THUNDERBOLT, MOVE_BUG_BUZZ, MOVE_VOLT_SWITCH, MOVE_ENERGY_BALL } },
    { SPECIES_VILEPLUME, { MOVE_GIGA_DRAIN, MOVE_SLUDGE_BOMB, MOVE_SLEEP_POWDER, MOVE_MOONBLAST } },
    { SPECIES_VIVILLON, { MOVE_BUG_BUZZ, MOVE_HURRICANE, MOVE_QUIVER_DANCE, MOVE_SLEEP_POWDER } },
    { SPECIES_VOLCARONA, { MOVE_FIERY_DANCE, MOVE_BUG_BUZZ, MOVE_QUIVER_DANCE, MOVE_GIGA_DRAIN } },
    { SPECIES_WAILORD, { MOVE_WATER_SPOUT, MOVE_HYDRO_PUMP, MOVE_ICE_BEAM, MOVE_HEAVY_SLAM } },
    { SPECIES_WEAVILE, { MOVE_ICICLE_CRASH, MOVE_KNOCK_OFF, MOVE_TRIPLE_AXEL, MOVE_ICE_SHARD } },
    { SPECIES_YANMEGA, { MOVE_BUG_BUZZ, MOVE_AIR_SLASH, MOVE_GIGA_DRAIN, MOVE_PROTECT } },
    { SPECIES_ZANGOOSE, { MOVE_FACADE, MOVE_CLOSE_COMBAT, MOVE_KNOCK_OFF, MOVE_SWORDS_DANCE } },
    { SPECIES_ZAPDOS, { MOVE_THUNDERBOLT, MOVE_HURRICANE, MOVE_HEAT_WAVE, MOVE_ROOST } },
    { SPECIES_ZOROARK, { MOVE_DARK_PULSE, MOVE_FLAMETHROWER, MOVE_SLUDGE_BOMB, MOVE_NASTY_PLOT } },
};

void CreateWildMon(enum Species species, u8 level)
{
    ZeroEnemyPartyMons();
    u32 personality = GetMonPersonality(species, GetSynchronizedGender(WILDMON_ORIGIN, species), PickWildMonNature(species), RANDOM_UNOWN_LETTER);
    CreateMonWithIVs(&gParties[B_TRAINER_OPPONENT_A][0], species, level, personality, OTID_STRUCT_PLAYER_ID, USE_RANDOM_IVS);
    GiveMonInitialMoveset(&gParties[B_TRAINER_OPPONENT_A][0]);
    // --- SAG v0.3: perfect IVs (+ Hidden Ability when available) on wild mons ---
    {
        struct Pokemon *wildMon = &gParties[B_TRAINER_OPPONENT_A][0];
        u32 perfectIv = MAX_PER_STAT_IVS;
        u32 hiddenAbility = NUM_NORMAL_ABILITY_SLOTS;
        SetMonData(wildMon, MON_DATA_HP_IV, &perfectIv);
        SetMonData(wildMon, MON_DATA_ATK_IV, &perfectIv);
        SetMonData(wildMon, MON_DATA_DEF_IV, &perfectIv);
        SetMonData(wildMon, MON_DATA_SPEED_IV, &perfectIv);
        SetMonData(wildMon, MON_DATA_SPATK_IV, &perfectIv);
        SetMonData(wildMon, MON_DATA_SPDEF_IV, &perfectIv);
        if (gSpeciesInfo[species].abilities[NUM_NORMAL_ABILITY_SLOTS] != ABILITY_NONE)
            SetMonData(wildMon, MON_DATA_ABILITY_NUM, &hiddenAbility);
        // SAG: competitive EV spread (252 attacking / 252 Speed / 6 HP)
        {
            u32 ev252 = 252, ev6 = 6, ev0 = 0;
            bool32 physical = gSpeciesInfo[species].baseAttack >= gSpeciesInfo[species].baseSpAttack;
            SetMonData(wildMon, MON_DATA_HP_EV, &ev6);
            SetMonData(wildMon, MON_DATA_ATK_EV, physical ? &ev252 : &ev0);
            SetMonData(wildMon, MON_DATA_SPATK_EV, physical ? &ev0 : &ev252);
            SetMonData(wildMon, MON_DATA_SPEED_EV, &ev252);
            SetMonData(wildMon, MON_DATA_DEF_EV, &ev0);
            SetMonData(wildMon, MON_DATA_SPDEF_EV, &ev0);
        }
        // SAG: custom competitive moveset (overrides the level-up set)
        {
            u32 mi, ms;
            for (mi = 0; mi < ARRAY_COUNT(sWildCompMoves); mi++)
            {
                if (sWildCompMoves[mi].species == species)
                {
                    for (ms = 0; ms < MAX_MON_MOVES; ms++)
                        SetMonMoveSlot(wildMon, sWildCompMoves[mi].moves[ms], ms);
                    break;
                }
            }
        }
        CalculateMonStats(wildMon);
    }
}

#ifdef BUGFIX
#define TRY_GET_ABILITY_INFLUENCED_WILD_MON_INDEX(wildPokemon, type, ability, ptr, count) TryGetAbilityInfluencedWildMonIndex(wildPokemon, type, ability, ptr, count)
#else
#define TRY_GET_ABILITY_INFLUENCED_WILD_MON_INDEX(wildPokemon, type, ability, ptr, count) TryGetAbilityInfluencedWildMonIndex(wildPokemon, type, ability, ptr)
#endif

bool8 TryGenerateWildMon(const struct WildPokemonInfo *wildMonInfo, enum WildPokemonArea area, u8 flags)
{
    u8 wildMonIndex = 0;
    u8 level;

    switch (area)
    {
    case WILD_AREA_LAND:
        if (TRY_GET_ABILITY_INFLUENCED_WILD_MON_INDEX(wildMonInfo->wildPokemon, TYPE_STEEL, ABILITY_MAGNET_PULL, &wildMonIndex, LAND_WILD_COUNT))
            break;
        if (TRY_GET_ABILITY_INFLUENCED_WILD_MON_INDEX(wildMonInfo->wildPokemon, TYPE_ELECTRIC, ABILITY_STATIC, &wildMonIndex, LAND_WILD_COUNT))
            break;
        if (OW_LIGHTNING_ROD >= GEN_8 && TRY_GET_ABILITY_INFLUENCED_WILD_MON_INDEX(wildMonInfo->wildPokemon, TYPE_ELECTRIC, ABILITY_LIGHTNING_ROD, &wildMonIndex, LAND_WILD_COUNT))
            break;
        if (OW_FLASH_FIRE >= GEN_8 && TRY_GET_ABILITY_INFLUENCED_WILD_MON_INDEX(wildMonInfo->wildPokemon, TYPE_FIRE, ABILITY_FLASH_FIRE, &wildMonIndex, LAND_WILD_COUNT))
            break;
        if (OW_HARVEST >= GEN_8 && TRY_GET_ABILITY_INFLUENCED_WILD_MON_INDEX(wildMonInfo->wildPokemon, TYPE_GRASS, ABILITY_HARVEST, &wildMonIndex, LAND_WILD_COUNT))
            break;
        if (OW_STORM_DRAIN >= GEN_8 && TRY_GET_ABILITY_INFLUENCED_WILD_MON_INDEX(wildMonInfo->wildPokemon, TYPE_WATER, ABILITY_STORM_DRAIN, &wildMonIndex, LAND_WILD_COUNT))
            break;

        wildMonIndex = ChooseWildMonIndex_Land();
        break;
    case WILD_AREA_WATER:
        if (TRY_GET_ABILITY_INFLUENCED_WILD_MON_INDEX(wildMonInfo->wildPokemon, TYPE_STEEL, ABILITY_MAGNET_PULL, &wildMonIndex, WATER_WILD_COUNT))
            break;
        if (TRY_GET_ABILITY_INFLUENCED_WILD_MON_INDEX(wildMonInfo->wildPokemon, TYPE_ELECTRIC, ABILITY_STATIC, &wildMonIndex, WATER_WILD_COUNT))
            break;
        if (OW_LIGHTNING_ROD >= GEN_8 && TRY_GET_ABILITY_INFLUENCED_WILD_MON_INDEX(wildMonInfo->wildPokemon, TYPE_ELECTRIC, ABILITY_LIGHTNING_ROD, &wildMonIndex, WATER_WILD_COUNT))
            break;
        if (OW_FLASH_FIRE >= GEN_8 && TRY_GET_ABILITY_INFLUENCED_WILD_MON_INDEX(wildMonInfo->wildPokemon, TYPE_FIRE, ABILITY_FLASH_FIRE, &wildMonIndex, WATER_WILD_COUNT))
            break;
        if (OW_HARVEST >= GEN_8 && TRY_GET_ABILITY_INFLUENCED_WILD_MON_INDEX(wildMonInfo->wildPokemon, TYPE_GRASS, ABILITY_HARVEST, &wildMonIndex, WATER_WILD_COUNT))
            break;
        if (OW_STORM_DRAIN >= GEN_8 && TRY_GET_ABILITY_INFLUENCED_WILD_MON_INDEX(wildMonInfo->wildPokemon, TYPE_WATER, ABILITY_STORM_DRAIN, &wildMonIndex, WATER_WILD_COUNT))
            break;

        wildMonIndex = ChooseWildMonIndex_Water();
        break;
    case WILD_AREA_ROCKS:
        wildMonIndex = ChooseWildMonIndex_Rocks();
        break;
    default:
    case WILD_AREA_FISHING:
    case WILD_AREA_HIDDEN:
        break;
    }

    level = ChooseWildMonLevel(wildMonInfo->wildPokemon, wildMonIndex, area);
    if (flags & WILD_CHECK_REPEL && !IsWildLevelAllowedByRepel(level))
        return FALSE;
    if (gMapHeader.mapLayoutId != LAYOUT_BATTLE_FRONTIER_BATTLE_PIKE_ROOM_WILD_MONS && flags & WILD_CHECK_KEEN_EYE && !IsAbilityAllowingEncounter(level))
        return FALSE;

    CreateWildMon(wildMonInfo->wildPokemon[wildMonIndex].species, level);
    return TRUE;
}

static u16 GenerateFishingWildMon(const struct WildPokemonInfo *wildMonInfo, u8 rod)
{
    u8 wildMonIndex = ChooseWildMonIndex_Fishing(rod);
    enum Species wildMonSpecies = wildMonInfo->wildPokemon[wildMonIndex].species;
    u8 level = ChooseWildMonLevel(wildMonInfo->wildPokemon, wildMonIndex, WILD_AREA_FISHING);

    UpdateChainFishingStreak();
    CreateWildMon(wildMonSpecies, level);
    return wildMonSpecies;
}

bool8 SetUpMassOutbreakEncounter(u8 flags)
{
    u16 i;

    if (flags & WILD_CHECK_REPEL && !IsWildLevelAllowedByRepel(gSaveBlock1Ptr->outbreakPokemonLevel))
        return FALSE;

    CreateWildMon(gSaveBlock1Ptr->outbreakPokemonSpecies, gSaveBlock1Ptr->outbreakPokemonLevel);
    for (i = 0; i < MAX_MON_MOVES; i++)
        SetMonMoveSlot(&gParties[B_TRAINER_OPPONENT_A][0], gSaveBlock1Ptr->outbreakPokemonMoves[i], i);

    return TRUE;
}

bool8 DoMassOutbreakEncounterTest(void)
{
    if (gSaveBlock1Ptr->outbreakPokemonSpecies != SPECIES_NONE
     && gSaveBlock1Ptr->location.mapNum == gSaveBlock1Ptr->outbreakLocationMapNum
     && gSaveBlock1Ptr->location.mapGroup == gSaveBlock1Ptr->outbreakLocationMapGroup)
    {
        if (Random() % 100 < gSaveBlock1Ptr->outbreakPokemonProbability)
            return TRUE;
    }
    return FALSE;
}

static bool8 EncounterOddsCheck(u16 encounterRate)
{
    if (Random() % MAX_ENCOUNTER_RATE < encounterRate)
        return TRUE;
    else
        return FALSE;
}

// Returns true if it will try to create a wild encounter.
static bool8 WildEncounterCheck(u32 encounterRate, bool8 ignoreAbility)
{
    encounterRate *= 16;
    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_MACH_BIKE | PLAYER_AVATAR_FLAG_ACRO_BIKE))
        encounterRate = encounterRate * 80 / 100;
    ApplyFluteEncounterRateMod(&encounterRate);
    ApplyCleanseTagEncounterRateMod(&encounterRate);
    if (LURE_STEP_COUNT != 0)
        encounterRate *= 2;
    if (!ignoreAbility && !GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SANITY_IS_EGG))
    {
        enum Ability ability = GetMonAbility(&gParties[B_TRAINER_PLAYER][0]);

        if (ability == ABILITY_STENCH && gMapHeader.mapLayoutId == LAYOUT_BATTLE_FRONTIER_BATTLE_PYRAMID_FLOOR)
            encounterRate = encounterRate * 3 / 4;
        else if (ability == ABILITY_STENCH)
            encounterRate /= 2;
        else if (ability == ABILITY_ILLUMINATE)
            encounterRate *= 2;
        else if (ability == ABILITY_WHITE_SMOKE)
            encounterRate /= 2;
        else if (ability == ABILITY_ARENA_TRAP)
            encounterRate *= 2;
        else if (ability == ABILITY_SAND_VEIL && gSaveBlock1Ptr->weather == WEATHER_SANDSTORM)
            encounterRate /= 2;
        else if (ability == ABILITY_SNOW_CLOAK && gSaveBlock1Ptr->weather == WEATHER_SNOW)
            encounterRate /= 2;
        else if (ability == ABILITY_QUICK_FEET)
            encounterRate /= 2;
        else if (ability == ABILITY_INFILTRATOR && OW_INFILTRATOR >= GEN_8)
            encounterRate /= 2;
        else if (ability == ABILITY_NO_GUARD)
            encounterRate *= 2;
    }
    if (encounterRate > MAX_ENCOUNTER_RATE)
        encounterRate = MAX_ENCOUNTER_RATE;
    return EncounterOddsCheck(encounterRate);
}

// When you first step on a different type of metatile, there's a 40% chance it
// skips the wild encounter check entirely.
static bool8 AllowWildCheckOnNewMetatile(void)
{
    if (Random() % 100 >= 60)
        return FALSE;
    else
        return TRUE;
}

bool8 AreLegendariesInSootopolisPreventingEncounters(void)
{
    if (gSaveBlock1Ptr->location.mapGroup != MAP_GROUP(MAP_SOOTOPOLIS_CITY)
     || gSaveBlock1Ptr->location.mapNum != MAP_NUM(MAP_SOOTOPOLIS_CITY))
    {
        return FALSE;
    }

    return FlagGet(FLAG_LEGENDARIES_IN_SOOTOPOLIS);
}

bool8 StandardWildEncounter(u16 curMetatileBehavior, u16 prevMetatileBehavior)
{
    u32 headerId;
    enum TimeOfDay timeOfDay;
    struct Roamer *roamer;

    if (sWildEncountersDisabled == TRUE)
        return FALSE;

    headerId = GetCurrentMapWildMonHeaderId();
    if (headerId == HEADER_NONE)
    {
        if (gMapHeader.mapLayoutId == LAYOUT_BATTLE_FRONTIER_BATTLE_PIKE_ROOM_WILD_MONS)
        {
            headerId = GetBattlePikeWildMonHeaderId();
            timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_LAND);

            if (prevMetatileBehavior != curMetatileBehavior && !AllowWildCheckOnNewMetatile())
                return FALSE;
            else if (WildEncounterCheck(gBattlePikeWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo->encounterRate, FALSE) != TRUE)
                return FALSE;
            else if (TryGenerateWildMon(gBattlePikeWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo, WILD_AREA_LAND, WILD_CHECK_KEEN_EYE) != TRUE)
                return FALSE;
            else if (!TryGenerateBattlePikeWildMon(TRUE))
                return FALSE;

            BattleSetup_StartBattlePikeWildBattle();
            return TRUE;
        }
        if (gMapHeader.mapLayoutId == LAYOUT_BATTLE_FRONTIER_BATTLE_PYRAMID_FLOOR)
        {
            headerId = gSaveBlock2Ptr->frontier.curChallengeBattleNum;
            timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_LAND);

            if (prevMetatileBehavior != curMetatileBehavior && !AllowWildCheckOnNewMetatile())
                return FALSE;
            else if (WildEncounterCheck(gBattlePyramidWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo->encounterRate, FALSE) != TRUE)
                return FALSE;
            else if (TryGenerateWildMon(gBattlePyramidWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo, WILD_AREA_LAND, WILD_CHECK_KEEN_EYE) != TRUE)
                return FALSE;

            GenerateBattlePyramidWildMon(SPECIES_NONE);
            BattleSetup_StartWildBattle();
            return TRUE;
        }
    }
    else
    {
        if (MetatileBehavior_IsLandWildEncounter(curMetatileBehavior) == TRUE)
        {
            timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_LAND);

            if (gWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo == NULL)
                return FALSE;
            else if (prevMetatileBehavior != curMetatileBehavior && !AllowWildCheckOnNewMetatile())
                return FALSE;
            else if (WildEncounterCheck(gWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo->encounterRate, FALSE) != TRUE)
                return FALSE;

            if (TryStartRoamerEncounter())
            {
                roamer = &gSaveBlock1Ptr->roamer[gEncounteredRoamerIndex];
                if (!IsWildLevelAllowedByRepel(roamer->level))
                    return FALSE;

                BattleSetup_StartRoamerBattle();
                return TRUE;
            }
            else
            {
                if (DoMassOutbreakEncounterTest() == TRUE && SetUpMassOutbreakEncounter(WILD_CHECK_REPEL | WILD_CHECK_KEEN_EYE) == TRUE)
                {
                    BattleSetup_StartWildBattle();
                    return TRUE;
                }

                // try a regular wild land encounter
                if (TryGenerateWildMon(gWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo, WILD_AREA_LAND, WILD_CHECK_REPEL | WILD_CHECK_KEEN_EYE) == TRUE)
                {
                    if (TryDoDoubleWildBattle())
                    {
                        struct Pokemon mon1 = gParties[B_TRAINER_OPPONENT_A][0];
                        TryGenerateWildMon(gWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo, WILD_AREA_LAND, WILD_CHECK_KEEN_EYE);
                        gParties[B_TRAINER_OPPONENT_A][1] = mon1;
                        BattleSetup_StartDoubleWildBattle();
                    }
                    else
                    {
                        BattleSetup_StartWildBattle();
                    }
                    return TRUE;
                }

                return FALSE;
            }
        }
        else if (MetatileBehavior_IsWaterWildEncounter(curMetatileBehavior) == TRUE
                 || (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING) && MetatileBehavior_IsBridgeOverWater(curMetatileBehavior) == TRUE))
        {
            timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_WATER);

            if (AreLegendariesInSootopolisPreventingEncounters() == TRUE)
                return FALSE;
            else if (gWildMonHeaders[headerId].encounterTypes[timeOfDay].waterMonsInfo == NULL)
                return FALSE;
            else if (prevMetatileBehavior != curMetatileBehavior && !AllowWildCheckOnNewMetatile())
                return FALSE;
            else if (WildEncounterCheck(gWildMonHeaders[headerId].encounterTypes[timeOfDay].waterMonsInfo->encounterRate, FALSE) != TRUE)
                return FALSE;

            if (TryStartRoamerEncounter())
            {
                roamer = &gSaveBlock1Ptr->roamer[gEncounteredRoamerIndex];
                if (!IsWildLevelAllowedByRepel(roamer->level))
                    return FALSE;

                BattleSetup_StartRoamerBattle();
                return TRUE;
            }
            else // try a regular surfing encounter
            {
                if (TryGenerateWildMon(gWildMonHeaders[headerId].encounterTypes[timeOfDay].waterMonsInfo, WILD_AREA_WATER, WILD_CHECK_REPEL | WILD_CHECK_KEEN_EYE) == TRUE)
                {
                    gIsSurfingEncounter = TRUE;
                    if (TryDoDoubleWildBattle())
                    {
                        struct Pokemon mon1 = gParties[B_TRAINER_OPPONENT_A][0];
                        TryGenerateWildMon(gWildMonHeaders[headerId].encounterTypes[timeOfDay].waterMonsInfo, WILD_AREA_WATER, WILD_CHECK_KEEN_EYE);
                        gParties[B_TRAINER_OPPONENT_A][1] = mon1;
                        BattleSetup_StartDoubleWildBattle();
                    }
                    else
                    {
                        BattleSetup_StartWildBattle();
                    }
                    return TRUE;
                }

                return FALSE;
            }
        }
    }

    return FALSE;
}

void RockSmashWildEncounter(void)
{
    u32 headerId = GetCurrentMapWildMonHeaderId();
    enum TimeOfDay timeOfDay;

    if (headerId != HEADER_NONE)
    {
        timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_ROCKS);

        const struct WildPokemonInfo *wildPokemonInfo = gWildMonHeaders[headerId].encounterTypes[timeOfDay].rockSmashMonsInfo;

        if (wildPokemonInfo == NULL)
        {
            gSpecialVar_Result = FALSE;
        }
        else if (WildEncounterCheck(wildPokemonInfo->encounterRate, TRUE) == TRUE
         && TryGenerateWildMon(wildPokemonInfo, WILD_AREA_ROCKS, WILD_CHECK_REPEL | WILD_CHECK_KEEN_EYE) == TRUE)
        {
            if (TryDoDoubleWildBattle())
            {
                struct Pokemon mon1 = gParties[B_TRAINER_OPPONENT_A][0];
                TryGenerateWildMon(wildPokemonInfo, WILD_AREA_ROCKS, WILD_CHECK_REPEL | WILD_CHECK_KEEN_EYE);
                gParties[B_TRAINER_OPPONENT_A][1] = mon1;
                BattleSetup_StartDoubleWildBattle();
                gSpecialVar_Result = TRUE;
            }
            else {
                BattleSetup_StartWildBattle();
                gSpecialVar_Result = TRUE;
            }
        }
        else
        {
            gSpecialVar_Result = FALSE;
        }
    }
    else
    {
        gSpecialVar_Result = FALSE;
    }
}

bool8 SweetScentWildEncounter(void)
{
    s16 x, y;
    u32 headerId;
    enum TimeOfDay timeOfDay;

    PlayerGetDestCoords(&x, &y);
    headerId = GetCurrentMapWildMonHeaderId();
    if (headerId == HEADER_NONE)
    {
        if (gMapHeader.mapLayoutId == LAYOUT_BATTLE_FRONTIER_BATTLE_PIKE_ROOM_WILD_MONS)
        {
            headerId = GetBattlePikeWildMonHeaderId();
            timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_LAND);

            if (TryGenerateWildMon(gBattlePikeWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo, WILD_AREA_LAND, 0) != TRUE)
                return FALSE;

            TryGenerateBattlePikeWildMon(FALSE);
            BattleSetup_StartBattlePikeWildBattle();
            return TRUE;
        }
        if (gMapHeader.mapLayoutId == LAYOUT_BATTLE_FRONTIER_BATTLE_PYRAMID_FLOOR)
        {
            headerId = gSaveBlock2Ptr->frontier.curChallengeBattleNum;
            timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_LAND);

            if (TryGenerateWildMon(gBattlePyramidWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo, WILD_AREA_LAND, 0) != TRUE)
                return FALSE;

            GenerateBattlePyramidWildMon(SPECIES_NONE);
            BattleSetup_StartWildBattle();
            return TRUE;
        }
    }
    else
    {
        if (MetatileBehavior_IsLandWildEncounter(MapGridGetMetatileBehaviorAt(x, y)) == TRUE)
        {
            timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_LAND);

            if (gWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo == NULL)
                return FALSE;

            if (TryStartRoamerEncounter())
            {
                BattleSetup_StartRoamerBattle();
                return TRUE;
            }

            if (DoMassOutbreakEncounterTest() == TRUE)
                SetUpMassOutbreakEncounter(0);
            else
                TryGenerateWildMon(gWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo, WILD_AREA_LAND, 0);

            BattleSetup_StartWildBattle();
            return TRUE;
        }
        else if (MetatileBehavior_IsWaterWildEncounter(MapGridGetMetatileBehaviorAt(x, y)) == TRUE)
        {
            timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_WATER);

            if (AreLegendariesInSootopolisPreventingEncounters() == TRUE)
                return FALSE;
            if (gWildMonHeaders[headerId].encounterTypes[timeOfDay].waterMonsInfo == NULL)
                return FALSE;

            if (TryStartRoamerEncounter())
            {
                BattleSetup_StartRoamerBattle();
                return TRUE;
            }

            TryGenerateWildMon(gWildMonHeaders[headerId].encounterTypes[timeOfDay].waterMonsInfo, WILD_AREA_WATER, 0);
            BattleSetup_StartWildBattle();
            return TRUE;
        }
    }

    return FALSE;
}

bool8 DoesCurrentMapHaveFishingMons(void)
{
    u32 headerId = GetCurrentMapWildMonHeaderId();
    enum TimeOfDay timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_FISHING);

    if (headerId != HEADER_NONE && gWildMonHeaders[headerId].encounterTypes[timeOfDay].fishingMonsInfo != NULL)
        return TRUE;
    else
        return FALSE;
}

void FishingWildEncounter(u8 rod)
{
    enum Species species;
    u32 headerId;
    s16 x, y;
    enum TimeOfDay timeOfDay;

    gIsFishingEncounter = TRUE;
    GetXYCoordsOneStepInFrontOfPlayer(&x, &y);
    if (CheckFeebasAtCoords(x, y) == TRUE)
    {
        u8 level = ChooseWildMonLevel(&gWildFeebas, 0, WILD_AREA_FISHING);

        species = gWildFeebas.species;
        CreateWildMon(species, level);
    }
    else
    {
        headerId = GetCurrentMapWildMonHeaderId();
        timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_FISHING);
        species = GenerateFishingWildMon(gWildMonHeaders[headerId].encounterTypes[timeOfDay].fishingMonsInfo, rod);
    }

    IncrementGameStat(GAME_STAT_FISHING_ENCOUNTERS);
    SetPokemonAnglerSpecies(species);
    BattleSetup_StartWildBattle();
}

u16 GetLocalWildMon(bool8 *isWaterMon)
{
    u32 headerId;
    enum TimeOfDay timeOfDay;
    const struct WildPokemonInfo *landMonsInfo;
    const struct WildPokemonInfo *waterMonsInfo;

    *isWaterMon = FALSE;
    headerId = GetCurrentMapWildMonHeaderId();
    if (headerId == HEADER_NONE)
        return SPECIES_NONE;

    timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_LAND);
    landMonsInfo = gWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo;

    timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_WATER);
    waterMonsInfo = gWildMonHeaders[headerId].encounterTypes[timeOfDay].waterMonsInfo;

    // Neither
    if (landMonsInfo == NULL && waterMonsInfo == NULL)
        return SPECIES_NONE;
    // Land Pokémon
    else if (landMonsInfo != NULL && waterMonsInfo == NULL)
        return landMonsInfo->wildPokemon[ChooseWildMonIndex_Land()].species;
    // Water Pokémon
    else if (landMonsInfo == NULL && waterMonsInfo != NULL)
    {
        *isWaterMon = TRUE;
        return waterMonsInfo->wildPokemon[ChooseWildMonIndex_Water()].species;
    }
    // Either land or water Pokémon
    if ((Random() % 100) < 80)
    {
        return landMonsInfo->wildPokemon[ChooseWildMonIndex_Land()].species;
    }
    else
    {
        *isWaterMon = TRUE;
        return waterMonsInfo->wildPokemon[ChooseWildMonIndex_Water()].species;
    }
}

u16 GetLocalWaterMon(void)
{
    u32 headerId = GetCurrentMapWildMonHeaderId();
    enum TimeOfDay timeOfDay;

    if (headerId != HEADER_NONE)
    {
        timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_WATER);

        const struct WildPokemonInfo *waterMonsInfo = gWildMonHeaders[headerId].encounterTypes[timeOfDay].waterMonsInfo;

        if (waterMonsInfo)
            return waterMonsInfo->wildPokemon[ChooseWildMonIndex_Water()].species;
    }
    return SPECIES_NONE;
}

bool8 UpdateRepelCounter(void)
{
    u16 repelLureVar = VarGet(VAR_REPEL_STEP_COUNT);
    u16 steps = REPEL_LURE_STEPS(repelLureVar);
    bool32 isLure = IS_LAST_USED_LURE(repelLureVar);

    if (InBattlePike() || CurrentBattlePyramidLocation() != PYRAMID_LOCATION_NONE)
        return FALSE;
    if (InUnionRoom() == TRUE)
        return FALSE;

    if (steps != 0)
    {
        steps--;
        if (!isLure)
        {
            VarSet(VAR_REPEL_STEP_COUNT, steps);
            if (steps == 0)
            {
                ScriptContext_SetupScript(EventScript_SprayWoreOff);
                return TRUE;
            }
        }
        else
        {
            VarSet(VAR_REPEL_STEP_COUNT, steps | REPEL_LURE_MASK);
            if (steps == 0)
            {
                ScriptContext_SetupScript(EventScript_SprayWoreOff);
                return TRUE;
            }
        }

    }
    return FALSE;
}

bool8 IsWildLevelAllowedByRepel(u8 wildLevel)
{
    u8 i;

    if (!REPEL_STEP_COUNT)
        return TRUE;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (I_REPEL_INCLUDE_FAINTED == GEN_1 || I_REPEL_INCLUDE_FAINTED >= GEN_6 || GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_HP))
        {
            if (!GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_IS_EGG))
                return wildLevel >= GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_LEVEL);
        }
    }

    return FALSE;
}

bool8 IsAbilityAllowingEncounter(u8 level)
{
    enum Ability ability;

    if (GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SANITY_IS_EGG))
        return TRUE;

    ability = GetMonAbility(&gParties[B_TRAINER_PLAYER][0]);
    if (ability == ABILITY_KEEN_EYE || ability == ABILITY_INTIMIDATE)
    {
        u8 playerMonLevel = GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_LEVEL);
        if (playerMonLevel > 5 && level <= playerMonLevel - 5 && !(Random() % 2))
            return FALSE;
    }

    return TRUE;
}

static bool8 TryGetRandomWildMonIndexByType(const struct WildPokemon *wildMon, enum Type type, u8 numMon, u8 *monIndex)
{
    u8 validIndexes[numMon]; // variable length array, an interesting feature
    u8 i, validMonCount;

    for (i = 0; i < numMon; i++)
        validIndexes[i] = 0;

    for (validMonCount = 0, i = 0; i < numMon; i++)
    {
        if (GetSpeciesType(wildMon[i].species, 0) == type || GetSpeciesType(wildMon[i].species, 1) == type)
            validIndexes[validMonCount++] = i;
    }

    if (validMonCount == 0 || validMonCount == numMon)
        return FALSE;

    *monIndex = validIndexes[Random() % validMonCount];
    return TRUE;
}

#include "data.h"

static u8 GetMaxLevelOfSpeciesInWildTable(const struct WildPokemon *wildMon, enum Species species, enum WildPokemonArea area)
{
    u8 i, maxLevel = 0, numMon = 0;

    switch (area)
    {
    case WILD_AREA_LAND:
        numMon = LAND_WILD_COUNT;
        break;
    case WILD_AREA_WATER:
        numMon = WATER_WILD_COUNT;
        break;
    case WILD_AREA_ROCKS:
        numMon = ROCK_WILD_COUNT;
        break;
    default:
    case WILD_AREA_FISHING:
    case WILD_AREA_HIDDEN:
        break;
    }

    for (i = 0; i < numMon; i++)
    {
        if (wildMon[i].species == species && wildMon[i].maxLevel > maxLevel)
            maxLevel = wildMon[i].maxLevel;
    }

    return maxLevel;
}

#ifdef BUGFIX
static bool8 TryGetAbilityInfluencedWildMonIndex(const struct WildPokemon *wildMon, enum Type type, enum Ability ability, u8 *monIndex, u32 size)
#else
static bool8 TryGetAbilityInfluencedWildMonIndex(const struct WildPokemon *wildMon, enum Type type, enum Ability ability, u8 *monIndex)
#endif
{
    if (GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SANITY_IS_EGG))
        return FALSE;
    else if (GetMonAbility(&gParties[B_TRAINER_PLAYER][0]) != ability)
        return FALSE;
    else if (Random() % 2 != 0)
        return FALSE;

#ifdef BUGFIX
    return TryGetRandomWildMonIndexByType(wildMon, type, size, monIndex);
#else
    return TryGetRandomWildMonIndexByType(wildMon, type, LAND_WILD_COUNT, monIndex);
#endif
}

static void ApplyFluteEncounterRateMod(u32 *encRate)
{
    if (FlagGet(FLAG_SYS_ENC_UP_ITEM) == TRUE)
        *encRate += *encRate / 2;
    else if (FlagGet(FLAG_SYS_ENC_DOWN_ITEM) == TRUE)
        *encRate = *encRate / 2;
}

static void ApplyCleanseTagEncounterRateMod(u32 *encRate)
{
    enum Item heldItem = GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HELD_ITEM);
    if (gItemsInfo[heldItem].holdEffect == HOLD_EFFECT_REPEL)
        *encRate = *encRate * 2 / 3;
}

bool8 TryDoDoubleWildBattle(void)
{
    if (GetSafariZoneFlag()
      || (WE_DOUBLE_WILD_REQUIRE_2_MONS && GetMonsStateToDoubles() != PLAYER_HAS_TWO_USABLE_MONS))
        return FALSE;
    if (FollowerNPCIsBattlePartner() && FNPC_FLAG_PARTNER_WILD_BATTLES != 0
     && (FNPC_FLAG_PARTNER_WILD_BATTLES == FNPC_ALWAYS || FlagGet(FNPC_FLAG_PARTNER_WILD_BATTLES)) && FNPC_NPC_FOLLOWER_WILD_BATTLE_VS_2 == TRUE)
        return TRUE;
    else if (FlagGet(WE_FLAG_FORCE_DOUBLE_WILD))
        return TRUE;
    else if (RandomPercentage(RNG_NONE, WE_DOUBLE_WILD_CHANCE))
        return TRUE;
    return FALSE;
}

bool8 StandardWildEncounter_Debug(void)
{
    u32 headerId = GetCurrentMapWildMonHeaderId();
    enum TimeOfDay timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_LAND);

    if (TryGenerateWildMon(gWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo, WILD_AREA_LAND, 0) != TRUE)
        return FALSE;

    DoStandardWildBattle_Debug();
    return TRUE;
}

u32 ChooseHiddenMonIndex(void)
{
    #ifdef ENCOUNTER_CHANCE_HIDDEN_MONS_TOTAL
        u8 rand = Random() % ENCOUNTER_CHANCE_HIDDEN_MONS_TOTAL;

        if (rand < ENCOUNTER_CHANCE_HIDDEN_MONS_SLOT_0)
            return 0;
        else if (rand >= ENCOUNTER_CHANCE_HIDDEN_MONS_SLOT_0 && rand < ENCOUNTER_CHANCE_HIDDEN_MONS_SLOT_1)
            return 1;
        else
            return 2;
    #else
        return 0xFF;
    #endif
}

bool32 MapHasNoEncounterData(void)
{
    return (GetCurrentMapWildMonHeaderId() == HEADER_NONE);
}
