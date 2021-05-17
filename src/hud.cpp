#include "hud.h"

#include <iostream>
#include <string>

const size_t line_size = 5;

HUD::HUD(const std::size_t frame_width, const std::size_t frame_height) {

    hud_frame.x = 0;
    hud_frame.y = 0;
    hud_frame.w = frame_width;
    hud_frame.h = frame_height;

    user_score.x = hud_frame.x;
    user_score.y = hud_frame.y;
    user_score.w = 1;
    user_score.h = hud_frame.h;
}

void HUD::UpdateHUDBackground(SDL_Renderer *sdl_renderer) {
  // Render frame HUD - white background
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderSetViewport(sdl_renderer, &hud_frame);
  SDL_RenderFillRect(sdl_renderer, &hud_frame);

  if (_has_enemy) {
    // Render a line separator between two score bars
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
    separator_line.x = hud_frame.w/2;
    separator_line.y = 0;
    separator_line.w = line_size;
    separator_line.h = hud_frame.h;
    SDL_RenderFillRect(sdl_renderer, &separator_line);
  }
}


void HUD::UpdateUserScore(SDL_Renderer *sdl_renderer, const int score) {
  // Render frame HUD - white background
  UpdateHUDBackground(sdl_renderer);

  // Render score bar
  user_score.w = score*_size_tile;
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0x00, 0xFF);
  SDL_RenderFillRect(sdl_renderer, &user_score);
  
  SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  SDL_RenderDrawRect(sdl_renderer, &user_score);
}

void HUD::UpdateEnemyScore(SDL_Renderer *sdl_renderer, const int score) {
}


