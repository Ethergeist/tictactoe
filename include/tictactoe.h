#ifndef TICTACTOE_H_
#define TICTACTOE_H_
#include <ctype.h>
#include <locale.h>
#include <regex.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

typedef enum TTT_BOARD_SIZE {
  TTT_MAX_BOARD_SIZE = 9,
  TTT_MAX_BOARD_WIDTH = 10
} TTT_BOARD_SIZE;
typedef enum TTT_CHARS {
  EMPTY_CHAR = L'\u25AB',
  X_CHAR = 'X',
  O_CHAR = 'O',
  CROSS_SEPARATOR_CHAR = L'\u253C',
  HORIZONTAL_SEPARATOR_CHAR = L'\u2500',
  VERTICAL_SEPARATOR_CHAR = L'\u2502'
} TTT_CHARS;
typedef enum TTT_GAME_STATUS { IN_PLAY, X_WINS, O_WINS, DRAW } TTT_GAME_STATUS;
typedef enum TTT_PLAYER_TYPE { HUMAN, COMPUTER } TTT_PLAYER_TYPE;

typedef struct TTT_Player {
  wchar_t character;
  TTT_PLAYER_TYPE type;
  int weights[TTT_MAX_BOARD_SIZE][TTT_MAX_BOARD_SIZE];
} TTT_Player;

typedef struct TTT_Position {
  uint32_t y;
  uint32_t x;
} TTT_Position;

typedef struct TTT_Line {
  uint32_t numX;
  uint32_t numO;
  uint32_t numEmpty;
  TTT_Position positions[TTT_MAX_BOARD_SIZE];
  TTT_Position empty[TTT_MAX_BOARD_SIZE];
} TTT_Line;

typedef struct TTT_Game {
  uint32_t size;
  wchar_t board[TTT_MAX_BOARD_SIZE][TTT_MAX_BOARD_WIDTH];
  TTT_Player players[2];
  TTT_Line lines[TTT_MAX_BOARD_SIZE + TTT_MAX_BOARD_SIZE + 2];
} TTT_Game;

static TTT_Game game;

void generateBoard();
void generateLines();
void generateWeights();
void getComputerInput(uint32_t player, uint32_t round);
void getHumanInput(uint32_t player);
char *getInput(char regexString[25], uint32_t length, wchar_t prompt[120], wchar_t failurePrompt[120]);
TTT_GAME_STATUS getUpdateGameStatus();
void printBoard();
void printOptions();
void runGame();
void setOptions();
char showMenu();

uint32_t tictactoe();
#endif // TICTACTOE_H_
