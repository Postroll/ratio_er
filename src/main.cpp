#include <SDL2/SDL.h>   
#include <cstdlib>
#include <iostream>
#include <vector>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <unistd.h>

void draw_rectangle_fill(SDL_Renderer* renderer, const SDL_Rect& rectangle, const SDL_Color& color);
void mousePress(SDL_MouseButtonEvent& b, int& xMouse, int& yMouse);
void addPoint(int x, int y);
int startRatioEr();
void tmpPoint(int x, int y);
void renderLine(SDL_Renderer *renderer, TTF_Font* Sans);
void renderText(SDL_Renderer *renderer, int num, int x, int y, TTF_Font* Sans);
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
        int dir = 0;
        int finished = 0;
};

struct line line;

int main(int argc, char* argv[])
{
    startRatioEr();
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
    TTF_Init();
    TTF_Font* Sans = TTF_OpenFont("/home/mlauro/Documents/ratio_er/ttf/OpenSans-Bold.ttf", 100);
    if (!Sans){
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
    }
    SDL_GetCurrentDisplayMode(0, &DM);
    auto Width = DM.w;
    auto Height = DM.h;
    int xMouse, yMouse;
    SDL_GetGlobalMouseState(&xMouse,&yMouse);

    addPoint(xMouse, yMouse);
    addPoint(xMouse, yMouse);
    
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
                tmpPoint(xMouse, yMouse);
                break;
            case SDL_MOUSEBUTTONDOWN:
                mousePress(events.button, xMouse, yMouse);
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

        renderLine(pRenderer, Sans);

        SDL_RenderPresent(pRenderer);
    }
    line.points.clear();
    line.finished = 0;
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return EXIT_SUCCESS;
}

void mousePress(SDL_MouseButtonEvent& b, int& xMouse, int& yMouse){
  if(b.button == SDL_BUTTON_LEFT && !line.finished){
    std::cout << "left click" <<std::endl;
    addPoint(xMouse, yMouse);
  }
  else if (b.button == SDL_BUTTON_RIGHT && !line.finished){
    std::cout << "fjjf" << std::endl;
    line.finished = 1;
    line.points.erase(line.points.end() - 1);
  }
}

void renderLine(SDL_Renderer *renderer, TTF_Font* Sans){
    int totalSize = 0;
    if (line.dir == 0){
        if (line.points.size() > 2)
            totalSize = max(line.points[0].y, line.points[line.points.size() - 1].y) - min(line.points[0].y, line.points[line.points.size() - 1].y);
        for (int i = 1; i < line.points.size(); i++){
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            if (totalSize > 0){
                const int size = max(line.points[i].y, line.points[i - 1].y) - min(line.points[i].y, line.points[i - 1].y);
                const int ratio = (size / (float) totalSize) * 100;
                if (ratio == 0)
                    return;
                const int y = line.points[i - 1].y - (line.points[i - 1].y - line.points[i].y) / 2;
                renderText(renderer, ratio, y, line.points[i].x + 20, Sans);
            }
            SDL_RenderDrawLine(renderer, line.points[i - 1].x, line.points[i - 1].y, line.points[i].x, line.points[i].y);
            SDL_RenderDrawLine(renderer, line.points[i].x - 10, line.points[i].y, line.points[i].x + 10, line.points[i].y);
        }
    }
    else {
        if (line.points.size() > 2)
            totalSize = max(line.points[0].x, line.points[line.points.size() - 1].x) - min(line.points[0].x, line.points[line.points.size() - 1].x);
        for (int i = 1; i < line.points.size(); i++){
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            if (totalSize > 0){
                const int size = max(line.points[i].x, line.points[i - 1].x) - min(line.points[i].x, line.points[i - 1].x);
                const int ratio = (size / (float) totalSize) * 100;
                if (ratio == 0)
                    return;
                const int x = line.points[i - 1].x - (line.points[i - 1].x - line.points[i].x) / 2;
                renderText(renderer, ratio, line.points[i].y + 20, x, Sans);
            }
            SDL_RenderDrawLine(renderer, line.points[i - 1].x, line.points[i - 1].y, line.points[i].x, line.points[i].y);
            SDL_RenderDrawLine(renderer, line.points[i].x, line.points[i].y - 10, line.points[i].x, line.points[i].y + 10);
        }
    }
}

void renderText(SDL_Renderer *renderer, int num, int y, int x, TTF_Font* Sans){
    SDL_Color White = {255, 255, 255};
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, std::to_string(num).c_str(), White);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    if (!Message){
        std::cout <<  SDL_GetError() << std::endl;
        return;
    }
    SDL_Rect Message_rect;
    Message_rect.w = num > 99 ? 100 : num > 9 ? 66 : 33;
    Message_rect.h = 100;
    if (line.dir == 0){
        Message_rect.x = x;
        Message_rect.y = y - (Message_rect.h / 4) * 3;
    }
    else {
        Message_rect.x = x - (Message_rect.w / 4) * 3;
        Message_rect.y = y;
    }
    const int ret = SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}

void tmpPoint(int x, int y){
    y -= 28;
    if (line.finished) return;
    if (line.points.size() == 2){
        const int xLength = abs(line.points[0].x - x);
        const int yLength = abs(line.points[0].y - y);
        xLength > yLength ? line.dir = 1 : line.dir = 0;
    }
    if (line.dir == 0){
        line.points[line.points.size() - 1].y = y;
        line.points[line.points.size() - 1].x = line.points[0].x;
    }
    else {
        line.points[line.points.size() - 1].y = line.points[0].y;
        line.points[line.points.size() - 1].x = x; 
    }
}

void addPoint(int x, int y){
    y -= 28;
    if (line.points.size() == 0)
        line.points.push_back(point(x, y));
    else if (line.dir == 0)
        line.points.push_back(point(line.points[0].x, y));
    else
        line.points.push_back(point(x, line.points[0].y));
}

int max(int a, int b) { return a > b ? a : b; }
int min(int a, int b) { return a < b ? a : b; }
