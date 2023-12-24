#ifndef INCLUDE_H
#define INCLUDE_H

#include <format>
#include <stdio.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <cmath>
#include <vector>
#include <algorithm>
#include <limits>

inline void EXIT_LOG_SDL_ERROR(const std::string &message)
{
    printf("%s %s\n", message.c_str(), SDL_GetError());
    exit(1);
}

inline void EXIT_LOG_IMG_ERROR(const std::string &message)
{
    printf("%s %s\n", message.c_str(), IMG_GetError());
    exit(1);
}

#endif
