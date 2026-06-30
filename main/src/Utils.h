#pragma once
#include "SDL.h"

inline bool CheckCollision(const SDL_Rect& a, const SDL_Rect& b) {
    return SDL_HasIntersection(&a, &b);
}


inline SDL_Rect ShrinkRect(const SDL_Rect& rect, int shrinkAmount) {
    SDL_Rect newRect;
    newRect.x = rect.x + shrinkAmount;
    newRect.y = rect.y + shrinkAmount;
    newRect.w = rect.w - 2 * shrinkAmount;
    newRect.h = rect.h - 2 * shrinkAmount;
    return newRect;
}

inline SDL_Rect AdjustHitbox(const SDL_Rect& rect, int shrinkLeft, int shrinkRight, int shrinkTop, int extendBottom)
{
    SDL_Rect newRect;
    newRect.x = rect.x + shrinkLeft;
    newRect.w = rect.w - shrinkLeft - shrinkRight;
    newRect.y = rect.y + shrinkTop;
    newRect.h = rect.h - shrinkTop + extendBottom;

    return newRect;
}
