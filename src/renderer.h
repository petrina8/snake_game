#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>
#include <SDL.h>
#include "snake.h"
#include "hud.h"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void SetHud(std::shared_ptr<HUD> hud) {
    _hud = hud;
  }


  void Render(Snake const snake, SDL_Point const &food, 
              int const &score_user, int const &score_enemy);
  void UpdateWindowTitle(int score, int fps);
  void RenderLevel1(Snake const snake, SDL_Point const &food, 
                    int const &score_user);
  void RenderLevel2(Snake const snake, SDL_Point const &food, int const &score_user, 
                    const std::vector<std::shared_ptr<SDL_Point>> obstacles);

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  SDL_Rect _gameplay;
  std::shared_ptr<HUD> _hud;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;

  void RenderFood(SDL_Point const &food, SDL_Rect block);
  void RenderSnake(Snake const snake, SDL_Rect block);

};

#endif