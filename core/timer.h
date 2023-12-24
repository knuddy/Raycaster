#ifndef TIMER_H
#define TIMER_H
#include "include.h"

class Timer
{
public:
    Timer()
    {
        mStartTicks = 0;
        mPausedTicks = 0;
        mStarted = false;
        mPaused = false;
    }

    void Start()
    {
        mStarted = true;
        mPaused = false;
        mStartTicks = SDL_GetTicks();
        mPausedTicks = 0;
    }

    void Stop()
    {
        mStartTicks = 0;
        mPausedTicks = 0;
        mStarted = false;
        mPaused = false;
    }

    void Pause()
    {
        if (mStarted && !mPaused)
        {
            mPaused = true;
            mPausedTicks = SDL_GetTicks() - mStartTicks;
            mStartTicks = 0;
        }
    }

    void Unpause()
    {
        if (mStarted && mPaused)
        {
            mPaused = false;
            mStartTicks = SDL_GetTicks() - mPausedTicks;
            mPausedTicks = 0;
        }
    }

    Uint32 GetTicks() const
    {
        if (mStarted)
        {
            if (mPaused)
            {
                return mPausedTicks;
            }

            return SDL_GetTicks() - mStartTicks;
        }
        return 0;
    }

    bool IsStarted() const
    {
        return mStarted;
    }

    bool IsPaused() const
    {
        return mStarted && mPaused;
    }

private:
    Uint32 mStartTicks;
    Uint32 mPausedTicks;
    bool mPaused;
    bool mStarted;
};

#endif
