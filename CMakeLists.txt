# This file is part of mattRPG.
#
# mattRPG is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# mattRPG is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with mattRPG.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.18)

project(mattRPG)

# Include custom modules.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/sdl2-cmake-modules)

# Find SDL2 and OpenGL
set(SDL2_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL2-2.0.14" CACHE BOOL "" FORCE)
set(SDL2_IMAGE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL2_image-2.0.5" CACHE BOOL "" FORCE)
set(SDL2_MIXER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL2_mixer-2.0.4" CACHE BOOL "" FORCE)
set(SDL2_NET_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL2_net-2.0.1" CACHE BOOL "" FORCE)
set(SDL2_TTF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL2_ttf-2.0.15" CACHE BOOL "" FORCE)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_net REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(OpenGL REQUIRED)

# Setup GLAD2
set(GLAD2_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/external/glad2/include")

# Setup GLM
add_subdirectory("${CMAKE_SOURCE_DIR}/external/glm")

# Setup nlohmann/json using FetchContent
include(FetchContent)

FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent.git
  GIT_TAG v3.7.3)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

if     ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # Setup compile options.
    add_compile_options(-Ofast -Wall -Werror -pedantic -Wno-error=maybe-uninitialized -Wno-error=unused-but-set-variable)
    set(CMAKE_C_STANDARD 99)
    set(CMAKE_CXX_STANDARD 14)

    # Setup address sanitizer.
    set(CMAKE_BUILD_TYPE "Debug")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

    # Setup undefined behaviour sanitizer.
    set(CMAKE_BUILD_TYPE "Debug")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=undefined")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=undefined")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=undefined")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
endif()

set(SOURCE_FILES source/ArrayRenderer.cpp
                 source/ArrayRenderer.hpp
                 source/Font.cpp
                 source/Font.hpp
                 source/GameApplication.cpp
                 source/GameApplication.hpp
                 source/GameMap.cpp
                 source/GameMap.hpp
                 source/GameMapTile.cpp
                 source/GameMapTile.hpp
                 source/GameState.hpp
                 source/GameStateManager.cpp
                 source/GameStateManager.hpp
                 source/InputManager.cpp
                 source/InputManager.hpp
                 source/Main.cpp
                 source/MusicTrack.cpp
                 source/MusicTrack.hpp
                 source/OverworldPlayer.cpp
                 source/OverworldPlayer.cpp
                 source/ResourceLoader.cpp
                 source/ResourceLoader.hpp
                 source/Shader.cpp
                 source/Shader.hpp
                 source/SpriteRenderer.cpp
                 source/SpriteRenderer.hpp
                 source/SoundEffect.cpp
                 source/SoundEffect.hpp
                 source/Texture2D.cpp
                 source/Texture2D.hpp
                 # GLAD2
                 external/glad2/src/gl.c)

# Define executable
include_directories(${CMAKE_SOURCE_DIR} ${OPENGL_INCLUDE_DIR} ${GLAD2_INCLUDE_DIR})
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glm::glm SDL2::Main SDL2::Image SDL2::Mixer SDL2::Net SDL2::TTF nlohmann_json::nlohmann_json)

# Set up Visual Studio filters.
function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

# TODO: CMake apparently now has better ways to do this
assign_source_group(${SOURCE_FILES})

# Set Visual Studio working directory to the base source directory.
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

# Set Visual Studio startup project.
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Copy SDL2 DLLs to output folder on Windows.
if(WIN32)
    message(STATUS "Copying SDL2 DLLs to build output directory.")
    file(GLOB SDL2_DLLS ${SDL2_INCLUDE_DIR}/../lib/${CMAKE_GENERATOR_PLATFORM}/*.dll)
    foreach(DLL ${SDL2_DLLS})
        message(STATUS "${DLL}")
        message(STATUS "${CMAKE_GENERATOR_PLATFORM}")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    endforeach()
endif()