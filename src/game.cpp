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


void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  ResetLevel();
  renderer.SetHud(_hud);


/*  while (running) {
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

*/

  score_user = 0;
  snake_user.ResetSnake();
  PlaceObstacles();

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake_user);
    CheckObstacleCollision();
    Update();

    renderer.RenderLevel2(snake_user, food, score_user, obstacles);

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
    if (score_user == 10) {
      level = 3;
      break;
    }

  }

  std::cout << "Go NExt Level!!" << std::endl;
}

void Game::ResetLevel() {
  if (level == 1) {
    _hud->SetHasEnemy(false);
    _hud->SetSizeTile(size_tile);
  } else if (level == 2) {
    
  }
}

//void Game::PlayLevel1() {}

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

// Check if cell is occupied by an obstacle.
bool Game::ObstaclesCell(int x, int y) {
  for (auto obstacle : obstacles) {
    if (x == (*obstacle.get()).x && y == (*obstacle.get()).y) {
      return true;
    }
  }
  return false;
}

// Place 12 obstacles on level2
void Game::PlaceObstacles() {
  int x, y, i{1};
  obstacles.clear();

  while (i <= 12) {
    x = random_w(engine);
    y = random_h(engine);

    if (!snake_user.SnakeCell(x, y) 
        && !snake_enemy.SnakeCell(x, y)
        && !ObstaclesCell(x, y)) {

      std::shared_ptr<SDL_Point> point = std::make_shared<SDL_Point>();
      point->x = x;
      point->y = y;

      obstacles.emplace_back(point);
      i++;
    }
  }
}

// Snake dies if its head collide to obstacle
void Game::CheckObstacleCollision() {
  // Check if the snake has died.
  for (auto obstacle : obstacles) {
    std::cout << "snake_user.headX:: " << snake_user.head_x << std::endl;
    std::cout << "obstacleX:: " << obstacle->x << std::endl;

    std::cout << "snake_user.headY:: " << snake_user.head_y << std::endl;
    std::cout << "obstacleY:: " << obstacle->y << std::endl;

    if (static_cast<int>(snake_user.head_x) == obstacle->x && 
        static_cast<int>(snake_user.head_y) == obstacle->y) {
      snake_user.alive = false;
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
    PlaceObstacles();
    // Grow snake and increase speed.
    snake_user.GrowBody();
    snake_user.speed += 0.02;
  }

}

int Game::GetScoreUser() const { return score_user; }
int Game::GetScoreEnemy() const { return score_enemy; }
int Game::GetSizeSnakeUser() const { return snake_user.size; }
