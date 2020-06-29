#include "game.h"
#include <iostream>
#include "SDL.h"


Game::Game(std::size_t width, std::size_t height)
    : snake(width, height),
      eng(dev()),
      random_w(0, static_cast<int>(width)),
      random_h(0, static_cast<int>(height))
{
  PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration)
{
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running)
  {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000)
    {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration)
    {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::UpdateStartSpeed()
{
  snake.SetStartingSpeed();
}

void Game::PlaceFood()
{
  int x, y;
  while (true)
  {
    x = random_w(eng);
    y = random_h(eng);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y))
    {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update()
{
  if (!snake.alive)
    return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food at new location
  if (food.x == new_x && food.y == new_y)
  {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }


std::string Platform::getPlatform()
{
    const char *platform = SDL_GetPlatform();
    std::string p(platform);
    platformUsed = p;
    return platformUsed;
};

void Platform::printPlatform()
{
    if (platformUsed != "")
        std::cout << "Platform Detected: " << platformUsed << std::endl;
    else
        std::cout << "No System Platform Detected." << std::endl;
}