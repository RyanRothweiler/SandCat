%%%%
%%%% ui layer predicates (use these in your game
%%%%

% visual elements:
%
%   ui_token(Ident)
%   ui_token_label(Ident,Label)
%   ui_token_color(Ident,ColorInHex)
%   ui_token_image(Url)
%
%   ui_space(Space)
%   ui_space_label(Ident,Label)
%   ui_space_color(Ident,ColorHex)
%   ui_space_size(Ident,DiameterPixels)
%   ui_space_image(Url)

% event management:
%
%   ui_triggers(UiEvent,GameEvent) -- triggers only if GameEvent possible
%   ui_ticker(Millis) -- triggers ui_tick every Millis
%   ui_timer(Millis) -- trigger ui_timeout once after Millis

% events:
%
%   ui_click_space(Space)
%   ui_click_token(Token)
%   ui_tick(TickNumber)
%   ui_timeout


%%%%
%%%% runtime logic
%%%%

%% ui helpers

ui_layout(engine_nowhere,0.5,1.25).

ui_location_total(Token,Space) :- ui_location(Token,Space).
ui_location_total(Token,engine_nowhere) :- ui_token(Token), \+ ui_location(Token,_).


%% event calculus

initiated(Fluent) :- happens(Event), initiates(Event,Fluent).
terminated(Fluent) :- happens(Event), terminates(Event,Fluent).

holds(Fluent) :- holds_transiently(Fluent).

holds_next(Fluent) :- initiated(Fluent).
holds_next(Fluent) :- holds(Fluent), \+ terminated(Fluent).

%% event triggering

happens(Event) :- happens_spontaneously(Event).
happens(GameEvent) :-
    happens_spontaneously(UserEvent),
    ui_triggers(UserEvent,GameEvent),
    possible(GameEvent).


ui_interesting(UserEvent) :-
    ui_triggers(UserEvent,GameEvent),
    possible(GameEvent).

%% initial conditions
timepoint(0).

%%%%
%%%% prolog library classics
%%%%

forall(Condition,Action) :- Condition, Action, fail.
forall(_,_).

% eof