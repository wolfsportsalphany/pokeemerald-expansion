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

## UPDATE @ v0.22 (autonomous loop)
World = 5 connected maps S-ward: Hudson Yards(Littleroot) -> Times Square(Oldale) -> Upper East Side(Dewford) -> Randalls Island(Mauville) -> Riverdale(Lavaridge). Gyms 1-4 wired (Altorelli/Sokotch/Randall/Randolph) + PC in Times Square. v0.23 adds The Bronx(Petalburg)+Gym5 Judge.
ALL 16 gym + E4 + Champion TEAMS done. Gyms 9-16 PARKED in rematch slots: ROXANNE_2=Linda BRAWLY_2=Sherm WATTSON_2=Brielle FLANNERY_2=Roman NORMAN_2=Murphy WINONA_2=Oak TATE_AND_LIZA_2=Kenny&Ricky(double) JUAN_2=Justin. Gym7 Jesse=TATE_AND_LIZA_1.
### PATTERNS (critical)
- RESILIENT BUILDS: short Monitors only. Dispatch with `gh workflow run` (NO `gh run watch` - long watch-Monitors die on loop wakeups). Each loop wakeup: poll `gh run view <id>`, download+commit+ship when success.
- BIG .party batches: Python heredoc writes block files to C:/Users/jared/sag_render/*.txt (Windows Python CANNOT write /tmp), MSYS awk reads /c/Users/.../X.txt + writes /tmp/tp. Avoids bash quoting.
- MAP EXTENSION: keep-everything reuse (do NOT clear NPCs, just clamp x/y<=25/19). Rustboro crop + force avenue cols 12-13 = PAV 0x3309. Connections up/down offset 0. Gym door from a NATURAL Rustboro door (doors at Rustboro (11,15),(12,15),(33,19),(30,28),(13,30),(27,34),(9,38),(26,46),(5,51)) - pick crop windows containing one, don't overwrite with avenue.
- GYM WIRING: auto-detect entrance map via glob data/maps/<Town>_Gym*/ (entrance = single _Gym or _1F, NOT _B1F/_2F); dest_map = MAP_<that>. Repoint that gym map's exit warp (dest MAP_<TOWN>) -> the town door warp index. Multi-floor gyms (Lavaridge) use _GYM_1F.

## MILESTONE @ v0.29 — ALL 16 GYMS + FULL ARC PLAYABLE
World = ~16 connected maps S-ward (Hudson Yards ... Montauk [Act I, gyms 1-8 in real interiors w/ badges] -> Paramus -> Verdanturf(Philly) -> Fallarbor(Atlanta) -> Pacifidlog(Hallandale) -> Slateport(IMG) -> Route119(Killington) -> Route120(Camp Pontiac) -> EverGrande(Central Synag/Gym16 Justin) -> League).
- Gyms 1-8: reused Hoenn gym INTERIORS (grant FLAG_BADGE01-08 -> opens E4 gate). Gyms 9-16: OVERWORLD TRAINERS (object_event trainer_type NORMAL + trainerbattle_single/_double, parked rematch-slot teams). Gym 15 = trainerbattle_double.
- Core loop COMPLETE: 16 gyms -> E4 (Sidney/Phoebe/Glacia/Drake = Cali/Dad/Mom/Steven) -> Champion (Wallace=Jared). All teams done.
### REMAINING (harder / needs human or playtest):
- Mewtwo's Island arc (post-Gym-8 story sequence: maps + Ash/Misty/Brock/Team Rocket bosses + catch Mewtwo / Justin catches Mew).
- Story scripting, NPCs, the roast dialogue, intro cutscene, ending/credits + wake in apt 28C.
- Custom sprites (all leaders use placeholder gfx; Act-II overworld leaders = OBJ_EVENT_GFX_YOUNGSTER) - needs an artist/Porymap.
- Wild encounters (city maps have none; player levels via the 99 Rare Candies in starting loadout).
- Badge count for E4 is the 8 Hoenn badges (gyms 1-8); gyms 9-16 are extra (no 9-16 badge flags).

## UPDATE @ v0.43 — Act-I gym leader POST-BATTLE roast (PR #94)
Reskinned all 8 Act-I gym leaders' post-victory re-talk lines (the line shown when you
talk to a beaten leader again) from vanilla filler into the SAG roast voice, matching
each leader's established persona. v0.35 had only done their intro+defeat lines.
- Edited `<Leader>PostBattle` text in each gym's `scripts.inc`: Roxanne(Altorelli),
  Brawly(Sokotch), Wattson(uptown), Flannery(Randolph), Winona(The Dean),
  Tate&Liza(Grandma), Juan(Giovanni). Norman's is `PetalburgCity_Gym_Text_DadGoingToKeepTraining`
  (not a *PostBattle symbol — he routes through an EventScript). Juan's preserves the
  "go to the League" wayfinding, reworded to point through Mewtwo's island first.
- Text-only, zero logic changes. Charmap note: only é and … (U+2026) are non-ASCII-safe;
  avoid em-dash (—) — not in the charmap.
- Likely NEXT roast targets: the 8 gym TRAINERS' Intro/Defeat/PostBattle (still vanilla
  in most gyms), Act-II overworld gym leaders 9-16, E4/Champion post-battle lines.

## DIALOGUE/LORE PASS @ v0.44 - v0.49 (PRs #95-#100)
Big autonomous roast + lore-consistency sweep of the **critical path**. All text-only,
all shipped (ROMs in versions/ + the VBA folder). Charmap rule reconfirmed: only `é` and
`…` are safe non-ASCII; **use `--` not the em-dash** (— is NOT in the charmap and fails the build).
- **v0.44** - E4 + Champion POST-BATTLE speeches (Cali/Dad/Mom/Steven/Jared voices); the
  speeches keep each room's "next room" progression cue.
- **v0.45** - Champion victory **cutscene** MALE-path (the only path forced-male SHANE hits):
  rival MAY->Justin, `{RIVAL}`->JUSTIN, Prof. Birch->mentor, Wallace->Jared. Fixed the
  vanilla "advice before you challenge the CHAMPION" lines that fired *after* the win.
- **v0.46** - all 8 Act-I gym **guides** (advice NPC): SAG leader + correct SAG type +
  roast hint, preserving each reused interior's real puzzle (dark gym / switch doors /
  pitfalls / SPEED+ACCURACY rooms / rotating doors / double battle / icy floor).
- **v0.47** - endgame: E4 badge-gate reception + **Hall of Fame** closing (Jared walks
  Shane in; birthday payoff). HoF cutscene + GameClear flow untouched.
- **v0.48** - the 8 gym-guide **post-victory** re-talk lines (dropped JUAN/HOENN, "your
  own father", STONE BADGE, wrong type themes).
- **v0.49** - all 8 gym **statues** + certified-trainers rolls: Hoenn city/leader names
  -> SAG leader + persona subtitle (e.g. "ALTORELLI'S GYM / Sidley Austin, Attorneys.").
- **v0.50** - the 8 gym **entrance signs** (in the Hoenn gym-town slots): LEADER: BRAWLY ->
  SOKOTCH etc. + SAG-type flavor. NOTE: slot != guaranteed-walked (e.g. Gym1's RustboroGym
  door was wired into Times Square/Oldale per the v0.14 note), so these are correct-if-seen
  and harmless if not. Needs a playtest to confirm which town signs are physically reached.
- **v0.51** - the 8 town-name signs -> NYC neighborhoods, matched to each slot's renamed
  region-map section so the sign == the map-name popup (RUSTBORO->MIDTOWN, DEWFORD->UPPER
  EAST SIDE, MAUVILLE->RANDALLS ISLAND, LAVARIDGE->RIVERDALE, PETALBURG->THE BRONX,
  FORTREE->WILLIAMSBURG, MOSSDEEP->OLD BROOKVILLE, SOOTOPOLIS->MONTAUK) + NYC taglines.
  Curly-quote tip: don't retype the “ ” -- match the ASCII *inside* them and they're preserved.

### SAG gym leaders + types (canonical, used across the dialogue):
Gym1 ALTORELLI/Dark, Gym2 SOKOTCH/Psychic, Gym3 RANDALL/Fighting, Gym4 RANDOLPH/Ghost,
Gym5 JUDGE/Flying, Gym6 THE DEAN/Steel, Gym7 GRANDMA(Jesse)/Fairy(double), Gym8 GIOVANNI/Ground.
E4 = Sidney/Cali, Phoebe/Dad, Glacia/Mom, Drake/Steven; Champion Wallace = Jared (brother);
rival = Justin ("little Gaer"); player = SHANE GAER (forced male).

### Critical-path dialogue is now ~COMPLETE. What still genuinely moves the needle:
- **Gym rank-and-file trainers** (~55 across the 8 gyms) still vanilla and still name-drop
  ROXANNE/ROCK etc. -- low value-per-word, big batch; do per-gym if at all.
- **Town/route/gym SIGNS** in SAG-traversed map slots may still read Hoenn names (needs a
  per-map check of which slots are actually walked vs just gym-interior hosts).
- **Badge names** on the trainer card are still Hoenn (STONE/KNUCKLE/...) -- likely a
  strings/graphics change, higher risk; verify before attempting.
- Bigger axes that need a human/playtest: custom sprites, wild encounters in city maps,
  a real post-credits "wake in the apartment" scene.

## "DO IT ALL" PASS @ v0.50 - v0.56 (PRs #101-#107) -- the Hoenn-text purge is COMPLETE
Every player-facing Hoenn reference on the critical path is now SAG. All text-only except
the ending (one safe map-script). Charmap rule still holds: `é` `…` `“` `”` ok; use `--` not `—`.
- **v0.50** gym entrance signs; **v0.51** town-name signs -> NYC neighborhoods.
- **v0.52** gyms 1-4 rank-and-file trainers + badge-receipt/register leader names.
- **v0.53** gyms 5-8 shown-on-win dialogue: receipts, registers, Mossdeep twins badge-give
  -> GRANDMA, and the whole Petalburg vanilla "your dad is the leader" cutscene -> JUDGE.
- **v0.54** gyms 5-8 rank-and-file trainers (Fortree -> THE DEAN, Petalburg drops "father",
  Mossdeep PSYCHIC -> FAIRY) + renamed Bird Keeper TRAINER_JARED's battle name JARED -> JEROME
  (so the only in-game JARED is the champion brother, TRAINER_WALLACE).
- **v0.55** renamed ALL 8 badges: STONE->GAVEL, KNUCKLE->SCHOLAR, DYNAMO->PENNANT,
  HEAT->MANOR, BALANCE->SLUGGER, FEATHER->VERDICT, MIND->FAMILY, RAIN->BOSS (display text
  only; flags/symbols/trainer-card art unchanged).
- **v0.56** POST-CREDITS BIRTHDAY SCENE: one-shot ON_FRAME msgbox in BrendansHouse_2F (the
  post-GameClear respawn), gated FLAG_SYS_GAME_CLEAR + FLAG_UNUSED_0x024. "{PLAYER} woke up...
  Happy birthday, Shane. Love, Jared." Msgbox-only, can't soft-lock.

### GENUINELY BLOCKED (need a human / asset that this env can't make):
- **Custom leader sprites** -- need an artist + Porymap; all leaders still use placeholder gfx.
- **Wild encounters in city maps** -- the urban tilesets have no grass/encounter metatiles, so
  walking encounters can't trigger without map tile work, and they don't suit NYC anyway
  (player levels via the 99 starting Rare Candies by design).
- **Playtest verification** -- the gym/town SIGNS (v0.50/51) and the v0.56 ending are
  correct-from-source but UNVERIFIED in-emulator. Boot SAG-v0.56, read a couple signs, and
  (with a cleared save) re-enter the apartment to confirm the birthday scene fires once.

### NEXT free unused flag after 0x024: FLAG_UNUSED_0x025.

## STRUCTURAL ASKS @ v0.57 - v0.59 (PRs #108-#110)
- **v0.57** retyped all 47 gym rank-and-file trainer PARTIES to their gym's SAG type
  (script: scratchpad/retype_trainers.py rebuilds trainers.party block-by-block; classes/
  names kept -- no 'lawyer' sprite exists). Leaders' teams untouched.
- **v0.58** gated the Hudson Yards LAPRAS gift behind FLAG_BADGE04_GET (was given at start).
- **v0.59** HUDSON YARDS APARTMENT BUILDING (the big map change):
  - Spawn moved from LittlerootTown(13,14) street -> MAP_OLDALE_TOWN_HOUSE1 (5,6) = Shane's
    apartment, "Floor 28" (new_game.c:142). Reused two ORPHANED interiors:
    OldaleTown_House1 = Floor 28 (Shane), OldaleTown_House2 = Floor 11 (Jared).
  - Both relabeled region_map_section -> MAPSEC_LITTLEROOT_TOWN (Hudson Yards popup).
  - SAGJared NPC moved from the Hudson Yards exterior into Floor 11 (OldaleTown_House2).
  - Elevator = an operator NPC in each apt (YES/NO floor select) that `warp`s 28<->11.
  - Building entrance: new LittlerootTown warp at the existing door tile (4,11) -> Floor 28.
  - **COORDINATES UNVERIFIED IN-EMULATOR** (set from tools/sag/render.py): spawn (5,6) in
    House1; door (4,11); elevator arrivals House1(4,7)/House2(4,6); objects. If the spawn or
    a warp lands wrong, it's a one-line coordinate tweak. PLAYTEST a NEW GAME first.

### STILL OPEN: lawfirm out of Times Square (#3)
Altorelli's gym = MAP_RUSTBORO_CITY_GYM, entered from OldaleTown (Times Square) warps at
(1,11)/(2,11). User wants it on its own block OUTSIDE Times Square. Needs a small CLEAN
exterior map for the lawfirm block -- none exists orphaned-and-empty (only full FRLG towns),
so this needs a freshly authored small exterior (hardest headless task). Recommend doing it
AFTER the v0.59 building coordinates are playtest-confirmed (same render-guess method).
