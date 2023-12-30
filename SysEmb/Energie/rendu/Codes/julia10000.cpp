// Standard C++ stuff
#include <iostream>
#include <complex>
#include <vector>
#include <array>

// SDL2 stuff
#include "SDL2/SDL.h"

// Other crazy stuffs
#define ScreenWidth  10000
#define ScreenHeight 10000

// Compile: g++ -std=c++20 -Wall -Wextra -pedantic julia-set-sdl2.cpp -o julia-set-sdl2 -lSDL2

void DrawJuliaSet(SDL_Surface *s, double real, double imag, int maxiter) {
    // Generate colours
    std::vector<std::array<int, 3>> colours;
    for (int col = 0; col < 256; col++) {
        std::array<int, 3> CurrentColour = {(col >> 5) * 36, (col >> 3 & 7) * 36, (col & 3) * 85};
        colours.push_back(CurrentColour);
    }

    std::complex<double> c = {real, imag}, z;

    for (int x = 0; x < s->w; x++) {
        for (int y = 0; y < s->h; y++) {
            z.real(1.5 * (x - s->w / 2) / (0.5 * s->w));
            z.imag((y - s->h / 2) / (0.5 * s->h));

            int i = maxiter;

            while (std::norm(z) < 4 && i > 0) {
                z = z * z + c;
                i--;
            }

            Uint32 *pixels = (Uint32 *)s->pixels;
            pixels[y * s->w + x] = SDL_MapRGB(s->format, colours[i][0], colours[i][1], colours[i][2]);
        }
    }
    SDL_SaveBMP(s, "screenShotJ.bmp");
}

int main(int argc, char *args[]) {
    const int MaximumIterations = 1000;    
    
    SDL_Window *window = NULL; // Define window
    SDL_Renderer *renderer = NULL; // Define renderer
    SDL_Surface *surface = NULL; //create surface to save the image
    
    // First things first: initialise video
    SDL_Init(SDL_INIT_EVERYTHING);
        
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // Create renderer
    surface = SDL_CreateRGBSurface(0, ScreenWidth, ScreenHeight, 32, 0, 0, 0, 0);

    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(renderer); // Clear screen32
    
    DrawJuliaSet(surface, -0.7, 0.27015, MaximumIterations); // Draw the Julia set
    SDL_RenderPresent(renderer); // Render it!
    
    // Create an event handler and a "quit" flag
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer); // Destroy renderer
    SDL_DestroyWindow(window); // Destroy window
    SDL_Quit();
    
    return 0;
}
