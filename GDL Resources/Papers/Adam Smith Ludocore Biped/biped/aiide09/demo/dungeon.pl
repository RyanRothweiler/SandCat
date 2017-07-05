%
room(exit).
room(hall).
room(hatch).
room(pit).
room(kiosk).
room(pantry).
room(vault1).
room(vault2).

link(exit,hall).
link(hall,hatch).
link(hatch,pit).
link(pit,kiosk).
link(kiosk,pantry).
link(pantry,vault1).
link(vault1,vault2).
link(vault2,kiosk).

ui_token(hero).
ui_token(monster(skormo)).
ui_token(monster(multhar)).
ui_token(monster(grogg)).
ui_token(potion(aquafina)).
ui_token(treasure(dusty_crown)).
ui_token(treasure(assorted_gems)).
ui_token(treasure(loose_coins)).

ui_space(R) :- room(R).

ui_location(hero,exit).
ui_location(treasure(loose_coins),hatch).
ui_location(monster(skormo),hatch).
ui_location(monster(multhar),pit).
ui_location(monster(grogg),kiosk).
ui_location(potion(aquafina),pantry).
ui_location(treasure(dusty_crown),vault1).
ui_location(treasure(assorted_gems),vault2).

ui_path(A,B) :- link(A,B).

ui_title('Crawl Test').
ui_instruction('INSTRUCTIONS:\n\
- you control the valiant "hero"\n\
- click adjacent rooms to move\n\
- click treasures in the same room to pick them up\n\
- click monsters in the same room to fight them\n\
  (before they fight you!)\n\
- return to the "exit" with your loot to get your score').
