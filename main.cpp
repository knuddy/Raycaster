#include <float.h>

#include "core/include.h"
#include "core/structures.h"
#include "core/timer.h"


#define MAP_WIDTH 24
#define MAP_HEIGHT 24
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define MINIMAP_SIZE 480
#define MINIMAP_MASK_SIZE 360
#define MAX_VIEW_DIST 20

constexpr int GAME_WIDTH = 320;
constexpr int GAME_HEIGHT = GAME_WIDTH * (SCREEN_WIDTH / SCREEN_HEIGHT);

constexpr int SCREEN_FPS = 60;
constexpr int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
constexpr int TILE_WIDTH = 64;
constexpr int TILE_HEIGHT = 64;
constexpr int MINIMAP_TILES_WIDE = MINIMAP_SIZE / TILE_WIDTH;
constexpr int MINIMAP_TILES_HIGH = MINIMAP_SIZE / TILE_HEIGHT;


std::vector<std::vector<int>> worldMap =
{
    {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 4, 6, 4, 4, 6, 4, 6, 4, 4, 4, 6, 4},
    {8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {8, 0, 3, 3, 0, 0, 0, 0, 0, 8, 8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
    {8, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
    {8, 0, 3, 3, 0, 0, 0, 0, 0, 8, 8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 4, 0, 0, 0, 0, 0, 6, 6, 6, 0, 6, 4, 6},
    {8, 8, 8, 8, 0, 8, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4, 4, 6, 0, 0, 0, 0, 0, 6},
    {7, 7, 7, 7, 0, 7, 7, 7, 7, 0, 8, 0, 8, 0, 8, 0, 8, 4, 0, 4, 0, 6, 0, 6},
    {7, 7, 0, 0, 0, 0, 0, 0, 7, 8, 0, 8, 0, 8, 0, 8, 8, 6, 0, 0, 0, 0, 0, 6},
    {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 6, 0, 0, 0, 0, 0, 4},
    {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 6, 0, 6, 0, 6, 0, 6},
    {7, 7, 0, 0, 0, 0, 0, 0, 7, 8, 0, 8, 0, 8, 0, 8, 8, 6, 4, 6, 0, 6, 6, 6},
    {7, 7, 7, 7, 0, 7, 7, 7, 7, 8, 8, 4, 0, 6, 8, 4, 8, 3, 3, 3, 0, 3, 3, 3},
    {2, 2, 2, 2, 0, 2, 2, 2, 2, 4, 6, 4, 0, 0, 6, 0, 6, 3, 0, 0, 0, 0, 0, 3},
    {2, 2, 0, 0, 0, 0, 0, 2, 2, 4, 0, 0, 0, 0, 0, 0, 4, 3, 0, 0, 0, 0, 0, 3},
    {2, 0, 0, 0, 0, 0, 0, 0, 2, 4, 0, 0, 0, 0, 0, 0, 4, 3, 0, 0, 0, 0, 0, 3},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 4, 4, 4, 4, 4, 6, 0, 6, 3, 3, 0, 0, 0, 3, 3},
    {2, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 1, 2, 2, 2, 6, 6, 0, 0, 5, 0, 5, 0, 5},
    {2, 2, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 2, 2, 0, 5, 0, 5, 0, 0, 0, 5, 5},
    {2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 5, 0, 5, 0, 5, 0, 5, 0, 5},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 5, 0, 5, 0, 0, 0, 0, 0, 5},
    {2, 2, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 2, 2, 0, 5, 0, 5, 0, 0, 0, 5, 5},
    {2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 5, 5, 5, 5, 5, 5, 5, 5, 5}
};

int ceilingMap[MAP_WIDTH][MAP_HEIGHT] =
{
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0},
    {0, 6, 0, 0, 6, 6, 6, 6, 6, 0, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0},
    {0, 6, 6, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0},
    {0, 6, 0, 0, 6, 6, 6, 6, 6, 0, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0},
    {0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 6, 6, 6, 6, 6, 0, 0, 0, 6, 0, 0, 0},
    {0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 0},
    {0, 0, 0, 0, 6, 0, 0, 0, 0, 6, 0, 6, 0, 6, 0, 6, 0, 0, 6, 0, 6, 0, 6, 0},
    {0, 0, 6, 6, 6, 6, 6, 6, 0, 0, 6, 0, 6, 0, 6, 0, 0, 0, 6, 6, 6, 6, 6, 0},
    {0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 6, 6, 6, 6, 6, 0},
    {0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 6, 0, 6, 0, 6, 0},
    {0, 0, 6, 6, 6, 6, 6, 6, 0, 0, 6, 0, 6, 0, 6, 0, 0, 0, 0, 0, 6, 0, 0, 0},
    {0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0},
    {0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 0},
    {0, 0, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 0},
    {0, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 0},
    {0, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 0, 0},
    {0, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 6, 0, 6, 0},
    {0, 0, 6, 6, 6, 6, 6, 0, 0, 0, 6, 6, 6, 0, 0, 6, 0, 6, 0, 6, 6, 6, 0, 0},
    {0, 6, 6, 6, 6, 6, 6, 6, 0, 6, 6, 6, 6, 6, 0, 0, 6, 0, 6, 0, 6, 0, 0, 0},
    {0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 6, 6, 6, 6, 6, 6, 6, 0, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 6, 6, 6, 6, 6, 0, 0, 0, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

double lightMap[MAP_WIDTH][MAP_HEIGHT] =
{
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1.25, 1.25, 1.25, 1.25, 1.25, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1.25, 1.35, 1.35, 1.35, 1.25, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1.25, 1.35, 1.8, 1.35, 1.25, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1.25, 1.35, 1.35, 1.35, 1.25, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1.25, 1.25, 1.25, 1.25, 1.25, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

double ceilingLightMap[MAP_WIDTH][MAP_HEIGHT] =
{
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1.05, 1.05, 1.05, 1.05, 1.05, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1.05, 1.15, 1.15, 1.15, 1.05, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1.05, 1.15, 1.25, 1.15, 1.05, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1.05, 1.15, 1.15, 1.15, 1.05, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1.05, 1.05, 1.05, 1.05, 1.05, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};


Vector position = {22.0, 11.5};
Vector direction = {-1.0, 0};
Vector plane = {0, 0.66};

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* gameTexture;
SDL_Texture* mapTexture;
SDL_Texture* minimapTargetTexture;
SDL_Texture* minimapMask;


Texture textures[12];

#define NUM_SPRITES 19

Sprite sprite[NUM_SPRITES] =
{
    {20.5, 11.5, 10}, //green light in front of playerstart
    //green lights in every room
    {18.5, 4.5, 10},
    {10.0, 4.5, 10},
    {10.0, 12.5, 10},
    {3.5, 6.5, 10},
    {3.5, 20.5, 10},
    {3.5, 14.5, 10},
    {14.5, 20.5, 10},

    //row of pillars in front of wall: fisheye test
    {18.5, 10.5, 9},
    {18.5, 11.5, 9},
    {18.5, 12.5, 9},

    //some barrels around the map
    {21.5, 1.5, 8},
    {15.5, 1.5, 8},
    {16.0, 1.8, 8},
    {16.2, 1.2, 8},
    {3.5, 2.5, 8},
    {9.5, 15.5, 8},
    {10.0, 15.1, 8},
    {10.5, 15.8, 8},
};

double ZBuffer[GAME_WIDTH];

int spriteOrder[NUM_SPRITES];
double spriteDistance[NUM_SPRITES];

bool quit = false;

SDL_Texture* CreateMinimapMask()
{
    Uint32 windowFormat = SDL_GetWindowPixelFormat(window);
    SDL_Texture* mask = SDL_CreateTexture(renderer, windowFormat, SDL_TEXTUREACCESS_STREAMING, MINIMAP_SIZE, MINIMAP_SIZE);
    Uint32* pixels;
    int pitch;
    SDL_LockTexture(mask, nullptr, reinterpret_cast<void **>(&pixels), &pitch);
    constexpr IVector center = {MINIMAP_SIZE / 2, MINIMAP_SIZE / 2};
    constexpr int radius = MINIMAP_MASK_SIZE / 2;
    constexpr int radiusSqr = radius * radius;

    SDL_PixelFormat* mappingFormat = SDL_AllocFormat(windowFormat);

    auto filled = SDL_MapRGBA(mappingFormat, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);;
    auto transparent = SDL_MapRGBA(mappingFormat, 0x00, 0x00, 0x00, SDL_ALPHA_TRANSPARENT);

    for (int x = 0; x < MINIMAP_SIZE; x++)
    {
        for (int y = 0; y < MINIMAP_SIZE; y++)
        {
            const IVector dist = {x - center.x, y - center.y};
            if (dist.x * dist.x + dist.y * dist.y < radiusSqr)
            {
                pixels[y * MINIMAP_SIZE + x] = filled;
            }
            else
            {
                pixels[y * MINIMAP_SIZE + x] = transparent;
            }
        }
    }
    SDL_UnlockTexture(mask);
    SDL_SetTextureBlendMode(mask, SDL_BLENDMODE_NONE);
    return mask;
}

void Init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        EXIT_LOG_SDL_ERROR("SDL could not initialize!");
    }

    window = SDL_CreateWindow("Raycaster", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * 2, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == nullptr)
    {
        EXIT_LOG_SDL_ERROR("Window could not be created!");
    }

    constexpr int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        EXIT_LOG_IMG_ERROR("SDL_image could not initialize!");
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        EXIT_LOG_SDL_ERROR("Could not create renderer!");
    }

    gameTexture = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_STREAMING, GAME_WIDTH, GAME_HEIGHT);
    mapTexture = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_TARGET, MINIMAP_SIZE, MINIMAP_SIZE);
    minimapMask = CreateMinimapMask();
    minimapTargetTexture = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_TARGET, MINIMAP_SIZE, MINIMAP_SIZE);
}

void Load()
{
    Texture_FromFile(&textures[0], window, renderer, "textures/eagle.png");
    Texture_FromFile(&textures[1], window, renderer, "textures/redbrick.png");
    Texture_FromFile(&textures[2], window, renderer, "textures/purplestone.png");
    Texture_FromFile(&textures[3], window, renderer, "textures/greystone.png");
    Texture_FromFile(&textures[4], window, renderer, "textures/bluestone.png");
    Texture_FromFile(&textures[5], window, renderer, "textures/mossy.png");
    Texture_FromFile(&textures[6], window, renderer, "textures/wood.png");
    Texture_FromFile(&textures[7], window, renderer, "textures/colorstone.png");
    Texture_FromFile(&textures[8], window, renderer, "textures/barrel.png");
    Texture_FromFile(&textures[9], window, renderer, "textures/pillar.png");
    Texture_FromFile(&textures[10], window, renderer, "textures/greenlight.png");
    Texture_FromFile(&textures[11], window, renderer, "textures/sky.png");
}

void Close()
{
    for (int i = 0; i < 8; i++)
    {
        Texture_Free(&textures[i]);
    }

    SDL_DestroyWindow(window);
    window = nullptr;

    IMG_Quit();
    SDL_Quit();
}

void Update(const double deltaTime)
{
    const double moveSpeed = deltaTime * 5.0;
    const double rotationSpeed = deltaTime * 4.0;

    SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            quit = true;
        }
    }

    const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);

    if (currentKeyStates[SDL_SCANCODE_ESCAPE])
    {
        quit = true;
    }

    if (currentKeyStates[SDL_SCANCODE_W])
    {
        const auto deltaX = position.x + direction.x * moveSpeed;
        const auto deltaY = position.y + direction.y * moveSpeed;

        if (worldMap[static_cast<int>(deltaX)][static_cast<int>(position.y)] == false)
        {
            position.x = deltaX;
        }
        if (worldMap[static_cast<int>(position.x)][static_cast<int>(deltaY)] == false)
        {
            position.y = deltaY;
        }
    }

    if (currentKeyStates[SDL_SCANCODE_S])
    {
        const auto deltaX = position.x - direction.x * moveSpeed;
        const auto deltaY = position.y - direction.y * moveSpeed;

        if (worldMap[static_cast<int>(deltaX)][static_cast<int>(position.y)] == false)
        {
            position.x = deltaX;
        }
        if (worldMap[static_cast<int>(position.x)][static_cast<int>(deltaY)] == false)
        {
            position.y = deltaY;
        }
    }

    if (currentKeyStates[SDL_SCANCODE_A])
    {
        const auto deltaX = position.x - direction.y * moveSpeed;
        const auto deltaY = position.y - -direction.x * moveSpeed;

        if (worldMap[static_cast<int>(deltaX)][static_cast<int>(position.y)] == false)
        {
            position.x = deltaX;
        }
        if (worldMap[static_cast<int>(position.x)][static_cast<int>(deltaY)] == false)
        {
            position.y = deltaY;
        }
    }

    if (currentKeyStates[SDL_SCANCODE_D])
    {
        const auto deltaX = position.x + direction.y * moveSpeed;
        const auto deltaY = position.y + -direction.x * moveSpeed;

        if (worldMap[static_cast<int>(deltaX)][static_cast<int>(position.y)] == false)
        {
            position.x = deltaX;
        }
        if (worldMap[static_cast<int>(position.x)][static_cast<int>(deltaY)] == false)
        {
            position.y = deltaY;
        }
    }

    if (currentKeyStates[SDL_SCANCODE_LEFT])
    {
        const double oldDirectionX = direction.x;
        const double cosRot = std::cos(rotationSpeed);
        const double sinRot = std::sin(rotationSpeed);

        direction.x = direction.x * cosRot - direction.y * sinRot;
        direction.y = oldDirectionX * sinRot + direction.y * cosRot;

        const double oldPlaneX = plane.x;
        plane.x = plane.x * cosRot - plane.y * sinRot;
        plane.y = oldPlaneX * sinRot + plane.y * cosRot;
    }

    if (currentKeyStates[SDL_SCANCODE_RIGHT])
    {
        const double oldDirectionX = direction.x;
        const double cosRot = std::cos(-rotationSpeed);
        const double sinRot = std::sin(-rotationSpeed);

        direction.x = direction.x * cosRot - direction.y * sinRot;
        direction.y = oldDirectionX * sinRot + direction.y * cosRot;

        const double oldPlaneX = plane.x;
        plane.x = plane.x * cosRot - plane.y * sinRot;
        plane.y = oldPlaneX * sinRot + plane.y * cosRot;
    }
}

inline int PosMod(const int i, const int n)
{
    return (i % n + n) % n;
}

void DrawMap()
{
    SDL_SetRenderTarget(renderer, mapTexture);
    SDL_SetRenderDrawColor(renderer, 0x32, 0x35, 0x33, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    IVector viewportWorldPosition = {
        static_cast<int>(position.x * TILE_WIDTH) - MINIMAP_SIZE / 2,
        static_cast<int>(position.y * TILE_HEIGHT) - MINIMAP_SIZE / 2
    };
    IVector viewportTilePosition = {viewportWorldPosition.x / TILE_WIDTH, viewportWorldPosition.y / TILE_HEIGHT};
    IVector viewportTileOffset = {viewportWorldPosition.x % TILE_WIDTH, viewportWorldPosition.y % TILE_HEIGHT};

    for (int x = viewportTilePosition.x; x <= viewportTilePosition.x + MINIMAP_TILES_WIDE + 1; x++)
    {
        for (int y = viewportTilePosition.y; y <= viewportTilePosition.y + MINIMAP_TILES_HIGH + 1; y++)
        {
            if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) continue;

            const auto texIndex = worldMap[x][y];
            if (texIndex > 0)
            {
                const Texture tex = textures[texIndex - 1];

                auto screenX = (x - viewportTilePosition.x) * TILE_WIDTH - viewportTileOffset.x;
                auto screenY = (y - viewportTilePosition.y) * TILE_HEIGHT - viewportTileOffset.y;

                SDL_Rect dstRect = {screenY, screenX, TILE_WIDTH, TILE_HEIGHT};
                SDL_RenderCopy(renderer, tex.tex, nullptr, &dstRect);
            }
        }
    }

    for (const auto sprite: sprite)
    {
        IVector worldPosition = {static_cast<int>(sprite.position.x * TILE_WIDTH) - TILE_WIDTH / 2, static_cast<int>(sprite.position.y * TILE_HEIGHT) - TILE_HEIGHT / 2};

        if (worldPosition.x + TILE_WIDTH < viewportWorldPosition.x || worldPosition.x > viewportWorldPosition.x + MINIMAP_SIZE  ||
            worldPosition.y + TILE_WIDTH < viewportWorldPosition.y || worldPosition.y > viewportWorldPosition.y + MINIMAP_SIZE) continue;

        IVector screenCoordinates = {worldPosition.x - viewportWorldPosition.x, worldPosition.y - viewportWorldPosition.y};

        Texture tex = textures[sprite.tex];
        SDL_Rect dstRect = {
            screenCoordinates.y,
            screenCoordinates.x,
            TILE_WIDTH,
            TILE_HEIGHT
        };
        SDL_RenderCopy(renderer, tex.tex, nullptr, &dstRect);
    }

    constexpr float playerSize = 24.0f;
    constexpr SDL_FRect playerRect = {
        MINIMAP_SIZE / 2 - playerSize / 2,
        MINIMAP_SIZE / 2 - playerSize / 2,
        playerSize,
        playerSize
    };
    SDL_RenderCopyExF( renderer, textures[0].tex, nullptr, &playerRect, std::atan2(direction.y, direction.x * -1) * (180 / std::numbers::pi), nullptr, SDL_FLIP_NONE);

    SDL_SetRenderTarget(renderer, minimapTargetTexture);
    SDL_RenderCopy(renderer, minimapMask, nullptr, nullptr);

    SDL_SetTextureBlendMode(mapTexture, SDL_BLENDMODE_MOD);
    // SDL_RenderCopyEx( renderer, mapTexture, nullptr, nullptr, std::atan2(direction.y * -1, direction.x * -1) * (180 / std::numbers::pi), nullptr, SDL_FLIP_NONE);
    SDL_RenderCopy( renderer, mapTexture, nullptr, nullptr);
    SDL_SetTextureBlendMode(mapTexture, SDL_BLENDMODE_BLEND);

    SDL_SetRenderTarget(renderer, nullptr);
    constexpr SDL_Rect destRect = {SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, minimapTargetTexture, nullptr, &destRect);
}


//sort algorithm
//sort the sprites based on distance
void sortSprites(int* order, double* dist, int amount)
{
    std::vector<std::pair<double, int>> sprites(amount);
    for (int i = 0; i < amount; i++)
    {
        sprites[i].first = dist[i];
        sprites[i].second = order[i];
    }
    std::ranges::sort(sprites);
    // restore in reverse order to go from farthest to nearest
    for (int i = 0; i < amount; i++)
    {
        dist[i] = sprites[amount - i - 1].first;
        order[i] = sprites[amount - i - 1].second;
    }
}

void Darken(Pixel* p, const double shadingPerc)
{
    p->r = p->r * (1.0 - shadingPerc);
    p->g = p->g * (1.0 - shadingPerc);
    p->b = p->b * (1.0 - shadingPerc);
}

void Lighten(Pixel* p, const double lightness)
{
    p->r = std::min(p->r * lightness, 255.0);
    p->g = std::min(p->g * lightness, 255.0);
    p->b = std::min(p->b * lightness, 255.0);
}

void DrawGame()
{
    void* pixels;
    int pitch;
    SDL_LockTexture(gameTexture, nullptr, &pixels, &pitch);
    auto buffer = static_cast<uint32_t *>(pixels);

    for (int x = 0; x < GAME_WIDTH; x++)
    {
        for (int y = 0; y < GAME_HEIGHT; y++)
        {
            buffer[y * GAME_WIDTH + x] = 0;
        }
    }

    const Texture skyTexure = textures[11];

    for (int x = 0; x < GAME_WIDTH; x++)
    {
        const double cameraX = 2 * x / static_cast<double>(GAME_WIDTH) - 1;
        const double playerAngle = std::atan2(direction.y, direction.x);
        const double textureColumn = skyTexure.width * ((std::atan2(1, cameraX) + playerAngle) / std::numbers::pi);
        const int texX = static_cast<int>(textureColumn) & skyTexure.width - 1;

        int rowStep = skyTexure.height / GAME_HEIGHT;
        for (int y = 0; y < GAME_HEIGHT / 2; y++)
        {
            const int texY = y * rowStep;
            buffer[y * GAME_WIDTH + x] = skyTexure.pixels[texY * skyTexure.width + texX].rgba;
        }
    }

    const Texture floorTexture = textures[3];

    for (int y = GAME_HEIGHT / 2; y < GAME_HEIGHT; y++)
    {
        Vector leftMostRay = {direction.x - plane.x, direction.y - plane.y};
        Vector rightMostRay = {direction.x + plane.x, direction.y + plane.y};

        double positionZ = 0.5 * GAME_HEIGHT;
        int p = y - GAME_HEIGHT / 2;
        double rowDistance = positionZ / p;

        Vector floorStep = {
            rowDistance * (rightMostRay.x - leftMostRay.x) / GAME_WIDTH,
            rowDistance * (rightMostRay.y - leftMostRay.y) / GAME_WIDTH
        };
        Vector floor = {
            position.x + rowDistance * leftMostRay.x,
            position.y + rowDistance * leftMostRay.y
        };

        double shadingPerc = 1 - p / positionZ - 0.25;
        shadingPerc = std::min(shadingPerc, 0.75);
        shadingPerc = std::max(shadingPerc, 0.0);

        for (int x = 0; x < GAME_WIDTH; x++)
        {
            IVector cell = {PosMod(static_cast<int>(floor.x), MAP_WIDTH), PosMod(static_cast<int>(floor.y), MAP_HEIGHT)};

            IVector floorTexCoord = {
                static_cast<int>(floorTexture.width * (floor.x - cell.x)) & (floorTexture.width - 1),
                static_cast<int>(floorTexture.height * (floor.y - cell.y)) & (floorTexture.height - 1)
            };

            Pixel floorPixel = floorTexture.pixels[floorTexCoord.y * floorTexture.width + floorTexCoord.x];
            Darken(&floorPixel, shadingPerc);
            Lighten(&floorPixel, lightMap[cell.x][cell.y]);
            buffer[y * GAME_WIDTH + x] = floorPixel.rgba;


            auto ceilingTexIndex = ceilingMap[cell.x][cell.y];
            if (ceilingTexIndex > 0)
            {
                Texture ceilingTexture = textures[ceilingTexIndex];

                IVector ceilTexCoord = {
                    static_cast<int>(ceilingTexture.width * (floor.x - cell.x)) & (ceilingTexture.width - 1),
                    static_cast<int>(ceilingTexture.height * (floor.y - cell.y)) & (ceilingTexture.height - 1)
                };

                Pixel ceilPixel = ceilingTexture.pixels[ceilTexCoord.y * ceilingTexture.width + ceilTexCoord.x];
                Darken(&ceilPixel, shadingPerc);
                Lighten(&ceilPixel, ceilingLightMap[cell.x][cell.y]);
                buffer[(GAME_HEIGHT - y - 1) * GAME_WIDTH + x] = ceilPixel.rgba;
            }


            floor.x += floorStep.x;
            floor.y += floorStep.y;
        }
    }

    for (int x = 0; x < GAME_WIDTH; x++)
    {
        const double cameraX = 2 * x / static_cast<double>(GAME_WIDTH) - 1;
        const Vector rayDirection = {direction.x + plane.x * cameraX, direction.y + plane.y * cameraX};
        IVector mapPosition = {static_cast<int>(position.x), static_cast<int>(position.y)};
        Vector sideDist;
        Vector deltaDist = {rayDirection.x == 0 ? 1e30 : std::abs(1 / rayDirection.x), rayDirection.y == 0 ? 1e30 : std::abs(1 / rayDirection.y)};
        double perpWallDist;
        IVector step;
        bool hit = false;
        int side = 0;

        if (rayDirection.x < 0)
        {
            step.x = -1;
            sideDist.x = (position.x - mapPosition.x) * deltaDist.x;
        }
        else
        {
            step.x = 1;
            sideDist.x = (mapPosition.x + 1.0 - position.x) * deltaDist.x;
        }

        if (rayDirection.y < 0)
        {
            step.y = -1;
            sideDist.y = (position.y - mapPosition.y) * deltaDist.y;
        }
        else
        {
            step.y = 1;
            sideDist.y = (mapPosition.y + 1.0 - position.y) * deltaDist.y;
        }

        while (!hit)
        {
            if (sideDist.x < sideDist.y)
            {
                sideDist.x += deltaDist.x;
                mapPosition.x += step.x;
                side = 0;
            }
            else
            {
                sideDist.y += deltaDist.y;
                mapPosition.y += step.y;
                side = 1;
            }

            if (worldMap[mapPosition.x][mapPosition.y] > 0)
            {
                hit = true;
            }
        }

        if (side == 0)
        {
            perpWallDist = (sideDist.x - deltaDist.x);
        }
        else
        {
            perpWallDist = (sideDist.y - deltaDist.y);
        }

        const int lineHeight = static_cast<int>((GAME_HEIGHT / perpWallDist));

        int drawStart = -lineHeight / 2 + GAME_HEIGHT / 2;
        if (drawStart < 0)
        {
            drawStart = 0;
        }

        int drawEnd = lineHeight / 2 + GAME_HEIGHT / 2;
        if (drawEnd >= GAME_HEIGHT)
        {
            drawEnd = GAME_HEIGHT - 1;
        }

        const int texNum = worldMap[mapPosition.x][mapPosition.y] - 1;
        const Texture tex = textures[texNum];

        double wallX;
        if (side == 0)
        {
            wallX = position.y + perpWallDist * rayDirection.y;
        }
        else
        {
            wallX = position.x + perpWallDist * rayDirection.x;
        }
        wallX -= std::floor(wallX);

        int texX = static_cast<int>(wallX * tex.width);
        if (side == 0 && rayDirection.x > 0) texX = tex.width - texX - 1;
        if (side == 1 && rayDirection.y < 0) texX = tex.width - texX - 1;

        const double texStep = 1.0 * tex.height / lineHeight;
        double texPos = (drawStart - GAME_HEIGHT / 2 + lineHeight / 2) * texStep;

        double shadingPerc = std::min(perpWallDist / MAX_VIEW_DIST, 0.75);
        double lightValue = lightMap[mapPosition.x][mapPosition.y];

        for (int y = drawStart; y < drawEnd; y++)
        {
            const int texY = static_cast<int>(texPos) & (tex.height - 1);
            texPos += texStep;
            auto p = tex.pixels[tex.height * texY + texX];
            Darken(&p, shadingPerc);
            Lighten(&p, lightValue);
            buffer[y * GAME_WIDTH + x] = p.rgba;
        }
        ZBuffer[x] = perpWallDist;
    }

    for (int i = 0; i < NUM_SPRITES; i++)
    {
        spriteOrder[i] = i;
        auto spriteXDist = position.x - sprite[i].position.x;
        auto spriteYDist = position.y - sprite[i].position.y;
        spriteDistance[i] = std::sqrt(spriteXDist * spriteXDist + spriteYDist * spriteYDist);
    }

    sortSprites(spriteOrder, spriteDistance, NUM_SPRITES);

    for (int i = 0; i < NUM_SPRITES; i++)
    {
        Sprite currentSprite = sprite[spriteOrder[i]];
        Texture tex = textures[currentSprite.tex];
        Vector spritePosition = {currentSprite.position.x - position.x, currentSprite.position.y - position.y};

        double invDet = 1.0 / (plane.x * direction.y - direction.x * plane.y);
        Vector transform = {
            invDet * (direction.y * spritePosition.x - direction.x * spritePosition.y),
            invDet * (-plane.y * spritePosition.x + plane.x * spritePosition.y)
        };

        int spriteScreenX = static_cast<int>(GAME_WIDTH / 2 * (1 + transform.x / transform.y));

        int spriteHeight = std::abs(static_cast<int>(GAME_HEIGHT / transform.y));
        int drawStartY = -spriteHeight / 2 + GAME_HEIGHT / 2;
        if (drawStartY < 0)
        {
            drawStartY = 0;
        }
        int drawEndY = spriteHeight / 2 + GAME_HEIGHT / 2;
        if (drawEndY >= GAME_HEIGHT)
        {
            drawEndY = GAME_HEIGHT - 1;
        }

        int spriteWidth = std::abs(static_cast<int>(GAME_HEIGHT / transform.y));
        int drawStartX = -spriteWidth / 2 + spriteScreenX;
        if (drawStartX < 0)
        {
            drawStartX = 0;
        }
        int drawEndX = spriteWidth / 2 + spriteScreenX;
        if (drawEndX >= GAME_WIDTH)
        {
            drawEndX = GAME_WIDTH - 1;
        }

        double shadingPerc = std::min(spriteDistance[i] / MAX_VIEW_DIST, 0.75);

        for (int stripe = drawStartX; stripe < drawEndX; stripe++)
        {
            int texX = 256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * tex.width / spriteWidth / 256;

            if (transform.y > 0 && stripe > 0 && stripe < GAME_WIDTH && transform.y < ZBuffer[stripe])
            {
                for (int y = drawStartY; y < drawEndY; y++)
                {
                    int d = y * 256 - GAME_HEIGHT * 128 + spriteHeight * 128;
                    int texY = d * tex.height / spriteHeight / 256;
                    auto p = tex.pixels[texY * tex.width + texX];
                    if (p.r != 0 || p.g != 0 || p.b != 0)
                    {
                        Darken(&p, shadingPerc);
                        Lighten(&p, lightMap[static_cast<int>(currentSprite.position.x)][static_cast<int>(currentSprite.position.y)]);
                        buffer[y * GAME_WIDTH + stripe] = p.rgba;
                    }
                }
            }
        }
    }

    SDL_UnlockTexture(gameTexture);
    SDL_Rect destRect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, gameTexture, nullptr, &destRect);
}

void Draw()
{
    SDL_SetRenderDrawColor(renderer, 255, 0x00, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    DrawGame();
    DrawMap();
    SDL_RenderPresent(renderer);
}


int main(int argc, char* argv[])
{
    setbuf(stdout, nullptr);

    Init();
    Load();

    Timer capTimer;
    Timer fpsTimer;
    Timer stepTimer;
    int countedFrames = 0;
    fpsTimer.Start();
    stepTimer.Start();

    while (!quit)
    {
        capTimer.Start();
        ++countedFrames;

        double avgFPS = countedFrames / (fpsTimer.GetTicks() / 1000.0);
        if (avgFPS > 2000000)
        {
            avgFPS = 0;
        }

        const double frameTime = stepTimer.GetTicks() / 1000.0;

        Draw();
        Update(frameTime);

        stepTimer.Start();

        const int frameTicks = capTimer.GetTicks();
        if (frameTicks < SCREEN_TICKS_PER_FRAME)
        {
            SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
        }
    }

    Close();
    return 0;
}
