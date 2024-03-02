# C_mistgame
A 2 player game in C, that involves the game of Gomuku but with the added element of a mist. 

This game was designed for a systems programming class as part of an introduction to the syntax of C. As you will possibly see, a large part of the code revolves around verifying that the inputs are valid. 

A 19 x 19 board is used in this game. Black (who starts) and White alternate placing stones, the goal is to create a diagonal, row or column of 5 or more stones. There is an added element of the 'mist', which prevents players from seeing parts of the board, but they can still place stones here. 

To play, use the commands 
```
who
term
resign
view
place <Column><Row>
history
```
