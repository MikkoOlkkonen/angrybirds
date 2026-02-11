## Source Code Overview

This folder contains the **entire game implementation** and all game **assets** (fonts, images, sounds, and level definitions).  
Only C++ source/header files and the assets actually used by the game should live here.

### Subdirectories

- **`fontfiles/`**: Lato fonts used for all in-game text and UI.
- **`imagefiles/`**: PNGs for birds, pigs, obstacles, slingshot, backgrounds, and menu elements.
- **`soundfiles/`**: WAV files for sound effects and background music.
- **`textfiles/`**:
  - `level1.txt`, `level2.txt`, `level3.txt`: main campaign levels
  - `sandboxlevel.txt`: level used by the sandbox / custom-level mode
  - `highscores.txt`: persistent high-score data

### Main C++ Components (high level)

The exact filenames may vary, but the main responsibilities are:

- **Game / Core**
  - `Game`: owns the SFML window, camera/view, and the stack of game states.
  - `GameState` (base class): interface for `processEvent`, `update`, and `render`.

- **States**
  - `NameState`: collects player name.
  - `MenuState`: main menu, level selection, sandbox access, and volume controls.
  - `LevelState`: core gameplay for each level (1–4).
  - `SandboxState`: level editor / sandbox gameplay.
  - `GameOverState`: win/lose screen, high scores, and navigation to next level or menu.

- **World & Physics**
  - `LevelData`: parses level text files and spawns the necessary objects.
  - `Object` (base), `Bird`, `Pig`, `Obstacle`: Box2D bodies + SFML sprites.
  - `CollisionListener`: Box2D contact listener used to apply damage, scoring, and object cleanup.

- **Rendering & UI**
  - `Render` (or similarly named module): draws the world, HUD, and UI.
  - Menu and button classes: manage clickable UI elements in the menus and game-over screens.

### Adding or Modifying Source Files

- Keep new gameplay features inside `src/` and group related functionality (e.g. new state, new object type).
- If you add new assets:
  - Place images in `imagefiles/`, sounds in `soundfiles/`, fonts in `fontfiles/`, and update any asset-loading code accordingly.
  - Ensure asset paths remain relative to the **project root** (`../src/...`) when running the executable.
- If you add new levels:
  - Create a new text file in `textfiles/` following the existing format.
  - Update the level-selection logic/state to reference your new file.