cmake_minimum_required(VERSION 3.28)

project(angry_birds LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_BUILD_TYPE Debug)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Include FetchContent for fetching external dependencies
include(FetchContent)

# Fetch SFML library
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# Fetch Box2D library
FetchContent_Declare(box2d
    GIT_REPOSITORY https://github.com/erincatto/box2d.git
    GIT_TAG v2.4.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(box2d)

# Gather all source files in src directory
file(GLOB SOURCES "src/*.cpp" "src/*.c" "src/*.hpp" "src/*.h")

# Add executable target
add_executable(angry_birds ${SOURCES})

# Set include directories
target_include_directories(angry_birds PRIVATE src)

# Link SFML and Box2D to the project
target_link_libraries(angry_birds PRIVATE sfml-graphics sfml-window sfml-system sfml-audio box2d)

# Set compiler warnings
if(MSVC)
    target_compile_options(angry_birds PRIVATE /Wall)
else()
    target_compile_options(angry_birds PRIVATE -Wall -Wextra -pedantic -Wno-missing-field-initializers)
endif()

# Special command for Windows to copy the OpenAL DLL
if(WIN32)
    add_custom_command(
        TARGET angry_birds
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:angry_birds>
        VERBATIM)
endif()

# Gather all test source files
file(GLOB TEST_SOURCES "tests/*.cpp" "tests/*.c" "tests/*.hpp" "tests/*.h")

# Add executable target for tests
add_executable(angry_birds_tests ${TEST_SOURCES})

# Link game source files to the tests
list(REMOVE_ITEM SOURCES ${CMAKE_SOURCE_DIR}/src/main.cpp) # Exclude main.cpp
target_sources(angry_birds_tests PRIVATE ${SOURCES})

# Link SFML and Box2D to the tests
target_link_libraries(angry_birds_tests PRIVATE sfml-graphics sfml-window sfml-system sfml-audio box2d)

# Include directories for tests
target_include_directories(angry_birds_tests PRIVATE src tests)