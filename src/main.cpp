#include <SDL2/SDL.h>   
#include <cstdlib>   
#include <iostream> 

void draw_rectangle_fill(SDL_Renderer* renderer, const SDL_Rect& rectangle, const SDL_Color& color);

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_Window* pWindow{ nullptr };     
    SDL_Renderer* pRenderer{ nullptr };
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    auto Width = DM.w;
    auto Height = DM.h;
    int xMouse, yMouse;
    // SDL_Window* SDL_CreateWindow(const char* title, int x, int y, int w, int h, Uint32 flags)
    if (SDL_CreateWindowAndRenderer(Width, Height, SDL_WINDOW_BORDERLESS | SDL_WINDOW_SHOWN, &pWindow, &pRenderer) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", SDL_GetError());        
        SDL_Quit();         
        return EXIT_FAILURE;
    }
    SDL_Event events;
    bool isOpen{ true };
    SDL_Rect rectangle1{ 0, 0, 300, 50 };
    rectangle1.x = Width / 2 - Width / 4;
    rectangle1.y = Height / 2 - Height / 4;
    while (isOpen)
    {
        while (SDL_PollEvent(&events))
        {
            switch (events.type)
            {
            case SDL_QUIT:
                isOpen = false;
                break;
            case SDL_MOUSEMOTION:
                SDL_GetGlobalMouseState(&xMouse,&yMouse);
                break;
            case SDL_KEYDOWN:
                break;
            case SDL_KEYUP:
                switch (events.key.keysym.scancode)
                {
                    case SDL_SCANCODE_LCTRL:
                        SDL_HideWindow(pWindow);
                        break;
                }
                break;
            }
        }
        SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
        SDL_RenderClear(pRenderer);

        SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
        // SDL_RenderDrawLine(pRenderer, 100, 100, 500, 100);

        SDL_RenderPresent(pRenderer);
    }
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return EXIT_SUCCESS;
}