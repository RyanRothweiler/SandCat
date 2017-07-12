

ui_title('GridSlasher!!!').

%% INTEGER HELPERS
int(X) :- member(X,[1,2,3,4,5]).
int_max(5).
point(X,Y) :- int(X), int(Y).


%% EXAMPLE TOKENS (DELETE THESE RIGHT AWAY)

ui_token(hero).
ui_token(wumpus).
ui_token(blob).
ui_token(rabbit).

ui_location(hero,g(1,1)).
ui_location(wumpus,g(1,3)).
ui_location(blob,g(2,4)).
ui_location(rabbit,carcass_pile).

%% UI_SPACES (those green token-holders)

ui_space(n).
ui_space(w).
ui_space(e).
ui_space(s).

ui_space(attack).

ui_space(carcass_pile).

ui_space(g(X,Y)) :- point(X,Y).

ui_space_label(g(X,Y),'') :- point(X,Y).
ui_space_size(g(X,Y),50) :- point(X,Y).
ui_token_image(hero,'http://github.com/rndmcnlly/iteration/raw/master/res/drawable/icon.png').
ui_space_color(D,0x888888) :- member(D,[n,e,w,s,attack]).
ui_token_color(hero,0x0000FF).
ui_title_color(0xFF8800).
ui_path_color(n,e,0x888888).
ui_path_color(s,w,0x888888).

%% UI_LAYOUT (arranging spaces on the screen)

ui_layout(n,0.15,0.7).
ui_layout(w,0.08,0.8).
ui_layout(e,0.22,0.8).
ui_layout(s,0.15,0.9).

ui_layout(attack,0.15,0.8).

ui_layout(carcass_pile,0.35,0.3).

ui_layout(g(X,Y),SX,SY) :-
	point(X,Y),
	int_max(Max),
	OX is 0.5,
	OY is 0.3,
	W is 0.5,
	H is 0.75,
	SX is OX+W*(X-1)/Max,
	SY is OY+H*(Y-1)/Max.

%% UI_PATHS (connecting spaces with lines)

ui_path(g(X,Y),g(X,Y1)) :-
	point(X,Y),
	int_max(Max),
	Y < Max,
	Y1 is Y+1.
	
ui_path(g(X,Y),g(X1,Y)) :-
	point(X,Y),
	int_max(Max),
	X < Max,
	X1 is X+1.

ui_path(n,s).
ui_path(e,w).

ui_instruction('Use n/s/e/w to navigate the grid.\nAttack monsters on adjacent spaces.').