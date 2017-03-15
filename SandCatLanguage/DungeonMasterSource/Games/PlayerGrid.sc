
role(Player)

true(PlayerX(2))
true(PlayerY(2))

true(GridMaxX(10))
true(GridMaxY(10))

event(MoveUp) - true(PlayerX(PlayerX + 1)) : true(Test).
event(MoveDown) - true(PlayerX(PlayerX - 1)).

.