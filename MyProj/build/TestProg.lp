animals(cat).
animals(dog).

food(cat).
food(dog).

board_x(1..10).
board_y(1..10).

cat(board_x(5)).

:- board_x(P), cat(board_x(P)).

move()