# Meeting Notes
In this file, you are required to take notes for your weekly meetings. 
In each meeting, you are required to discuss:

1. What each member has done during the week?
2. Are there challenges or problems? Discuss the possible solutions
3. Plan for the next week for everyone
4. Deviations and changes to the project plan, if any


# Meeting 06.11.2024 14::15

**Participants**: 
1. Emmi Koskenranta
2. Veera Juuti
3. Linnea Neuvonen
4. Mikko Olkkonen
5. Liisa Särkiö

## Summary of works
1. Mikko Olkkonen
   
   Importing librarys. Results are in 'libs'
   Results are in `<libs>`. Resolved the identified problems.
   Participated in making project plan.

2. Emmi Koskenranta

  Initializing classes Game and Level and their constructors, making initial vesions of IO functions.
  Participated in making project plan.

3. Veera Juuti
   Participated in making project plan and planned structure of object classes.

4. Liisa Särkiö
   Same as above

5. Linnea Neuvonen
   Participated in making project plan and planned structure and requirements of GUI.

## Challenges

1. External libraries do not work in CMakeLists.txt.
2. Distribution of functions of classes Game and Level is not clear.
3. Understanding structure and big picture of the project is not clear.
4. No one knows how to use external libraries.

## Actions
1. Mikko gets external libraries to work in CMakeLists.txt.
2. Emmi will specify distribution of functions of classes Game and Level.
3. Liisa starts with classes Bird, Pig and one subclass of Bird
4. Veera starts defining class Object and Obstacle
5. Linnea is going to start with graphics interface start menu.
6. Mikko is going to get familiar with physics library and start working on implementing gravity in Level(?) class.

> Please reflect these action decisions in your git commit messages so that 
> your group members and advisor can follow the progress.

## Project status 
Game and Level calsses initialized and IO's initialized, external libraries imported. 



# Meeting 13.11.2024 14::00

**Participants**: 
1. Emmi Koskenranta
2. Veera Juuti
3. Linnea Neuvonen
4. Mikko Olkkonen
5. Liisa Särkiö
6. Mikhail

## Summary of works
1. Mikko Olkkonen
   
   Got external libraries to work. Results are in `libs` and `CMakeLists.txt`.
   Started working on physics for objects. Results are in `object.hpp` and `object.cpp`
   
2. Emmi Koskenranta

  Initialized GameState class and subclasses and started working on state manager. Results are in 
  `game.cpp`, `game.hpp`, `gamestate.hpp`, `menustate.hpp`, `levelstate.hpp`, `gameoverstate.hpp`,
  `leveldata.hpp`, `leveldata.cpp`

3. Veera Juuti
   Created and worked on classes Obstacle and Object. Made constructors and methods.
   Results are in `object.hpp`, `object.cpp`,
   `obstacle.hpp`.

4. Liisa Särkiö
   Created and worked on classes Bird and Pig. Made constructors and methods.
   Results are in `pig.hpp`,
   `bird.hpp`.

5. Linnea Neuvonen
   Started working on GUI: window creation, eventloop and other graphics stuff.
   Results in `main.cpp` (To be moved to game.cpp)

## Challenges

1. Errors prevent running the program.
2. Methods needed for objects unclear.
3. Unclear how and where physics should be implemented.
4. Nobody has thought about slingshot.
5. Graphics do not show yet.
6. Render class should be separate.


## Actions
1. Everyone solves errors so that program can be run.
2. Emmi and Linnea will identify methods needed for game logic and graphics.
3. Liisa and Veera will implement functions identified above.
4. Mikko will continue getting familiar with box2d and implement more physics.
5. Mikko will find out how physics should work for slingshot.
6. Liisa will start Slingshot class.
7. Emmi will integrate Slingshot to levels.
8. Linnea will work on getting a graphics window to show up and get objects on it.
9. Mikko will try to get above objects to move?
10. Linnea will start with Render class where objects are drawn.

> Please reflect these action decisions in your git commit messages so that 
> your group members and advisor can follow the progress.

## Project status 
External libraries finally work, errors prevent running, game state managing started, 
game object classes started, physics implementation started for objects, user interface 
creation started.


# Meeting 20.11.2024 14::00

**Participants**: 
1. Emmi Koskenranta
2. Veera Juuti
3. Linnea Neuvonen
4. Mikko Olkkonen
5. Liisa Särkiö

## Summary of works
1. Mikko Olkkonen
   
   Got the physics to work, made birds to fly, fine-tuned buttons. Results are in
    `menustate.hpp`, `levelstate.hpp`, `gameoverstate.hpp`,
  `leveldata.hpp`, `leveldata.cpp`
   
2. Emmi Koskenranta

  Worked on the states and rendering, initialized slingshot
  `game.cpp`, `game.hpp`, `menustate.hpp`, `levelstate.hpp`, `gameoverstate.hpp`,
  `leveldata.hpp`, `leveldata.cpp`, `slingshot.hpp`

3. Veera Juuti
   Worked in understanding project structure and physics.

4. Liisa Särkiö
   Made test class and went through the project structure 
   Results are in `tests.hpp`

5. Linnea Neuvonen
   Fixed issues with CMake. 

## Challenges

1. Make objects (birds, pigs, obstacles etc.) disappear when they get destroyed
2. We don't have the levels planned yet
3. Our slingshot is in the wrong place
4. Make the birds appear on the slingshot before they are dragged
5. Audio works unexpectedly


## Actions
1. Plan levels in textfiles - Linnea
2. Make different kinds of obstacles - Veera
3. Get the objects to disappear - Mikko
4. Implement the score calculation - Liisa
5. Implement the highscore system - Emmi
6. Get the birds' special actions to work - Mikko
7. Make more tests - everyone
8. Make pictures the right shape so they work correctly with the physics - Emmi

> Please reflect these action decisions in your git commit messages so that 
> your group members and advisor can follow the progress.

## Project status 
Graphic interface works (there are different windows), the birds can be shot into the air, physics work. Navigating between the different states. 


# Meeting 27.11.2024 14::15

**Participants**: 
1. Emmi Koskenranta
2. Veera Juuti
3. Linnea Neuvonen
4. Liisa Särkiö

## Summary of works
1. Mikko Olkkonen
   Made zoom to levelstate.
   Results in `levelstate.hpp`.
   
2. Emmi Koskenranta
   Implemented player name query and adding score to high scores after win. Results in
  `namestate.hpp`, `game.cpp`.

3. Veera Juuti
   Made glass obstacle and added lose sound. Results in `gameoverstate.hpp` and `glass_obstacle.hpp`

4. Liisa Särkiö
   Worked on getting the program to run and cmake errors fixed.

5. Linnea Neuvonen
   Worked on getting the program to run and cmake errors fixed. 

## Challenges

1. Sometimes memory allocation problems.
2. All sounds are the same.
3. All sounds play when opening the level.
4. Using special action also makes the bird to move to slingshot.


## Actions
1. Plan levels in textfiles - Linnea
2. Implement the score calculation - Liisa
3. Get the birds' special actions to work (see problem 4.) - Mikko
4. Make more tests - everyone
5. Get started on documentation - Veera, Linnea, Liisa
6. Work on audios - Veera
7. Make stars to win screen - Emmi

> Please reflect these action decisions in your git commit messages so that 
> your group members and advisor can follow the progress.

## Project status 
Basically works. Sometimes segmentation errors and memory allocation errors. Next
more special features.