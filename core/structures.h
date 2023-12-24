#ifndef STRUCTURES_H
#define STRUCTURES_H
#include "include.h"

union Pixel
{
    struct
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };

    uint32_t rgba;
};

struct Texture
{
    SDL_Texture* tex;
    int width, height;
    Pixel* pixels;
};

inline void Texture_Free(Texture* texture)
{
    if (texture != nullptr)
    {
        SDL_DestroyTexture(texture->tex);
        texture->tex = nullptr;
        texture->pixels = nullptr;
        texture->width = 0;
        texture->height = 0;
    }
}

inline bool Texture_FromFile(Texture* texture, SDL_Window* window, SDL_Renderer* renderer, std::string path)
{
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());

    if (loadedSurface == nullptr)
    {
        EXIT_LOG_IMG_ERROR(std::format("Unable to load image {}!", path));
    }

    SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0x00, 0x00, 0x00 ) );

    SDL_Surface* optimizedSurface = SDL_ConvertSurfaceFormat(loadedSurface, SDL_GetWindowPixelFormat(window), 0);
    if (optimizedSurface == nullptr)
    {
        EXIT_LOG_SDL_ERROR("Unable to convert loaded surface to display format!");
    }

    texture->tex = SDL_CreateTextureFromSurface(renderer, optimizedSurface);
    texture->width = optimizedSurface->w;
    texture->height = optimizedSurface->h;
    texture->pixels = new Pixel[optimizedSurface->w * optimizedSurface->h];

    const auto surfacePixels = static_cast<Pixel*>(optimizedSurface->pixels);

    for(int y = 0; y < optimizedSurface->h; y++)
    {
        for(int x = 0; x < optimizedSurface->w; x++)
        {
            texture->pixels[optimizedSurface->w * y + x] = surfacePixels[optimizedSurface->w  * y + x];
        }
    }

    SDL_FreeSurface(loadedSurface);
    SDL_FreeSurface(optimizedSurface);
    return texture->tex != nullptr;
}

struct Vector
{
    double x, y;
};

struct IVector
{
    int x, y;
};

struct Sprite
{
    Vector position;
    int tex;
};

struct Thing
{
    Vector position;
    int textureIndex;
};


struct Level
{
    Texture* textures;
    int textureCount;

    std::vector<std::vector<int>> wallMap;
    std::vector<std::vector<int>> floorMap;
    std::vector<std::vector<int>> ceilingMap;
    std::vector<std::vector<int>> lightMap;

    int mapWidth;
    int mapHeight;

    int skyTexture;

    Thing* things;
    int thingCount;

};



#endif
