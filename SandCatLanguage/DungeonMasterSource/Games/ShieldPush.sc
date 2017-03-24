
// Roles
//role(Player)

// Initial state
true(GridMaxX(10))
true(GridMinX(0))
true(GridMaxY(10))
true(GridMinY(0))

// This is used only to communicate with the display stuff
// The display stuff looks specifically for certain fluents names for it's values.
true(UseGrid())


true(PlayerX(2))
true(PlayerY(3))

// This is used only to communicate with the display stuff
true(UsePlayerPos())

true(BlockX(2))
true(BlockY(2))

true(ShieldPosX(5))
true(ShieldPosY(7))


// Events

// Moving Up

event(MoveUp) - true(PlayerY(PlayerY - 1)) : conditional(PlayerY > GridMinY) & ! [ conditional(PlayerY - 1 = BlockY) & conditional(PlayerX = BlockX) ].
event(PushBlockUp) - true(PlayerY(PlayerY - 1)), true(BlockY(BlockY - 1)) : conditional(PlayerY > GridMinY) & [ conditional(PlayerY - 1 = BlockY) & conditional(PlayerX = BlockX) ] & conditional(BlockY > GridMinY).

// Moving Down

event(MoveDown) - true(PlayerY(PlayerY + 1)) : conditional(PlayerY < GridMaxY) & ! [ conditional(PlayerY + 1 = BlockY) & conditional(PlayerX = BlockX) ].
event(PushBlockDown) - true(PlayerY(PlayerY + 1)), true(BlockY(BlockY + 1)) : conditional(PlayerY < GridMaxY) & [ conditional(PlayerY + 1 = BlockY) & conditional(PlayerX = BlockX) ] & conditional(BlockY < GridMaxY).

// Moving Left

event(MoveLeft) - true(PlayerX(PlayerX - 1)) : conditional(PlayerX > GridMinX) & ! [ conditional(PlayerX - 1 = BlockX) & conditional(PlayerY = BlockY) ].
event(PushBlockLeft) - true(PlayerX(PlayerX - 1)), true(BlockX(BlockX - 1)) : conditional(PlayerX > GridMinX) & [ conditional(PlayerX - 1 = BlockX) & conditional(PlayerY = BlockY) ] & conditional(BlockX > GridMinX).

// Moving Right

event(MoveRight) - true(PlayerX(PlayerX + 1)) : conditional(PlayerX < GridMaxX) & ! [ conditional(PlayerX + 1 = BlockX) & conditional(PlayerY = BlockY) ].
event(PushBlockRight) - true(PlayerX(PlayerX + 1)), true(BlockX(BlockX + 1)) : conditional(PlayerX < GridMaxX) & [ conditional(PlayerX + 1 = BlockX) & conditional(PlayerY = BlockY) ] & conditional(BlockX < GridMaxX).

// Shield pickup / release

event(PickupShield) - true(PlayerHoldingShield) : conditional(PlayerX = ShieldPosX) & conditional(PlayerY = ShieldPosY).
event(ReleaseShield) - false(PlayerHoldingShield) : conditional(PlayerHoldingShield = 0).


// Bind keys to events

bind(MoveDown DownArrow)
bind(PushBlockDown S)

bind(MoveUp UpArrow)
bind(PushBlockUp W)

bind(MoveLeft LeftArrow)
bind(PushBlockLeft A)

bind(MoveRight RightArrow)
bind(PushBlockRight D)

bind(PickupShield Q)
bind(ReleaseShield E)
.