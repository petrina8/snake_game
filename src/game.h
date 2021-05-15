#ifndef GAME_H
#define GAME_H

#include <random>
#include <memory>

#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "hud.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height, std::size_t screen_width);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScoreUser() const;
  int GetScoreEnemy() const;
  int GetSizeSnakeUser() const;

  void InitLevel();

 private:
  Snake snake_user;
  Snake snake_enemy;
  SDL_Point food;

  std::shared_ptr<HUD> _hud;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score_user{0};
  int score_enemy{0};
  int level{1};

  bool has_enemy{false};

  void PlaceFood();
  void Update();
};

#endif