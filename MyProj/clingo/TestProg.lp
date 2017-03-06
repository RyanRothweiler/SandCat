#include "EC/DEC.lp".

timepoint(0..10).



% Agents (Nouns) -----------------

boat(boatOne).
settler(john).



% Fluents (noun state) -----------------

fluent(settler_ready_to_board(S)) :- settler(S).
fluent(settler_boarded(S, B)) :- settler(S), boat(B).



% Events (What things can happen) -----------------

event(settler_awake(S)) :- settler(S).
event(board_boat(S, B)) :- settler(S), boat(B).



% init / term (What events change what state) -----------------

initiates(settler_awake(S), settler_ready_to_board(S), T) :- settler(S), timepoint(T).
initiates(board_boat(S, B), settler_boarded(S, B), T) :- settler(S), boat(B), timepoint(T).

% initial state -----------------



% what happened -----------------

happens(setter_awake(john), 1).