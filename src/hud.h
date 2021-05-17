#ifndef HUD_H
#define HUD_H

#include <vector>

#include "SDL.h"


class HUD {
 public:
  HUD(const std::size_t frame_width, const std::size_t frame_height);
  HUD(){}

  // Getters/Setters
  void SetHasEnemy(bool has_enemy) {_has_enemy = has_enemy; }
  void SetSizeTile(size_t size_tile) {_size_tile = size_tile; }

  void UpdateHUDBackground(SDL_Renderer *sdl_renderer);
  void UpdateUserScore(SDL_Renderer *sdl_renderer, const int score);
  void UpdateEnemyScore(SDL_Renderer *sdl_renderer, const int score);


 private:
  SDL_Rect hud_frame;
  SDL_Rect user_score;
  SDL_Rect enemy_score;
  SDL_Rect separator_line;

  bool _has_enemy{false};
  size_t _size_tile{128};
};

#endif