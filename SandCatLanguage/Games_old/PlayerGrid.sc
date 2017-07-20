true(GridMaxX(5))
true(GridMinX(0))
true(GridMaxY(5))
true(GridMinY(0))

true(PlayerX(2))
true(PlayerY(3))

true(BlockX(2))
true(BlockY(2))

event(MoveUp) - true(PlayerY(PlayerY - 1)) : conditional(PlayerY > GridMinY) & ! [ conditional(PlayerY - 1 = BlockY) & conditional(PlayerX = BlockX) ].
event(MoveDown) - true(PlayerY(PlayerY + 1)) : conditional(PlayerY < GridMaxY) & ! [ conditional(PlayerY + 1 = BlockY) & conditional(PlayerX = BlockX) ].
event(MoveLeft) - true(PlayerX(PlayerX - 1)) : conditional(PlayerX > GridMinX) & ! [ conditional(PlayerX - 1 = BlockX) & conditional(PlayerY = BlockY) ].
event(MoveRight) - true(PlayerX(PlayerX + 1)) : conditional(PlayerX < GridMaxX) & ! [ conditional(PlayerX + 1 = BlockX) & conditional(PlayerY = BlockY) ].










// Bind keys to events
bind(MoveDown DownArrow)
bind(MoveUp UpArrow)
bind(MoveLeft LeftArrow)
bind(MoveRight RightArrow)

true(UsePlayerPos())
true(UseGrid())
.
