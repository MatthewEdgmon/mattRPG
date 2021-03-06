/**
 * This file is part of mattRPG.
 *
 * mattRPG is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mattRPG is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mattRPG.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <fstream>
#include <iostream>
#include <vector>

// GLAD2
#include <glad/gl.h>

// SDL2
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_net.h"
#include "SDL_ttf.h"

// GLM
#include <glm/glm.hpp>
#include <glm/ext.hpp>

// json
#include <nlohmann/json.hpp>

#include "ArrayRenderer.hpp"
#include "Font.hpp"
#include "GameApplication.hpp"
#include "GameMap.hpp"
#include "GameMapLayer.hpp"
#include "GameMapTile.hpp"
#include "GameWorld.hpp"
#include "InputManager.hpp"
#include "OverworldPlayer.hpp"
#include "ResourceLoader.hpp"
#include "Shader.hpp"
#include "SpriteRenderer.hpp"
#include "Texture2D.hpp"

void gl_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param) {

    auto const src_str = [source]() {
		switch(source) {
		    case GL_DEBUG_SOURCE_API: return "API";
		    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
		    case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
		    case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
		    case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
		    case GL_DEBUG_SOURCE_OTHER: return "OTHER";
            default: return "???";
        }
	}();

	auto const type_str = [type]() {
		switch(type) {
		    case GL_DEBUG_TYPE_ERROR: return "ERROR";
		    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
		    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
		    case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
		    case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
		    case GL_DEBUG_TYPE_MARKER: return "MARKER";
		    case GL_DEBUG_TYPE_OTHER: return "OTHER";
            default: return "???";
		}
	}();

	auto const severity_str = [severity]() {
		switch(severity) {
		    case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
		    case GL_DEBUG_SEVERITY_LOW: return "LOW";
		    case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
		    case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
            default: return "???";
		}
	}();

	std::cout << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << '\n';
}

int GameApplication::Main(int argc, char** argv) {
    stored_argc = argc;
    stored_argv = argv;

    Initialize();
    Loop();
    Shutdown();

    return 0;
}

void GameApplication::Initialize() {

    SDL_version sdl_compiled_version, sdl_linked_version;
    SDL_VERSION(&sdl_compiled_version);
    SDL_GetVersion(&sdl_linked_version);

    std::cout << "Compiled with SDL2 version:       " << static_cast<int>(sdl_compiled_version.major) << "." << static_cast<int>(sdl_compiled_version.minor) << "." << static_cast<int>(sdl_compiled_version.patch) << '\n';
    std::cout << "Linked with SDL2 version:         " << static_cast<int>(sdl_linked_version.major) << "." << static_cast<int>(sdl_linked_version.minor) << "." << static_cast<int>(sdl_linked_version.patch) << '\n';

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "Failed to initialize SDL. SDL_GetError(): " << SDL_GetError() << '\n';
        exit(-1);
    }

    SDL_version img_compiled_version;
    SDL_IMAGE_VERSION(&img_compiled_version);
    const SDL_version* img_linked_version = IMG_Linked_Version();

    std::cout << "Compiled with SDL2_image version: " << static_cast<int>(img_compiled_version.major) << "." << static_cast<int>(img_compiled_version.minor) << "." << static_cast<int>(img_compiled_version.patch) << '\n';
    std::cout << "Linked with SDL2_image version:   " << static_cast<int>(img_linked_version->major) << "." << static_cast<int>(img_linked_version->minor) << "." << static_cast<int>(img_linked_version->patch) << '\n';

    if(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP) == 0) {
        std::cout << "Failed to initialize SDL_image. IMG_GetError(): " << IMG_GetError() << '\n';
        exit(-1);
    }

    SDL_version mix_compiled_version;
    SDL_MIXER_VERSION(&mix_compiled_version);
    const SDL_version* mix_linked_version = Mix_Linked_Version();

    std::cout << "Compiled with SDL2_mixer version: " << static_cast<int>(mix_compiled_version.major) << "." << static_cast<int>(mix_compiled_version.minor) << "." << static_cast<int>(mix_compiled_version.patch) << '\n';
    std::cout << "Linked with SDL2_mixer version:   " << static_cast<int>(mix_linked_version->major) << "." << static_cast<int>(mix_linked_version->minor) << "." << static_cast<int>(mix_linked_version->patch) << '\n';

    if(Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID | MIX_INIT_OPUS) == 0) {
        std::cout << "Failed to initialize SDL_mixer. Mix_GetError(): " << Mix_GetError() << '\n';
        exit(-1);
    }

    SDL_version net_compiled_version;
    SDL_NET_VERSION(&net_compiled_version);
    const SDL_version* net_linked_version = SDLNet_Linked_Version();

    std::cout << "Compiled with SDL2_net version:   " << static_cast<int>(net_compiled_version.major) << "." << static_cast<int>(net_compiled_version.minor) << "." << static_cast<int>(net_compiled_version.patch) << '\n';
    std::cout << "Linked with SDL2_net version:     " << static_cast<int>(net_linked_version->major) << "." << static_cast<int>(net_linked_version->minor) << "." << static_cast<int>(net_linked_version->patch) << '\n';

    if(SDLNet_Init() != 0) {
        std::cout << "Failed to initialize SDL_net. SDLNet_GetError(): " << SDLNet_GetError() << '\n';
        exit(-1);
    }

    SDL_version ttf_compiled_version;
    SDL_TTF_VERSION(&ttf_compiled_version);
    const SDL_version* ttf_linked_version = TTF_Linked_Version();

    std::cout << "Compiled with SDL2_ttf version:   " << static_cast<int>(ttf_compiled_version.major) << "." << static_cast<int>(ttf_compiled_version.minor) << "." << static_cast<int>(ttf_compiled_version.patch) << '\n';
    std::cout << "Linked with SDL2_ttf version:     " << static_cast<int>(ttf_linked_version->major) << "." << static_cast<int>(ttf_linked_version->minor) << "." << static_cast<int>(ttf_linked_version->patch) << '\n';

    if(TTF_Init() != 0) {
        std::cout << "Failed to initialize SDL_ttf. TTF_GetError(): " << TTF_GetError() << '\n';
        exit(-1);
    }

    sdl_window = SDL_CreateWindow("mattRPG", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

    if(sdl_window == NULL) {
        std::cout << "Failed to create SDL window. SDL_GetError(): " << SDL_GetError() << '\n';
        exit(-1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    sdl_gl_context = SDL_GL_CreateContext(sdl_window);

    if(sdl_gl_context == NULL) {
        std::cout << "Failed to create SDL OpenGL context. SDL_GetError(): " << SDL_GetError() << '\n';
        exit(-1);
    }

    if(SDL_GL_SetSwapInterval(1) != 0) {
        std::cout << "Failed to set vertical sync. SDL_GetError(): " << SDL_GetError() << '\n';
        exit(-1);
    }

    int glad_version = gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress);

    std::cout << "Loaded OpenGL " << GLAD_VERSION_MAJOR(glad_version) << "." << GLAD_VERSION_MINOR(glad_version) << " using GLAD2.\n";

    // Disable VSync
    SDL_GL_SetSwapInterval(0);

    // Enable KHR_debug
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(gl_message_callback, nullptr); // gl_message_callback defined above
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE); // Disable NOTIFICATION level debug messages.

    // Enable alpha blending.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    SDL_AudioSpec want, have;

    SDL_zero(want);
    want.freq = 44100;
    want.format = AUDIO_S16SYS;
    want.channels = 1;
    want.samples = 2048;
    want.callback = NULL;

    sdl_audio_device_id = SDL_OpenAudioDevice(NULL, 0, &want, &have, SDL_AUDIO_ALLOW_ANY_CHANGE);

    if(sdl_audio_device_id == 0) {
        std::cout << "Failed to open any audio device. SDL_GetError(): " << SDL_GetError() << '\n';
        exit(-1);
    }

    for(int i = 0; i < SDL_NumJoysticks(); i++) {
        if(SDL_IsGameController(i)) {
sdl_game_controller = SDL_GameControllerOpen(i);
        }
    }

    if (sdl_game_controller == NULL) {
        std::cout << "Failed to assign game controller in SDL. SDL_GetError(): " << SDL_GetError() << '\n';
    }

    SDL_DisableScreenSaver();

    std::cout << "SDL setup complete.\n";

    input_manager = std::make_unique<InputManager>(this);

    std::cout << "GameApplication subsystem initialization complete.\n";

    is_running = true;
}

void GameApplication::Loop() {

    glm::mat4 projection_matrix = glm::ortho(0.0f, static_cast<float>(window_width), static_cast<float>(window_height), 0.0f, -1.0f, 1.0f);

    // Font
    ResourceLoader::LoadFont("./resource/Fonts/Kenney Future Square.ttf", 32, "kenney_future_square");
    ResourceLoader::LoadFont("./resource/Fonts/Alagard.ttf", 32, "alagard");
    ResourceLoader::LoadFont("./resource/Fonts/Romulus.ttf", 32, "romulus");

    // Shaders
    ResourceLoader::LoadShader("./resource/Shaders/sprite.vert.glsl", "./resource/Shaders/sprite.frag.glsl", nullptr, "sprite");
    ResourceLoader::GetShader("sprite").Use();
    ResourceLoader::GetShader("sprite").SetInteger("image", 0);
    ResourceLoader::GetShader("sprite").SetVector2f("TexCoordShift", 0.0f, 0.0f);
    ResourceLoader::GetShader("sprite").SetMatrix4f("projection", projection_matrix);

    ResourceLoader::LoadShader("./resource/Shaders/array.vert.glsl", "./resource/Shaders/array.frag.glsl", nullptr, "array");
    ResourceLoader::GetShader("array").Use();
    ResourceLoader::GetShader("array").SetInteger("texarray", 0);
    ResourceLoader::GetShader("array").SetIntegerUnsigned("diffuse_layer", 0);
    ResourceLoader::GetShader("array").SetVector2f("TexCoordShift", 0.0f, 0.0f);
    ResourceLoader::GetShader("array").SetMatrix4f("projection", projection_matrix);

    // UI Elements
    //ResourceLoader::LoadTexture("./resource/external/moderna-graphical-interface/toolbar.png", true, "ui_toolbar");

    // Sprites
    std::string player_idles[] = {
        "player_idle1",
        "player_idle2",
        "player_idle3",
        "player_idle4"
    };

    ResourceLoader::LoadTexture("./resource/CreaturePack/Rampart/Hunter/HunterIdle(Frame 1).png", true, true, player_idles[0]);
    ResourceLoader::LoadTexture("./resource/CreaturePack/Rampart/Hunter/HunterIdle(Frame 2).png", true, true, player_idles[1]);
    ResourceLoader::LoadTexture("./resource/CreaturePack/Rampart/Hunter/HunterIdle(Frame 3).png", true, true, player_idles[2]);
    ResourceLoader::LoadTexture("./resource/CreaturePack/Rampart/Hunter/HunterIdle(Frame 4).png", true, true, player_idles[3]);

    ResourceLoader::LoadGameWorld("./resource/test.ldtk", "world");

    // Renderers
    ArrayRenderer* array_renderer = new ArrayRenderer(ResourceLoader::GetShader("array"));
    SpriteRenderer* sprite_renderer = new SpriteRenderer(ResourceLoader::GetShader("sprite"));

    std::ifstream input_file("./resource/test.ldtk");

    nlohmann::json input_json;

    input_file >> input_json;
    input_file.close();

    if(input_json.contains("defs")) {

        for(auto tileset : input_json.find<std::string>("defs").value().find<std::string>("tilesets").value()) {
            std::string file_name = tileset.find("relPath").value();
            ResourceLoader::LoadTextureArray(std::string("./resource/" + file_name).c_str(), true, true, tileset["identifier"], 16, 16);
        }
    }

    int idle_loop = 0;

    while(is_running) {

        frame_start = SDL_GetPerformanceCounter();
        ticks_start = SDL_GetTicks();

        while(SDL_PollEvent(&sdl_event)) {
        
            switch(sdl_event.type) {
                /* Handled by InputManager. */
                case SDL_CONTROLLERAXISMOTION:
                case SDL_CONTROLLERBUTTONDOWN:
                case SDL_CONTROLLERBUTTONUP:
                case SDL_FINGERMOTION:
                case SDL_FINGERDOWN:
                case SDL_FINGERUP:
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                case SDL_JOYAXISMOTION:
                case SDL_JOYBALLMOTION:
                case SDL_JOYHATMOTION:
                case SDL_JOYBUTTONDOWN:
                case SDL_JOYBUTTONUP:
                case SDL_MOUSEMOTION:
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                case SDL_MOUSEWHEEL:
                case SDL_MULTIGESTURE:
                    input_manager->HandleInput(&sdl_event);
                    break;

                /* All other events handled here. */
                case SDL_JOYDEVICEADDED:
                case SDL_JOYDEVICEREMOVED:
                case SDL_AUDIODEVICEADDED:
                case SDL_AUDIODEVICEREMOVED:
                case SDL_CONTROLLERDEVICEADDED:
                case SDL_CONTROLLERDEVICEREMOVED:
                case SDL_CONTROLLERDEVICEREMAPPED:
                case SDL_DOLLARGESTURE:
                case SDL_DOLLARRECORD:
                case SDL_DROPFILE:
                case SDL_DROPTEXT:
                case SDL_DROPBEGIN:
                case SDL_DROPCOMPLETE:
                    break;
                case SDL_QUIT:
                    RequestExit();
                    break;
                case SDL_SYSWMEVENT:
                case SDL_TEXTEDITING:
                case SDL_TEXTINPUT:
                case SDL_USEREVENT:
                case SDL_WINDOWEVENT:
                default:
                    break;
            }            
        }

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        // Tileset test
        //for(auto i = 0; i < ResourceLoader::GetTexture("GrassBiome").GetSubImageCount(); i++) {
        //    array_renderer->DrawArray(ResourceLoader::GetTexture("GrassBiome"), i, glm::vec2(((i % 16) * 16), ((i / 16) * 16)));
        //}

        // Gameworld test
        //for(auto map : ResourceLoader::GetGameWorld("world").GetMaps()) {
        auto tile_size = ResourceLoader::GetGameWorld("world").GetTileSize();
        auto map = ResourceLoader::GetGameWorld("world").GetMaps().at(0);
            // Maybe in C++23 we'll get a reverse range based for loop.
            for(size_t i = (map.GetLayers().size() - 1); i > 0; i--) {
                auto layer = map.GetLayers().at(i);
                // Only render if it's Tiles. Entities handled elsewhere.
                if(layer.GetLayerType() == GameMapLayerType::Tiles) {
                    for(size_t y = 0; y < layer.GetTiles().size(); y++) {
                        for(size_t x = 0; x < layer.GetTiles()[y].size(); x++) {
                            if(layer.GetTiles()[y][x].GetTileSetIndex() != 0) {
                                array_renderer->DrawArray(ResourceLoader::GetTexture(layer.GetTileSetName()), layer.GetTiles()[y][x].GetTileSetIndex(), glm::vec2(x * tile_size, y * tile_size));
                            }
                        }
                    }
                }
            }
        //}

        sprite_renderer->DrawSprite(ResourceLoader::GetTexture(player_idles[idle_loop]), glm::vec2((window_width / 2), (window_height / 2)), glm::vec2(16, 16));

        idle_loop++;

        if(idle_loop == 4) {
            idle_loop = 0;
        }

        ResourceLoader::GetFont("alagard").Draw(sprite_renderer, std::to_string(frame_rate).c_str(), 32, 32);
        ResourceLoader::GetFont("alagard").Draw(sprite_renderer, "The quick brown fox jumps over the lazy dog.", 32, 128);
        ResourceLoader::GetFont("kenney_future_square").Draw(sprite_renderer, "The quick brown fox jumps over the lazy dog.", 32, 160);
        ResourceLoader::GetFont("romulus").Draw(sprite_renderer, "The quick brown fox jumps over the lazy dog.", 32, 192);

        SDL_GL_SwapWindow(sdl_window);
    
        frame_end = SDL_GetPerformanceCounter();
        ticks_end = SDL_GetTicks();

        // FPS Calculation using SDL timer.
        frame_time = (ticks_end - ticks_start);
        frame_rate = 1000.0f / frame_time;

        // FPS Calculation using high precision timer (doesn't work haha).
        perf_freq = SDL_GetPerformanceFrequency();
        //frame_time = (frame_end - frame_start) / static_cast<double>(perf_freq);
        //frame_rate = 1 / (frame_time * 1000.0);

        frame_count++;

        SDL_Delay(1);
    }
}

void GameApplication::Shutdown() {

    SDL_EnableScreenSaver();

    SDL_GameControllerClose(sdl_game_controller);

    SDL_CloseAudioDevice(sdl_audio_device_id);

    SDL_GL_DeleteContext(sdl_gl_context);

    SDL_DestroyWindow(sdl_window);

    TTF_Quit();

    SDLNet_Quit();

    Mix_Quit();

    IMG_Quit();

    SDL_Quit();
}