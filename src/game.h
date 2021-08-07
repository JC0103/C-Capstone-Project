#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

class Renderer;

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetScore2() const;
  int GetSize() const;
  int GetSize2() const;

  bool GetTwoPlayers() const;
  void SetTwoPlayers(bool two_players);
  void NoOfPlayersInput();

  void PrintResult();

 private:
  Snake snake;
  Snake snake2;
  SDL_Point food;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};
  int score2{0};

  void PlaceFood();
  void Update();

  void Snake1EatSnake2();
  void Snake2EatSnake1();

  bool _twoplayers{false};

};

#endif