cmake_minimum_required(VERSION 3.28) 
project(Annoyed_Flocks) 

# Standards and Directories
set(CMAKE_CXX_STANDARD 20) 
set(CMAKE_CXX_STANDARD_REQUIRED ON) 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) 

include(FetchContent) 

# --- Fetch GoogleTest ---
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip 
)

# --- Fetch Box2D ---
FetchContent_Declare(
  box2d
  GIT_REPOSITORY https://github.com/erincatto/box2d.git
  GIT_TAG        v2.4.1
)

# --- Fetch SFML ---
FetchContent_Declare(
  sfml
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG        2.6.x 
)

# Optional: Disable SFML features you don't need to speed up build time
set(SFML_BUILD_AUDIO ON CACHE BOOL "" FORCE)
set(SFML_BUILD_GRAPHICS ON CACHE BOOL "" FORCE)
set(SFML_BUILD_WINDOW ON CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest box2d sfml) 

# --- GLOB Files ---
# Automatically find all .cpp files in src and .h files in include
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

# --- Main Project ---
# Combined the main project executable with globbed files 
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
#add_executable(${PROJECT_NAME} "../src/main.cpp" )

# Ensure the compiler can find headers in the 'include' directory
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Link against SFML, Box2D, and system libs 
target_link_libraries(${PROJECT_NAME} 
    box2d 
    sfml-graphics 
    sfml-window 
    sfml-system 
    sfml-audio 
    sfml-network
)

# --- Unit Tests ---
#add_executable(RunUnitTests ${HEADERS})
# Remove a specific file from the list
# Get the absolute path to the file you want to exclude
set(MAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# Remove it specifically from the test source list
list(REMOVE_ITEM SOURCES "${MAIN_FILE}")

add_executable(RunUnitTests ${HEADERS} ${SOURCES} "test_src/RunTests.cpp")
target_include_directories(RunUnitTests PUBLIC include)
target_link_libraries(RunUnitTests 
    gtest 
    gtest_main 
    box2d 
    sfml-graphics 
    sfml-audio 
    sfml-network 
    sfml-system
) 

add_test(NAME RunUnitTests COMMAND RunUnitTests) 

# --- Housekeeping ---
set_property(GLOBAL PROPERTY USE_FOLDERS ON) 