#include <SDL2/SDL.h>   
#include <cstdlib>
#include <iostream>
#include <vector>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <X11/Xlib.h>
#include "X11/keysym.h"

void draw_rectangle_fill(SDL_Renderer* renderer, const SDL_Rect& rectangle, const SDL_Color& color);
void mousePress(SDL_MouseButtonEvent& b, int& xMouse, int& yMouse);
void addPoint(int x, int y);
int startRatioEr();
void tmpPoint(int x, int y);
void renderLine(SDL_Renderer *renderer);
void renderText(SDL_Renderer *renderer, int num, int x, int y);
int max(int a, int b);
int min(int a, int b);

class point {
    private:
    public: 
        int x;
        int y;
        point (int x, int y): x(x), y(y) {};
};

class line {
    public:
        std::vector<point> points;
        int count = 0;
};

struct line line;

bool key_is_pressed(KeySym ks) {
    Display *dpy = XOpenDisplay(":0");
    char keys_return[32];
    XQueryKeymap(dpy, keys_return);
    KeyCode kc2 = XKeysymToKeycode(dpy, ks);
    bool isPressed = !!(keys_return[kc2 >> 3] & (1 << (kc2 & 7)));
    XCloseDisplay(dpy);
    return isPressed;
}

bool ctrl_is_pressed() {
    return key_is_pressed(XK_Control_L) && key_is_pressed(XK_Shift_L);
}

int main(int argc, char* argv[])
{
    int flag = 0;
    while (1){
        int pressed = ctrl_is_pressed();
        if (pressed && flag == 0){
            flag = 1;
            startRatioEr();
        }
        if (!pressed)
            flag = 0;
    }
}

int startRatioEr(){
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
    SDL_GetGlobalMouseState(&xMouse,&yMouse);
    addPoint(xMouse, yMouse);
    addPoint(xMouse, yMouse);
    TTF_Init();
    
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
                tmpPoint(line.points[0].x, yMouse);
                break;
            case SDL_MOUSEBUTTONDOWN:
                mousePress(events.button, line.points[0].x, yMouse);
                break;
            case SDL_KEYDOWN:
                switch (events.key.keysym.scancode)
                {
                    case SDL_SCANCODE_LCTRL:
                        isOpen = false;
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (events.key.keysym.scancode)
                {
                    case SDL_SCANCODE_LCTRL:
                        isOpen = false;
                        break;
                }
                break;
            }
        }
        SDL_SetWindowOpacity(pWindow, 0.5);
        SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 0);
        SDL_RenderClear(pRenderer);

        renderLine(pRenderer);

        SDL_RenderPresent(pRenderer);
    }
    line.points.clear();
    line.count = 0;
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return EXIT_SUCCESS;
}

void mousePress(SDL_MouseButtonEvent& b, int& xMouse, int& yMouse){
  if(b.button == SDL_BUTTON_LEFT){
    std::cout << "left click" <<std::endl;
    addPoint(xMouse, yMouse);
  }
}

void renderLine(SDL_Renderer *renderer){
    int totalSize = 0;
    if (line.points.size() > 2)
        totalSize = max(line.points[0].y, line.points[line.points.size() - 1].y) - min(line.points[0].y, line.points[line.points.size() - 1].y);
    for (int i = 1; i < line.points.size(); i++){
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        if (totalSize > 0){
            const int size = max(line.points[i].y, line.points[i - 1].y) - min(line.points[i].y, line.points[i - 1].y);
            const int ratio = (size / (float) totalSize) * 100;
            if (ratio == 0)
                return;
            const int y = (line.points[i].y + line.points[i - 1].y) / 2;
            renderText(renderer, ratio, y, line.points[i].x + 20);
        }
        SDL_RenderDrawLine(renderer, line.points[i - 1].x, line.points[i - 1].y, line.points[i].x, line.points[i].y - 28);
        SDL_RenderDrawLine(renderer, line.points[i].x - 10, line.points[i].y - 28, line.points[i].x + 10, line.points[i].y - 28);
    }
}

void renderText(SDL_Renderer *renderer, int num, int y, int x){
    TTF_Font* Sans = TTF_OpenFont("OpenSans-Bold.ttf", 24);
    SDL_Color White = {255, 255, 255};
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, std::to_string(num).c_str(), White);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    if (!Message){
        std::cout <<  SDL_GetError() << std::endl;
        return;
    }
    SDL_Rect Message_rect; //create a rect
    Message_rect.w = 100; // controls the width of the rect
    Message_rect.h = 100; // controls the height of the rect
    Message_rect.x = x;  //controls the rect's x coordinate 
    Message_rect.y = y - Message_rect.h / 2; // controls the rect's y coordinte
    const int ret = SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}

void tmpPoint(int x, int y){
    line.points[line.points.size() - 1].x = x;
    line.points[line.points.size() - 1].y = y;
}

void addPoint(int x, int y){
    line.points.push_back(point(x, y));
    line.count++;
}

int max(int a, int b) { return a > b ? a : b; }
int min(int a, int b) { return a < b ? a : b; }