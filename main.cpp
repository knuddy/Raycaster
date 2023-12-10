#include <format>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>


void EXIT_LOG_SDL_ERROR(const std::string &message)
{
    printf("%s %s\n", message.c_str(), SDL_GetError());
    exit(0);
}

void EXIT_LOG_IMG_ERROR(const std::string &message)
{
    printf("%s %s\n", message.c_str(), IMG_GetError());
    exit(0);
}


constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;

SDL_Window* window = nullptr;
SDL_Surface* screenSurface = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* texture = nullptr;
SDL_Texture* viewportTexture = nullptr;

void init()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        EXIT_LOG_SDL_ERROR("SDL could not initialize!");
    }

    window = SDL_CreateWindow("Raycaster", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if(window == nullptr)
    {
        EXIT_LOG_SDL_ERROR("Window could not be created!");
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if(renderer == nullptr)
    {
        EXIT_LOG_SDL_ERROR("Renderer could not be created!");
    }

    constexpr int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if(!(IMG_Init(imgFlags) & imgFlags))
    {
        EXIT_LOG_IMG_ERROR("SDL_image could not initialize!");
    }

    screenSurface = SDL_GetWindowSurface(window);
}

SDL_Surface* loadSurface(std::string path)
{
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());

    if(loadedSurface == nullptr)
    {
        EXIT_LOG_IMG_ERROR(std::format("Unable to load image {}!", path));
    }

    SDL_Surface* optimizedSurface = SDL_ConvertSurface(loadedSurface, screenSurface->format, 0);

    if(optimizedSurface == nullptr)
    {
        EXIT_LOG_SDL_ERROR(std::format("Unable to optimize image {}!", path));
    }

    SDL_FreeSurface(loadedSurface);
    return optimizedSurface;
}

SDL_Texture* loadTexture(std::string path)
{
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == nullptr)
    {
        EXIT_LOG_IMG_ERROR(std::format("Unable to load image {}!", path));
    }

    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if(newTexture == nullptr)
    {
        EXIT_LOG_SDL_ERROR(std::format("Unable to create texture from {}!", path));
    }

    SDL_FreeSurface(loadedSurface);
    return newTexture;
}

void loadMedia()
{
    texture = loadTexture("textures/texture.png");
    viewportTexture = loadTexture("textures/viewport.png");
}

void close()
{
    SDL_DestroyTexture(texture);
    texture = nullptr;

    SDL_DestroyTexture(viewportTexture);
    viewportTexture = nullptr;

    SDL_DestroyRenderer(renderer);
    renderer = nullptr;

    SDL_DestroyWindow(window);
    window = nullptr;

    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[])
{
    setbuf(stdout, nullptr);

    init();
    loadMedia();

    bool quit = false;
    SDL_Event e;

    while(!quit)
    {
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        SDL_Rect topLeftViewport;
        topLeftViewport.x = 0;
        topLeftViewport.y = 0;
        topLeftViewport.w = SCREEN_WIDTH / 2;
        topLeftViewport.h = SCREEN_HEIGHT / 2;

        SDL_RenderSetViewport(renderer, &topLeftViewport);
        SDL_RenderCopy(renderer, viewportTexture, nullptr, nullptr);

        SDL_Rect topRightViewport;
        topRightViewport.x = SCREEN_WIDTH / 2;
        topRightViewport.y = 0;
        topRightViewport.w = SCREEN_WIDTH / 2;
        topRightViewport.h = SCREEN_HEIGHT / 2;

        SDL_RenderSetViewport(renderer, &topRightViewport);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);

        SDL_Rect bottomViewport;
        bottomViewport.x = 0;
        bottomViewport.y = SCREEN_HEIGHT / 2;
        bottomViewport.w = SCREEN_WIDTH;
        bottomViewport.h = SCREEN_HEIGHT / 2;

        SDL_RenderSetViewport(renderer, &bottomViewport);
        SDL_RenderCopy(renderer, viewportTexture, nullptr, nullptr);

        SDL_RenderPresent(renderer);
    }

    return 0;
}