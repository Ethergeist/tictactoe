#include <ctype.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

enum { MAX_OUTPUT_HEIGHT = 12, MAX_OUTPUT_WIDTH = 14 };

typedef enum PLAYER_TYPE { HUMAN, COMPUTER } PLAYER_TYPE;

typedef struct Position {
  int x;
  int y;
} Position;

typedef struct LineStats {
  bool onlyX;
  bool onlyO;
  bool contiguous;
  int emptyPositions;
  Position empty[9];
} LineStats;

typedef struct TicTacToe {
  int size;
  PLAYER_TYPE X;
  PLAYER_TYPE O;
  wchar_t board[MAX_OUTPUT_HEIGHT][MAX_OUTPUT_WIDTH];
  int weights_X[MAX_OUTPUT_HEIGHT][MAX_OUTPUT_WIDTH];
  int weights_O[MAX_OUTPUT_HEIGHT][MAX_OUTPUT_WIDTH];

} TicTacToe;

static TicTacToe game;

char *getInput(char inputValidator[25], int inputLength, wchar_t prompt[120],
               wchar_t failurePrompt[120]) {
  regex_t validator;
  char *input = malloc(inputLength * sizeof(char));
  regcomp(&validator, inputValidator, 0);
  do {
    wprintf(L"%ls", prompt);
    fgets(input, inputLength, stdin);
    while (strchr(input, '\n') == NULL && getchar() != '\n')
      continue;
    if (regexec(&validator, input, 0, NULL, 0) == 0)
      break;
    wprintf(L"%ls", failurePrompt);
  } while (1);
  regfree(&validator);
  return input;
}

bool play() {
  char *choice =
      getInput("[Q|q|P|p]", 2, L"(P)lay game or (Q)uit? ",
               L"I don't understand, please P to play a game, or Q to quit.\n");
  bool result = tolower(choice[0]) == 'p';
  free(choice);
  return result;
}

void setGameOptions() {
  char *boardSize;
  char *player1;
  char *player2;
  game.size = 3;
  game.X = HUMAN;
  game.O = HUMAN;
  boardSize = getInput("[3|5|7|9]", 2,
                       L"Enter Board Size [3, 5, 7, 9]: ", L"Invalid board size.\n");

  game.size = atoi(boardSize);
  free(boardSize);
  player1 =
      getInput("[h|H|c|C]", 2, L"Is Player 1 a (H)uman or the (C)omputer? ",
               L"Invalid player type.\n");
  game.X = tolower(player1[0]) == L'h' ? HUMAN : COMPUTER;
  free(player1);
  player2 =
      getInput("[h|H|c|C]", 2, L"Is Player 2 a (H)uman or the (C)omputer? ",
               L"Invalid player type.\n");
  free(player2);
  game.O = tolower(player2[0]) == L'h' ? HUMAN : COMPUTER;
  wprintf(L"---Selected Game----\nBoard Size: %d\nPlayer 1: %ls\nPlayer 2: %ls\n--------------------\n",
         game.size, game.X == HUMAN ? L"Human" : L"Computer",
         game.O == HUMAN ? L"Human" : L"Computer");
}

void generateBoard() {
  int width = game.size + 3;
  int height = game.size + 2;
  for (int y = 0; y < MAX_OUTPUT_HEIGHT; y++) {
    for (int x = 0; x < MAX_OUTPUT_WIDTH; x++) {
      // Null terminate the strings
      if (x >= width || y >= height) {
        game.board[y][x] = '\0';
        continue;
      }
      // Make sure the strings end in newline
      if (x == width - 1) {
        game.board[y][x] = '\n';
        continue;
      }
      // Print X coordinates
      if (y == 0) {
        // Set value of X as character
        game.board[y][x] = (x - 1) + '0';
        // Corner space is empty
        if(x == 0) {
          game.board[y][x] = ' ';
        }
        // Vertical Separator
        if(x == 1) {
          game.board[y][x] = L'\u2502';
        }
        continue;
      }
      if (y == 1) {
        // Horizontal Separator
        game.board[y][x] = L'\u2500';
        // Vertical Separator
        if(x == 1) {
          game.board[y][x] = L'\u253C';
        }
        continue;
      }

      if (x == 0 && y > 1) {
        game.board[y][x] = (y - 1) + '0';
        continue;
      }

      if (x == 1) {
        game.board[y][x] = L'\u2502';
        continue;
      }
      game.board[y][x] = L'\u25AB';
    }
  }
}

void printBoard() {
  for (int y = 0; y < MAX_OUTPUT_HEIGHT; y++) {
    wprintf(L"%ls", game.board[y]);
  }
}

void calculateScore() {
  const int lineStart = 2;
  const int lineEnd = game.size + 2;
  // Rows
  for(int y = lineStart; y < lineEnd; y++) {
    LineStats stats = {
      .onlyX = true,
      .onlyO = true,
      .contiguous = false,
      .emptyPositions = 0,
      .empty = { NULL }
    };
    for(int x = lineStart; x < lineEnd; x++) {
      stats.onlyX = game.board[y][x] == 'X';
      stats.onlyO = game.board[y][x] == 'O';

      if(game.board)

    }
  }
  // Columns
  for(int x = lineStart; x < (game.size + 2); x++) {

  };
  // Diagonals
  for(int i = lineStart; i < lineEnd; i++) {

  }
  for(int i = lineEnd; i > lineStart; i++){

  }
}

void runGame() {
  printBoard();
  calculateScore();
}

int main(int argc, char **argv) {
  setlocale(LC_CTYPE, "");
  while (play()) {
    setGameOptions();
    generateBoard();
    runGame();
  };
  return EXIT_SUCCESS;
}
