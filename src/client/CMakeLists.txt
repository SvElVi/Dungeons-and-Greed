cmake_minimum_required(VERSION 3.16)
project(client)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# Use vendored libs
set(SDLTTF_VENDORED ON)

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(../../vendored/SDL ${CMAKE_CURRENT_BINARY_DIR}/build_sdl EXCLUDE_FROM_ALL)

# This assumes all sources are available in vendored/[source]
add_subdirectory(../../vendored/SDL_ttf ${CMAKE_CURRENT_BINARY_DIR}/build_sdl_ttf EXCLUDE_FROM_ALL)
add_subdirectory(../../vendored/SDL_image ${CMAKE_CURRENT_BINARY_DIR}/build_sdl_image EXCLUDE_FROM_ALL)
add_subdirectory(../../vendored/SDL_mixer ${CMAKE_CURRENT_BINARY_DIR}/build_sdl_mixer EXCLUDE_FROM_ALL)
add_subdirectory(../../vendored/SDL_net ${CMAKE_CURRENT_BINARY_DIR}/build_sdl_net EXCLUDE_FROM_ALL)

# Create your game executable target as usual
add_executable(client WIN32 ../client/main.c ../lib/player.c ../lib/events.c ../lib/render.c ../lib/inits.c ../lib/world/world.c)

# Links the lib folder to the project
target_include_directories(client PRIVATE ../lib)

# Link to the actual SDL3 library.
target_link_libraries(client PRIVATE SDL3_net::SDL3_net SDL3_mixer::SDL3_mixer SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf SDL3::SDL3)