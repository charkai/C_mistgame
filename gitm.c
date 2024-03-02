// name: Charlie Kairaitis
// unikey: ckai8237
// SID: 510417314

#include <stdio.h> 
#include <string.h>

#define BOARDSIZE 19
#define MAXCMDBUFFER 11 // the maximum command size is 10 (place XYY\n) and we then need space for the null byte in the string at the end 
#define ASCIINUMBERLETTEROFFSET 64 // Utilised for ASCII conversion from number to letter- calculated that 64 is the (CONSTANT) difference between a number between 1-26 and it's corresponding Uppercase letter, so this is used for translation purposes

char board[BOARDSIZE][BOARDSIZE]; // 2D array for storing rows and columns of the board 
int history[BOARDSIZE*BOARDSIZE][2] = {{0}}; // 2D array - outer array has space for all possible moves and inner array has 2 spaces for the X and Y coordinate of each move. All integer values initialised as 0 at first - so that there is no risk of randomness
char gameStatus; // the status of the game is tracked as a char - 'p' = playing, 'o' = white win, '#' = black win, and 't' = tie 
int mistCentre[2]; // Array of size 2 for the X and Y coordinate of the centre of the mist 

char checkStatus() {
    if (gameStatus == 'p') {
        int freeSpacesCount = 0;
        for (int y = 0; y <= BOARDSIZE - 5; y++) {
            for (int x = 0; x <= BOARDSIZE - 5; x++) {
                if (board[y][x] != '.') {
                    // Check for a win horizontally (5 identical pieces in a row)
                    if (board[y][x] == board[y][x+1] && board[y][x] == board[y][x+2] && board[y][x] == board[y][x+3] && board[y][x] == board[y][x+4]) {
                        return board[y][x];
                    }

                    // Check for a win vertically (5 identical pieces in a column)
                    if (board[y][x] == board[y+1][x] && board[y][x] == board[y+2][x] && board[y][x] == board[y+3][x] && board[y][x] == board[y+4][x]) {
                        return board[y][x];
                    }

                    // Check for a win diagonally (5 identical pieces on a diagonal)
                    if (board[y][x] == board[y+1][x+1] && board[y][x] == board[y+2][x+2] && board[y][x] == board[y+3][x+3] && board[y][x] == board[y+4][x+4]) {
                        return board[y][x];
                    }
                }
                else {
                    freeSpacesCount++; // at the same time, checking to see if there are blank spaces. If there are none we will have a tie
                }
            }
        }
        // check for spaces in the remainder of the board!
        for (int y = BOARDSIZE - 4; y < BOARDSIZE; y++) {
            for (int x = BOARDSIZE - 4; x < BOARDSIZE; x++) {
                if (board[y][x] == '.') {
                    freeSpacesCount++;
                }
            }
        }
        if (freeSpacesCount == 0) {
            return 't';
        } 
        else {
            return 'p';
        }
    }
    else {
        return gameStatus;
    }    
}

int turnLetterToNumber(char letter) {
    if (letter < 'A' || letter > 'S') {
        return 0; 
    }
    // Utilising ASCII conversion to translate between letter and Number 
    // Using an ASCII table - the difference between any given uppercase letter and 'A' will be some number 0-18 (which translates to a position in an array)- which we can then create as a coordinate 1-19 by adding 1
    return (letter - 'A') + 1;

}

char turnNumberToLetter(int number) {
    if (number > BOARDSIZE || number < 0) {
        return '\0';
    }
    // UTILISING the ascii conversion
    return number + ASCIINUMBERLETTEROFFSET; 
}

void resign(char player) {
    if (player == 'B') {
        gameStatus = 'o';
    }
    else {
        gameStatus = '#';
    }
}

void generateHistory() {
    for (int i = 0; i < BOARDSIZE * BOARDSIZE; i++) {
        if (history[i][0] == 0) {
            break;
        }
        else {
            printf("%c", turnNumberToLetter(history[i][0]));
        }

        if (history[i][1] == 0) {
            break;
        }
        else {
            printf("%d", history[i][1]);
        }
    }
    printf("\n");
}

void view() {
    printf("%c", turnNumberToLetter(mistCentre[0]));
    printf("%d,", mistCentre[1]); 

    // NOTE that the coordinates have an offset of 1 in the array (because array ordering starts at 0)
    // so a centre coordinate of 10, 10 will be at position 9, 9 in the 2D array
    int xCoordinate = mistCentre[0] - 1;
    int yCoordinate = mistCentre[1] - 1;

    for (int y = yCoordinate + 3; y >= yCoordinate - 3; y--) {
        for (int x = xCoordinate - 3; x <= xCoordinate + 3; x++) {
            if (x >= BOARDSIZE || y >= BOARDSIZE || x < 0 || y < 0) {
                printf("x");
            }
            else {
                printf("%c", board[y][x]);
            }
        }
    }
    printf("\n");

}

int main() {
    char userCommand[MAXCMDBUFFER] = {0}; // initialise as null to start
    int numberOfMoves = 0; 
    char player = 'B'; // Player is demarcated by a character - 'B' = black, and 'W' = white. Black always starts
    gameStatus = 'p'; 

    // The centre of the mist is initialised at 10, 10 (J10)
    mistCentre[0] = 10; 
    mistCentre[1] = 10;

    // Fill board with symbol '.' to represent empty space 
    for (int i = 0; i < BOARDSIZE; i++) {
        for (int j = 0; j < BOARDSIZE; j++) {
            board[i][j] = '.'; 
        }        
    }

    while (gameStatus == 'p') {
        if (fgets(userCommand, MAXCMDBUFFER, stdin) != NULL) {
            if (strncmp(userCommand, "place ", 6) == 0 && strlen(userCommand) > 7) {

                // if we have exceeded the appropriate buffer size, something is invalid, and we need to handle it properly
                if (userCommand[strlen(userCommand)-1] != '\n') {
                
                    // To discern whether it's an invalid coordinate or a regular invalid input, we can check
                    // if we have ANY spaces in the remainder of the original command AND the buffer overflow, than we've got an INVALID case

                    int whitespaceCount = 0;
                    // We have already accounted for the one whitespace in 'place ', so we can start from index 6 (directly after the first space) and count to the end of the string first
                    for (int i = 6; i < MAXCMDBUFFER; i++) {
                        if (userCommand[i] == ' ') {
                            whitespaceCount++;
                        }
                    } 

                    // Checking for whitespace in buffer overflow
                    // Simultaneously, this will clear out the buffer overflow so that it doesn't screw up future inputs
                    char c;
                    while (1) {
                        c = getchar();
                        if (c == ' ') {
                            whitespaceCount++;
                        }
                        else if (c == '\n' || c == EOF) {
                            break;
                        }
                    }

                    if (whitespaceCount != 0) {
                        printf("Invalid!\n");
                        continue;
                    }
                    else {
                        printf("Invalid coordinate\n");
                        continue;
                    }
                }
                else {
                    // if there's more than one whitespace in the string, we have an INVALID situation
                    int whitespaceCount = 0; 
                    
                    // same logic as above 
                    for (int i = 6; i < strlen(userCommand); i++){
                        if (userCommand[i] == ' ') {
                            whitespaceCount++;
                        }
                    }
                    if (whitespaceCount != 0) {
                        printf("Invalid!\n");
                        continue;
                    }

                    int xCoordinate = turnLetterToNumber(userCommand[6]);

                    // If x coordinate is in invalid format, print an error message
                    if (xCoordinate == 0) {
                        printf("Invalid coordinate\n");
                        continue;
                    }
                    
                    int yCoordinate = 0;
                    sscanf(&userCommand[7], "%d", &yCoordinate);

                    // If the y coordinate is out of bounds or in invalid format (INCLUDING with leading 0s), print an error message 
                    if (yCoordinate > BOARDSIZE || yCoordinate <= 0 || userCommand[7] == '0') {
                        printf("Invalid coordinate\n"); 
                        continue; 
                    }
                    
                    // Check whether position is already occupied
                    if (board[yCoordinate-1][xCoordinate-1] != '.') {
                        printf("Occupied coordinate\n");
                        continue;
                    }

                    // Fill the board with the player piece 
                    // Note that we shift the x and y coordinate back by one
                    // as indexes of arrays start at 0, 0 (whereas our first coordinate is 1, 1)
                    if (player == 'B') {
                        board[yCoordinate-1][xCoordinate-1] = '#'; 
                    }
                    else {
                        board[yCoordinate-1][xCoordinate-1] = 'o';
                    }

                    // Alternate players
                    if (player == 'B') {
                        player = 'W';
                    }
                    else if (player == 'W') {
                        player = 'B'; 
                    }

                    // Update history 
                    history[numberOfMoves][0] = xCoordinate; 
                    history[numberOfMoves][1] = yCoordinate;

                    // increase the number of moves that have been made successfully
                    numberOfMoves++;

                    // Move the mist- using the coordinates given to us in the specifications
                    mistCentre[0] = 1 + ((5 * (xCoordinate * xCoordinate)) + (3 * xCoordinate) + 4) % 19;
                    mistCentre[1] = 1 + ((4 * (yCoordinate * yCoordinate)) + (2 * yCoordinate) - 4) % 19;
                }
            }

            // IF we have any other type of command- check if we have buffer overflow
            else if (userCommand[strlen(userCommand)-1] != '\n') {

                // if we do, we want to clear out the buffer overflow, and then print an invalid message and restart the loop
                char c;
                while (1) {
                    c = getchar();
                    
                    if (c == '\n' || c == EOF) {
                        break;
                    }
                }
                printf("Invalid!\n");
                continue;
            }
            else if (strcmp(userCommand, "who\n") == 0) {
                printf("%c\n", player);
            }
            else if (strcmp(userCommand, "term\n") == 0) {
                return 1;
            }
            else if (strcmp(userCommand, "resign\n") == 0) {
                resign(player); 
            }
            else if (strcmp(userCommand, "history\n") == 0) {
                generateHistory();
            }
            else if (strcmp(userCommand, "view\n") == 0) {
                view();
            }
            else {
                printf("Invalid!\n");
                continue; 
            }
        }
        gameStatus = checkStatus(); 

        if (gameStatus != 'p') {
            break;
        }
    }

    // End game conditions
    if (gameStatus == '#') {
        printf("Black wins!\n");
    }
    else if (gameStatus == 'o') {
        printf("White wins!\n");
    }
    else if (gameStatus == 't') {
        printf("Wow, a tie!\n");
    }

    generateHistory();

    printf("Thank you for playing!\n");
    return 0;
}