#include "renderer.h"

#include <iostream>
#include <string>
#include <SDL_ttf.h>

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

  if (sdl_window == nullptr) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (sdl_renderer == nullptr) { 
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // create gameplay area
  _gameplay.x = 0;
  _gameplay.y = hud_height;
  _gameplay.w = screen_width;
  _gameplay.h = screen_height - hud_height;

  //Initialize JPG loading
  imgFlags = IMG_INIT_PNG;
  if (!( IMG_Init( imgFlags ) & imgFlags )) {
      std::cerr <<  "SDL_image could not initialize! SDL_image Error\n" << IMG_GetError() << "\n";
  } else {
    //Get window surface
    image_surface = SDL_GetWindowSurface(sdl_window);
  }
}

Renderer::~Renderer() {
  SDL_DestroyTexture(sdl_texture);
  SDL_FreeSurface(image_surface);
  SDL_DestroyRenderer(sdl_renderer);
  SDL_DestroyWindow(sdl_window);
  IMG_Quit();
  SDL_Quit();
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}

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

// Level with obstacles
void Renderer::RenderLevel2(Snake const snake, SDL_Point const &food, int const &score_user, 
                            const std::vector<std::shared_ptr<SDL_Point>> obstacles) {
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

  // Render purple obstacles
  SDL_SetRenderDrawColor(sdl_renderer, 0x80, 0x00, 0x80, 0xFF);
  for (auto obstacle : obstacles) {
    block.x = obstacle->x * block.w;
    block.y = obstacle->y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

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

void Renderer::DrawImage(const char *image_path) {
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  SDL_Rect m_image_position;
  m_image_position.x = 0;
  m_image_position.y = 0;
  m_image_position.w = 640;
  m_image_position.h = 480;
  
  image_surface = IMG_Load(image_path);
  sdl_texture = SDL_CreateTextureFromSurface(sdl_renderer, image_surface);

  if (!image_surface) {
    std::cout << "Fail to load image!" << std::endl;
    return;
  } else {
    SDL_RenderCopy(sdl_renderer, sdl_texture, NULL, NULL);
    SDL_RenderPresent(sdl_renderer);
    SDL_Delay(2000);
  }
}
