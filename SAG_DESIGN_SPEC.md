> **CANONICAL SOURCE OF TRUTH: `SAG_BIRTHDAY_PLAN.md`** (the full 1,300-line bible: world,
> geography, all 16 gyms + leaders, the Mewtwo's Island arc, the full cast & personas, the rival
> arc, the Elite Four). READ IT FIRST. This file is a thin geo/art supplement, not the cast list.

# SAG — Design Spec (geo-accurate NYC world + name representations)

Living design doc. Captures research-backed decisions so the game can be cleaned up
to align, and so a *from-scratch* custom version could be built straight from this
(the pokeemerald CODE doesn't port to a new engine; this DESIGN does).

## 1. Geographically-accurate world (researched)
Real NYC positions of the SAG neighborhoods (sources: Wikipedia for each place):

| Order | Neighborhood (slot) | Real location | Notes |
|---|---|---|---|
| start | **Hudson Yards** (Littleroot) | West Midtown Manhattan, 30th–41st St, Hudson R. | Shane's apartment building |
| → | **Times Square / Midtown** (Oldale) | Midtown Manhattan, ~42nd–47th (Times Sq), 34th–59th (Midtown) | central Manhattan |
| → | **Upper East Side** (Dewford) | Manhattan, east side, ~59th–96th | "old money" |
| ↑ N | **Randalls Island** (Mauville) | East/Harlem River island off NE Manhattan; bridges to E. Harlem, S. Bronx, Astoria | parks & ballfields |
| ↑ N | **The Bronx / Yankee Stadium** (Petalburg) | South Bronx, ~161st; north of Manhattan over the Harlem River | **JUDGE gym = Aaron Judge / Yankees** |
| ↑ N | **Riverdale** (Lavaridge) | NW Bronx, leafy, wealthy; north of the Bronx proper | quiet end of the Bronx |
| ↓→ SE | **Williamsburg** (Fortree) | North Brooklyn, on the East River, across from LES/Midtown-east | hip/artsy |
| → E | **Old Brookville** (Mossdeep) | Nassau County, North Shore "Gold Coast" of Long Island, ~25 mi E of Manhattan | wealthy LI; **GRANDMA gym** |
| → E end | **Montauk** (Sootopolis) | Tip of Long Island's South Fork, ~118 mi E | the finale; **GIOVANNI / Team Rocket** |

**Accurate flow:** Manhattan (Hudson Yards → Times Sq → UES) → up into the Bronx
(Randalls Is. → Yankee Stadium → Riverdale) → over to Brooklyn (Williamsburg) → out
east on Long Island (Old Brookville → Montauk). Current SAG order is close but linear;
a *truly* accurate world is 2D (needs a connection re-wire — risky to the live game).

## 2. Name representation guide
**Public / researched — align in-game to these:**
- **JUDGE** (Gym 5, Bronx, Flying) = **Aaron Judge**, Yankees slugger → pinstripes, "the big
  leagues", out-of-the-park; gym sits in the Bronx (Yankee Stadium). ✔ baseball persona set.
- **GIOVANNI** (Gym 8, Montauk, Ground) = Team Rocket boss → mob/syndicate. ✔ (Mewtwo arc.)
- **ALTORELLI** (Gym 1, Dark, lawyer) = "**Sidley Austin**" — a real global-elite firm
  (M&A, litigation, white-collar; "prestigious, not cutthroat"). → high-powered corporate
  law firm. ✔ persona set; could add firm-specific flavor.
- **THE DEAN** (Gym 6, Williamsburg, Steel) = academic/legal authority → "order in the court".
- Neighborhood vibes (above) → town flavor/NPCs.

**Personal — Shane's real people (NOT Google-able; need Jared/the user to fill in):**
ALTORELLI*, SOKOTCH, RANDALL, RANDOLPH, GRANDMA (Jesse), and the family E4 (CALI, DAD, MOM,
STEVEN) + rival JUSTIN. *(Altorelli may be both a person and the firm.)*
→ For each, one line on who they are / their vibe so trainers/NPCs/teams represent them.

## 3. Building / cleanup rules
- Every visible building should be **enterable** (real interior) or not be there.
- Multi-floor buildings use the **real elevator** (`MULTI_SAG_FLOORS` floor-select menu),
  not invisible portal rooms. (Apartment = Lobby + Fl 28 Shane + Fl 11 Jared, v0.61.)
- Reuse only **orphaned** interiors (0 external warp-refs) and relabel their region section.

## 4. Art-blocked (needs a human artist / Porymap — can't do headless)
- **Region-map base image** (`graphics/pokenav/region_map/map.png`) is still the Hoenn
  coastline. A real **NYC-shaped** map needs new pixel art. (Label POSITIONS are data-editable.)
- **Custom leader/NPC sprites** (all still use placeholder overworld gfx).
- **Bespoke NYC street/building tiles** (currently reused Hoenn tilesets).
These are the "nothing reused / fully custom" layer — commission art or build in an engine
where you control assets; this spec is the design source either way.
