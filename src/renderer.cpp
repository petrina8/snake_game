#include "renderer.h"
#include <iostream>
#include <string>

const size_t hud_height = 40;

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, 
                   const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) { 
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // create gameplay area
  _gameplay.x = 0;
  _gameplay.y = hud_height;
  _gameplay.w = screen_width;
  _gameplay.h = screen_height - hud_height;

}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}


void Renderer::Render(Snake const snake, SDL_Point const &food, 
                      int const &score_user, int const &score_enemy) {
  SDL_Rect block;
  block.w = _gameplay.w / grid_width;
  block.h = _gameplay.h / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  _hud->UpdateUserScore(sdl_renderer, score_user);
  //hud.UpdateEnemyScore(sdl_renderer, score);

  /*SDL_Rect myRect;
  SDL_RenderGetViewport(sdl_renderer, &myRect);

  std::cout << "myRect.X: " << myRect.x << std::endl;
  std::cout << "myRect.Y: " << myRect.y << std::endl;
  std::cout << "myRect.w: " << myRect.w << std::endl;
  std::cout << "myRect.h: " << myRect.h << std::endl;
  */

  // Render gameplay
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0xFF, 0x00);
  SDL_RenderSetViewport(sdl_renderer, &_gameplay);

  RenderFood(food, block);
  RenderSnake(snake, block);
  // Render snake's body
  /*SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);
*/
  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

/*void Renderer::RenderNextLevel(Snake const snake, SDL_Point const &food, 
                               int const &score_user, int const &level) {
                      
}
*/

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}


// level with obstacles
void Renderer::RenderLevel1(Snake const snake, SDL_Point const &food, 
                            int const &score_user) {
  SDL_Rect block;
  block.w = _gameplay.w / grid_width;
  block.h = _gameplay.h / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  _hud->UpdateUserScore(sdl_renderer, score_user);

  // Render gameplay
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0xFF, 0x00);
  SDL_RenderSetViewport(sdl_renderer, &_gameplay);

  RenderFood(food, block);
  RenderSnake(snake, block);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);  
}



void Renderer::RenderFood(SDL_Point const &food, SDL_Rect block) {
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);
}


void Renderer::RenderSnake(Snake const snake, SDL_Rect block) {
  // Render snake's body
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }
  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);
}
