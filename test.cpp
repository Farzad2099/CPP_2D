
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <iostream>
#include <cmath>

int main() {
    const int screenWidth = 1000;
    const int screenHeight = 2000;

    const int centerX = screenWidth / 2;
    const int centerY = screenHeight / 2;
    const int radius = 500;

    double angle;

    std::cout << "Enter angle in radians: " << std::flush;

    if (!(std::cin >> angle)) {
        std::cerr << "Invalid angle input.\n";
        return 1;
    }

    // Point on the circle
    const double x = radius * std::cos(angle);
    const double y = radius * std::sin(angle);

    const int pointX =
        centerX + static_cast<int>(std::lround(x));

    const int pointY =
        centerY - static_cast<int>(std::lround(y));

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init failed: "
                  << SDL_GetError() << '\n';
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Trigonometry",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screenWidth,
        screenHeight,
        SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        std::cerr << "Window error: "
                  << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if (!renderer) {
        std::cerr << "Renderer error: "
                  << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            if (event.type == SDL_KEYDOWN &&
                event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
        }

        // Background
        SDL_SetRenderDrawColor(renderer, 0, 0, 200, 255);
        SDL_RenderClear(renderer);

        // Grid
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

        for (int px = 0; px < screenWidth; px += 50) {
            SDL_RenderDrawLine(
                renderer, px, 0, px, screenHeight
            );
        }

        for (int py = 0; py < screenHeight; py += 50) {
            SDL_RenderDrawLine(
                renderer, 0, py, screenWidth, py
            );
        }

        // X and Y axes
        SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);

        SDL_RenderDrawLine(
            renderer, 0, centerY, screenWidth, centerY
        );

        SDL_RenderDrawLine(
            renderer, centerX, 0, centerX, screenHeight
        );

        // Axis tick marks
        for (int px = 0; px < screenWidth; px += 50) {
            SDL_RenderDrawLine(
                renderer, px, centerY - 6,
                px, centerY + 6
            );
        }

        for (int py = 0; py < screenHeight; py += 50) {
            SDL_RenderDrawLine(
                renderer, centerX - 6, py,
                centerX + 6, py
            );
        }

        // Circle
        circleColor(
            renderer,
            centerX,
            centerY,
            radius,
            0x50B4FFFF
        );

        // Green horizontal line:
        // Origin -> vertical projection on X axis
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

        SDL_RenderDrawLine(
            renderer,
            centerX,
            centerY,
            pointX,
            centerY
        );

        // Make the green line thicker
        SDL_RenderDrawLine(
            renderer,
            centerX,
            centerY - 1,
            pointX,
            centerY - 1
        );

        SDL_RenderDrawLine(
            renderer,
            centerX,
            centerY + 1,
            pointX,
            centerY + 1
        );

        // Yellow vertical line:
        // Projection on X axis -> point on circle
        thickLineColor(
            renderer,
            pointX,
            centerY,
            pointX,
            pointY,
            5,
            0xFFFF00FF
        );

        // Orange hypotenuse:
        // Origin -> point on circle
        thickLineColor(
            renderer,
            centerX,
            centerY,
            pointX,
            pointY,
            5,
            0xFF8C00FF
        );

        // Point on circumference
        filledCircleColor(
            renderer,
            pointX,
            pointY,
            8,
            0xFF5050FF
        );

        // Origin marker
        SDL_Rect origin = {
            centerX - 5,
            centerY - 5,
            10,
            10
        };

        SDL_SetRenderDrawColor(renderer, 255, 80, 80, 255);
        SDL_RenderFillRect(renderer, &origin);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

