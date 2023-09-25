#include "tictactoe.h"
#include <stdint.h>

void generateBoard() {
  for (uint32_t y = 0; y < TTT_MAX_BOARD_SIZE; y++) {
    for (uint32_t x = 0; x < TTT_MAX_BOARD_SIZE; x++) {
      if (x >= game.size || y == game.size) {
        game.board[y][x] = '\0';
        continue;
      }
      game.board[y][x] = EMPTY_CHAR;
    }
  }
  generateLines();
}

void generateLines() {
  uint32_t numLines = 0;
  // Rows
  for (uint32_t y = 0; y < game.size; y++) {
    game.lines[numLines] =
        (TTT_Line){.numEmpty = game.size, .numO = 0, .numX = 0};
    for (uint32_t x = 0; x < game.size; x++) {
      game.lines[numLines].empty[x] = (TTT_Position){.y = y, .x = x};
      game.lines[numLines].positions[x] = (TTT_Position){.y = y, .x = x};
    }
    numLines++;
  }
  // Columns
  for (uint32_t x = 0; x < game.size; x++) {
    game.lines[numLines] =
        (TTT_Line){.numEmpty = game.size, .numO = 0, .numX = 0};
    for (uint32_t y = 0; y < game.size; y++) {
      game.lines[numLines].empty[y] = (TTT_Position){.y = y, .x = x};
      game.lines[numLines].positions[y] = (TTT_Position){.y = y, .x = x};
    }
    numLines++;
  }
  // Diagonals
  TTT_Line topLeftToBottomRight = {.numEmpty = game.size, .numO = 0, .numX = 0};
  TTT_Line bottomLeftToTopRight = {.numEmpty = game.size, .numO = 0, .numX = 0};
  for (uint32_t i = 0; i < game.size; i++) {
    topLeftToBottomRight.empty[i] = (TTT_Position){.y = i, .x = i};
    topLeftToBottomRight.positions[i] = (TTT_Position){.y = i, .x = i};
    bottomLeftToTopRight.empty[i] = (TTT_Position){.y = (game.size - 1) - i, .x = i};
    bottomLeftToTopRight.positions[i] = (TTT_Position){.y = (game.size -1) - i, .x = i};
  }
  game.lines[numLines++] = topLeftToBottomRight;
  game.lines[numLines] = bottomLeftToTopRight;
}

void generateWeights() {
  uint32_t middle = (game.size / 2);
  game.players[0].weights[0][0] = 5;
  game.players[1].weights[0][0] = 5;
  game.players[0].weights[0][game.size - 1] = 5;
  game.players[1].weights[0][game.size - 1] = 5;
  game.players[0].weights[game.size - 1][0] = 5;
  game.players[1].weights[game.size - 1][0] = 5;
  game.players[0].weights[game.size - 1][game.size - 1] = 5;
  game.players[1].weights[game.size - 1][game.size - 1] = 5;
  game.players[0].weights[middle][middle] = 5;
  game.players[1].weights[middle][middle] = 5;
  for (uint32_t ln = 0; ln < game.size + game.size + 2; ln++) {
    TTT_Line line = game.lines[ln];
    for(uint32_t p = 0; p < game.size; p++) {
      TTT_Position pos = line.positions[p];
      if(game.board[pos.y][pos.x] != EMPTY_CHAR) {
        game.players[0].weights[pos.y][pos.x] = 0;
        game.players[1].weights[pos.y][pos.x] = 0;
      }
    }
    if (line.numEmpty == 1) {
      if (line.numX == (game.size - 1)) {
        game.players[0].weights[line.empty[0].y][line.empty[0].x] = 27;
        game.players[1].weights[line.empty[0].y][line.empty[0].x] = 20;
      }
      if (line.numO == (game.size - 1)) {
        game.players[0].weights[line.empty[0].y][line.empty[0].x] = 20;
        game.players[1].weights[line.empty[0].y][line.empty[0].x] = 27;
      }
    }
    if (line.numX == 0 || line.numO == 0) {
      for (uint32_t ep = 0; ep < line.numEmpty; ep++) {
        TTT_Position emptyPos = line.empty[ep];
        if (line.numX > 0) {
          game.players[0].weights[emptyPos.y][emptyPos.x] += line.numX;
          game.players[1].weights[emptyPos.y][emptyPos.x] += line.numX * 2;
        }
        if (line.numO > 0) {
          game.players[1].weights[emptyPos.y][emptyPos.x] += line.numO;
          game.players[0].weights[emptyPos.y][emptyPos.x] += line.numO * 2;
        }
      }
    }
  }
}

void getComputerInput(uint32_t player, uint32_t round) {
  generateWeights();
  TTT_Position playerMove = {.x = 0, .y = 0};
  wprintf(L"Thinking...\n");
  uint32_t weight = 0;
  for (int y = 0; y < game.size; y++) {
    for (int x = 0; x < game.size; x++) {
      if (game.board[y][x] == EMPTY_CHAR) {
        if ((game.board[playerMove.y][playerMove.x] != EMPTY_CHAR) ||
            (game.players[player].weights[y][x] > weight) ||
            (game.players[player].weights[y][x] == weight && (rand() % 2))) {
          playerMove.x = x;
          playerMove.y = y;
          weight = game.players[player].weights[y][x];
        }
      }
    }
  }
  game.board[playerMove.y][playerMove.x] = game.players[player].character;
  wprintf(L"%lc chose %d, %d.\n", game.players[player].character, playerMove.y + 1,
          playerMove.x + 1);
}

void getHumanInput(uint32_t player) {
  TTT_Position playerMove;
  while (true) {
    char *playerInput;
    playerInput = getInput("[0-9],[0-9]", 4, L"Enter your move (y,x): ",
                           L"Invalid move, please enter as \"y,x\".\n");
    playerMove = (TTT_Position){.y = atoi((char[2]){playerInput[0] - 1, '\0'}),
                                .x = atoi((char[2]){playerInput[2] - 1, '\0'})};
    free(playerInput);
    if (game.board[playerMove.y][playerMove.x] != EMPTY_CHAR) {
      wprintf(L"Sorry, the space at %lc,%lc is already taken. Please choose "
              L"another.\n",
              playerMove.y + '0', playerMove.x + '0');
      continue;
    }
    game.board[playerMove.y][playerMove.x] = game.players[player].character;
    wprintf(L"%lc chose %d, %d.\n", game.players[player].character,
            playerMove.y + 1, playerMove.x + 1);
    break;
  }
}

char *getInput(char regexString[25], uint32_t length, wchar_t prompt[120],
               wchar_t failurePrompt[120]) {
  regex_t validator;
  char *input = malloc(length * sizeof(char));
  regcomp(&validator, regexString, 0);
  do {
    wprintf(L"%ls", prompt);
    fgets(input, length, stdin);
    while (strchr(input, '\n') == NULL && getchar() != '\n')
      continue;
    if (regexec(&validator, input, 0, NULL, 0) == 0)
      break;
    wprintf(L"%ls", failurePrompt);
  } while (1);
  regfree(&validator);
  return input;
}

TTT_GAME_STATUS getUpdateGameStatus() {
  uint32_t unwinnableLines = 0;
  for (uint32_t ln = 0; ln < game.size + game.size + 2; ln++) {
    game.lines[ln].numX = 0;
    game.lines[ln].numO = 0;
    game.lines[ln].numEmpty = 0;
    memset(&game.lines[ln].empty, 0, sizeof(game.lines[ln].empty));
    for (uint32_t pn = 0; pn < game.size; pn++) {
      TTT_Position pos = game.lines[ln].positions[pn];
      if (game.board[pos.y][pos.x] == X_CHAR) {
        game.lines[ln].numX++;
        continue;
      }
      if (game.board[pos.y][pos.x] == O_CHAR) {
        game.lines[ln].numO++;
        continue;
      }
      game.lines[ln].empty[game.lines[ln].numEmpty] = pos;
      game.lines[ln].numEmpty++;
    }
    if (game.lines[ln].numX == game.size) {
      return X_WINS;
    }
    if (game.lines[ln].numO == game.size) {
      return O_WINS;
    }
    if (game.lines[ln].numX > 0 && game.lines[ln].numO > 0) {
      unwinnableLines++;
    }
  }
  if (unwinnableLines == (game.size * 2) + 2) {
    return DRAW;
  }
  return IN_PLAY;
}

void printBoard() {
  wchar_t x_coordinates[game.size + 1];
  wchar_t x_border[game.size + 1];
  for (int x = 0; x < game.size; x++) {
    if (x == game.size + 1) {
      x_coordinates[x] = '\0';
      x_border[x] = '\0';
    }
    x_coordinates[x] = (x + 1) + L'\uFF10';
    x_border[x] = HORIZONTAL_SEPARATOR_CHAR;
  }
  x_coordinates[game.size] = '\0';
  x_border[game.size] = '\0';
  wprintf(L"  %ls\n", x_coordinates);
  for (int y = 0; y < game.size; y++) {
    wprintf(L"%lc%ls\n", (y + 1) + L'\uFF10', game.board[y]);
  }
}

void printOptions() {
  wprintf(L"---Selected Game----\nBoard Size: %d\nPlayer 1: %ls\nPlayer 2: "
          L"%ls\n--------------------\n",
          game.size, game.players[0].type == HUMAN ? L"Human" : L"Computer",
          game.players[1].type == HUMAN ? L"Human" : L"Computer");
}

void runGame() {
  uint32_t currentPlayer = rand() % 2;
  uint32_t round = 1;
  TTT_GAME_STATUS status = IN_PLAY;
  generateBoard();
  printBoard();
  while (status == IN_PLAY) {
    wprintf(L"%lc's Turn,\n", game.players[currentPlayer].character);
    switch (game.players[currentPlayer].type) {
    case HUMAN:
      getHumanInput(currentPlayer);
      break;
    case COMPUTER:
      getComputerInput(currentPlayer, round);
      break;
    }
    printBoard();
    status = getUpdateGameStatus();
    if (status == IN_PLAY) {
      currentPlayer = currentPlayer == 0 ? 1 : 0;
      round++;
    }
  }
  switch (status) {
  case X_WINS:
  case O_WINS:
    wprintf(L"%lc Wins!\n", game.players[currentPlayer].character);
    break;
  case DRAW:
    wprintf(L"Game was a Draw.\n");
    break;
  default:
    wprintf(L"You shouldn't be here!\n");
  }
}

void setOptions() {
  char *boardSizeInput;
  char *player1Input;
  char *player2Input;

  boardSizeInput = getInput("[3|5|7|9]", 2, L"Enter Board Size [3, 5, 7, 9]: ",
                            L"Invalid board size.\n");

  game.size = atoi(boardSizeInput);
  free(boardSizeInput);
  player1Input =
      getInput("[h|H|c|C]", 2, L"Is Player 1 a (H)uman or the (C)omputer? ",
               L"Invalid player type.\n");
  game.players[0].type = tolower(player1Input[0]) == L'h' ? HUMAN : COMPUTER;
  free(player1Input);
  player2Input =
      getInput("[h|H|c|C]", 2, L"Is Player 2 a (H)uman or the (C)omputer? ",
               L"Invalid player type.\n");
  game.players[1].type = tolower(player2Input[0]) == L'h' ? HUMAN : COMPUTER;
  free(player2Input);
  printOptions();
}

char showMenu() {
  char *choice =
      getInput("[Q|q|o|O|P|p]", 2, L"(P)lay game, (O)ptions, or (Q)uit? ",
               L"I don't understand, please P to play a game, O for options, "
               L"or Q to quit.\n");
  char result = tolower(choice[0]);
  free(choice);
  return result;
}

uint32_t tictactoe() {
  setlocale(LC_CTYPE, "");
  game.size = 3;
  game.players[0] = (TTT_Player){.character = X_CHAR, .type = HUMAN};
  game.players[1] = (TTT_Player){.character = O_CHAR, .type = COMPUTER};
  char menuChoice = showMenu();
  while (menuChoice != 'q') {
    switch (menuChoice) {
    case 'p':
      runGame();
      break;
    case 'o':
      setOptions();
      break;
    }
    menuChoice = showMenu();
  }
  return EXIT_SUCCESS;
};
