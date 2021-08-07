#include "game.h"
#include <iostream>
#include <string>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      snake2(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  NoOfPlayersInput();
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

  while (running) {
    frame_start = SDL_GetTicks();
    // Input, Update, Render - the main game loop.
    if(_twoplayers == true){
      controller.HandleInput(running, snake, snake2);
    }
    else{
      controller.HandleInput(running, snake);
    }
    Update();
    renderer.Render(snake, snake2, food, this);
    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, score2, frame_count, this);
      frame_count = 0;
      title_timestamp = frame_end;
      if((!snake.alive) || (!snake2.alive)){break;}
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y) && !snake2.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() {
  if ((!snake.alive) || (!snake2.alive)) {return;}

  snake.Update();
  snake2.Update();
  if (_twoplayers == true){
    
    Snake1EatSnake2();
    Snake2EatSnake1();
  }

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);
  int new_x2 = static_cast<int>(snake2.head_x);
  int new_y2 = static_cast<int>(snake2.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.01;
  }

  if (food.x == new_x2 && food.y == new_y2){
    score2++;
    PlaceFood();
    snake2.GrowBody();
    snake2.speed += 0.01;
  }
}

void Game::Snake1EatSnake2(){
  for (auto const &item : snake2.body) {
    if (snake.CaptureHeadCell().x == item.x && snake.CaptureHeadCell().y == item.y) {
    snake.alive = false;
    score -= 5;
    }
  }
}

void Game::Snake2EatSnake1(){
  for (auto const &item : snake.body) {
    if (snake2.CaptureHeadCell().x == item.x && snake2.CaptureHeadCell().y == item.y) {
    snake.alive = false;
    score2 -= 5;
    }
  }
}


int Game::GetScore() const { return score; }
int Game::GetScore2() const { return score2; }
int Game::GetSize() const { return snake.size; }
int Game::GetSize2() const { return snake2.size; }

bool Game::GetTwoPlayers() const {return _twoplayers;}

void Game::SetTwoPlayers(bool two_players) {_twoplayers = two_players;}

void Game::NoOfPlayersInput(){
  std::string s;
  std::cout << "One Player or Two Players?\n";
  std::cin >> s;
  while(true){
    if( s == "one" || s == "One" || s == "ONE" || s == "1"){
      SetTwoPlayers(false);
      break;
    }
    else if( s == "two" || s == "Two" || s == "TWO" || s == "2"){
      SetTwoPlayers(true);
      break;
    }
    else {
      std::cout << "Invalid Input Values, please input '1' / 'one' or '2' / 'two'" << std::endl;
      std::cin >> s;
    }
  }
}

void Game::PrintResult(){
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Player 1 Score: " << GetScore() << "\n";
  std::cout << "Player 1 Size: " << GetSize() << "\n";
  if (_twoplayers == true){
    std::cout << "Player 2 Score: " << GetScore2() << "\n";
    std::cout << "Player 2 Size: " << GetSize2() << "\n";
    if (GetScore() > GetScore2()){std::cout << "Player 1 Wins!\n";}
    else if (GetScore() < GetScore2()){std::cout << "Player 2 Wins!\n";}
    else {std::cout << "Draw!\n";}
  }
}