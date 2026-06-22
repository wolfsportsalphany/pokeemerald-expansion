# SAG Version — Birthday Gift Build Plan

> A personalized fork of **Pokémon Polished Crystal**, built as a birthday present for
> **Shane Austin Gaer (SAG)**.
> This document is the living source of truth. As details come in, they get logged here,
> then implemented and checked off.

- **Recipient:** Shane Austin Gaer ("SAG")
- **From:** _(to be filled in)_
- **Base:** Polished Crystal v3.2.3 (pokecrystal disassembly) — kept for its QOL & modern features
- **Working branch:** `claude/polished-crystal-fork-s4rysf`
- **Started:** 2026-06-21

### Base decision (2026-06-21)
Shane has already played Polished Crystal and loves the Gen 1/2 era. Considered switching to a
fresh base (vanilla `pret/pokecrystal`, Gen 1 `pokered`/`pokeyellow`, the 1997 `pokegold-spaceworld`
demo, or the open-source **Crystal Legacy** at `github.com/cRz-Shadows/Pokemon_Crystal_Legacy`).
**Decision: stay on Polished Crystal** — keep its quality-of-life and modern features as the
engine/foundation, and **build a fresh, personalized story & content layer "from the ground up"
on top of it.** The personalization is what makes it Shane's, not the base.

> Approach note: "from the ground up" content means we mostly **repurpose/replace** existing PC
> story, characters, and events with Shane-themed versions (which frees ROM space as we go),
> rather than only piling new data onto an already-98.8%-full ROM.

---

## ★★ STAGING SUMMARY — current state before we start building ★★
The complete picture as of 2026-06-21, so we're aligned before cutting code.

### ⚠️ PLATFORM DECISION (2026-06-21): move to GBA — fork pokeemerald + expansion
Goal is a **GBA (.gba)** ROM. Polished Crystal is **GBC** (no GBA version exists; a .gbc does run
on GBA hardware/emulators, but we want a true GBA game). **Chosen base: `pret/pokeemerald` +
`rh-hideout/pokeemerald-expansion`.** Rationale:
- **Porymap visual map editor** → makes the giant custom NYC region actually buildable (vs blind on GBC).
- **Expansion** = modern QOL (Gen 1–9 species, Abilities, Phys/Special split, Natures, Fairy, modern
  moves/items) — matches/beats Polished Crystal's features.
- Clean, open, well-documented decomp; the standard base for total-conversion hacks.
- **NOT Radical Red** (CFRU binary hack, no clean source to fork).
- **All design in this doc ports over unchanged.** Only the 3 small pokecrystal code edits
  (Justin/Hudson Yards/Shane renames) are throwaway and get redone on the new base.
- **NEXT STEP (pending):** stand up the pokeemerald+expansion repo (new repo or repurpose this one),
  install devkitARM toolchain, verify a clean baseline `.gba` build, then port the opening.

### What the game IS
A total-conversion of **Polished Crystal** (keep its engine/QOL/features) where the whole content
layer is rebuilt as **Shane Austin Gaer's life, mapped onto one big interconnected NYC/tri-state
region**. Every line of dialogue, character, and Pokémon casting is new. **Full-roast** comedic tone.

### Locked foundations
- **Base/engine:** Polished Crystal (builds clean here — RGBDS 1.0.1 verified).
- **World:** ONE region = full Johto+Kanto landmass combined, re-skinned to **real tri-state
  geography** (Manhattan core; Bronx/Hudson Valley/Killington north; LI east; NJ/Philly west/SW;
  Brooklyn south; ocean + Mewtwo Island; far-south college leg). Wild Pokémon themed per area.
- **Progression:** **Gyms 1–8 (NYC/suburbs) → [8th badge] courier Dragonite + MEWTWO ISLAND arc
  (replaces mid-game E4) → Gyms 9–16 (college/later-life) → [16th badge] ELITE FOUR (+ Champion/
  final boss TBD).**
- **Pokémon:** same Pokédex; re-cast starters/legendaries/encounters/all teams; themed to opponent
  & location. **All Pokémon get max IVs + competitive natures (universal).** Favorites featured:
  **Eevee, Primeape, Haunter (not Gengar), Dragonite, Mewtwo.** Legendaries are **enemies-only**
  (gyms 9–16 + E4 wield them) **except Mewtwo & Mew** (caught on Mewtwo Island).
- **Establishments:** location-based Marts (CVS/Duane Reade/Target/bodega…) & Centers (CityMD/
  hospital/spa…); **Joe's Pizza** chain; **bars** & **weed dispensaries** (full roast).

### GAME FLOW (start → finish)
1. **Open:** wake in **apt 28C, Hudson Yards**; party = **Lv1 Gastly + Lv1 Mankey**.
2. **Floor 11:** **Jared** (Shane's brother) sends Shane off (gear) and gifts the **Eevee** (only one in game).
3. **Hudson Yards** plaza (doormen **Robert/Eric/Jose/Jonathon**) → **Times Square** (funny route).
4. **Justin** blocks Sidley Austin for the **first rival battle** (Eevee vs Eevee; Pilates brag) →
   **GYM 1: Sidley Austin** (Altorelli; multi-story office; Russian-spy subplot seed).
5. **Gyms 2–8** across NYC/suburbs; family & story NPCs woven through (Mom/Dad West Village, Cali
   Williamsburg, Grandma Linda + uncles Matt/Jason/Donut in Paramus, Old Brookville memorial);
   opponents begin fielding legendaries in later gyms.
6. **[8th badge] courier Dragonite** invitation (battle+catch) → **MEWTWO ISLAND** (*First Movie*
   climax: battle Team Rocket, Ash, Misty, Brock, and **Justin**; **Armored Mewtwo** two-phase
   mirror-team boss → catch Mewtwo; **Mew** catchable after).
7. **Gyms 9–16** (UPenn/APES, Emory, IMG, Camp Pontiac, **Killington/Wobbly Barn**, **Central Synagogue = Gym 16**).
8. **[16th badge] ELITE FOUR** (+ Champion / secret final boss — cast TBD).

### Ready to BUILD now (the vertical slice)
The **opening through Gym 1** is fully specified: apt 28C → floor 11 (Eevee) → Hudson Yards →
Times Square → Justin → Sidley Austin gym.

### Still OPEN (won't block the opening)
- Gyms **14–16** locations/leaders; **gym types** (keep PC's vs re-theme); **Elite Four + Champion
  + secret final boss** cast.
- Exact **starting Eevee level**; the **species→nature** table; **Joe's** exact role.
- Custom **Armored Mewtwo sprite** + **mirror-team** custom code (build-phase engineering).

## ★ NEW-REPO HANDOFF (GBA base) — start here in the new session
- **Base forked (2026-06-21):** `rh-hideout/pokeemerald-expansion` → your fork (e.g.
  `wolfsportsalphany/pokemon-sag-version`). This is the GBA engine going forward.
- **Source of truth:** copy **this `SAG_BIRTHDAY_PLAN.md`** into the new repo first thing — it holds
  the entire design (world/geography, 15 gyms + buildouts, Mewtwo Island arc, cast, mechanics).
- **First steps in the new session:**
  1. Copy `SAG_BIRTHDAY_PLAN.md` over; commit.
  2. Install toolchain (**devkitARM** + agbcc/modern gcc per the fork's `INSTALL.md`); verify a clean
     baseline **`.gba`** build.
  3. Set up **Porymap** for visual map editing (the custom NYC region).
  4. Re-apply the throwaway pokecrystal edits on the new base: default player name **Shane**, rival
     default **Justin**, starting town **Hudson Yards**, starting party **Lv1 Gastly + Lv1 Mankey**
     (+ Jared's Eevee).
  5. Build the opening slice (Hudson Yards → Times Square → Gym 1 Sidley Austin → Justin).
- **Still to design before/while building:** **Gym 16**, the **Elite Four (×4)**, the **Champion**,
  and the **secret final boss**.

## Status Legend
- [ ] Not started
- [~] In progress
- [x] Done & verified

---

## Phase 0 — Project setup & tracking
- [x] Create this tracking document
- [ ] Decide final ROM name / branding strings (Makefile `NAME`, `VERSION`, `AUTHOR`)

## Phase 1 — Build toolchain (so we can produce a playable ROM)  ✅ DONE
- [x] Install build deps (make, gcc, bison, libpng, pkg-config)
- [x] Install **RGBDS v1.0.1** (built from source; `rgbasm v1.0.1`)
- [x] Verify a clean **baseline** build produces `polishedcrystal-3.2.3.gbc` (2 MB, exit 0)
- [x] Header/size sanity OK (md5 `4e53ffed7ab34d599276e68f7e4497ed`)

> ⚠️ **SPACE CONSTRAINT:** baseline ROM is **98.8% full** (only ~25 KB / 2 MB free).
> Adding large new content (many trainers, lots of dialogue, new maps) will require freeing
> space or it won't fit. Favor *edits/replacements* over *additions*; keep new text tight.

## Project scope (2026-06-21) — TOTAL STORY CONVERSION
Keep Polished Crystal's **engine, QOL, and modern features**. Rebuild the **entire content layer
from the ground up**, personalized for Shane:
- **Whole story rewritten** — new narrative, dialogue, characters, events, themes.
- **Game flow redone** — progression/order, key story beats, gym/badge or equivalent structure,
  pacing, where you go and why.
- **All trainers redone** — every trainer party (Gym Leaders, Elite Four, rivals, route trainers,
  bosses) re-tuned/re-themed; rosters, levels, items, AI as needed.
- Personal touches woven throughout (rival = Justin, courier Dragonite, champion cameos, baseball
  references, dedication, etc. — see details below).

> This is a big, ongoing build. We do it **incrementally and track every piece in this doc**, so
> the gift can ship in coherent milestones (intro + early game first, then outward).
> **Still needed from Shane's family (you):** the high-level STORY concept/premise and the new
> game flow (region order, who the key characters are, the central goal). Firehose welcome.

## ★ MASTER DIRECTIVE (2026-06-21): make it feel COMPLETELY NEW
Change **every piece of dialogue, every character, and every Pokémon** so nothing reads as stock
Polished Crystal. This is the largest possible content scope — effectively a full reskin + rewrite
on top of PC's engine. It's very doable but it's a marathon, so we work systematically and bank it
in milestones.

### Scale (so we plan realistically)
- **Dialogue:** thousands of text strings across `data/text/*.asm` + every `maps/*.asm` script.
- **Characters:** all NPCs/trainers — names, personalities, sprites where it matters.
- **Pokémon — RESOLVED (2026-06-21):** keep the **same Pokédex** (all existing species, sprites,
  cries, dex). "New Pokémon" means **re-cast which Pokémon appear situationally**:
  - **Shane's starting trio: Eevee + Level 1 Gastly + Level 1 Mankey** (all favorites). Gastly &
    Mankey owned at start; **Eevee gifted by Jared on floor 11** (the only obtainable Eevee).
  - different **legendaries** (and their encounter events),
  - re-chosen **wild encounter tables** per area,
  - **every opponent's team** rebuilt (rivals, gym leaders, E4, route trainers, bosses).
  - No species renames, no new sprites/fakemon. This keeps it light + ships on time.
  - **CASTING RULE:** the Pokémon chosen must be **on-theme with the opponent's persona AND the
    location.** Examples: baseball coach → hard-hitting Fighting/athletic mons; "Russian spy" →
    sneaky Poison/Dark; Grandma Linda → sweet-but-tough Fairy/old-school mons; frat brother →
    rowdy/"party" mons; gritty NYC streets → urban mons (Rattata, Grimer/Muk, Pidgey, Meowth);
    Yankee Stadium → flashy heavy hitters. Starters & legendaries likewise picked to fit the
    story beat where they're obtained.
  - **SHANE'S FAVORITE LINES — feature prominently:**
    - **Primeape line (Mankey → Primeape)** — give it a starring role: available/obtainable early
      and meaningfully (great fit for the baseball/Fighting/NYC-tough-guy vibe), showcased on key
      ally/opponent teams. A signature 'mon for Shane.
    - **Dragonite line (Dratini → Dragonair → Dragonite)** — already special via the **courier
      Dragonite** one-off event; lean into the line as a prestige/aspirational Pokémon.
    - **Haunter — SPECIFICALLY Haunter, not Gengar.** Shane loves Haunter itself; feature Haunter
      prominently (ace/ally/opponent), but **stop at Haunter** — don't push the Gengar evolution
      (Haunter is the star, the "perfect form" here).
  - **LEGENDARIES — use them AGAINST the player, don't hand them out:**
    - Shane doesn't love legendaries, so **the player generally can't obtain them** — instead the
      **second-half GYM LEADERS (gyms 9–16) and the ELITE FOUR field legendaries in their parties**
      as serious threats. Legendaries become intimidating opponent power, not collectibles.
    - **MEWTWO (+ Mew) are the deliberate exceptions** — catchable only via the Mewtwo Island arc
      (his favorite, earned through the *First Movie* climax). Concentrate player-obtainable
      legendary content into that one arc; keep others as enemies-only.
    - **★ ARMORED MEWTWO STORYLINE** — a central plot arc themed on *Pokémon: The First Movie*
      (Armored Mewtwo). Ties directly to the **courier Dragonite** event (the Dragonite that
      delivers Mewtwo's invitation letter) → one cohesive first-movie storyline: a mysterious
      letter/invite → tracking the experiments → confronting **Armored Mewtwo**.
    - *Art note:* the armor is a **custom sprite** (one-off). Doable as a single special asset;
      if art slips, fall back to standard Mewtwo sprite. (We otherwise avoid new sprites.)
    - **★ TWO-PHASE BOSS MECHANIC:**
      1. **Phase 1 — Trainer battle vs Armored Mewtwo:** fought as a *trainer*-controlled boss
         (so it can't be caught yet; armor = unbeatable-feeling, tuned tough).
         - **★ MIRROR TEAM:** Mewtwo's party is a **clone of the PLAYER's current team, each member
           +5 levels.** (On-theme: Mewtwo is a clone — it copies you.) *Impl: custom code that
           reads `wPartyMon` species/moves at battle start and builds the enemy party as copies at
           +5 levels — not a static party. Non-trivial engine work; flag for build phase.*
      2. **On defeat → cutscene:** the **armor breaks/shatters.**
      3. **Phase 2 — Wild Mewtwo battle:** immediately transition into a **wild** encounter with
         the now-unarmored Mewtwo → **the player can capture it.**
      - *Impl:* scripted trainer battle that, on victory, triggers a cutscene then starts a wild
        battle (engine supports scripted/forced wild encounters). Set Mewtwo's wild level/catch
        rate so it's a real (but fair) capture moment — the payoff of the whole arc.

### Method (how we churn through it without drowning)
1. **Define the "bible" first:** the new world's theme, the renamed Pokémon list, and the cast of
   characters. Everything else references this for consistency. (Lives in this doc.)
2. **Work region-by-region, file-by-file:** rewrite text + scripts in batches, committing often.
3. **Trainers re-themed as we pass through each area** (ties into dialogue).
4. **Track coverage** with a checklist of text files / maps so we know what's done vs remaining.
5. Rebuild frequently to catch breakage early (text macros, character limits, name lengths).

### Coverage tracker (filled in as we go)
- [ ] World/theme "bible" written
- [ ] Pokémon rename list (species → new names)  — scope = (A/B/C) TBD
- [ ] Character/cast list
- [ ] Intro & New Bark / starting town
- [ ] (further areas added as we reach them)

## ★ WORLD BIBLE — "NYC / NY SUBURBS" THEME (Shane's life as a region)
The unified region = **Shane's life journey**, centered on **New York City**, radiating out to the
suburbs (NJ, Long Island) and his later life travels (Philadelphia, Atlanta, Bradenton FL).
Early game leans hard into the **NYC / NY-suburbs** vibe. Player = **Shane** (the hero).

### Locations → map roles (DRAFT — to refine into towns/routes/gyms)
**NYC core (heart of the region):**
- **West Village** — *later childhood home*; **MOM & DAD live here.** → strong "home town"
  (the New Bark / player's-house analog where the adventure begins).
- **Upper East Side** — *early-childhood memories sprinkled here*; home of **The Town School**
  (Shane's UES school). Nostalgic early-game town/district.
  - **★ Shane's past childhood homes (explorable, locked 2026-06-21):** make these enterable
    apartments with nostalgic memory dialogue / Easter eggs:
    - **245 E 58th St — apt 17A**
    - **330 E 72nd St — floor 3**
    - **525 E 72nd St — apt 44F**
    Each = a small explorable interior with personal callbacks (old rooms, "this is where…"
    memories, maybe a hidden item or photo). Cluster them around the UES district near the
    Town School / John Jay Park.
- **Randalls Island** — *baseball* fields (early-childhood baseball). → park/route w/ baseball NPCs.
- **Hudson Yards** — Shane's **current home** & **THE STARTING POINT**: his **1-bedroom apt on the
  28th floor (apt C)**. His **brother Jared** lives in the same building, in **apartment 11I,
  floor 11**. → sleek modern city hub; the game opens here (New-Bark/player's-room analog).
- **Williamsburg, Brooklyn** — younger sister **Cali** (at **Brooklyn Law School**).
- **Central Synagogue (NYC)** — landmark; key building/event (bar-mitzvah-era memory?).
- **Sidley Austin law firm (NYC)** + **John Altorelli** & **"the Russian spy" he dated** —
  law-internship storyline NPCs.
- **Underage bars (NYC)** — nightlife flavor NPCs / hidden spots.
- **Riverdale Country School (NYC/Bronx)** — another school location.

**Sports venues (prime GYM / arena candidates):**
- **Yankee Stadium (Bronx)** — baseball → top gym candidate (ties Shane's HS baseball).
- **Madison Square Garden (MSG)** — arena/gym candidate.
- **Barclays Center (Brooklyn)** — arena/gym candidate.
- **IMG Academy (Bradenton, FL)** — sports academy → athletics gym candidate.

**NY suburbs / tri-state:**
- **Paramus (NJ)** — **Grandma Linda's** home; the **3 crazy uncles** (dad's brothers):
  **Matt & Jason (twins)** + **Donut (short, Napoleon complex)**.
- **Old Brookville (Long Island)** — mother's parents' home; **grandparents are deceased** →
  handle tenderly (memorial / remembered place, NOT a battle gag).

**College / later-life journey (farther region areas):**
- **University of Pennsylvania, School of Arts & Sciences (Philadelphia)** — **APES** frat; beer
  with friends.
- **Emory University Law School (Atlanta)** — drank beer with friends (girls & guys).
- **Camp Pontiac (NY)** — summer camp; nostalgic woods/lake route.

### Cast
| Person | Role in game | Placed at |
|--------|--------------|-----------|
| **Shane** | Player / hero | starts West Village |
| **Justin** | **Rival** — short, Napoleon complex; **owns a Pilates studio** he insists beats being a lawyer; first battle blocks the Sidley Austin entrance | follows Shane's journey |
| **Merrick** | **Justin's brother** — genuinely nice; warm foil to Justin's ego | helpful NPC; encourages Shane, maybe gives items/tips |
| **Mom & Dad** | Parents (home NPCs) | West Village |
| **Cali** (younger sister) | Family NPC (law student) | Williamsburg / Brooklyn Law |
| **Jared** | **Shane's brother** & gift-giver — same building, **apt 11I**; sends Shane off (gear) + the **Eevee**; later the **Champion** | Hudson Yards |
| **Grandma Linda** | Family NPC | Paramus |
| **Uncle Matt** & **Uncle Jason** | Twins, "crazy uncles" | Paramus |
| **Uncle Donut** | Short, Napoleon complex (cf. Justin) | Paramus |
| Maternal grandparents | Remembered (deceased) — respectful memorial | Old Brookville |
| **John Altorelli** | **Gym 1 Leader** — took over Sidley Austin as his gym | Sidley Austin, NYC |
| **"The Russian spy"** (Altorelli's GF) | Back together w/ Altorelli; co-runs Gym 1; Russian goons | Sidley Austin, NYC |
| **Ms. Sokotch** | **Gym 2 Leader** — glasses, mean & ugly, terrifying schoolteacher | The Town School, UES (76th & East End) |
| **Steven** & **Jake** | Trainers in the John Jay Park gauntlet after Gym 2 (friends/classmates?) | John Jay Park, UES |
| **Dominic A.A. Randolph** | **Gym 4 Leader** — British old guy, posh headmaster | Riverdale Country School, the Bronx |
| **Aaron Judge** | **Gym 5 Leader** — pro baseball slugger, "the big leagues" | Yankee Stadium, the Bronx |
| **Jesse** | **Gym 7 Leader** — sweet old Black woman, former family nanny (warm, not roast) | Old Brookville, Long Island |
| **Giovanni** | **Gym 8 Leader** — Team Rocket boss; Montauk overrun w/ Rocket; bridges to Mewtwo Island | Montauk Lighthouse |
| **Grandma Linda** | **Gym 9 Leader** — the whole town knows her | Paramus, NJ |
| **Uncles Matt, Jason (twins), Donut (short)** | **Gym 9 goons** — Grandma's grunt trainers | Paramus, NJ |
| **The Shermfather** | **Gym 10 Leader** — frat-boss of APES "4K" | UPenn, Philadelphia |
| **Madi** (+ **Brielle**) | **Gym 11 Leader** — blonde ex-classmate (Brielle alongside) | Emory Law, Atlanta |
| **Roman** | **Gym 12 Leader** — old guy who runs the snack bar (secret boss) | The Hemispheres, Hallandale Beach FL |
| **J.R. Murphy** | **Gym 13 Leader** — baseball (catcher); elite-academy boss | IMG Academy, Bradenton FL |
| **Professor Oak** | **Gym 14 Leader** — researching an undiscovered legendary | Killington, VT (mountaintop) |
| **Kenny** | Killington **slope NPC** — Dad's friend; chairlift-spitting lore | Killington, VT |
| **Kenny & Ricky** | **Gym 15 co-Leaders** (twin double battle) — old twin doctors who own the camp; glasses + white hair *(distinct from Gym-14 Kenny)* | Camp Pontiac, Copake NY |
| **Justin** (rival) | **Gym 16 Leader (FINAL gym)** — *hostile takeover* of Central Synagogue (mirrors Altorelli seizing Sidley/Gym 1); the climactic rival battle | Central Synagogue, NYC |
| **Rabbi Angela** | Central Synagogue member — young-ish Korean Jewish woman; publicly objects that Justin seized the gym (the congregation doesn't want him, but has to allow it) | Central Synagogue, NYC |

### Locked decisions (2026-06-21)
- **Start point:** Hudson Yards, Shane's 1-bedroom apt, **28th floor (apt C)**. ✅
- **Tone:** **FULL ROAST** — lean into the underage bars, law-internship, Russian-spy, short-uncle
  jokes, etc. Comedic and irreverent. (Still handle deceased grandparents respectfully.) ✅
- **Jared:** **Shane's brother**; same building, **apartment 11I, floor 11**; the floor-11 send-off + Eevee giver, and the endgame **Champion**. ✅
- **Pokémon:** same Pokédex; re-cast starters/legendaries/encounters/opponent teams. ✅

### Still open
- **Gyms** = a lighter, SEPARATE layer (see below) — not built from the family. Leaders/types TBD.
- **Secret final boss** (Red analog) — TBD.

## ★ OPENING SEQUENCE — LOCKED & READY TO BUILD (2026-06-21)
The concrete first slice of the game (Act I start):

1. **Apt 28C, Hudson Yards** — Shane wakes up in his 1-bedroom apartment on the **28th floor**.
   Tutorial/room beat, then head out to the elevator.
2. **Elevator → 11th floor** — Shane takes the elevator **down to the 11th floor to visit his
   brother Jared** (he lives on floor 11, in **apt 11I**).
3. **CHANGED (2026-06-21): Shane starts with a full PARTY** (no single-Eevee gift). The party is
   pre-set at new-game start (see "Starting party" below). The **11th-floor visit becomes a
   send-off**: his **brother Jared** sees Shane off (gives gear — Pokédex / Town Map / running shoes /
   key item) and hands over the **Eevee**, with the comedic bit.
   - **STARTING PARTY (confirmed 2026-06-21): Eevee + Level 1 Gastly + Level 1 Mankey.** All three
     of his favorites (Eevee + Haunter & Primeape lines). Gastly & Mankey are **Lv1**; Eevee level
     TBD (default low, e.g. Lv5 as a "found" mon — confirm if you want a specific level).
   - **Eevee delivery (keeps the jokes):** Shane already has Gastly + Mankey; **Jared hands over
     the Eevee on floor 11.** This preserves the **only obtainable Eevee**, Justin's **Eevee-mirror
     battle**, and the **"Jared said it's rare / that's why he wasn't invited to the wedding"** gags.
4. **Downstairs = Hudson Yards (explorable hub)** — the lobby/street level is the explorable
   **Hudson Yards** area. **Doormen NPCs: Robert, Eric, Jose, and Jonathon.** (Give each a distinct
   funny personality.)
5. **Route = TIMES SQUARE** — the "route" from Hudson Yards to the first gym is **Times Square** —
   **make it FUNNY**: costumed characters (knockoff Elmo/Spider-Man), the Naked Cowboy, aggressive
   flyer-handers, tourists blocking the path, ticket scalpers as "trainers," etc.
6. **FIRST RIVAL BATTLE — Justin** — right **before Shane first enters Sidley Austin**, **Justin
   runs up and blocks the entrance** and challenges him to a battle. In his intro he brags that
   he **owns a Pilates studio**, which (in his mind) is **way better than being a lawyer** — peak
   Napoleon-complex energy. (This is Shane's first encounter with the rival.)
   - **Justin's starter = an Eevee too**, and he sneers that **Jared was wrong — "it's actually
     not that rare."** (Rival-mirror battle: Eevee vs Eevee. Justin's Eevee is NPC-owned, so it
     doesn't break "Shane's is the only *obtainable* Eevee.")
   - Justin also snipes that **that kind of stupidity is exactly why Jared didn't get invited to
     his wedding.** (More rival roast fuel.)
7. **FIRST GYM = Sidley Austin law firm (Midtown)** — a **MULTI-STORY gym** (navigate floors of the
   office building, like an elevator/floor-puzzle gym). Set in Shane's corporate law firm.
   Leader likely **John Altorelli**; ties into the law-internship / "Russian spy" subplot.
   Full roast — billable hours, associates/paralegals as grunt trainers, conference-room battles.

> **Pokémon note:** **Eevee** = the opening gift / signature companion (only one in the game).
> Other "starters/legendaries" re-cast per the casting rule. NOTE: this moves **Sidley Austin from
> the late-game law tower to GYM #1** — gym layout/order is being driven by the real-life flow now.

## ★ DESIGN PRIORITY (2026-06-21): build the WORLD from Shane's life
**Use the biographical info to build the TOWNS, MAPS, and STORIES — not as a gym-leader roster.**
The real places become the region's towns/areas; the real people become the **resident NPCs and
storylines** living in them; real events (baseball, frat, law internship, the Russian spy, the
crazy uncles) become **quests/story beats**. Gyms sit on top as a separate, lighter progression.

### Region towns & story beats (the heart of the build)
Re-skin PC's existing maps into these NYC/NY-themed towns; each carries its real-life vibe,
resident NPCs, and a personal story beat. (Order = rough travel flow from the start.)

| Town / area (re-skin of PC map) | Real-life vibe | Resident NPCs | Personal story beat (full roast) |
|---|---|---|---|
| **Hudson Yards** (START) | glass high-rises, adult life | **Jared** (Shane's brother; apt 11I, floor 11); doormen **Robert, Eric, Jose, Jonathon** | wake in apt 28C → elevator to fl 11 → Jared gifts **Eevee** + send-off → explore Hudson Yards lobby/street |
| **Times Square** (Route 1) | tourist-trap chaos | costumed characters, Naked Cowboy, scalpers | the funny "route" from Hudson Yards to the office — see Opening seq. |
| **West Village** | brownstone childhood home | **Mom & Dad** | the family home; parents send you off / give gear |
| **Upper East Side** | posh; **The Town School** | childhood friends, teachers | grade-school throwback; earliest memories |
| **Randalls Island** | real explorable ballfields/park island | baseball teammates/coach | Shane's HS baseball; a "big game" event; fishing off the water |
| **Central Park** | big iconic green expanse | joggers, dog-walkers, chess hustlers | major hub park — wild encounters, trainers, hidden items |
| **Riverside Park / West Side Hwy greenway** | LONG, NARROW riverfront greenway | runners, bikers | super-long **4-tiles-wide** green path along the Hudson; **connects Chelsea Piers (Gym 5) ↔ West Village** |
| **Central Synagogue** | **GYM 16 (final)** + early landmark | **Justin** (rival, took it over) + congregants incl. **Rabbi Angela** | early bar-mitzvah memory; final gym = Justin's takeover |
| **Williamsburg, Bklyn** | hipster law-student life | **Cali** (sister, Brooklyn Law) | visit lil sis; she ribs Shane |
| **Bronx** | stadium district | Yankees fans | **Yankee Stadium** landmark/event |
| **Paramus, NJ** | mall suburbia | **Grandma Linda**, uncles **Matt & Jason** (twins), **Donut** (short) | the "crazy uncles" chaos; Grandma feeds you; mall jokes |
| **Old Brookville, LI** | quiet, leafy | (memorial) | mother's late parents' home — tender remembered place |
| **Sidley Austin (Midtown)** — **GYM #1** | corporate law tower / his office | **John Altorelli** (likely GYM 1 leader), **"the Russian spy"** | first gym battle at the firm; law-internship espionage subplot (full roast) |
| **NYC nightlife** | dive bars | bartenders/buddies | underage-drinking hidden spots / sidequest |
| **UPenn (Philly)** | campus + **APES** frat | frat brothers | beer-soaked frat chaos |
| **Emory (Atlanta)** | grad-school campus | college friends (girls & guys) | benders & buddies |
| **IMG Academy (FL)** | elite sports academy | coaches/athletes | athletic boot-camp arc |
| **Camp Pontiac (NY)** | summer camp, lake/woods | camp friends | nostalgic wilderness route |

> Landmarks like **MSG** and **Barclays** = set-piece event venues (concerts/games, special battles)
> rather than full towns. **Riverdale Country School** = secondary school cameo.

### Recurring citywide establishments (re-skinned, LOCATION-DEPENDENT)
- **PokéMart → real chains, varying by location:** **CVS, Rite-Aid, Duane Reade, Walmart, Target,
  bodegas**, and **Wawa in Philadelphia**. Pick whatever fits each neighborhood (posh UES pharmacy
  vs gritty bodega vs suburban Target vs Philly Wawa). Same Mart function, many storefronts.
- **Pokémon Center → real heal spots, varying by location too:** e.g., **CityMD / urgent care,
  hospitals (NYU Langone, Mount Sinai), a spa, a vet clinic** — location-appropriate. Same "heal
  your team" function, different skins per area.
- **Joe's Pizza** — recurring **chain in every neighborhood**, but **NOT** the Pokémon Center.
  It's a flavor/social food spot (counter NPC, maybe sells a cheap healing "slice" item or buffs).
  Consistent Joe's storefront everywhere ties the city together.
- **Bars** — citywide nightlife spots (the underage-drinking roast included). Social hubs, quirky
  NPCs, maybe drinking-themed items/sidequests. Full roast.
- **Dispensaries (weed)** — NYC-style dispensaries as recurring establishments. Comedic NPCs/items,
  full roast. (Keep it jokey/over-the-top, in the spirit of the gift.)

### Gyms — secondary layer (lighter, TBD)
- Keep PC's 16-gym backbone, but gyms are **their own thing**, distributed across the towns above.
- Open: are gym leaders **generic themed trainers**, or a few hand-picked characters? Keep PC gym
  **types** (faster) vs re-theme. We'll lock this *after* the world/story takes shape.
- **Justin (rival)** still recurs throughout as Shane's foil regardless of gym design.

## ★ GAME PROGRESSION / STRUCTURE (locked 2026-06-21)
1. **Gyms 1–8** — NYC & suburbs arc (start: Hudson Yards → Gym 1 Sidley Austin → …).
2. **After 8th badge → MEWTWO ISLAND arc** — courier **Dragonite** arrives with Mewtwo's
   invitation (battle + catch it), then the *First Movie* climax on Mewtwo Island. **This replaces
   the usual mid-game Elite Four.** Catch Mewtwo; Mew becomes catchable after.
3. **Gyms 9–16** — college & later-life arc (Philly/Atlanta/IMG/etc.).
4. **After 16th badge → ELITE FOUR** — E4 = **Cali → Dad → Mom → Steven** (Hoenn Champion), then **CHAMPION: Jared** (Shane's brother). *(See "ELITE FOUR & CHAMPION" below.)*
5. **POST-GAME (locked 2026-06-22):** **Red** is the hidden secret superboss atop the **Empire State Building**, with cross-region champions (Cynthia/Wallace/Steven, etc.) as the climb up to him. Red lives in the post-game — *not* a gym.

## ★ RIVAL ARC — JUSTIN'S BATTLES (locked 2026-06-22; ~12 fights)
Justin recurs across the whole game with an escalating, overcompensating Napoleon-complex arc. His
**Eevee evolution** across the fights is a *decide-later* option. ✅ = already in the doc, ➕ = new.

1. ✅ **Times Square**, pre-Gym 1 — first battle, Eevee vs Eevee; Pilates brag.
2. ✅ **Central Park**, pre-Gym 2 — approach rematch.
3. ➕ **Randalls Island**, after Gym 3 — baseball turf; he claims he was the better ballplayer.
4. ➕ **Brooklyn Bridge**, crossing to Gym 6 — iconic mid-bridge set-piece battle.
5. ➕ **Montauk**, at Gym 8 — he blunders into the Team Rocket takeover.
6. ✅ **Mewtwo Island**, post-Gym 8 — he tags along to the island (Act-I capstone).
7. ➕ **GW Bridge / Fort Lee**, Act II open, pre-Gym 9 — "you're *still* doing this?"
8. ➕ **Airport** (a Fly hop) — running gag: Justin keeps turning up at airports.
9. ➕ **Killington chairlift**, at Gym 14 — cold-weather comedy.
10. ➕ **Camp Pontiac**, at Gym 15 — nostalgic childhood-camp battle.
11. ➕ **Central Synagogue = GYM 16** — Justin's hostile takeover *is* the climactic rival battle (see Gym 16).
12. ➕ **Pokémon League gate**, before the Champion — final battle; Napoleon-complex payoff (earns respect or fully melts down).

> Life-specific beats to confirm: was Justin a **baseball** guy (#3)? did Shane & Justin go to **camp** together (#10)? weave the **Pilates / Jared's-wedding** roast into specific fights.

## ★ ELITE FOUR & CHAMPION (locked 2026-06-22 — cast)
The endgame after the 16th badge. Shane's closest family is the final wall — the journey ends at home.
1. **CALI** (E4 #1) — Shane's **younger sister**; Williamsburg, Brooklyn Law School. (Reappears from the Gym 6 beat.)
2. **DAD** (E4 #2) — lives in the **West Village**.
3. **MOM** (E4 #3) — lives in the **West Village**.
4. **STEVEN** (E4 #4) — **Steven Stone, Hoenn Champion** (the pokeemerald base's own Champion; Steel-type ace). The lone "outsider" wall in an otherwise all-family Elite Four, right before Jared. *(Already in the base game → easy to wire up. Swap to Cynthia if preferred.)*
5. **CHAMPION: JARED** — **Shane's brother**, the final boss to beat. *(Bookend: Jared also gives Shane his starter **Eevee** at the very start — the one who begins the journey ends it.)*

## ★ ENDGAME ARC — MEWTWO ISLAND (*First Movie* climax; replaces mid-game E4)
New location: **Mewtwo Island** ("New Island"), sitting **far off the eastern coast of Long Island**
in the open Atlantic — reached by **surfing east from Montauk through a movie-accurate hurricane**
(unlocked after the 8th badge via the courier Dragonite's invitation). The arc **closely mirrors
the plot & beats of *Pokémon: The First Movie***
— invitation by Dragonite → chosen trainers summoned → stormy boat/crossing → island mansion →
Mewtwo's reveal & monologue → clone reveal → trainer battles → the Armored Mewtwo confrontation →
Mew's appearance → resolution. Hit the iconic story beats (re-flavored for our world/roster).

**Flow:**
1. Beat **Gym 8 (Montauk)** → **courier Dragonite** flies into Montauk with the invitation →
   **battle & catch the Dragonite** (the one-off) → accept invite → **SURF EAST from Montauk
   through a movie-accurate HURRICANE** to **Mewtwo Island**, which sits **far off the eastern
   coast of Long Island** in the open Atlantic. (Recreate the First Movie's violent storm crossing
   — huge waves, lightning; a dramatic, perilous surf to the island.)
2. **On the island:** invited-trainer arrivals, Mewtwo's mansion/lab, clone-machine intrigue —
   re-create the movie's set pieces and tone.
3. **Battle gauntlet — the player fights them ALL:**
   - **Team Rocket** (the experiment's backers — grunts + a boss; Jessie/James/Meowth flavor).
   - **Ash**, **Misty**, and **Brock** (anime cameos as trainers, themed teams).
   - **Justin** — the **rival also got an invite** and is one of the chosen trainers on the island
     (mirrors the movie's invited trainers). Battle him here too, in full Napoleon-complex form.
4. **★ Armored Mewtwo (two-phase boss):**
   - Phase 1: trainer battle vs **Armored Mewtwo** with the **mirror team** (clone of player's
     party, +5 levels). Phase 2: armor shatters → **wild Mewtwo** → **catchable.**
5. **Mew appears** and becomes **catchable after** the Mewtwo battle (movie's other legendary).
6. **Arc ends → Shane WAKES BACK UP in apt 28C, Hudson Yards** (First-Movie-style dreamlike
   aftermath / memory haze — did it even happen?). This transitions into **Act II.**
7. **Act II kickoff:** head home to the **West Village**, where **Dad** tells Shane he needs to
   **go visit Paramus** → leads into **Gym 9 (Paramus / Grandma Linda)**.

> Opponents to script here: Team Rocket (grunts + boss), Ash, Misty, Brock. Teams themed to each
> (Ash = mixed ace incl. Pikachu; Misty = Water; Brock = Rock/Ground), tuned to post-8-badge level.

## ★ GEOGRAPHICAL MAP PLAN — ONE INTERCONNECTED REGION (real tri-state geography)
Design the whole region to mirror **real Northeast geography** (compressed to GB scale), so it
reads as one continuous, sensible map. NYC is the dense core; arms radiate by true compass
direction; water bodies are real; the southern college leg is a long journey south.

### Region shape & connectivity (compass-accurate)
```
                         [ KILLINGTON, VT ]        far NORTH — snowy mountains (Ice)
                          ski slopes + Wobbly Barn
                                  |
                       [ Hudson Valley — CAMP PONTIAC ]   woods + lake (Bug/Grass/Water)
                                  |
   [ PARAMUS, NJ ] ——— [ THE BRONX ] ——————————— [ LONG ISLAND ]
   malls/suburbs (W,    Yankee Stadium,            Old Brookville, beaches,
   across Hudson R.)    Riverdale (N)               Sound (E)
            \                |                        /
             \        ===== MANHATTAN CORE =====     /
              \   Riverside Pk (W riverfront greenway, 4-wide)
                  Hudson Yards (START, W midtown)
                  Times Sq → Midtown (Sidley Austin GYM 1)
                  Central Park (central green hub)
                  Upper East Side / Town School (NE)
                  West Village (S)  ·  Central Synagogue (Midtown E)
                  Randalls Island (NE, in the East River)
                         |                        |
                  [ Hudson R. ]            [ East R. ]
                         \                  /
                          [ BROOKLYN ]  Williamsburg / Barclays (S/SE)
                                  |
                          [ NY HARBOR / ATLANTIC ]
                                  |
                          [ MEWTWO ISLAND ]   out at sea (unlocks after 8th badge)

   ── SOUTHERN LEG (Act II; reached by Amtrak/flight from Penn Station) ──
   [ PHILADELPHIA / UPenn ] ——S——> [ ATLANTA / Emory ] ——S——> [ FLORIDA ]
        (~SW of NYC)                  (deep south, warm)         IMG/Bradenton (W coast)
                                                                 + Hallandale Beach "The
                                                                 Hemispheres" (SE coast, hoops)
```
- **Water/surf routes:** Hudson River (W), East River (E), NY Harbor (S), Long Island Sound (E).
  **Mewtwo Island = far EAST of Montauk** in the open Atlantic (surf there through a hurricane after
  Gym 8). HM Surf/fishing gate the islands & coastlines.
- **North spine:** Manhattan → Bronx → Hudson Valley (Camp Pontiac) → Killington, VT (coldest/
  northernmost). Temperature/biome shifts colder as you go north (snow at Killington).
- **Southern leg** is intentionally far (real distance) — handled as an Act-II travel hop, not a
  walkable connection, matching reality.

### Wild Pokémon by area (same Pokédex, themed to place)
| Area | Biome | Themed wild Pokémon (examples) |
|---|---|---|
| Manhattan streets/subway | gritty urban | Rattata/Raticate, Pidgey, Grimer/Muk (trash), Koffing (exhaust), Zubat (subway), Meowth, Ekans |
| Central Park / Riverside Park | city greenery | Caterpie/Weedle, Oddish, Bellsprout, Sentret, Hoothoot, Spearow, Pidgey, Sunkern |
| Randalls Island | waterfront ballfields | Mankey/Machop (athletes), Magikarp/Poliwag/Goldeen (fishing), normal mons |
| Hudson/East River, Harbor | water/surf | Tentacool, Magikarp, Goldeen, Krabby, Staryu, Horsea, Shellder |
| The Bronx | tough urban | Machop, Mankey, Houndour, Murkrow, Raticate, Magnemite (trains) |
| Brooklyn (Williamsburg) | hipster/arty | Smeargle, Sneasel, Murkrow, Meowth, normal/dark oddities |
| Long Island (Old Brookville) | estates + beach/Sound | Krabby, Shellder, Staryu, Corsola, Oddish, normal mons; beach fishing |
| Fort Lee, NJ (over the GW Bridge) | NJ town, food spots | Rattata, Meowth, Magnemite, Growlithe; **Hiram's** (hot dogs) & **Dong Bang** (Korean BBQ) as food/heal spots |
| Paramus, NJ | malls/suburbs | Rattata, Growlithe, Snubbull, Magnemite (stores), Electrode (carts), normal |
| Camp Pontiac (Hudson Valley) | woods + lake | Caterpie, Weedle, Scyther, Pinsir, Oddish, Teddiursa, Poliwag (lake), Hoothoot/Spinarak (night) |
| Killington, VT | snowy mountains | Swinub, Sneasel, Delibird, Jynx, Teddiursa/Ursaring, Phanpy, Snover-likes (Ice) |
| Philadelphia (UPenn) | historic city | Geodude, Rattata, Growlithe, Magnemite, urban mons |
| Atlanta (Emory) | warm south | Growlithe/Vulpix, Bellsprout, Sunkern, Houndour, Stantler (woods) |
| Florida (IMG, Bradenton) | tropical coast/swamp | Totodile-line (wild gators), Krabby, Exeggcute (palms), Slugma, Machop (athletes), beach water |
| Hallandale Beach, FL ("The Hemispheres") | retiree resort / beachfront courts | Slowpoke (slow retirees), Psyduck, Hitmonchan/Hitmonlee (ballers), Exeggutor, Corsola, Chansey (the nurse/early-bird crowd) |

> **Note:** Shane's favorites stay special: **Mankey** features around Randalls/Bronx (Fighting),
> **Gastly/Haunter** in spooky night spots (Central Park at night, old buildings), **Eevee** stays
> Jared-only, **Dragonite** stays the Mewtwo-Island one-off.

## ★ LOCATION & GYM BUILDOUT (in progress — going through each, in order)
Per-location detail captured here as we walk through them. Format per entry: **map concept →
resident NPCs → story beat → (if gym) leader / type / team theme / puzzle.** Gym **types** default
to PC's existing types under new skins unless we re-theme (your call per gym).

### ★ GYM TYPES & BADGES (locked 2026-06-22 — supersedes the per-gym "Type: TBD" lines)
15 distinct mono-types + an Eeveelution finale. (Grass = Central Park wilds; Electric/Dragon → E4/Champion.)

| # | Gym (leader) | Type | Badge |
|---|---|---|---|
| 1 | Sidley Austin (Altorelli) | **Dark** | Counsel |
| 2 | The Town School (Ms. Sokotch) | **Psychic** | Honor Roll |
| 3 | Randalls Island (youth baseball) | **Fighting** | Sandlot |
| 4 | Riverdale Country School (Randolph) | **Ghost** | Old Manor |
| 5 | Yankee Stadium (Aaron Judge) | **Flying** | Pinstripe |
| 6 | Brooklyn Law (→ Cali) | **Steel** | Gavel |
| 7 | Old Brookville (Jesse) | **Fairy** | Hearth |
| 8 | Montauk Lighthouse (Giovanni) | **Ground** | Rocket |
| 9 | Paramus (Grandma Linda) | **Normal** | Family |
| 10 | UPenn frat "4K" (Shermfather) | **Poison** | Keg |
| 11 | Emory Law (Madi) | **Fire** | Peach |
| 12 | Hallandale "Hemispheres" (Roman) | **Water** | Tide |
| 13 | IMG Academy (J.R. Murphy) | **Rock** | Prospect |
| 14 | Killington (Prof. Oak) | **Ice** | Summit |
| 15 | Camp Pontiac (Kenny & Ricky) | **Bug** | Cabin |
| 16 | Central Synagogue (Justin) | **Eeveelutions (mixed)** | Rival |

> Notes: Gym 4 Ghost lets **Gastly/Haunter** (Shane's fave) headline a gym; Gym 16 = Justin's full **eeveelution** army (his Eevee obsession) instead of one type. Override any cell freely.

### ★ COMPETITIVE BATTLE BUILDS (ALL gyms + endgame) — WIP, one at a time
**Every notable trainer** — all **16 gym leaders**, the Mewtwo-Island bosses, the **Elite Four**, the
**Champion**, and **rival Justin** — fields a **full 6-Pokémon team**, built to be **genuinely hard**.
- **Two methods:** **Pokémon-canon characters** (Giovanni, Prof. Oak, Steven, Red, guest champs) → their
  **canon/signature** teams. **Real-life people** (Altorelli, Sokotch, Randolph, Judge, Jesse, Grandma
  Linda, Shermfather, Madi, Roman, J.R. Murphy, Kenny & Ricky, Justin, Cali, Dad, Mom, Jared) → a
  **themed competitive Smogon singles team** from a **tier matched to gym order**.
- **Species flavor:** lean **Kanto-151** (Shane's favorites) + a **sprinkle of newer standouts** per team.
  Modern (Gen-9) mechanics/sets on a Gen-1-heavy roster. *(Kanto has no Dark-types → "Dark/villain" gyms = menacing Kanto mons + newer Dark sprinkles.)*
- **Difficulty:** **brutal, shifted early** (player-side scaling toggles to be added later).
- **Tier ladder (real-life leaders):** G1 **NU** · G2 **RU** · G3 **RU** · G4 **UU** · G5 **UU** · G6 **UU/OU**
  · G7 **OU** · G9 **OU** · G10 **OU** · G11 **OU** · G12 **OU** · G13 **OU+** · G15 **OU+/Ubers** · G16 Justin
  **Ubers** · E4 **OU+/Ubers** · Champion Jared **Ubers**. *(G8 Giovanni & G14 Oak = canon.)*

#### GYM 1 — John Altorelli (Sidley Austin) — Dark/villain lean (NU-caliber, brutal) — ~Lv 18
Real-life leader → themed team. Corporate-coup + Russian-spy boss. Kanto menace + Dark sprinkles. Ace = **Kingambit**.
1. **Kingambit** @ Black Glasses — *Supreme Overlord* — Adamant — 252 Atk / 4 Def / 252 Spe — **Sucker Punch · Iron Head · Kowtow Cleave · Swords Dance** *(ACE — the ruthless senior partner; newer sprinkle)*
2. **Gengar** @ Life Orb — *Cursed Body* — Timid — 252 SpA / 4 SpD / 252 Spe — **Shadow Ball · Sludge Wave · Nasty Plot · Focus Blast** *(Kanto — the slippery fixer)*
3. **Alakazam** @ Focus Sash — *Magic Guard* — Timid — 252 SpA / 4 SpD / 252 Spe — **Psychic · Shadow Ball · Focus Blast · Encore** *(Kanto — the cold mastermind)*
4. **Persian** @ Silk Scarf — *Technician* — Jolly — 252 Atk / 4 Def / 252 Spe — **Fake Out · Knock Off · U-turn · Throat Chop** *(Kanto — the smooth-talking corporate cat)*
5. **Weezing** @ Black Sludge — *Levitate* — Bold — 252 HP / 252 Def / 4 SpD — **Sludge Bomb · Will-O-Wisp · Pain Split · Toxic Spikes** *(Kanto — toxic-corporate wall)*
6. **Weavile** @ Heavy-Duty Boots — *Pressure* — Jolly — 252 Atk / 4 Def / 252 Spe — **Triple Axel · Knock Off · Ice Shard · Low Kick** *(newer sprinkle — the Russian-spy assassin)*

#### GYM 8 — Giovanni (Montauk Lighthouse) — Ground / canon Gen-1 Rocket — ~Lv 53
Ace = **Nidoking**. Historical-best Giovanni, built competitively.
1. **Nidoking** @ Life Orb — *Sheer Force* — Modest — 252 SpA / 4 SpD / 252 Spe — **Earth Power · Sludge Wave · Ice Beam · Flamethrower** *(ACE; Sheer Force cancels Life Orb recoil → nukes)*
2. **Nidoqueen** @ Black Sludge — *Sheer Force* — Bold — 252 HP / 252 Def / 4 SpA — **Earth Power · Sludge Wave · Ice Beam · Stealth Rock** *(bulky hazard setter)*
3. **Rhydon** @ **Eviolite** — *Lightning Rod* — Adamant — 252 HP / 4 Atk / 252 Def — **Earthquake · Stone Edge · Megahorn · Swords Dance** *(the "sick" pick — Eviolite makes the NFE wall absurdly bulky; Lightning Rod patches its Electric weakness)*
4. **Marowak** @ **Thick Club** — *Rock Head* — Adamant — 252 Atk / 252 Spe / 4 HP — **Bonemerang · Stone Edge · Fire Punch · Double-Edge** *(Thick Club doubles Attack → nuclear)*
5. **Dugtrio** @ Focus Sash — *Arena Trap* — Jolly — 252 Atk / 4 Def / 252 Spe — **Earthquake · Stone Edge · Sucker Punch · Reversal** *(traps & revenge-kills — Giovanni mind-games)*
6. **Persian** @ Silk Scarf — *Technician* — Jolly — 252 Atk / 4 Def / 252 Spe — **Fake Out · U-turn · Knock Off · Throat Chop** *(his signature cat; fast utility/pivot. Swap to Rhyperior for raw power over flavor.)*

### Proposed order / skeleton (DRAFT — reorder freely)
**ACT I — NYC & suburbs (Gyms 1–8), then Mewtwo Island:**
0. **Hudson Yards** — START hub (not a gym)
   → route: **Times Square**
1. **Sidley Austin / Midtown** — GYM 1 *(LOCKED; Altorelli + Russian-spy GF + Russian goons)*
   → route: **Central Park** → **Central Synagogue** (passed early as a landmark; the gym here is the **FINAL Gym 16** — now **Justin's** hostile takeover; see below) → Town School
2. **The Town School (UES, 76th & East End)** — GYM 2 *(LOCKED; leader **Ms. Sokotch**)*
   → **before** Gym 2: **Justin** rival battle on the **Central Park** approach *(moved here 2026-06-22)*
   → after Gym 2: **John Jay Park** — battle **Steven & Jake** (Justin present, but does **not** battle)
3. **Randalls Island** — GYM 3 *(LOCKED; BASEBALL — teammates from Uptown/CYO/Thunder/Gothams/
   Riverdale/Titans; N&E of the Town School, an island)*
4. **Riverdale Country School (the Bronx)** — GYM 4 *(LOCKED; leader **Dominic A.A. Randolph**, British old guy)*
5. **Yankee Stadium (the Bronx)** — GYM 5 *(LOCKED; leader **Aaron Judge** — PRO baseball; after Riverdale)*
   → **Chelsea Piers** (LOCATION, not a gym) + **West Village** (Mom & Dad), via Riverside greenway
   → route after West Village: **Canal Street → Brooklyn Bridge → Williamsburg**
6. **Brooklyn Law School (Williamsburg)** — GYM 6 *(LOCKED; **Cali** here — beat the leader to get her)*
   → route: **Brooklyn → east onto Long Island**
7. **Old Brookville (Long Island)** — GYM 7 *(LOCKED; leader **Jesse**, sweet old former nanny;
   coexists with grandparents' memorial)*
   → route: **continue east to the tip of Long Island**
8. **Montauk Lighthouse** — GYM 8 *(LOCKED; LAST Act-I gym; leader **Giovanni** / Team Rocket boss — bridges to Mewtwo Island)*
   → **after Gym 8: courier DRAGONITE arrives in MONTAUK (battle+catch) → sail to MEWTWO ISLAND arc**
**ACT II — opens by waking in apt 28C → West Village (Dad) → Paramus. (Gyms 9–16), then Elite Four:**
9. **Paramus, NJ** — GYM 9 *(LOCKED; leader **Grandma Linda**, goons = uncles **Matt/Jason/Donut**,
   whole town knows them; reached via **West Side Hwy → GW Bridge → Fort Lee → Paramus**;
   **Fort Lee** LOCATION has **Hiram's** hot dogs & **Dong Bang** Korean BBQ)*
10. **UPenn / APES frat house "4K" (Philly)** — GYM 10 *(LOCKED; 3-story frat-house gym; leader **The Shermfather**; from Paramus south to Philly)*
11. **Emory Law School (Atlanta)** — GYM 11 *(LOCKED; leader **Madi**, blonde ex-classmate, +**Brielle**;
    FLY from Philly; locations: Emory baseball field, Fox Bros BBQ)*
12. **Hallandale Beach, FL — "The Hemispheres"** — GYM 12 *(LOCKED; BASKETBALL old-folks resort;
   leader **Roman** (old guy who runs the snack bar); FLY from Atlanta)*
13. **IMG Academy (Bradenton FL)** — GYM 13 *(LOCKED; BASEBALL; leader **J.R. Murphy**; path across FL from Hallandale)*
14. **Killington, VT** — GYM 14 *(LOCKED; Ice, gym ATOP the mountain; leader **Professor Oak**
   (researching an undiscovered legendary); Kenny=slope NPC; **Wobbly Barn** + **Jamaican jerk**
   on the slopes; FLY from Florida)*
15. **Camp Pontiac (Copake, NY)** — GYM 15 *(LOCKED; TWO leaders **Kenny & Ricky**; south from Killington)*
16. **Central Synagogue (NYC)** — GYM 16 *(LOCKED 2026-06-22; FINAL gym; **JUSTIN (rival) has seized the synagogue as his gym** — mirrors Altorelli/Gym 1. Congregation (incl. **Rabbi Angela**) forced to allow it; trainers = synagogue members. The climactic rival battle.)*
   → from **Camp Pontiac**, return **south to Manhattan** for the homecoming finale
   → **16th badge → ELITE FOUR (+ Champion / secret final boss)**

> Non-gym story spots/LOCATIONS to thread between gyms: **Central Synagogue** (between Central Park
> & Town School), **Chelsea Piers** (sports complex), **West Village** (Mom & Dad), **Riverside
> Park** greenway, NYC **bars** & **dispensaries**, the UES **childhood homes**. **Joe's Pizza** +
> location-based Marts/Centers everywhere.

### #0 — HUDSON YARDS (start hub) — DRAFT
- **Map concept:** sleek glass high-rise + ground-level plaza/street (the real Hudson Yards "Vessel"
  could be a landmark set-piece). Interior: Shane's **apt 28C** (bedroom/start), elevator,
  **floor 11** (Jared's apt 11I), lobby.
- **NPCs:** **Jared** (Shane's brother — send-off, gives gear + the **Eevee**), doormen **Robert,
  Eric, Jose, Jonathon** (each a distinct funny personality), neighbors.
- **Story beat:** wake in 28C → elevator to fl 11 → Jared (his brother) gives the Eevee + sends Shane off →
  exit into Hudson Yards plaza → head to **Times Square** (route) → Gym 1.
- **Not a gym.** Heal/shop: a location-appropriate Center + Mart here (e.g., a CityMD + a Duane
  Reade), plus a **Joe's Pizza**.

### #1 — SIDLEY AUSTIN / MIDTOWN — GYM 1 — DRAFT
- **Map concept:** corporate law-firm skyscraper, **multi-story gym** (ride elevators between
  floors; each floor a mini-area with trainers — reception, bullpen, conference rooms, partner's
  corner office at the top). Floor-puzzle navigation.
- **Approach (route):** **Times Square** — funny gauntlet (knockoff Elmo/Spider-Man, Naked Cowboy,
  scalpers as trainers, flyer-handers blocking the way, tourist crowds).
- **Rival gate:** **Justin** blocks the entrance for the **first rival battle** (Eevee vs Eevee;
  Pilates-studio brag; Jared/wedding roast).
- **STORY (locked 2026-06-21):** **John Altorelli and his Russian-spy girlfriend are back
  together**, and together they've **taken over Sidley Austin and converted the firm into his
  gym.** It's now their turf — a corporate-coup vibe. **His goons are all Russian** (Russian
  agents/heavies as the grunt trainers throughout the floors).
- **Leader:** **John Altorelli** (atop the tower), with the **Russian spy GF** as his right-hand
  (mini-boss / co-leader fight, or a guaranteed battle just before him). **Type:** TBD — keep PC
  Gym-1 type or re-theme. Given the Russian-spy angle, **Poison/Dark "spy" flavor** could fit
  better than white-collar; decide when we set gym types.
- **Team theme:** Altorelli = sharp corporate-lawyer mons; the spy GF + Russian goons = sneaky
  Poison/Dark "agent" mons. Full roast — billable hours, vodka jokes, "is colluding," etc.
- **Badge/reward:** TBD name.

### CENTRAL SYNAGOGUE — GYM 16 (FINAL gym; JUSTIN's takeover) — DRAFT
- **The final gym (locked 2026-06-22):** Central Synagogue — Shane's own synagogue/community on the
  **Upper East Side** — is the last gym before the Elite Four. Seen **early** as a passed-by landmark
  on the Gym 1→2 route (brief bar-mitzvah-era memory); the gym opens for the **homecoming finale**
  when Shane returns to Manhattan from Camp Pontiac (Gym 15).
- **Early walk-by KEPT + wild encounters (locked 2026-06-22):** the early visit on the Gym 1→2 route
  stays. **Pokémon are catchable *inside* the synagogue** — a small indoor encounter pool, available
  from that first early visit (not just at the endgame). *(Pick the species set at the encounter pass — something fitting/meaningful.)*
- **Leader (CHANGED 2026-06-22): JUSTIN — the rival.** Justin has **taken the gym over by force** and
  installed himself as its leader — a **hostile takeover** that deliberately **mirrors John Altorelli
  seizing Sidley Austin at Gym 1** (the game is bookended by takeovers). This is the **climactic rival
  battle**, the peak of Justin's Napoleon-complex arc.
- **Rabbi Angela (locked 2026-06-22):** a **young-ish Korean Jewish woman**, a member of the synagogue
  (no longer the leader). She makes clear to Shane that **the congregation does NOT want Justin as their
  gym leader — but he seized it, so they have no choice but to let it stand.** She quietly roots for Shane.
- **Trainers:** still **the synagogue community** — congregants, cantor, board members, Hebrew-school
  teachers, b'nai-mitzvah kids, the gabbai — but now **reluctantly fielded under Justin's regime**
  (resentful flavor; some may subtly help Shane).
- **Type / Badge:** TBD — Justin's team is his strongest of the game (rival capstone). Confirm at the type pass.

### #2 — THE TOWN SCHOOL (Upper East Side, 76th & East End) — GYM 2 — DRAFT
- **Approach (route):** from Gym 1, head **through Central Park** to reach the Upper East Side.
  **Central Park** = the green route between Gyms 1 and 2 (joggers, dog-walkers, chess hustlers as
  trainers; bug/grass/normal wilds; Gastly at night).
- **PRE-GYM RIVAL BATTLE (moved here 2026-06-22):** on the **Central Park** approach to the Town
  School (at the gym gate), **Justin** reappears to block Shane for a **rival battle — fought
  *before* Gym 2.** *(This is the rival fight relocated out of the post-gym John Jay Park gauntlet.)*
- **Location:** **The Town School**, **76th St & East End Ave**, Upper East Side.
- **Leader (locked 2026-06-21):** **Ms. Sokotch** — give her **glasses**, and make her **mean and
  ugly** (the strict, terrifying schoolteacher). Full-roast battle-axe energy.
- **Map concept:** prep-school building — classrooms, hallways, lockers; students/teachers as grunt
  trainers; detention/principal's-office vibe.
- **Type:** TBD (school → could be Normal or Psychic "teacher's pet/smarty" theme).
- **Badge/reward:** TBD name.
- **POST-GYM BATTLES (updated 2026-06-22):** right after beating the Town School gym, in **John Jay
  Park** (real UES park, ~76th by the East River, right outside the school), Shane battles a
  gauntlet of **Steven** and **Jake** — two back-to-back trainer battles. **Justin is also present
  at John Jay Park, but does NOT battle Shane here** (story-beat/taunt cameo only; his rival battle
  now happens *before* the Town School gym). *(Steven & Jake = new characters — friends/classmates; confirm.)*

### #3 — RANDALLS ISLAND — GYM 3 (BASEBALL) — DRAFT
- **Approach:** from **John Jay Park**, head **north & east to Randalls Island** (an island, across
  the water — geographically correct: NE of the UES). Surf/bridge crossing to reach it.
- **Theme (locked 2026-06-21):** **BASEBALL gym** on the ballfields. Shane's HS/youth baseball
  showcase.
- **Grunt trainers = people Shane actually played ball with**, grouped by his real teams/leagues:
  **Uptown, CYO, the Thunder, the Gothams, Riverdale, the Titans.** (Use these as the trainer
  classes/teams you fight across the fields — dugouts, bases, outfield as the gym "rooms.")
- **Type:** likely **Fighting** (athletic hitters) — "swing for the fences." Confirm at type pass.
- **Leader:** TBD (a coach? a standout teammate?). **Badge/reward:** TBD name.
- *Note:* this makes **Randalls = the baseball gym**, so the earlier **Yankee Stadium** slot
  (Gym 5 draft) should be re-themed to avoid duplication (e.g., a pro-sports/Flying "big leagues"
  gym, or a non-baseball venue). Flagged.

### #4 — RIVERDALE COUNTRY SCHOOL — GYM 4 — DRAFT
- **Location:** **Riverdale Country School** (NW **Bronx**, north of Randalls/Manhattan — fits the
  north spine). Reached heading north from Randalls Island.
- **Leader (locked 2026-06-21):** **Dominic A.A. Randolph** — a **British old guy** (the posh,
  proper headmaster). Dry wit, refined-roast energy.
- **Map concept:** elite prep-school campus — quad, classrooms, headmaster's study; faculty &
  prep-school students as grunt trainers (contrast to Town School's vibe).
- **Type:** TBD (another school gym — differentiate from Gym 2: maybe Psychic/Normal "old-money
  academia," or Steel "stiff upper lip"). Confirm at type pass.
- **Badge/reward:** TBD name.

### #5 — YANKEE STADIUM — GYM 5 (AARON JUDGE) — DRAFT
- **Location:** **Yankee Stadium** (South **Bronx**) — comes right after Riverdale (both Bronx).
- **Leader (locked 2026-06-21):** **Aaron Judge** — PRO baseball, "the big leagues." The pinnacle
  baseball gym (vs Randalls = Shane's own youth/amateur ball). A genuine slugger boss.
- **Map concept:** the ballpark — dugouts, bullpen, the field, the stands; pro players/coaches as
  grunt trainers; walk-up-music flavor.
- **Type:** likely **Fighting** (power hitters) or a "big-league" Normal/Flying flavor. Confirm.
- **Badge/reward:** TBD (a baseball-themed badge).

### CHELSEA PIERS — LOCATION (NOT a gym, changed 2026-06-21)
- **Location:** **Chelsea Piers** sports complex, Hudson River waterfront (west side), **just north
  of the West Village**. Explorable hangout: golf range, **batting cages**, ice rink, gymnastics,
  pools — flavor + possible mini-games (batting cages!), athlete NPCs. **No gym here.**
- **Adjacent story town: WEST VILLAGE** — **Mom & Dad** live here (Shane's later childhood home).
  Family story beat (visit the folks, gift/heal, parental roast).
- **Route Chelsea Piers ↔ West Village = the West Side Highway / Riverside Park greenway** (the
  long, narrow **4-tiles-wide** riverfront green path along the Hudson).

### #6 — BROOKLYN LAW SCHOOL (Williamsburg) — GYM 6 — DRAFT
- **Route (locked 2026-06-21):** from **West Village**, take **Canal Street** → over the **Brooklyn
  Bridge** → into **Williamsburg, Brooklyn**. (Great set-piece route: Canal St market chaos, then
  the iconic bridge crossing into Brooklyn.)
- **Location:** **Brooklyn Law School** campus, where **Cali** (Shane's younger sister) is a student.
- **Story beat:** Shane has to **get to / free Cali on campus**, which is **gated behind defeating
  the school's gym leader.** Beat the gym → reach Cali (sibling reunion / she joins or helps).
  *(Confirm the exact beat — is Cali stuck/"captured" by the gym, and Shane springs her? Logged as
  "get Cali by beating the leader.")*
- **Map concept:** law-school campus — lecture halls, library, mock courtroom; law students as
  grunt trainers (gunner/1L roast energy).
- **Leader / Type / Badge:** TBD (a dean/professor? law-themed; could be Psychic "know-it-all" or
  Normal). Confirm at type pass.

### #7 — OLD BROOKVILLE (Long Island) — GYM 7 — DRAFT
- **Route:** from **Williamsburg/Brooklyn**, head **east onto Long Island** to **Old Brookville**
  (leafy Nassau County estates). Geographically correct (Brooklyn → LI east).
- **Leader (locked 2026-06-21):** **Jesse** — an **old Black woman who used to be the family
  nanny**, a **super sweet lady.** WARM tone (not roast) — affectionate, proud-of-you energy; a
  gentle-but-surprisingly-tough gym. A heartfelt highlight.
- **Coexists with:** the **maternal grandparents' remembered home** (they've passed) — keep this
  tender/respectful; Old Brookville is the warm, emotional town of the early game.
- **Map concept:** big LI estate / the old family house & grounds; kindly household staff &
  neighbors as grunt trainers.
- **Type / Badge:** TBD (warm but tough — maybe Normal/Fairy). Confirm at type pass.
- **★ STORY BRIDGE (locked 2026-06-21):** after Shane beats her, **Jesse gives him the news that
  Montauk is overrun with Team Rocket and sends him there** (east to the tip of LI) → leads into
  Gym 8 / the Rocket takeover.

### #8 — MONTAUK LIGHTHOUSE — GYM 8 (LAST Act-I gym) — DRAFT
- **Location:** a **lighthouse in Montauk** — the far **eastern tip of Long Island** (ocean's
  edge). Iconic. Could re-skin PC's existing **Olivine Lighthouse** map (multi-floor climb).
- **Position in flow:** continues east on Long Island past Old Brookville to the very tip. **This
  is the 8th gym.**
- **★ MONTAUK IS OVERRUN WITH TEAM ROCKET (locked 2026-06-21):** the whole town/lighthouse is a
  **Team Rocket takeover** — Rocket grunts everywhere as the trainers (storm the town, fight up the
  lighthouse). Big set-piece "liberate Montauk" beat, like a Rocket-HQ raid.
- **Leader: GIOVANNI** — the **Team Rocket boss**, fought at the **top of the lighthouse** as the
  Gym 8 leader. **Perfect bridge:** beating Giovanni leads straight into **Mewtwo Island**, where
  **Team Rocket** is behind the Mewtwo experiment (Giovanni can flee/foreshadow toward the island).
  **Type:** Ground/Rocket (canon-ish). Badge TBD.
- **★ AFTER GYM 8 (locked 2026-06-21): the courier DRAGONITE arrives in MONTAUK** with Mewtwo's
  invitation (battle + catch it) → **sail from Montauk's ocean tip out to MEWTWO ISLAND** → the
  *First Movie* arc. (Montauk = the literal jumping-off point to the island. Perfect.)

> ✅ **GYM COUNT RESOLVED:** Act I = exactly **8** (Sidley, Town School, Randalls, Riverdale,
> Yankee, Brooklyn Law, Old Brookville, Montauk). Act-II candidates (UPenn, Emory, IMG, Camp
> Pontiac, Killington, Hallandale, Central Synagogue, Paramus) = **8** → fits 9–16 cleanly.

### ACT II OPENING + #9 — PARAMUS, NJ — GYM 9 (Grandma Linda) — DRAFT
- **Act II kickoff (locked 2026-06-21):** after Mewtwo Island, Shane **wakes in apt 28C** → goes
  home to the **West Village** → **Dad tells him to visit Paramus.**
- **Route to Paramus (locked):** from the **West Village**, take the **West Side Highway** north up
  to the **George Washington Bridge**, cross into NJ to **FORT LEE**, then on to **Paramus**.
- **FORT LEE (LOCATION, next to Paramus):** NJ town just over the GW Bridge. Landmarks:
  **Hiram's** (famous hot dogs) and **Dong Bang** (Korean BBQ) — make them enterable food spots
  (flavor NPCs, maybe heal/buff items). Korean-BBQ + hot-dog-joint gags.
- **PARAMUS = GYM 9:**
  - **Leader (locked): GRANDMA LINDA** — the gym leader, at her Paramus home/turf.
  - **Goons (locked): the uncles MATT, JASON (twins) & DONUT (short)** = her grunt trainers
    (you battle all of them on the way to Grandma). **The whole town knows them** — every NPC
    references the famous/crazy uncles and Grandma Linda.
  - **Tone:** affectionate full-roast family chaos (mall suburbia, "eat something," uncle antics,
    Donut's Napoleon-complex bit echoing Justin).
  - **Type / Badge:** TBD (Grandma → Fairy/Normal? uncles → mixed). Confirm at type pass.

### #10 — UPENN / APES FRAT HOUSE "4K" (Philadelphia) — GYM 10 — DRAFT
- **Route:** from **Paramus, NJ**, head **south to Philadelphia** (UPenn) — the start of the
  college leg.
- **Location (locked 2026-06-21):** the **APES frat house**, a **3-story** building nicknamed
  **"4K"** — a **multi-story gym** (climb the frat house floor by floor: basement/party floor,
  bedrooms, rooftop).
- **Leader (locked): "The Shermfather"** — frat-boss gym leader (Godfather-meets-frat energy).
- **Theme:** beer-soaked frat chaos — pledges/brothers as grunt trainers, beer-pong tables, solo
  cups, "is this a darty?" Full roast.
- **Type / Badge:** TBD (Poison "party/keg" flavor fits, or Fighting). Confirm at type pass.
- **★ WAWA (locked 2026-06-21):** put a **Wawa** in Philly as the local PokéMart re-skin (the
  iconic Philly convenience store — hoagies, late-night runs). Flavor NPCs + Mart function.

### #11 — EMORY LAW SCHOOL (Atlanta) — GYM 11 — DRAFT
- **Travel (locked 2026-06-21):** from **Philadelphia, FLY to Atlanta** (a flight hop — the
  Act-II long-distance travel; e.g., board a plane / fast-travel from a Philly airport).
- **Location:** **Emory Law School**, Atlanta. **Gym 11** is here.
- **Nearby LOCATIONS (locked):**
  - **Emory baseball field** — explorable (another nod to Shane's baseball; teammates/pickup game).
  - **Fox Bros Bar-B-Q** — famous Atlanta BBQ joint; enterable food spot (flavor NPCs, heal/buff
    items, brisket gags).
- **Theme:** grad-school / law-campus down south; law students + southern flavor as grunt trainers.
  Full roast (benders, "y'all," grad-school burnout).
- **Leader (locked 2026-06-21): MADI** — a **blonde female ex-classmate** of Shane's. With **Brielle**
  (another girl) also present — co-leader / right-hand / fellow trainer at the gym. Full-roast
  ex-classmate banter.
- **Type / Badge:** TBD. Confirm at type pass.

### #12 — HALLANDALE BEACH, FL — "THE HEMISPHERES" — GYM 12 — DRAFT
- **Travel (locked 2026-06-21):** from **Atlanta, FLY to The Hemispheres** (Hallandale Beach, SE
  Florida coast).
- **Location:** **"The Hemispheres"** — an old-folks **retirement resort**, BASKETBALL-centric;
  retirees as ballers/grunt trainers (pickup games, walkers, early-bird specials — full roast).
- **Leader (locked): ROMAN** — the **old guy who runs the snack bar.** Unassuming snack-bar
  attendant who turns out to be the gym boss (deceptively tough). "You want a hot dog or a beating?"
- **Type / Badge:** TBD (basketball/athletic → Fighting or Normal). Confirm at type pass.

### #13 — IMG ACADEMY (Bradenton, FL) — GYM 13 (BASEBALL) — DRAFT
- **Route (locked 2026-06-21):** a **path across Florida** from **The Hemispheres (Hallandale, SE
  coast)** to **Bradenton (SW coast)** — overland route (Everglades/highway flavor; wild gators).
- **Location:** **IMG Academy** — the elite sports academy. **BASEBALL-themed gym** (Shane's 4th
  baseball-flavored gym: Randalls youth ball → Yankee pros → now IMG elite-prospect academy).
- **Leader (locked): J.R. MURPHY** — pro/prospect catcher as the gym boss; hard-nosed academy
  trainer energy. Elite athletes/coaches as grunt trainers.
- **Type / Badge:** TBD (athletic/baseball → Fighting). Confirm at type pass.

### #14 — KILLINGTON, VT — GYM 14 (skiing / Ice) — DRAFT
- **Travel (locked 2026-06-21):** from **Florida (IMG), FLY to Killington, Vermont** — far north,
  snowy mountains (big climate swing south→north).
- **Mountain layout:** ride the **chairlift up the slopes**; the **GYM is at the TOP of the
  mountain.** Skiers/snowboarders as grunt trainers; Ice theme.
- **On-slope LOCATIONS:** the **Wobbly Barn** (heal/après-ski spot) and a **Jamaican jerk
  restaurant** on the slopes (food spot, flavor NPCs — jerk-chicken-in-the-snow gag).
- **Leader (CHANGED 2026-06-21): PROFESSOR OAK** — he's up on the mountain **conducting research
  on an UNDISCOVERED LEGENDARY** (a snow/ice legendary he's tracking). Oak as a gym leader is a fun
  twist; tie his battle/research to the mystery legendary (sightings, a research-lab room at the
  summit, an encounter/discovery event). Fits the "legendaries on second-half leaders" rule.
- **Kenny (Dad's friend)** — keep him as a **slope NPC** here (the chairlift-spitting-on-people
  bit) rather than the leader. (This also de-duplicates Kenny vs the Camp Pontiac Kenny.)
- **Type / Badge:** TBD (Ice; Oak's team incl. the legendary?). Confirm at type pass.

### #15 — CAMP PONTIAC (Copake, NY) — GYM 15 — DRAFT
- **Route (locked 2026-06-21):** from **Killington, VT**, head **south to Camp Pontiac in Copake,
  NY** (Hudson Valley / Columbia County — geographically correct, VT → just south into NY).
- **Location:** summer **Camp Pontiac** — woods, lake, cabins, mess hall; campers/counselors as
  grunt trainers; nostalgic-camp Bug/Grass/Water vibe.
- **Leaders (locked): KENNY & RICKY** — **old TWIN doctors who OWN the camp**; both have **glasses
  and white hair.** Two gym leaders → **twin double-battle / tag-team** gym (matching twin sprites).
  Eccentric old-doctor-camp-owner energy.
- **Note:** this **camp Kenny (twin doctor)** is distinct from the **Killington Kenny (Dad's
  friend)** — same first name, different characters. (Flagging in case you want to rename one.)
- **Type / Badge:** TBD (camp → Bug/Grass/Water). Confirm at type pass.

## ★ GAMEPLAY MECHANICS / DATA TWEAKS
- **Max IVs + competitive natures for all Pokémon (2026-06-21):**
  - **Max IVs/DVs** (perfect) on Pokémon — so every catch is competitively viable. (PC keeps DVs
    for color variation but separates natures; we force perfect DVs.)
  - **Competitive nature per species, chosen by the line's FINAL evolution's role:** build a
    per-evolution-line nature table — physical attackers → **Adamant** (or **Jolly** if speed-
    reliant), special attackers → **Modest**/**Timid**, bulky/defensive → **Bold/Impish/Calm/
    Careful**, mixed/utility → neutral, etc. Every member of a line inherits the nature picked for
    its final evo (e.g., Gastly/Haunter → nature optimized for the line; Mankey/Primeape → physical
    attacker nature).
  - **Scope (confirmed 2026-06-21): EVERYTHING — universal.** Wild encounters, the player's party,
    every opponent (rivals, ALL trainers, gym leaders, E4, bosses, in-game gifts/trades). No
    Pokémon anywhere has weak IVs or a non-competitive nature.
  - *Impl:* (1) force perfect DVs in the wild/gift generation path; (2) add a species→nature lookup
    (by final-evo line) and apply on generation/gift. Build the full nature table during build phase.

## Phase 2 — Content rebuild (the gift) — DETAILS BELOW
Implemented from the "Personalization Details" section as details come in.
- [ ] Story rewrite (narrative, key dialogue, events)
- [ ] Game flow / progression redesign
- [ ] All trainers re-themed & re-tuned
- [ ] Branding / title
- [ ] Intro speech
- [ ] Default names (rival = Justin)
- [ ] In-game personal messages / NPCs
- [ ] Credits dedication
- [ ] Custom characters / events / Pokémon (courier Dragonite, champion cameos)

## Phase 3 — Build, verify & deliver
- [ ] Rebuild the personalized ROM cleanly (no errors)
- [ ] Commit & push all changes to the working branch
- [ ] Deliver the playable ROM file

---

## Personalization Details (Shane's firehose — captured here)

> Everything Shane wants in the game gets logged under the right heading.
> Nothing is implemented until it's written down here first.

### Branding / Title screen
_(awaiting details)_

### Player / Rival / character names

**Rival → "Justin"**
- Default rival name = **Justin** (instead of "Silver").
- **Very short** → swap his overworld sprite (and ideally battle sprite) to a shorter/smaller
  character model. Implementation: point the rival to an existing short sprite (e.g. a youngster/
  kid-sized overworld sprite) rather than drawing new art, unless custom art is wanted.
- **Napoleonic complex** → rewrite his battle/encounter dialogue so he's arrogant, hot-tempered,
  and overcompensating — constantly asserting dominance, touchy about his height, big talk.
- Files: name default in `engine/events/specials.asm`; rival overworld/battle sprite assignment
  (TBD — locate rival sprite constant + gfx); rival dialogue across his battle scripts.

### Intro / new-game speech
_(awaiting details)_

### In-game messages, NPCs, signs, easter eggs
_(awaiting details)_

### Custom Pokémon / movesets / teams

**Shane's favorite: DRAGONITE — the "courier Dragonite" from Pokémon: The First Movie**
- The Dragonite that delivers Mewtwo's invitation letter at the start of the first movie.
- Goal: make **this specific Dragonite obtainable as a one-off (single, unique) encounter.**
- **TRIGGER (locked):** after the player **defeats Gym 8 (Montauk Lighthouse)**, the **courier
  Dragonite arrives in MONTAUK with Mewtwo's invitation.** The Dragonite is then **battleable AND
  catchable** right there — the one-off Dragonite. Accepting the invite → **sail from Montauk's
  ocean tip to MEWTWO ISLAND** (the endgame arc).
- **Ties into the ★ Armored Mewtwo storyline** (see Featured Pokémon) — the letter it delivers is
  Mewtwo's invitation; the two events form one cohesive *First Movie* arc.
- Implementation ideas (TBD):
  - A static/scripted one-time encounter (like the legendary/roaming static battles), not a
    wild-grass repeatable spawn — so it's a "deliver-the-letter" themed event.
  - Themed presentation: it arrives carrying a letter (tie the encounter script to a mail/letter
    item or a short cutscene), then can be caught/given.
  - Possibly give it a fitting moveset/level and maybe a held Mail item as a nod to the movie.
  - Location: TBD (somewhere meaningful / discoverable).
- Files (to locate during impl): static encounter / special wild battle scripts, a map script for
  the event, Dragonite species data, mail/item data if we include the letter.

### Guest characters from other regions (Champions / Gym Leaders)
- Include cross-region notables as encounterable NPCs/trainers, e.g.:
  - **Cynthia** (Sinnoh Champion)
  - **Wallace** (Hoenn Champion / Sootopolis leader)
  - **Steven Stone** (Hoenn Champion)
  - **etc.** — more to be named (Lance & Red already exist in base game).
- Implementation: add them as battleable trainers / cameo NPCs. Each needs a trainer-class +
  party definition, overworld sprite, and placement/dialogue script. Naturally pairs with the
  Hoenn/Sinnoh expansion (they can headline those regions), but cameos can also be dropped into
  the existing world as special post-game battles.
- Files (to locate): trainer party data (`data/trainers/`), trainer class constants, overworld
  sprites, and map event scripts for placement.

### Inside jokes / references / personal touches

**Shane = high school baseball player**
- Weave baseball references in: e.g., player's bedroom could have a baseball/bat decoration or a
  trophy with custom flavor text; an NPC could reference his playing days; a sign or the TV could
  mention a game. Trainer flavor / dialogue can use baseball metaphors ("swing for the fences!").
- Possible nods: a "home run" themed moment, baseball cap on the player sprite (stretch — sprite
  edit), or a held item / gift item framed as a "lucky bat/ball."

### Credits / dedication message
_(awaiting details)_

### World structure — ONE big unified region, 16 Gym Leaders (2026-06-21)
- **Want:** axe the separate Johto/Kanto identity and present **one big continuous region** with a
  single **16-gym** progression.
- **Why it fits:** Polished Crystal already has **16 badges** (8 Johto + 8 Kanto) and the engine
  supports that whole progression — so we reuse the 16-gym backbone and re-theme it as one region.
- **Keep the FULL area:** retain the entire landmass of both Johto **and** Kanto (all existing
  maps/routes/towns stay) — we want that big explorable world. We're not shrinking it, just
  unifying it. ~2 regions' worth of area = one large region.
- **Pragmatic build (feasible for the gift):** keep the existing map *geometry/connections* but
  **re-skin and re-narrate** them into one cohesive new region — new region name, new town/route
  names, unified town map narrative, one continuous journey across all 16 gyms, no "fly to Kanto"
  region break in the story. Re-theme the 16 Gym Leaders into our new cast.
- **Heavy/stretch version:** draw brand-new map layouts + custom tilesets from scratch (huge art
  task). Default to re-skin-and-reflow unless we decide to invest in new geometry.
- Cross-region champions (Cynthia/Wallace/Steven, etc.) → fold into the new cast as Gym Leaders,
  Elite Four, or special battles within the one region.

### Hoenn & Sinnoh new regions ❌ DESCOPED — superseded by the single unified region above.

---

## Known customization hook locations (researched)
For reference when implementing — exact spots in the codebase:

| What | File | Notes |
|------|------|-------|
| ROM name / version / author | `Makefile` (lines 1–15) | `NAME`, `VERSION`, `AUTHOR`; `COPYRIGHT` string also shows in-game credits |
| Title screen text/graphics | `engine/movie/title.asm`, `gfx/title/` | "POLISHED CRYSTAL" tilemap @ ~line 64; logo/version/crystal art are LZ-compressed 2bpp |
| Default player names | `data/player/default_names.asm` | Chris/Kris/Crys/Krys by gender |
| Default rival name | `engine/events/specials.asm` | "Silver" |
| Intro speech (Prof. Elm) | `data/text/common.asm` | `_ElmText1`–`_ElmText7` (~lines 3160–3240) |
| Starting bedroom (interactables) | `maps/PlayersHouse2F.asm` | PC, Radio, Journal, Poster — good for a custom note |
| Mom's house (1F) | `maps/PlayersHouse1F.asm` | Mom dialogue, TV, fridge, etc. |
| Credits sequence | `data/credits_script.asm`, `engine/movie/credits.asm` | natural spot for a dedication |

---

## Build notes / log
- Environment: Linux, has make/gcc/git/python3. RGBDS **not** preinstalled — must build v1.0.1 from source.
- _(build results logged here as we go)_
