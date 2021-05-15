#include <iostream>

#include "game.h"
#include "SDL.h"


const size_t hud_height = 40;
size_t size_tile = 128;

Game::Game(std::size_t grid_width, std::size_t grid_height, std::size_t screen_width)
    : snake_user(grid_width, grid_height),
      snake_enemy(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  
  _hud = std::make_shared<HUD>(screen_width, hud_height);

  PlaceFood();
}

void Game::InitLevel() {
  if (level == 1) {
    _hud->SetHasEnemy(false);
    _hud->SetSizeTile(size_tile);
  }
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  InitLevel();
  renderer.SetHud(_hud);

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake_user);
    Update();

    renderer.RenderLevel1(snake_user, food, score_user);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
    // Go to level 2
    if (score_user == 3) {
      level = 2;
      break;
    }
  }

  std::cout << "Go NExt Level!!" << std::endl;


}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);

    std::cout << "X: " << x << std::endl;
    std::cout << "Y: " << y << std::endl;

    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake_user.SnakeCell(x, y) 
        && !snake_enemy.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() {
  if (!snake_user.alive) return;

  snake_user.Update();

  int new_x = static_cast<int>(snake_user.head_x);
  int new_y = static_cast<int>(snake_user.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score_user++;
    PlaceFood();
    // Grow snake and increase speed.
    snake_user.GrowBody();
    snake_user.speed += 0.02;
  }
}

int Game::GetScoreUser() const { return score_user; }
int Game::GetScoreEnemy() const { return score_enemy; }
int Game::GetSizeSnakeUser() const { return snake_user.size; }