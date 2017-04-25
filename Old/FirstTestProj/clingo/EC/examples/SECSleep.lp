timepoint(0..5).
agent(john).
fluent(awake(X)) :- agent(X).
event(wake_up(X)) :- agent(X).
event(fall_asleep(X)) :- agent(X).
initiates(wake_up(A),awake(A),T) :- agent(A),timepoint(T).
terminates(fall_asleep(A),awake(A),T) :- agent(A),timepoint(T).
holds_at(awake(john),0).
-released_at(F,0) :- fluent(F).
happens(fall_asleep(john),1).
happens(wake_up(john),2).

% End of file.














#include "EC/DEC.lp".

timepoint(0..9).

human(john).

fluent(sick(A)) :- human(A).
fluent(healthy(A)) :- human(A).

% holds_at(healthy(john, 0)).

event(plague).
event(begin).

initiates(plague, sick(A), T) :- human(A), timepoint(T).
initiates(begin, healthy(A), T) :- human(A), timepoint(T).

happens(begin, 0).
happens(plague, 5).