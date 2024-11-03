#include <SDL2/SDL.h>
#include <cmath>
#include <complex>
#include <algorithm>

const int WIDTH = 800;
const int HEIGHT = 600;
double zoom = 4.0; // Initial zoom level
double offsetX = 0.0, offsetY = 0.0; // Offsets for dragging
bool dragging = false;
int dragStartX, dragStartY;

void setPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
    Uint32* pixels = (Uint32*)surface->pixels;
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        pixels[y * surface->w + x] = color;
    }
}

Uint32 getColor(std::complex<double> z) {
    double angle = std::arg(z);
    double radius = std::abs(z);

    double normalizedAngle = -(angle + M_PI) / (2 * M_PI); // Shift angle to [0, 1]

    // Calculate RGB values based on the angle (hue)
    Uint8 r_color = static_cast<Uint8>((0.5 + 0.5 * -sin(normalizedAngle * 2 * M_PI + M_PI / 2)) * 255);
    Uint8 g_color = static_cast<Uint8>((0.5 + 0.5 * -sin(normalizedAngle * 2 * M_PI - 2 * M_PI / 3 + M_PI / 2)) * 255);
    Uint8 b_color = static_cast<Uint8>((0.5 + 0.5 * -sin(normalizedAngle * 2 * M_PI - 4 * M_PI / 3 + M_PI / 2)) * 255);

        // Scale luminosity based on the radius
    double luminosityScale = 8.0 / (8.0 + radius); // Increases with radius
    r_color = static_cast<Uint8>(r_color * luminosityScale);
    g_color = static_cast<Uint8>(g_color * luminosityScale);
    b_color = static_cast<Uint8>(b_color * luminosityScale);
        // Blend towards white based on the radius
    double blendToWhiteFactor = 0.2 / (0.2 + radius);
    r_color = static_cast<Uint8>(r_color + blendToWhiteFactor * (255 - r_color));
    g_color = static_cast<Uint8>(g_color + blendToWhiteFactor * (255 - g_color));
    b_color = static_cast<Uint8>(b_color + blendToWhiteFactor * (255 - b_color));
    return SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888), r_color, g_color, b_color);
}

void render(SDL_Surface* surface) {
    for (int x = 0; x < WIDTH; ++x) {
        for (int y = 0; y < HEIGHT; ++y) {
            double real = (x - WIDTH / 2.0 + offsetX) / (WIDTH / zoom);
            double imag = (y - HEIGHT / 2.0 + offsetY) / (HEIGHT / zoom);
            std::complex<double> z(real, imag);
            Uint32 color = getColor(z);
            setPixel(surface, x, y, color);
        }
    }
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Domain Coloring - Identity Function", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
    SDL_Surface* surface = SDL_GetWindowSurface(window);

    render(surface);
    SDL_UpdateWindowSurface(window);

    SDL_Event event;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEWHEEL) {
                if (event.wheel.y > 0) {
                    zoom *= 0.9; // Zoom in
                } else if (event.wheel.y < 0) {
                    zoom *= 1.1; // Zoom out
                }
                render(surface);
                SDL_UpdateWindowSurface(window);
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    dragging = true;
                    dragStartX = event.button.x;
                    dragStartY = event.button.y;
                }
            } else if (event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    dragging = false;
                }
            } else if (event.type == SDL_MOUSEMOTION && dragging) {
                int dx = -(event.motion.x - dragStartX);
                int dy = -(event.motion.y - dragStartY);
                // Reverse direction and increase speed
                offsetX += dx * (zoom / WIDTH * 8.0);
                offsetY += dy * (zoom / HEIGHT * 14.0);
                dragStartX = event.motion.x;
                dragStartY = event.motion.y;
                render(surface);
                SDL_UpdateWindowSurface(window);
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
