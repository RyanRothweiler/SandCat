
// Roles
role(Player)

// Initial state
true(GridMaxX(10))
true(GridMinX(0))
true(GridMaxY(10))
true(GridMinY(0))

true(PlayerX(GridMaxX / 2))
true(PlayerY(GridMaxY / 2))

// Events
event(MoveDown) - true(PlayerY(PlayerY - 1)) : conditional(PlayerY > GridMin).

.