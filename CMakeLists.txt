cmake_minimum_required(VERSION 3.16)
project(GreedyDelvers)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# Use vendored libs
set(SDLTTF_VENDORED ON)

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# This assumes all sources are available in vendored/[source]
add_subdirectory(vendored/SDL_ttf EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_image EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_mixer EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_net EXCLUDE_FROM_ALL)

# Create your game executable target as usual
add_executable(GreedyDelvers WIN32 src/main.c src/events.c src/inits.c)

# Link to the actual SDL3 library.
target_link_libraries(GreedyDelvers PRIVATE SDL3_net::SDL3_net SDL3_mixer::SDL3_mixer SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf SDL3::SDL3)