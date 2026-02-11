## Angry Birds (C++ / SFML)

An Angry Birds–style 2D game implemented in modern C++ using **SFML** for graphics/audio and **Box2D** for physics.  
You can build and run it locally with CMake on macOS, Linux, or other platforms with a C++17 toolchain.

### Features
- **Multiple levels** with different arrangements of pigs and obstacles
- **Sandbox mode** for experimenting and creating your own level
- **Physics-based gameplay** using Box2D (slingshot, collisions, damage)
- **High scores** persisted to a text file
- **State-based UI**: name input, main menu, gameplay, sandbox, and game-over screens

---

## Project Structure

- **`src/`**: All game source code and assets
  - `*.cpp`, `*.hpp`: game logic, states, rendering, physics, etc.
  - `fontfiles/`: Lato font files used by UI
  - `imagefiles/`: PNGs for birds, pigs, obstacles, backgrounds, and UI
  - `soundfiles/`: WAV files for sound effects and music
  - `textfiles/`: level definitions (`level1.txt`–`level3.txt`, `sandboxlevel.txt`) and `highscores.txt`
- **`tests/`**: Unit tests for core game functionality
- **`.vscode/`**: Editor configuration (optional for VS Code / Cursor)
- **`CMakeLists.txt`**: CMake build configuration

> **Important:** The game expects to be run from the **project root** so that asset paths like `../src/...` resolve correctly.

---

## Build Instructions

### Prerequisites
- CMake **3.28+** (or slightly lower if you adjust `cmake_minimum_required`)
- A **C++17**-capable compiler (e.g. Clang, GCC, MSVC)
- Internet access on first configure/build (CMake’s `FetchContent` will download SFML and Box2D)

### Configure and Build

```bash
cd /angrybirds
mkdir -p build
cd build
cmake ..
cmake --build .
```

This produces executables in `build/bin/`:
- `angry_birds` – the main game
- `angry_birds_tests` – unit tests

---

## Running the Game

Always run the game from the **project root** (not from inside `build/`), otherwise asset loading will fail:

```bash
cd /angrybirds
./build/bin/angry_birds
```

Basic controls (may vary slightly depending on implementation):
- **Mouse drag + release**: pull back and launch the current bird with the slingshot
- **Mouse wheel / scroll**: zoom the camera in and out
- **Mouse click**: interact with menu buttons and UI

The usual flow is:
1. Enter your name.
2. Choose a level or sandbox from the main menu.
3. Launch birds to knock down structures and defeat all pigs.
4. View the game-over screen and optionally proceed to the next level or return to the menu.

---

## Running Tests

After building:

```bash
cd /angrybirds
./build/bin/angry_birds_tests
```

These tests cover:
- Loading valid and invalid level files
- Basic object damage behavior
- Menu button initialization and layout

You can add your own tests in the `tests/` folder (see its `readme.md` for details).

---

## Level Files

Level definitions live in `src/textfiles/` and are plain text. A typical level file looks like:

```text
Birds
Red
Red
Yellow

Pigs
Normal 812 584
King 907 584

Obstacles
Wood 809 552
Stone 860 499
Glass 787 578

Star
858 526
```

- **Birds**: list of bird types (`Red`, `Yellow`, …), one per line.
- **Pigs**: `Normal x y` or `King x y` (pixel positions).
- **Obstacles**: `Wood x y`, `Stone x y`, `Glass x y`, etc.
- **Star**: optional `x y` bonus/star element.

`sandboxlevel.txt` is used for the sandbox / custom level.

---

## High-Level Architecture

- **`Game`**: owns the main SFML window, view, and the stack of game states.
- **`GameState`**: abstract base for states like Name, Menu, Level, Sandbox, and Game Over.
- **`LevelData`**: parses text level files and creates the corresponding game objects.
- **`Object` / `Bird` / `Pig` / `Obstacle`**: Box2D bodies + SFML sprites for physical entities.
- **`CollisionListener`**: listens to Box2D contacts to apply damage, scoring, and cleanup.
- **`Render`**: draws the world, UI, and backgrounds each frame.

The main loop repeatedly:
1. Polls SFML events and forwards them to the active state.
2. Steps the Box2D world with a fixed timestep.
3. Renders the current state to the window.

---

## Development Notes

- If CMake 3.28 is not available on your system, you can try lowering the version requirement in `CMakeLists.txt` (e.g. to 3.18) as long as your CMake supports `FetchContent`.
- VS Code / Cursor users may want to adjust `.vscode/c_cpp_properties.json` to match their local compiler and include paths.
- Asset paths are hard-coded relative to the **current working directory**; use the project root when running or debugging.

---

## License / Attribution

Please add licensing and asset-attribution information here if required (for example, if any fonts, images, or sounds have specific license terms).