# SAG Romhack — Build Status & Resume Guide

Personalized total-conversion Pokemon GBA romhack (birthday gift for Shane "SAG").
16-gym NYC/tri-state region, roast tone, competitive battles.
This fork = `wolfsportsalphany/pokeemerald-expansion`. Design bible: `SAG_BIRTHDAY_PLAN.md`.

## Approach (confirmed with user)
Keep the pokeemerald **engine**; build the **world 100% from scratch**.
No truck/Birch intro, no starter selection. Player spawns directly in Hudson Yards
with Eevee/Gastly/Mankey.

## CURRENT STATE — v0.10.0 (latest built ROM)
- **Hudson Yards** = the `MAP_LITTLEROOT_TOWN` slot, re-laid with the urban **Rustboro**
  secondary tileset, 26x20. Player spawns at tile **(13,14)** with the party.
  The map currently has NO warps/NPCs/scripts (cleared) -> you can walk the block but
  cannot leave town or enter buildings yet.
- `src/new_game.c`: `WarpToTruck()` redirects spawn to LITTLEROOT_TOWN(13,14);
  `SAG_GiveStartingLoadout()` gives the party + items + `FlagSet(FLAG_SYS_POKEMON_GET)`.
- `data/maps/LittlerootTown/scripts.inc` = empty stub (`LittlerootTown_MapScripts:: .byte 0`).
- Region names -> NYC in `src/data/region_map/region_map_sections.json`
  (Midtown, Upper East Side, The Bronx, Montauk, Chelsea Piers, Williamsburg, ...).
- Trainers (`src/data/trainers.party`): Gym1 ALTORELLI(Dark), Gym2 SOKOTCH(Psychic),
  Gym3 RANDALL(Fight/Normal), Gym4 RANDOLPH(Ghost), Gym5 JUDGE(Flying),
  Gym6 THE DEAN(Steel), Gym8 GIOVANNI(Ground). **Gym7 (Tate&Liza double) NOT done.**
- Systems: MAX_MONEY, reusable TMs, all TMs/HMs + 99 vitamins/items at start,
  wild mons get perfect IVs + hidden ability.
- ROMs: `versions/v0.1.0 ... v0.10.0`; also copied to
  `C:\Users\jared\Gaming\Emulation\Games\VisualBoyAdvance\`.

## TOOLCHAIN (tools/sag/)
- `maptool.py show <map.bin> <w> <h>` ; `maptool.py stamp <map.bin> <w> <h> <sx sy bw bh dx dy>`
- `render.py <map.bin> <w> <h> <primdir> <secdir> <out.png>`
  -> render to `C:/Users/jared/sag_render/X.png`, then **Read the PNG to SEE the map**.
- Map cell = u16: metatile=v&0x3FF, collision=(v>>10)&3, elev=(v>>12)&0xF.
  metatile <512 = primary tileset, >=512 = secondary.
- tileset dir from symbol: gTileset_Rustboro -> data/tilesets/secondary/rustboro, etc.

## BUILD & SHIP (per change)
    cd /tmp/emr; git fetch --depth 1 origin master -q; git checkout -B master origin/master -q
    git checkout -b feat/X ; <edits via sed/awk/python> ; git add -A; git commit -m "..."
    git push -u origin feat/X ; gh pr create ... ; gh pr merge <url> --merge --delete-branch
    REPO=wolfsportsalphany/pokeemerald-expansion
    gh workflow run release-rom.yml --repo $REPO --ref master -f version=vX.Y.Z
    sleep 15; RID=$(gh run list --repo $REPO --workflow release-rom.yml --limit 1 --json databaseId -q '.[0].databaseId')
    gh run watch $RID --repo $REPO --exit-status
    gh run download $RID --repo $REPO -n sag-rom -D /tmp/rom
    mkdir -p versions/vX.Y.Z-name; cp /tmp/rom/*.gba versions/vX.Y.Z-name/SAG-vX.Y.Z.gba
    git add -f versions/... ; git commit -m "..." ; git push origin master
    cp /tmp/rom/*.gba "/c/Users/jared/Gaming/Emulation/Games/VisualBoyAdvance/SAG-vX.Y.Z.gba"

## GOTCHAS
- Edits happen in `/tmp/emr` via the Monitor bash tool (Read/Edit tools can't see it).
  Render PNGs to `C:\Users\...` so the Read tool can view them.
- `*.gba` is gitignored -> `git add -f`.
- TRAINER_NAME_LENGTH=10, MAP_NAME_LENGTH=16 (overflow = "excess elements in array initializer").
- Clearing a map's `object_events` -> you MUST also blank its `scripts.inc` (it references
  auto-generated `LOCALID_*`), to `<Map>_MapScripts::` + `.byte 0`.
- Don't pipe `make` through `tail` (masks compile errors) — already fixed in release-rom.yml.
- Build ~20 min. Use Monitor timeout_ms=3000000 for the watch; TaskOutput block max=600000ms.
- ROM changes only show on a **FRESH new game** (old saves/save-states mask map edits).

## TO RESUME (fresh session)
    # if /tmp/emr is gone:
    git clone https://github.com/wolfsportsalphany/pokeemerald-expansion /tmp/emr
    cd /tmp/emr && git checkout master && mkdir -p /c/Users/jared/sag_render
Then read this file + `SAG_BIRTHDAY_PLAN.md`. gh + git are already authenticated.

## NEXT STEPS (in order)
1. Make Shane's apartment **enterable**: add a warp at the apartment door -> an interior
   map (reuse a Pokemon Center / building interior), + warp back to (13,14)-ish.
2. Add a **south exit** warp from Hudson Yards -> the next map.
3. Build **Times Square** as the next map (new layout + connect). Continue the region
   map-by-map toward the full 16-gym graph.
4. **Gym 7** leader (Jesse, Fairy, DOUBLE battle — TRAINER_TATE_AND_LIZA_1).
5. Rival **Justin**'s evolving teams (TRAINER_MAY_*/BRENDAN_*).
6. Dialogue/story scripts (intro, gym leaders, the roast).
7. Populate NPCs: doormen, family, the Canon 6 gift mons.

## UPDATE @ v0.14.0
World = 3 connected maps: **Hudson Yards (Littleroot)** -> south -> **Times Square (Oldale)** -> south -> **Upper East Side (Dewford)**. Gym 1 Altorelli (RustboroGym) + a Pokemon Center wired in Times Square. Tools: tools/sag/{maptool,render,swatch}.py.

### HARD LESSONS (Dewford cost ~6 builds)
- **Do NOT clear/blank entangled Hoenn town slots.** Dewford is in Mr. Briney's boat questline; other maps (Route104) reference LOCALID_DEWFORD_BRINEY/BOAT + its texts, so clearing its object_events/scripts.inc = link errors. Oldale + Littleroot were self-contained (safe).
- **Clone is shallow (--depth 1).** `HEAD~1`/old commits don't exist until `git fetch --unshallow origin`.
- **To repair a broken reused slot:** restore its map.json+scripts.inc from a KNOWN-GOOD commit, verified locally with `grep -c walk_fastest_right` == 0 (ancient fork commits use renamed movement macros that the current assembler rejects). Working Dewford = commit c27930ea.
- **GOING FORWARD: use FRESH REGISTERED MAP IDs** (new data/maps/<Name> + map_groups.json entry), NOT repurposed town slots — zero entanglement.
- **Pre-verify locally before the ~20-min build:** grep bad macros, json valid, collision reachability (tools print collision ASCII), render PNG.
