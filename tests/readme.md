## Test Suite Overview

This folder contains **unit and integration-style tests** for the Angry Birds C++ game.  
Tests validate key gameplay and UI behavior such as level loading, object damage, and menu buttons.

### How to Build and Run Tests

From the project root:

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
```

Then run the test executable from the **project root** so asset paths resolve correctly:

```bash
cd /angrybirds   # or your clone path
./build/bin/angry_birds_tests
```

The tests will print their results to the terminal (pass/fail per test).

### Existing Tests (Summary)

The exact filenames may differ, but the current suite covers at least:

- **Object damage tests**
  - Verifies that objects (e.g. `YellowBird`) take damage as expected when colliding.

- **Level loading tests**
  - **Valid level file**: ensures `level1.txt` loads correctly, creating the expected birds, pigs, obstacles, ground, and optional star.
  - **Invalid level file**: checks that trying to load a non-existent level throws an appropriate exception.

- **Menu button tests**
  - Confirms that the main menu initializes the expected number of buttons (e.g. “Level 1”, “Level 2”, “My Level”, “Sandbox”, “Exit/Back”).
  - Some tests for button click/hover behavior may be present but commented out (they can be re-enabled or expanded if needed).

### Adding New Tests

When you implement new features or refactor existing ones:

- Create a new `*.cpp` file in this folder for your tests.
- Include the relevant headers from `src/` and link against the game library/executable as configured in `CMakeLists.txt`.
- Prefer **small, focused tests** that check one behavior (e.g. “bird launches correctly”, “high score file updates after game over”).
- Keep tests deterministic (avoid hard-coding real-time delays or depending on external state beyond the provided assets).

Document any non-obvious or complex tests here to make it easier for others to understand what is covered.