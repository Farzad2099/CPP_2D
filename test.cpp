
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <cmath>
#include <cstdio>

// ============================================================
// 1. Main Screen and Coordinate Settings
// ============================================================

// Screen dimensions
const int SCREEN_WIDTH  = 1000;
const int SCREEN_HEIGHT = 2000;

// Screen center and coordinate origin
const int CENTER_X = SCREEN_WIDTH / 2;
const int CENTER_Y = SCREEN_HEIGHT / 2;

// Circle radius and outline thickness
const int RADIUS = 500;
const int CIRCLE_THICKNESS = 8;

// Mathematical constant PI
const double PI = 3.14159265358979323846;


// ============================================================
// 2. Draw a Thick Circle
//    The circle is yellow with a thickness of 8 pixels.
// ============================================================

void drawThickCircle(SDL_Renderer* renderer,
                     Sint16 x, Sint16 y,
                     Sint16 radius, int thickness)
{
    // Draw concentric circles to create a thicker outline
    for (int i = 0; i < thickness; i++) {
        circleRGBA(
            renderer,
            x, y,
            static_cast<Sint16>(radius - i),
            255, 255, 0, 255
        );
    }
}


// ============================================================
// 3. Draw Text with a Custom Font Size
//    SDL_ttf allows the font size to be adjusted.
// ============================================================

void drawText(SDL_Renderer* renderer,
              TTF_Font* font,
              const char* text,
              int x, int y,
              SDL_Color color,
              bool centered = false)
{
    // Render the text into an SDL surface
    SDL_Surface* surface =
        TTF_RenderUTF8_Blended(font, text, color);

    if (!surface) {
        return;
    }

    // Convert the surface into a texture
    SDL_Texture* texture =
        SDL_CreateTextureFromSurface(renderer, surface);

    if (!texture) {
        SDL_FreeSurface(surface);
        return;
    }

    // Calculate the destination rectangle
    SDL_Rect destination;
    destination.w = surface->w;
    destination.h = surface->h;

    // Center the text if requested
    destination.x = centered ? x - surface->w / 2 : x;
    destination.y = centered ? y - surface->h / 2 : y;

    // Draw the text texture
    SDL_RenderCopy(renderer, texture, nullptr, &destination);

    // Release temporary resources
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}


// ============================================================
// 4. Main Function
// ============================================================

int main(int argc, char* argv[])
{
    // --------------------------------------------------------
    // Read the angle from the user in radians
    // --------------------------------------------------------

    double angle;

    std::cout << "Enter angle in radians: ";

    if (!(std::cin >> angle) ||
        angle < 0 || angle > 2 * PI) {
        std::cerr << "Angle must be between 0 and 2*PI.\n";
        return 1;
    }


    // --------------------------------------------------------
    // Initialize SDL video subsystem
    // --------------------------------------------------------

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init error: "
                  << SDL_GetError() << '\n';
        return 1;
    }

    // Initialize the SDL_ttf font subsystem
    if (TTF_Init() != 0) {
        std::cerr << "TTF_Init error: "
                  << TTF_GetError() << '\n';

        SDL_Quit();
        return 1;
    }


    // --------------------------------------------------------
    // Create the application window
    // --------------------------------------------------------

    SDL_Window* window = SDL_CreateWindow(
        "Radian Geometry",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        std::cerr << "Window error: "
                  << SDL_GetError() << '\n';

        TTF_Quit();
        SDL_Quit();
        return 1;
    }


    // --------------------------------------------------------
    // Create the SDL renderer
    // --------------------------------------------------------

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED
    );

    if (!renderer) {
        std::cerr << "Renderer error: "
                  << SDL_GetError() << '\n';

        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Keep a fixed logical coordinate system when resizing
    SDL_RenderSetLogicalSize(
        renderer, SCREEN_WIDTH, SCREEN_HEIGHT
    );


    // --------------------------------------------------------
    // Load fonts
    // Axis labels use 24-pixel text.
    // The angle display uses a larger 36-pixel font.
    // --------------------------------------------------------

    const char* fontPath = "/system/fonts/Roboto-Regular.ttf";

    TTF_Font* axisFont = TTF_OpenFont(fontPath, 24);
    TTF_Font* angleFont = TTF_OpenFont(fontPath, 36);

    if (!axisFont || !angleFont) {
        std::cerr << "Font loading error: "
                  << TTF_GetError() << '\n';

        if (axisFont) TTF_CloseFont(axisFont);
        if (angleFont) TTF_CloseFont(angleFont);

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);

        TTF_Quit();
        SDL_Quit();
        return 1;
    }


    // --------------------------------------------------------
    // Calculate the point on the circle using trigonometry
    // SDL's Y-axis increases downward, so the sine term is
    // subtracted when calculating the screen Y coordinate.
    // --------------------------------------------------------

    const int pointX =
        CENTER_X + static_cast<int>(
            RADIUS * std::cos(angle)
        );

    const int pointY =
        CENTER_Y - static_cast<int>(
            RADIUS * std::sin(angle)
        );


    // --------------------------------------------------------
    // Define text colors
    // --------------------------------------------------------

    const SDL_Color labelColor = {208, 216, 232, 255};
    const SDL_Color angleColor = {255, 255, 255, 255};


    // --------------------------------------------------------
    // Main application loop
    // --------------------------------------------------------

    bool running = true;
    SDL_Event event;

    while (running) {

        // Process window events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }


        // ====================================================
        // 5. Draw the Background and Grid
        // ====================================================

        SDL_SetRenderDrawColor(renderer, 15, 20, 35, 255);
        SDL_RenderClear(renderer);

        // Draw vertical grid lines at 50-pixel intervals
        SDL_SetRenderDrawColor(renderer, 40, 48, 65, 255);

        for (int x = 0; x <= SCREEN_WIDTH; x += 50) {
            SDL_RenderDrawLine(
                renderer, x, 0, x, SCREEN_HEIGHT
            );
        }

        // Draw horizontal grid lines at 50-pixel intervals
        for (int y = 0; y <= SCREEN_HEIGHT; y += 50) {
            SDL_RenderDrawLine(
                renderer, 0, y, SCREEN_WIDTH, y
            );
        }


        // ====================================================
        // 6. Draw the X and Y Coordinate Axes
        // ====================================================

        SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);

        // Horizontal X-axis
        SDL_RenderDrawLine(
            renderer, 0, CENTER_Y,
            SCREEN_WIDTH, CENTER_Y
        );

        // Vertical Y-axis
        SDL_RenderDrawLine(
            renderer, CENTER_X, 0,
            CENTER_X, SCREEN_HEIGHT
        );


        // ====================================================
        // 7. Draw Axis Tick Marks
        //    Tick spacing remains 50 pixels.
        // ====================================================

        for (int i = -RADIUS; i <= RADIUS; i += 50) {

            // Tick marks along the X-axis
            SDL_RenderDrawLine(
                renderer,
                CENTER_X + i, CENTER_Y - 7,
                CENTER_X + i, CENTER_Y + 7
            );

            // Tick marks along the Y-axis
            SDL_RenderDrawLine(
                renderer,
                CENTER_X - 7, CENTER_Y - i,
                CENTER_X + 7, CENTER_Y - i
            );
        }


        // ====================================================
        // 8. Draw the Yellow Circle with 8-Pixel Thickness
        // ====================================================

        drawThickCircle(
            renderer,
            CENTER_X, CENTER_Y,
            RADIUS, CIRCLE_THICKNESS
        );


        // ====================================================
        // 9. Draw the Right Triangle
        // ====================================================

        // Horizontal leg: green
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

        SDL_RenderDrawLine(
            renderer,
            CENTER_X, CENTER_Y,
            pointX, CENTER_Y
        );

        // Vertical leg: yellow
        SDL_SetRenderDrawColor(renderer, 255, 220, 0, 255);

        SDL_RenderDrawLine(
            renderer,
            pointX, CENTER_Y,
            pointX, pointY
        );

        // Hypotenuse: orange
        SDL_SetRenderDrawColor(renderer, 255, 140, 0, 255);

        SDL_RenderDrawLine(
            renderer,
            CENTER_X, CENTER_Y,
            pointX, pointY
        );


        // ====================================================
        // 10. Draw the Circle Point and Coordinate Origin
        // ====================================================

        // Point on the circle: red
        filledCircleRGBA(
            renderer, pointX, pointY, 9,
            255, 50, 80, 255
        );

        // Coordinate origin: white
        filledCircleRGBA(
            renderer, CENTER_X, CENTER_Y, 5,
            255, 255, 255, 255
        );


        // ====================================================
        // 11. Draw Coordinate Labels
        //     Display 1, 2, 3... instead of 100, 200, 300...
        //     The label spacing remains 100 pixels.
        // ====================================================

        for (int i = -RADIUS; i <= RADIUS; i += 100) {

            // Skip the origin label
            if (i == 0) {
                continue;
            }

            char label[16];

            // Convert pixel offsets into displayed coordinate units
            std::snprintf(
                label, sizeof(label), "%d", i / 100
            );

            // Draw the X-axis label
            drawText(
                renderer, axisFont, label,
                CENTER_X + i, CENTER_Y + 24,
                labelColor, true
            );

            // Draw the Y-axis label
            drawText(
                renderer, axisFont, label,
                CENTER_X + 24, CENTER_Y - i,
                labelColor, true
            );
        }


        // ====================================================
        // 12. Display the Angle in Radians
        //     The angle uses a larger 36-pixel font.
        // ====================================================

        char angleText[64];

        std::snprintf(
            angleText, sizeof(angleText),
            "Angle: %.3f rad", angle
        );

        // Position the angle text near the top-left corner
        drawText(
            renderer, angleFont, angleText,
            20, 20, angleColor
        );


        // ====================================================
        // 13. Present the Frame
        // ====================================================

        SDL_RenderPresent(renderer);

        // Limit CPU usage with a short delay
        SDL_Delay(16);
    }


    // ========================================================
    // 14. Clean Up Resources Before Exiting
    // ========================================================

    TTF_CloseFont(axisFont);
    TTF_CloseFont(angleFont);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();

    return 0;
}

