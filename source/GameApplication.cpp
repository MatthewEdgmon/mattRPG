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

#include "GameApplication.hpp"
#include "GameMap.hpp"
#include "GameMapTile.hpp"
#include "InputManager.hpp"
#include "OverworldPlayer.hpp"
#include "RenderDebug.hpp"
#include "ResourceLoader.hpp"
#include "Shader.hpp"
#include "SpriteRenderer.hpp"
#include "Texture2D.hpp"
#include "Tileset.hpp"

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

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
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

    if(sdl_game_controller == NULL) {
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
    ResourceLoader::LoadFont("./resource/Kenney Future Square.ttf", 32, "kenney_future_square");
    ResourceLoader::LoadFont("./resource/Alagard.ttf", 32, "alagard");
    ResourceLoader::LoadFont("./resource/Romulus.ttf", 32, "romulus");

    // Shaders
    ResourceLoader::LoadShader("./resource/Shaders/sprite.vert.glsl", "./resource/Shaders/sprite.frag.glsl", nullptr, "sprite");
    ResourceLoader::GetShader("sprite").SetInteger("image", 0, true);
    ResourceLoader::GetShader("sprite").SetMatrix4f("projection", projection_matrix);
    ResourceLoader::GetShader("sprite").Use();

    ResourceLoader::LoadShader("./resource/Shaders/debug.vert.glsl", "./resource/Shaders/debug.frag.glsl", nullptr, "debug");

    // UI Elements
    //ResourceLoader::LoadTexture("./resource/external/moderna-graphical-interface/toolbar.png", true, "ui_toolbar");

    // Sprites
    std::string player_idles[] = {
        "player_idle1",
        "player_idle2",
        "player_idle3",
        "player_idle4"
    };

    int idle_loop = 0;

    ResourceLoader::LoadTexture("./resource/CreaturePack/Rampart/Hunter/HunterIdle(Frame 1).png", true, true, player_idles[0]);
    ResourceLoader::LoadTexture("./resource/CreaturePack/Rampart/Hunter/HunterIdle(Frame 2).png", true, true, player_idles[1]);
    ResourceLoader::LoadTexture("./resource/CreaturePack/Rampart/Hunter/HunterIdle(Frame 3).png", true, true, player_idles[2]);
    ResourceLoader::LoadTexture("./resource/CreaturePack/Rampart/Hunter/HunterIdle(Frame 4).png", true, true, player_idles[3]);

    OverworldPlayer player(std::vector<std::string>(player_idles));

    GameMap game_map(16, 25, 25);

    //for(size_t x = 0; x < 25; x++) {
    //    for(size_t y = 0; y < 25; y++) {
    //        game_map.ChangeTile(x, y, GameMapTile("grass", x, y));
    //    }
    //}

    // Tiles
    ResourceLoader::LoadTextureAtlas("./resource/OverworldPack/GrassBiome/GB-LandTileset.png", true, true, "grass", glm::vec2(16, 16), glm::vec2(31, 31));

    std::vector<Tileset> tilesets;

    SpriteRenderer* sprite_renderer = new SpriteRenderer(ResourceLoader::GetShader("sprite"));

    std::ifstream input("./resource/test.ldtk");
    nlohmann::json input_json;
    input >> input_json;
    input.close();

    if(input_json.empty()) {
        std::cout << "Input file is empty." << std::endl;
    } else {
        
        if(input_json.contains("defs")) {

            nlohmann::json defs = input_json.find<std::string>("defs").value();
            nlohmann::json defs_tilesets = defs["tilesets"];

            tilesets.push_back(Tileset(defs_tilesets.at(0)["identifier"], defs_tilesets.at(0)["relPath"], defs_tilesets.at(0)["pxWid"], defs_tilesets.at(0)["pxHei"]));

        }

        if(input_json.contains("levels")) {

            nlohmann::json level0 = input_json.find<std::string>("levels").value().at(0);

            //std::cout << level0.find<std::string>("identifier").value() << std::endl;
            game_map.Resize((level0["pxWid"] / 16), (level0["pxHei"] / 16));


        }

    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while(is_running) {

        frame_start = SDL_GetPerformanceCounter();

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

        ResourceLoader::GetShader("debug").Use();

        RenderDebug::RenderDebugLine(glm::vec2(1.0f, 1.0f), glm::vec2(64.0f, 64.0f), glm::vec3(1.0f));

        ResourceLoader::GetShader("sprite").Use();

        game_map.Draw(sprite_renderer);
        
        sprite_renderer->DrawSprite(ResourceLoader::GetTexture(player_idles[idle_loop]), glm::vec2((window_width / 2), (window_height / 2)), glm::vec2(32, 32));

        ResourceLoader::GetFont("alagard").Draw(sprite_renderer, std::to_string(frame_rate).c_str(), 32, 32);
        
        ResourceLoader::GetFont("kenney_future_square").Draw(sprite_renderer, "TTF Font Test", 32, 256, 0xFF, 0x00, 0x00);
        ResourceLoader::GetFont("romulus").Draw(sprite_renderer, "TTF Font Test", 32, 64, 0x00, 0x00, 0xFF);

        idle_loop++;

        if(idle_loop == 4) {
            idle_loop = 0;
        }

        SDL_GL_SwapWindow(sdl_window);
    
        frame_end = SDL_GetPerformanceCounter();

        perf_freq = SDL_GetPerformanceFrequency();
        frame_time = (frame_end - frame_start) / static_cast<double>(perf_freq);
        frame_rate = 1 / (frame_time * 1000.0);

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