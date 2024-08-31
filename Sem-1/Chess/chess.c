#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function displays the board
// Note x increase in the right and y increase downwards
void displayBoard(char *arrangement[8][8], int turn) {
  system("clear");
  printf("CONSOLE CHESS GAME\n");
  printf("ENTER STANDARD LONG ALGREBRAIC CHESS NOTATION \nENTER exit TO EXIT");

  printf("\n┼");
  for (int j = 0; j<8; j++) {
    printf("────┼");
  }
  // Displays the board iteratively in chunks/pairs of two
  for (int i = 0; i<4; i++) {
    printf("\n┼"); // n = 2i
    for (int j = 0; j<4; j++) {
      printf("\033[48;5;245m  %s \033[0m┼", arrangement[2*i][2*j]); // n = 2j
      printf("\033[48;5;232m  %s \033[0m┼", arrangement[2*i][2*j+1]); // n = 2j+1
    }
    printf(" %d\n┼", 8-(2*i));
    for (int j = 0; j<8; j++) {
      printf("────┼");
    }


    printf("\n┼"); // n = 2i+1
    for (int j = 0; j<4; j++) {
      printf("\033[48;5;232m  %s \033[0m┼", arrangement[2*i+1][2*j]); // n = 2j
      printf("\033[48;5;245m  %s \033[0m┼", arrangement[2*i+1][2*j+1]); // n = 2j+1
    }
    printf(" %d\n┼", 8-(2*i)-1);
    for (int j = 0; j<8; j++) {
      printf("────┼");
    }
  }

  printf("\n┼──A─┼──B─┼──C─┼──D─┼──E─┼──F─┼──G─┼──H─┼");
  turn ? printf("\nBlack to move") : printf("\nWhite to move");
  printf("\n");
}

// Functions checks whether a piece exists on the block. Mainly used to make sure the path
// the piece need to travel are empty. Returns 0 if empty and 1 if occupied
int pieceExists(char* toCheck) {
  char* pieces[12] = {"♔", "♕", "♗", "♘", "♖", "♙", "♚", "♛", "♝", "♞", "♜", "♟"};

  for (int i=0; i<12; i++) {
    if (toCheck == pieces[i]) {
      return 1;
    }
  }
  return 0;
}

// Functions checks whether the player will capture a piece or not.
// Used to verify pawn diagonal movement and other piece implementation
int checkPieceCapture(char* toCheck, int turn) {
  // return points based on the piece
  // returns 10 for king, 11 for empty spot and 13 if fails all conditions
  switch (turn) {
    case 1: // Black
      if (toCheck == "♚") {
        return 10; // give check to opp
      }
      else if (toCheck ==  "♛") {
        return 9;
      }
      else if (toCheck ==  "♝") {
        return 3;
      }
      else if (toCheck ==  "♞") {
        return 3;
      }
      else if (toCheck ==  "♜") {
        return 5;
      }
      else if (toCheck ==  "♟") {
        return 1;
      }
      else if (toCheck == " "){
        return 11;
      }
      break;
    case 0:
      if (toCheck == "♔") {
        return 10; // give check to opp
      }
      else if (toCheck ==  "♕") {
        return 9;
      }
      else if (toCheck ==  "♗") {
        return 3;
      }
      else if (toCheck ==  "♘") {
        return 3;
      }
      else if (toCheck ==  "♖") {
        return 5;
      }
      else if (toCheck ==  "♙") {
        return 1;
      }
      else if (toCheck == " "){
        return 11;
      }
      break;
  }
  return 13;
}


// Function checks the validty of the move given by the user.
// Empty paths and check and checkmates are todo.
int checkValidity(char* arrangement[8][8], char input[10], int turn, char* pieces[2][6]) {
  char piece = input[0];
  int initPosn[2] = {7-(input[2]-49), input[1]-97}; // {row, coulumn}
  int finalPosn[2] = {7-(input[4]-49), input[3]-97}; // {row, coulumn}

  // Rook movement is defined as having same x or y coordinate
  // Diagonal movement is defined as having the same difference between initX & finalX and initY & finalY
  // Knight movement is L shaped
  // King movment identical to Queen but one space only

  switch (piece) {
    case 'K':
      if ((turn == 0 && arrangement[initPosn[0]][initPosn[1]] == "♚") || (turn == 1 && arrangement[initPosn[0]][initPosn[1]] == "♔")) {
        // Vertical and Horizontal movement
        if (initPosn[0] == finalPosn[0] || initPosn[1] == finalPosn[1]) {
          if (abs(initPosn[0] - finalPosn[0]) == 1 || abs(initPosn[1] - finalPosn[1]) == 1) {
            if (checkPieceCapture(arrangement[finalPosn[0]][finalPosn[1]], turn) < 12){
              return 1;
            }
            else {
              return 0;
            }
          }
        }
        // Diagonal Movement
        else if (abs(initPosn[0] - finalPosn[0]) == abs(initPosn[1] - finalPosn[1]) == 1) {
          if (checkPieceCapture(arrangement[finalPosn[0]][finalPosn[1]], turn) < 12){
            return 1;
          }
          else {
            return 0;
          }
        }
      }
      break;
    case 'Q':
      if ((turn == 0 && arrangement[initPosn[0]][initPosn[1]] == "♛") || (turn == 1 && arrangement[initPosn[0]][initPosn[1]] == "♕")) {
        // If Queen  moves horizontally, check if path is empty or not
        if (initPosn[0] == finalPosn[0]) { // same row
          if (finalPosn[1] > initPosn[1]) { // if we move right
            for (int i = 1; i < finalPosn[1]-initPosn[1]-1; i++) {
              if (pieceExists(arrangement[finalPosn[0]][initPosn[1]+i])) {
                //printf("%s\n", arrangement[finalPosn[0]][initPosn[1]+i]);
                return 0;
              }
            }

            if (checkPieceCapture(arrangement[finalPosn[0]][finalPosn[1]], turn) < 12){
              return 1;
            }
            else {
              return 0;
            }
          }
          else if (finalPosn[1] < initPosn[1]) { // if we move left
            for (int i = 1; i < initPosn[1] - finalPosn[1]; i++) {
              if (pieceExists(arrangement[finalPosn[0]][finalPosn[1]+i])) {
                return 0;
              }
            }
            if (checkPieceCapture(arrangement[finalPosn[0]][finalPosn[1]], turn) < 12){
              return 1;
            }
            else {
              return 0;
            }
          }
        }
        // If queen moves veritcally, check if path is empty or not
        else if (initPosn[1] == finalPosn[1]) { // same coloumn
          if (finalPosn[0] > initPosn[0]) { // if we move down
            for (int i = 1; i < finalPosn[0]-initPosn[0]-1; i++) {
              if (pieceExists(arrangement[initPosn[0]+i][initPosn[1]])) {
                return 0;
              }
            }
            if (checkPieceCapture(arrangement[finalPosn[0]][finalPosn[1]], turn) < 12){
              return 1;
            }
            else {
              return 0;
            }
          }
          else if (finalPosn[0] < initPosn[0]) { // if we move up
            for (int i = 1; i < initPosn[0] - finalPosn[0]; i++) {
              if (pieceExists(arrangement[finalPosn[0]+i][initPosn[1]])) {
                return 0;
              }
            }
            if (checkPieceCapture(arrangement[finalPosn[0]][finalPosn[1]], turn) < 12){
              return 1;
            }
            else {
              return 0;
            }
          }
        }
        // Diagonal Movement
        if (abs(initPosn[0] - finalPosn[0]) == abs(initPosn[1] - finalPosn[1])) { // Check if valid diagonal

          if (finalPosn[0] < initPosn[0] && finalPosn[1] > initPosn[1]) { // if we go top-right
            for (int i = 1; i < finalPosn[1]-initPosn[1]; i++) {
              if (pieceExists(arrangement[finalPosn[0]+i][finalPosn[1]-i])) {
                return 0;
              }
            }
            if (checkPieceCapture(arrangement[finalPosn[0]][finalPosn[1]], turn) < 12){
              return 1;
            }
            else {
              return 0;
            }
          }

          else if (finalPosn[0] > initPosn[0] && finalPosn[1] > initPosn[1]) { // if we go bottom-right
            for (int i = 1; i < finalPosn[1]-initPosn[1]; i++) {
              if (pieceExists(arrangement[finalPosn[0]-i][finalPosn[1]-i])) {
                return 0;
              }
            }
            if (checkPieceCapture(arrangement[finalPosn[0]][finalPosn[1]], turn) < 12){
              return 1;
            }
            else {
              return 0;
            }
          }
          else if (finalPosn[0] > initPosn[0] && finalPosn[1] < initPosn[1]) { // if we go bottom-left
            for (int i = 1; i < finalPosn[1]-initPosn[1]; i++) {
              if (pieceExists(arrangement[finalPosn[0]-i][finalPosn[1]+i])) {
                return 0;
              }
            }
            if (checkPieceCapture(arrangement[finalPosn[0]][finalPosn[1]], turn) < 12){
              return 1;
            }
            else {
              return 0;
            }
          }
          else if (finalPosn[0] < initPosn[0] && finalPosn[1] < initPosn[1]) { // if we go top-left
            for (int i = 1; i < finalPosn[1]-initPosn[1]; i++) {
              if (pieceExists(arrangement[finalPosn[0]+i][finalPosn[1]+i])) {
                return 0;
              }
            }
            if (checkPieceCapture(arrangement[finalPosn[0]][finalPosn[1]], turn) < 12){
              return 1;
            }
            else {
              return 0;
            }
          }
        }
      }
      break;

    case 'B':
      if ((turn == 0 && arrangement[initPosn[0]][initPosn[1]] == "♝") || (turn == 1 && arrangement[initPosn[0]][initPosn[1]] == "♗")) {
        if (abs(initPosn[0] - finalPosn[0]) == abs(initPosn[1] - finalPosn[1])) { // Check if valid diagonal

          if (finalPosn[0] < initPosn[0] && finalPosn[1] > initPosn[1]) { // if we go top-right
            for (int i = 1; i < finalPosn[1]-initPosn[1]; i++) {
              if (pieceExists(arrangement[finalPosn[0]+i][finalPosn[1]-i])) {
                return 0;
              }
            }
            if (checkPieceCapture(arrangement[finalPosn[0]][finalPosn[1]], turn) < 12){
              return 1;
            }
            else {
              return 0;
            }
          }

          else if (finalPosn[0] > initPosn[0] && finalPosn[1] > initPosn[1]) { // if we go bottom-right
            for (int i = 1; i < finalPosn[1]-initPosn[1]; i++) {
              if (pieceExists(arrangement[finalPosn[0]-i][finalPosn[1]-i])) {
                return 0;
              }
            }
            if (checkPieceCapture(arrangement[finalPosn[0]][finalPosn[1]], turn) < 12){
              return 1;
            }
            else {
              return 0;
            }
          }
          else if (finalPosn[0] > initPosn[0] && finalPosn[1] < initPosn[1]) { // if we go bottom-left
            for (int i = 1; i < finalPosn[1]-initPosn[1]; i++) {
              if (pieceExists(arrangement[finalPosn[0]-i][finalPosn[1]+i])) {
                return 0;
              }
            }
            if (checkPieceCapture(arrangement[finalPosn[0]][finalPosn[1]], turn) < 12){
              return 1;
            }
            else {
              return 0;
            }
          }
          else if (finalPosn[0] < initPosn[0] && finalPosn[1] < initPosn[1]) { // if we go top-left
            for (int i = 1; i < finalPosn[1]-initPosn[1]; i++) {
              if (pieceExists(arrangement[finalPosn[0]+i][finalPosn[1]+i])) {
                return 0;
              }
            }
            if (checkPieceCapture(arrangement[finalPosn[0]][finalPosn[1]], turn) < 12){
              return 1;
            }
            else {
              return 0;
            }
          }
        }
      }
      break;
    case 'N':
      if ((turn == 0 && arrangement[initPosn[0]][initPosn[1]] == "♞") || (turn == 1 && arrangement[initPosn[0]][initPosn[1]] == "♘")) {
        // Vertical L
        if (abs(initPosn[0] - finalPosn[0]) == 1 && abs(initPosn[1] - finalPosn[1]) == 2) {
          if (checkPieceCapture(arrangement[finalPosn[0]][finalPosn[1]], turn) < 12){
            return 1;
          }
          else {
            return 0;
          }
        }
        // Horizontal L
        else if (abs(initPosn[0] - finalPosn[0]) == 2 && abs(initPosn[1] - finalPosn[1]) == 1) {
          if (checkPieceCapture(arrangement[finalPosn[0]][finalPosn[1]], turn) < 12){
            return 1;
          }
          else {
            return 0;
          }
        }
      }
      break;
    case 'R':
      if ((turn == 0 && arrangement[initPosn[0]][initPosn[1]] == "♜") || (turn == 1 && arrangement[initPosn[0]][initPosn[1]] == "♖")) {
        // If rook moves horizontally, check if path is empty or not
        if (initPosn[0] == finalPosn[0]) { // same row
          if (finalPosn[1] > initPosn[1]) { // if we move right
            for (int i = 1; i < finalPosn[1]-initPosn[1]-1; i++) {
              if (pieceExists(arrangement[finalPosn[0]][initPosn[1]+i])) {
                return 0;
              }
            }
            if (checkPieceCapture(arrangement[finalPosn[0]][finalPosn[1]], turn) < 12){
              return 1;
            }
            else {
              return 0;
            }
          }
          else if (finalPosn[1] < initPosn[1]) { // if we move left
            for (int i = 1; i < initPosn[1] - finalPosn[1]; i++) {
              if (pieceExists(arrangement[finalPosn[0]][finalPosn[1]+i])) {
                return 0;
              }
            }
            if (checkPieceCapture(arrangement[finalPosn[0]][finalPosn[1]], turn) < 12){
              return 1;
            }
            else {
              return 0;
            }
          }
        }
        // If rook moves veritcally, check if path is empty or not
        else if (initPosn[1] == finalPosn[1]) { // same coloumn
          if (finalPosn[0] > initPosn[0]) { // if we move down
            for (int i = 1; i < finalPosn[0]-initPosn[0]-1; i++) {
              if (pieceExists(arrangement[initPosn[0]+i][initPosn[1]])) {
                return 0;
              }
            }
            if (checkPieceCapture(arrangement[finalPosn[0]][finalPosn[1]], turn) < 12){
              return 1;
            }
            else {
              return 0;
            }
          }
          else if (finalPosn[0] < initPosn[0]) { // if we move up
            for (int i = 1; i < initPosn[0] - finalPosn[0]; i++) {
              if (pieceExists(arrangement[finalPosn[0]+i][initPosn[1]])) {
                return 0;
              }
            }
            if (checkPieceCapture(arrangement[finalPosn[0]][finalPosn[1]], turn) < 12){
              return 1;
            }
            else {
              return 0;
            }
          }
        }
      }
      break;
    default:
      printf("\nError");
  }
  return 0;
}

// Function checks the validty of pawn movement
int checkPawnValidity (char* arrangement[8][8], char input[10], int turn, char* pieces[2][6]) {
  int initPosn[2] = {7-(input[1]-49), input[0]-97}; // {row, coulumn}
  int finalPosn[2] = {7-(input[3]-49), input[2]-97}; // {row, coulumn}

  if (initPosn[1] == finalPosn[1]) {
    // Pawns in second aisles
    if ((initPosn[0] == 1 && turn == 1 && finalPosn[0] - initPosn[0] == 2 && arrangement[initPosn[0]+1][initPosn[1]]==" ") || (initPosn[0] == 6 && turn == 0 && initPosn[0]-finalPosn[0]==2 && arrangement[initPosn[0]-1][initPosn[1]]==" ")){
      return 1;
    }
    // Otherwise
    else if ((finalPosn[0] - initPosn[0] == 1 && turn == 1) || (initPosn[0]-finalPosn[0]==1 && turn == 0)) {
      return 1;
    }
  }

  // DIAGNONAL ATTACK CODE
  else if ((finalPosn[0] - initPosn[0] == abs(finalPosn[1]-initPosn[1]) == 1 && turn == 1) || (initPosn[0]-finalPosn[0] == abs(finalPosn[1]-initPosn[1]) == 1 && turn == 0)) {

    if (checkPieceCapture(arrangement[finalPosn[0]][finalPosn[1]], turn) < 11){
      return 1;
    }

  }

  return 0;
}


// Function returns 1 if there exists a check on the board
int checkChecks (char* arrangement[8][8], int turn) {
  char* pieces[2][6] = {{"♚", "♛", "♝", "♞", "♜", "♟"}, {"♔", "♕", "♗", "♘", "♖", "♙"}};
  char* oppKing = turn ? "♚" : "♔";
  // i = row, j = coulumn
  for (int i=0; i<8; i++) {
    for (int j=0; j<8; j++){
      // Rook Check
      if (arrangement[i][j] == pieces[turn][4]) {
        // Check vertically above
        for (int k=i-1; k>=0; k--) {
          if (pieceExists(arrangement[k][j])) {
            if (arrangement[k][j] == oppKing) {
              return 1;
            }
            else {
              break;
            }
          }
        }
        // Checks vertically below
        for (int k=i+1; k<8; k++) {
          if (pieceExists(arrangement[k][j])) {
            if (arrangement[k][j] == oppKing) {
              return 1;
            }
            else {
              break;
            }
          }
        }

        // Check horizontally left
        for (int k=j-1; k>=0; k--) {
          if (pieceExists(arrangement[i][k])) {
            if (arrangement[i][k] == oppKing) {
              return 1;
            }
            else {
              break;
            }
          }
        }
        // Checks horizontally right
        for (int k=j+1; k<8; k++) {
          if (pieceExists(arrangement[i][k])) {
            if (arrangement[i][k] == oppKing) {
              return 1;
            }
            else {
              break;
            }
          }
        }
      }

      // Bishop Check
      else if (arrangement[i][j] == pieces[turn][2]) {
        // Check bottom right
        for (int k=1; (i+k < 8) && (j+k < 8); k++) {
          if (pieceExists(arrangement[i+k][j+k])) {
            if (arrangement[i+k][j+k] == oppKing) {
              return 1;
            }
            else {
              break;
            }
          }
        }

        // Check bottom left
        for (int k=1; (i+k < 8) && (0 <= j-k); k++) {
          if (pieceExists(arrangement[i+k][j-k])) {
            if (arrangement[i+k][j-k] == oppKing) {
              return 1;
            }
            else {
              break;
            }
          }
        }

        // Check top right
        for (int k=1; (0 <= i-k) && (j+k < 8); k++) {
          if (pieceExists(arrangement[i-k][j+k])) {
            if (arrangement[i-k][j+k] == oppKing) {
              return 1;
            }
            else {
              break;
            }
          }
        }
        // Check top left
        for (int k=1; (0 <= i-k < 8) && (0 <=j-k); k++) {
          if (pieceExists(arrangement[i-k][j-k])) {
            if (arrangement[i-k][j-k] == oppKing) {
              return 1;
            }
            else {
              break;
            }
          }
        }
      }

      // Queen Check
      else if (arrangement[i][j] == pieces[turn][1]) {
        // Check vertically above
        for (int k=i-1; k>=0; k--) {
          if (pieceExists(arrangement[k][j])) {
            if (arrangement[k][j] == oppKing) {
              return 1;
            }
            else {
              break;
            }
          }
        }
        // Checks vertically below
        for (int k=i+1; k<8; k++) {
          if (pieceExists(arrangement[k][j])) {
            if (arrangement[k][j] == oppKing) {
              return 1;
            }
            else {
              break;
            }
          }
        }

        // Check horizontally left
        for (int k=j-1; k>=0; k--) {
          if (pieceExists(arrangement[i][k])) {
            if (arrangement[i][k] == oppKing) {
              return 1;
            }
            else {
              break;
            }
          }
        }
        // Checks horizontally right
        for (int k=j+1; k<8; k++) {
          if (pieceExists(arrangement[i][k])) {
            if (arrangement[i][k] == oppKing) {
              return 1;
            }
            else {
              break;
            }
          }
        }

        // Check bottom right
        for (int k=1; (i+k < 8) && (j+k < 8); k++) {
          if (pieceExists(arrangement[i+k][j+k])) {
            if (arrangement[i+k][j+k] == oppKing) {
              return 1;
            }
            else {
              break;
            }
          }
        }

        // Check bottom left
        for (int k=1; (i+k < 8) && (0 <= j-k); k++) {
          if (pieceExists(arrangement[i+k][j-k])) {
            if (arrangement[i+k][j-k] == oppKing) {
              return 1;
            }
            else {
              break;
            }
          }
        }

        // Check top right
        for (int k=1; (0 <= i-k) && (j+k < 8); k++) {
          if (pieceExists(arrangement[i-k][j+k])) {
            if (arrangement[i-k][j+k] == oppKing) {
              return 1;
            }
            else {
              break;
            }
          }
        }
        // Check top left
        for (int k=1; (0 <= i-k < 8) && (0 <=j-k); k++) {
          if (pieceExists(arrangement[i-k][j-k])) {
            if (arrangement[i-k][j-k] == oppKing) {
              return 1;
            }
            else {
              break;
            }
          }
        }
      }

      // Knight Check
      else if (arrangement[i][j] == pieces[turn][3]) {
        // following loop requires visualisation
        // there is difference of the goto position of knight in 2 or 1 in either x and y
        // both { 2&2 and 1&1 } cannot be satisfied at same time
        // we run and nested loop and see if the pointers' sum equals 3

        for (int k=-2; k<3; k++) {
          for (int l=-2; l<3; l++) {
            if ((abs(k) + abs(l) == 3) && i+k < 8 && j+l < 8 && 0<=i+k && 0<j+l) {
              if (arrangement[i+k][j+l] == oppKing) {
                return 1;
              }
            }
          }
        }
      }

      // Pawn check
      else if (arrangement[i][j] == pieces[turn][5]) {
        if (turn) { // Black pawns
          if (j+1<8 && arrangement[i+1][j+1] == oppKing) {
            return 1;
          }
          if (0<j-1 && arrangement[i+1][j-1] == oppKing) {
            return 1;
          }
        }
        else { // White pawns
          if (j+1<8 && arrangement[i-1][j+1] == oppKing) {
            return 1;
          }
          if (0<j-1 && arrangement[i-1][j-1] == oppKing) {
            return 1;
          }
        }
      }
    }
  }
  return 0;
}


// If the proposed move if able to defend the check then return 1
// else return 0
int moveDefendsCheck(char* arrangement[8][8], char input[10], int turn, int checkOnBoard) {
  if (checkOnBoard) {
    int initPosn[2] = {7-(input[2]-49), input[1]-97}; // {row, coulumn}
    int finalPosn[2] = {7-(input[4]-49), input[3]-97}; // {row, coulumn}
    char* _arrangement[8][8];
    int _turn = turn ? 0 : 1;

    for (int i=0; i<8; i++) {
      for (int j=0; j<8; j++) {
        _arrangement[i][j] = arrangement[i][j];
      }
    }
    _arrangement[finalPosn[0]][finalPosn[1]] = _arrangement[initPosn[0]][initPosn[1]];
    _arrangement[initPosn[0]][initPosn[1]] = " ";
    if (checkChecks(_arrangement, _turn)) {
      return 0;
    }
    else {
      return 1;
    }
  }
  return 1;
}


// Function returns 1 if there exists a checkmate on the board
// It loops over the entire board and tries all the legal moves available for opp
// if none of the legal moves can prevent a check on the board
// then checkmate is declared.
int checkMate (char* arrangement[8][8], int turn) {
  char* pieces[2][6] = {{"♚", "♛", "♝", "♞", "♜", "♟"}, {"♔", "♕", "♗", "♘", "♖", "♙"}};
  char* oppKing = turn ? "♚" : "♔";
  int _turn = turn ? 0 : 1;
  char* _arrangement[8][8];

  for (int n=0; n<8; n++) {
    for (int m=0; m<8; m++) {
      _arrangement[n][m] = arrangement[n][m];
    }
  }

  // i = row, j = coulumn
  for (int i=0; i<8; i++) {
    for (int j=0; j<8; j++){
      // Rook
      if (arrangement[i][j] == pieces[turn][4]) {
        // Check vertically above
        for (int k=i-1; k>=0; k--) {
          if (!pieceExists(arrangement[k][j]) || (checkPieceCapture(arrangement[k][j], turn)<12)) {
            _arrangement[k][j] = _arrangement[i][j];
            _arrangement[i][j] = " ";
            if (!checkChecks(_arrangement, _turn)) {
              return 0;
            }
            for (int n=0; n<8; n++) {
              for (int m=0; m<8; m++) {
                _arrangement[n][m] = arrangement[n][m];
              }
            }
          }
          else {
            break;
          }
        }
        // Checks vertically below
        for (int k=i+1; k<8; k++) {
          if (!pieceExists(arrangement[k][j]) || (checkPieceCapture(arrangement[k][j], turn)<12)) {
            _arrangement[k][j] = _arrangement[i][j];
            _arrangement[i][j] = " ";
            if (!checkChecks(_arrangement, _turn)) {
              return 0;
            }
            for (int n=0; n<8; n++) {
              for (int m=0; m<8; m++) {
                _arrangement[n][m] = arrangement[n][m];
              }
            }
          }
          else {
            break;
          }
        }

        // Check horizontally left
        for (int k=j-1; k>=0; k--) {
          if (!pieceExists(arrangement[i][k]) || (checkPieceCapture(arrangement[i][k], turn)<12)) {
            _arrangement[i][k] = _arrangement[i][j];
            _arrangement[i][j] = " ";
            if (!checkChecks(_arrangement, _turn)) {
              return 0;
            }
            for (int n=0; n<8; n++) {
              for (int m=0; m<8; m++) {
                _arrangement[n][m] = arrangement[n][m];
              }
            }
          }
          else {
            break;
          }
        }
        // Checks horizontally right
        for (int k=j+1; k<8; k++) {
          if (!pieceExists(arrangement[i][k]) || (checkPieceCapture(arrangement[i][k], turn)<12)) {
            _arrangement[i][k] = _arrangement[i][j];
            _arrangement[i][j] = " ";
            if (!checkChecks(_arrangement, _turn)) {
              return 0;
            }
            for (int n=0; n<8; n++) {
              for (int m=0; m<8; m++) {
                _arrangement[n][m] = arrangement[n][m];
              }
            }
          }
          else {
            break;
          }
        }
      }

      // Bishop
      else if (arrangement[i][j] == pieces[turn][2]) {
        // Check bottom right
        for (int k=1; (i+k < 8) && (j+k < 8); k++) {
          if (!pieceExists(arrangement[i+k][j+k]) || (checkPieceCapture(arrangement[i+k][j+k], turn)<12)) {
            _arrangement[i+k][j+k] = _arrangement[i][j];
            _arrangement[i][j] = " ";
            if (!checkChecks(_arrangement, _turn)) {
              return 0;
            }
            for (int n=0; n<8; n++) {
              for (int m=0; m<8; m++) {
                _arrangement[n][m] = arrangement[n][m];
              }
            }
          }
          else {
            break;
          }
        }

        // Check bottom left
        for (int k=1; (i+k < 8) && (0 <= j-k); k++) {
          if (!pieceExists(arrangement[i+k][j-k]) || (checkPieceCapture(arrangement[i+k][j-k], turn)<12)) {
            _arrangement[i+k][j-k] = _arrangement[i][j];
            _arrangement[i][j] = " ";
            if (!checkChecks(_arrangement, _turn)) {
              return 0;
            }
            for (int n=0; n<8; n++) {
              for (int m=0; m<8; m++) {
                _arrangement[n][m] = arrangement[n][m];
              }
            }
          }
          else {
            break;
          }
        }

        // Check top right
        for (int k=1; (0 <= i-k) && (j+k < 8); k++) {
          if (!pieceExists(arrangement[i-k][j+k]) || (checkPieceCapture(arrangement[i-k][j+k], turn)<12)) {
            _arrangement[i-k][j+k] = _arrangement[i][j];
            _arrangement[i][j] = " ";
            if (!checkChecks(_arrangement, _turn)) {
              return 0;
            }
            for (int n=0; n<8; n++) {
              for (int m=0; m<8; m++) {
                _arrangement[n][m] = arrangement[n][m];
              }
            }
          }
          else {
            break;
          }
        }
        // Check top left
        for (int k=1; (0 <= i-k < 8) && (0 <=j-k); k++) {
          if (!pieceExists(arrangement[i-k][j-k]) || (checkPieceCapture(arrangement[i-k][j-k], turn)<12)) {
            _arrangement[i-k][j-k] = _arrangement[i][j];
            _arrangement[i][j] = " ";
            if (!checkChecks(_arrangement, _turn)) {
              return 0;
            }
            for (int n=0; n<8; n++) {
              for (int m=0; m<8; m++) {
                _arrangement[n][m] = arrangement[n][m];
              }
            }
          }
          else {
            break;
          }
        }
      }

      // Queen
      else if (arrangement[i][j] == pieces[turn][1]) {
        // Check vertically above
        for (int k=i-1; k>=0; k--) {
          if (!pieceExists(arrangement[k][j]) || (checkPieceCapture(arrangement[k][j], turn)<12)) {
            _arrangement[k][j] = _arrangement[i][j];
            _arrangement[i][j] = " ";
            if (!checkChecks(_arrangement, _turn)) {
              return 0;
            }
            for (int n=0; n<8; n++) {
              for (int m=0; m<8; m++) {
                _arrangement[n][m] = arrangement[n][m];
              }
            }
          }
          else {
            break;
          }
        }
        // Checks vertically below
        for (int k=i+1; k<8; k++) {
          if (!pieceExists(arrangement[k][j]) || (checkPieceCapture(arrangement[k][j], turn)<12)) {
            _arrangement[k][j] = _arrangement[i][j];
            _arrangement[i][j] = " ";
            if (!checkChecks(_arrangement, _turn)) {
              return 0;
            }
            for (int n=0; n<8; n++) {
              for (int m=0; m<8; m++) {
                _arrangement[n][m] = arrangement[n][m];
              }
            }
          }
          else {
            break;
          }
        }

        // Check horizontally left
        for (int k=j-1; k>=0; k--) {
          if (!pieceExists(arrangement[i][k]) || (checkPieceCapture(arrangement[i][k], turn)<12)) {
            _arrangement[i][k] = _arrangement[i][j];
            _arrangement[i][j] = " ";
            if (!checkChecks(_arrangement, _turn)) {
              return 0;
            }
            for (int n=0; n<8; n++) {
              for (int m=0; m<8; m++) {
                _arrangement[n][m] = arrangement[n][m];
              }
            }
          }
          else {
            break;
          }
        }
        // Checks horizontally right
        for (int k=j+1; k<8; k++) {
          if (!pieceExists(arrangement[i][k]) || (checkPieceCapture(arrangement[i][k], turn)<12)) {
            _arrangement[i][k] = _arrangement[i][j];
            _arrangement[i][j] = " ";
            if (!checkChecks(_arrangement, _turn)) {
              return 0;
            }
            for (int n=0; n<8; n++) {
              for (int m=0; m<8; m++) {
                _arrangement[n][m] = arrangement[n][m];
              }
            }
          }
          else {
            break;
          }
        }

        // Check bottom right
        for (int k=1; (i+k < 8) && (j+k < 8); k++) {
          if (!pieceExists(arrangement[i+k][j+k]) || (checkPieceCapture(arrangement[i+k][j+k], turn)<12)) {
            _arrangement[i+k][j+k] = _arrangement[i][j];
            _arrangement[i][j] = " ";
            if (!checkChecks(_arrangement, _turn)) {
              return 0;
            }
            for (int n=0; n<8; n++) {
              for (int m=0; m<8; m++) {
                _arrangement[n][m] = arrangement[n][m];
              }
            }
          }
          else {
            break;
          }
        }

        // Check bottom left
        for (int k=1; (i+k < 8) && (0 <= j-k); k++) {
          if (!pieceExists(arrangement[i+k][j-k]) || (checkPieceCapture(arrangement[i+k][j-k], turn)<12)) {
            _arrangement[i+k][j-k] = _arrangement[i][j];
            _arrangement[i][j] = " ";
            if (!checkChecks(_arrangement, _turn)) {
              return 0;
            }
            for (int n=0; n<8; n++) {
              for (int m=0; m<8; m++) {
                _arrangement[n][m] = arrangement[n][m];
              }
            }
          }
          else {
            break;
          }
        }

        // Check top right
        for (int k=1; (0 <= i-k) && (j+k < 8); k++) {
          if (!pieceExists(arrangement[i-k][j+k]) || (checkPieceCapture(arrangement[i-k][j+k], turn)<12)) {
            _arrangement[i-k][j+k] = _arrangement[i][j];
            _arrangement[i][j] = " ";
            if (!checkChecks(_arrangement, _turn)) {
              return 0;
            }
            for (int n=0; n<8; n++) {
              for (int m=0; m<8; m++) {
                _arrangement[n][m] = arrangement[n][m];
              }
            }
          }
          else {
            break;
          }
        }
        // Check top left
        for (int k=1; (0 <= i-k < 8) && (0 <=j-k); k++) {
          if (!pieceExists(arrangement[i-k][j-k]) || (checkPieceCapture(arrangement[i-k][j-k], turn)<12)) {
            _arrangement[i-k][j-k] = _arrangement[i][j];
            _arrangement[i][j] = " ";
            if (!checkChecks(_arrangement, _turn)) {
              return 0;
            }
            for (int n=0; n<8; n++) {
              for (int m=0; m<8; m++) {
                _arrangement[n][m] = arrangement[n][m];
              }
            }
          }
          else {
            break;
          }
        }
      }

      // Knight
      else if (arrangement[i][j] == pieces[turn][3]) {
        // following loop requires visualisation
        // there is difference of the goto position of knight in 2 or 1 in either x and y
        // both { 2&2 and 1&1 } cannot be satisfied at same time
        // we run and nested loop and see if the pointers' sum equals 3

        for (int k=-2; k<3; k++) {
          for (int l=-2; l<3; l++) {
            if ((abs(k) + abs(l) == 3) && i+k < 8 && j+l < 8 && 0<=i+k && 0<j+l) {
              if (!pieceExists(arrangement[i+k][j+l]) || (checkPieceCapture(arrangement[i+k][j+l], turn)<12)) {
                _arrangement[i+k][j+l] = _arrangement[i][j];
                _arrangement[i][j] = " ";
                if (!checkChecks(_arrangement, _turn)) {
                  return 0;
                }
                for (int n=0; n<8; n++) {
                  for (int m=0; m<8; m++) {
                    _arrangement[n][m] = arrangement[n][m];
                  }
                }
              }
              else {
                break;
              }
            }
          }
        }
      }

      // Pawn
      else if (arrangement[i][j] == pieces[turn][5]) {
        if (turn) { // Black pawns
          if (i == 1) { // pawns in the first  row
            if (!pieceExists(arrangement[i+1][j]) && !pieceExists(arrangement[i+2][j])) {
              _arrangement[i+2][j] = _arrangement[i][j];
              _arrangement[i][j] = " ";
              if (!checkChecks(_arrangement, _turn)) {
                printf("p");
                return 0;
              }
              for (int n=0; n<8; n++) {
                for (int m=0; m<8; m++) {
                  _arrangement[n][m] = arrangement[n][m];
                }
              }
            }
          }
          if (!pieceExists(arrangement[i+1][j])) {
            _arrangement[i+1][j] = _arrangement[i][j];
            _arrangement[i][j] = " ";
            if (!checkChecks(_arrangement, _turn)) {
              printf("p");
              return 0;
            }
            for (int n=0; n<8; n++) {
              for (int m=0; m<8; m++) {
                _arrangement[n][m] = arrangement[n][m];
              }
            }
          }
          if (checkPieceCapture(arrangement[i+1][j+1], turn)) {
            _arrangement[i+1][j+1] = _arrangement[i][j];
            _arrangement[i][j] = " ";
            if (!checkChecks(_arrangement, _turn)) {
              printf("p");
              return 0;
            }
            for (int n=0; n<8; n++) {
              for (int m=0; m<8; m++) {
                _arrangement[n][m] = arrangement[n][m];
              }
            }
          }
          if (checkPieceCapture(arrangement[i+1][j-1], turn)) {
            _arrangement[i+1][j-1] = _arrangement[i][j];
            _arrangement[i][j] = " ";
            if (!checkChecks(_arrangement, _turn)) {
              printf("p");
              return 0;
            }
            for (int n=0; n<8; n++) {
              for (int m=0; m<8; m++) {
                _arrangement[n][m] = arrangement[n][m];
              }
            }
          }
        }
        else { // White pawns
          if (i == 7) { // pawns in the first  row
            if (!pieceExists(arrangement[i-1][j]) && !pieceExists(arrangement[i-2][j])) {
              _arrangement[i-2][j] = _arrangement[i][j];
              _arrangement[i][j] = " ";
              if (!checkChecks(_arrangement, _turn)) {
                return 0;
              }
              for (int n=0; n<8; n++) {
                for (int m=0; m<8; m++) {
                  _arrangement[n][m] = arrangement[n][m];
                }
              }
            }
          }

          if (!pieceExists(arrangement[i-1][j])) {
            _arrangement[i-1][j] = _arrangement[i][j];
            _arrangement[i][j] = " ";
            if (!checkChecks(_arrangement, _turn)) {
              return 0;
            }
            for (int n=0; n<8; n++) {
              for (int m=0; m<8; m++) {
                _arrangement[n][m] = arrangement[n][m];
              }
            }
          }
          if (checkPieceCapture(arrangement[i-1][j+1], turn)) {
            _arrangement[i-1][j+1] = _arrangement[i][j];
            _arrangement[i][j] = " ";
            if (!checkChecks(_arrangement, _turn)) {
              return 0;
            }
            for (int n=0; n<8; n++) {
              for (int m=0; m<8; m++) {
                _arrangement[n][m] = arrangement[n][m];
              }
            }
          }
          if (checkPieceCapture(arrangement[i-1][j-1], turn)) {
            _arrangement[i-1][j-1] = _arrangement[i][j];
            _arrangement[i][j] = " ";
            if (!checkChecks(_arrangement, _turn)) {
              return 0;
            }
            for (int n=0; n<8; n++) {
              for (int m=0; m<8; m++) {
                _arrangement[n][m] = arrangement[n][m];
              }
            }
          }
        }
      }

      // King
      else if (arrangement[i][j] == pieces[turn][5]) {
        for (int k=-1; k<2; k++){
          for (int l=-1; l<2; l++) {
            if (i+k>=0 && i+k<8 && j+l<8 && j+l>=0 && abs(k)+abs(l)!=0) {
              if (!pieceExists(arrangement[i+k][j+l]) || (checkPieceCapture(arrangement[i+k][j+l], turn)<12)) {
                _arrangement[i+k][j+l] = _arrangement[i][j];
                _arrangement[i][j] = " ";
                if (!checkChecks(_arrangement, _turn)) {
                  return 0;
                }
                for (int n=0; n<8; n++) {
                  for (int m=0; m<8; m++) {
                    _arrangement[n][m] = arrangement[n][m];
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return 1;
}


void main() {
  char* arrangement[8][8] = {
  {"♖", "♘", "♗", "♕", "♔", "♗", "♘", "♖"},
  {"♙", "♙", "♙", "♙", "♙", "♙", "♙", "♙"},
  {" ", " ", " ", " ", " ", " ", " ", " "},
  {" ", " ", " ", " ", " ", " ", " ", " "},
  {" ", " ", " ", " ", " ", " ", " ", " "},
  {" ", " ", " ", " ", " ", " ", " ", " "},
  {"♟", "♟", "♟", "♟", "♟", "♟", "♟", "♟"},
  {"♜", "♞", "♝", "♛", "♚", "♝", "♞", "♜"}
  };

  char input[10];
  int turn = 0; // 0 = white, 1 = Black
  char* pieces[2][6] = {{"♚", "♛", "♝", "♞", "♜", "♟"}, {"♔", "♕", "♗", "♘", "♖", "♙"}};
  int checkOnBoard = 0;
  displayBoard(arrangement, turn);

  while (1) {
    printf("\n>>> ");
    scanf("%s", &input);

    if (!strcmp(input, "exit")) {
      break;
    }
    else if (strlen(input)==5) {
      if (input[0] == 'K' || input[0] == 'Q' || input[0] == 'B' || input[0] == 'N' || input[0] == 'R') {
        if (input[1]-97 < 8 && input[1]-97 >= 0 && input[3]-97 < 8 && input[3]-97 >= 0 && input[2]-49 < 8 && input[2]-49 >= 0 && input[4]-49 < 8 && input[4]-49 >= 0 ) {


          if (checkValidity(arrangement, input, turn, pieces)) {
            if (moveDefendsCheck(arrangement, input, turn, checkOnBoard)) {
              arrangement[7-(input[4]-49)][input[3]-97] = arrangement[7-(input[2]-49)][input[1]-97];
              arrangement[7-(input[2]-49)][input[1]-97] = " ";

              if (checkChecks(arrangement, turn)) {
                checkOnBoard = 1;
              }
              else {
                checkOnBoard = 0;
              }
              turn = turn ? 0 : 1;
              displayBoard(arrangement, turn);
            }
            else {
              printf("\nILLEGAL MOVE");
            }
          }

          else {
            printf("\nINVALID MOVE");
          }

        }
        else {
          printf("\nINVALID INPUT");
        }
      }
      else {
        printf("\nINVALID INPUT");
      }
    }

    else if (strlen(input)==4) {
      if (input[0]-97 < 8 && input[0]-97 >= 0 && input[2]-97 < 8 && input[2]-97 >= 0 && input[1]-49 < 8 && input[1]-49 >= 0 && input[3]-49 < 8 && input[3]-49 >= 0 ) {
        if (checkPawnValidity(arrangement, input, turn, pieces)) {
          if (moveDefendsCheck(arrangement, input, turn, checkOnBoard)) {
            arrangement[7-(input[3]-49)][input[2]-97] = arrangement[7-(input[1]-49)][input[0]-97];
            arrangement[7-(input[1]-49)][input[0]-97] = " ";

            // pawn promotes to queen
            if ((7-(input[3]-49) == 7)||(7-(input[3]-49) == 0)) {
              char promo;
              printf("Promote to piece (Q, B, N, R)\n>>> ");
              scanf(" %c", &promo);
              if (promo == 'Q') {
                arrangement[7-(input[3]-49)][input[2]-97] = pieces[turn][1];
              }
              else if (promo == 'B') {
                arrangement[7-(input[3]-49)][input[2]-97] = pieces[turn][2];
              }
              else if (promo == 'N') {
                arrangement[7-(input[3]-49)][input[2]-97] = pieces[turn][3];
              }
              else if (promo == 'R') {
                arrangement[7-(input[3]-49)][input[2]-97] = pieces[turn][4];
              }
            }
            if (checkChecks(arrangement, turn)) {
              checkOnBoard = 1;
            }
            else {
              checkOnBoard = 0;
            }
            turn = turn ? 0 : 1;
            displayBoard(arrangement, turn);
          }
          else {
            printf("\nILLEGAL MOVE");
          }
        }
        else {
          printf("\nINVALID MOVE");
        }
      }
      else {
        printf("\nINVALID INPUT");
      }
    }
    else {
      printf("\nINVALID INPUT");
    }

    if (checkOnBoard) {
      if (checkMate(arrangement, turn)) {
        turn ? printf("\nBlack checkmated.\nWhite wins\n") : printf("\nWhite checkmated.\nBlack wins\n");
        break;
      }
    }
  }
}
