#include "tictactoe.h"
#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


const int MAX_OUTPUT_HEIGHT = 12;
const int MAX_OUTPUT_WIDTH = 14;
typedef enum PLAYER_TYPE { HUMAN, COMPUTER } PLAYER_TYPE;
typedef struct TicTacToe {
  int size;
  PLAYER_TYPE X;
  PLAYER_TYPE O;
  char board[MAX_OUTPUT_HEIGHT][MAX_OUTPUT_WIDTH];
  int weights[MAX_OUTPUT_HEIGHT][MAX_OUTPUT_WIDTH];
} TicTacToe;


static TicTacToe game;

char *getInput(char inputValidator[25], int inputLength, char prompt[120],
               char failurePrompt[120]) {
  regex_t validator;
  char *input = malloc(inputLength * sizeof(char));
  regcomp(&validator, inputValidator, 0);
  do {
    printf("%s", prompt);
    fgets(input, inputLength, stdin);
    while (strchr(input, '\n') == NULL && getchar() != '\n')
      continue;
    if (regexec(&validator, input, 0, NULL, 0) == 0)
      break;
    printf("%s", failurePrompt);
  } while (1);
  regfree(&validator);
  return input;
}

int play() {
  char *choice =
      getInput("[Q|q|P|p]", 2, "(P)lay game or (Q)uit? ",
               "I don't understand, please P to play a game, or Q to quit.\n");
  int result = 0;
  if (tolower(choice[0]) == 'p')
    result = 1;
  free(choice);
  return result;
}

void getGameSettings() {
  char *boardSize;
  char *player1;
  char *player2;
  game.size = 3;
  game.X = HUMAN;
  game.O = HUMAN;
  boardSize = getInput("[3|4|5|6|7|8|9|10]", 2, "Enter Board Size 3-10: ",
                   "Invalid board size.\n");

  game.size = atoi(boardSize);
  free(boardSize);
  player1 =
      getInput("[h|H|c|C]", 2, "Is Player 1 a (H)uman or the (C)omputer? ",
               "Invalid player type.\n");
  game.X = tolower(player1[0]) == 'h' ? HUMAN : COMPUTER;
  free(player1);
  player2 =
      getInput("[h|H|c|C]", 2, "Is Player 2 a (H)uman or the (C)omputer? ",
               "Invalid player type.\n");
  free(player2);
  game.O = tolower(player2[0]) == 'h' ? HUMAN : COMPUTER;
  printf("---Selected Game----\nBoard Size: %d\nPlayer 1: %s\nPlayer 2: %s\n",
         game.size, game.X == HUMAN ? "Human" : "Computer",
         game.O == HUMAN ? "Human" : "Computer");
}

void generateBoard() {
  int width = game.size + 3;
  int height = game.size + 2;
  for (int y = 0; y < MAX_OUTPUT_HEIGHT; y++) {
    for(int x = 0; x < MAX_OUTPUT_WIDTH; x++) {
      if(x >= width || y >= height) {
        game.board[y][x] = '\0';
        continue;
      }
      if(x == width -1) {
        game.board[y][x] = '\n';
        continue;
      }
      if(y == 1) {
       game.board[y][x] = '=';
       continue;
      }
      if(y == 0) {
        if(x == 0 || x == 1) {
          game.board[y][x] = ' ';
        } else {
          if(x > 1) {
            game.board[y][x] = (x-1) + '0';
          }
        }
        continue;
      }
      if(x == 0) {
        game.board[y][x] = (y-1) + '0';
        continue;
      }

      if(y > 1 && x == 1) {
        game.board[y][x] = '|';
        continue;
      }
      game.board[y][x] = '.';
    }
  }
}

void printBoard() {
  for (int y = 0; y < MAX_OUTPUT_HEIGHT; y++) {
    printf("%s", game.board[y]);
  }
  printf("\n");
}

void runGame() {
  generateBoard();
  printBoard();
}

int main(int argc, char **argv) {
  while (play()) {
    getGameSettings();
    runGame();
  };
  return EXIT_SUCCESS;
}
