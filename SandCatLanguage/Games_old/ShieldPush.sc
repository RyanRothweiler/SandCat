// Initial state
GridMaxX(10).
GridMinX(0).
GridMaxY(10).
GridMinY(0).

PlayerX(2).
PlayerY(3).

BlockX(3).
BlockY(3).

ShieldPosX(5).
ShieldPosY(7).


// Moving player. Can't move into the block. 
event(MoveUp) - PlayerY(PlayerY - 1) : 
	conditional(PlayerY > GridMinY) and not [ conditional(PlayerY - 1 = BlockY) and conditional(PlayerX = BlockX) ].
event(MoveDown) - PlayerY(PlayerY + 1) : 
	conditional(PlayerY < GridMaxY) and not [ conditional(PlayerY + 1 = BlockY) and conditional(PlayerX = BlockX) ].
event(MoveLeft) - PlayerX(PlayerX - 1) : 
	conditional(PlayerX > GridMinX) and not [ conditional(PlayerX - 1 = BlockX) and conditional(PlayerY = BlockY) ].
event(MoveRight) - PlayerX(PlayerX + 1) : 
	conditional(PlayerX < GridMaxX) and not [ conditional(PlayerX + 1 = BlockX) and conditional(PlayerY = BlockY) ].

// Pushing the block.
event(PushBlockUp) - PlayerY(PlayerY - 1), BlockY(BlockY - 1) : 
	conditional(PlayerY > GridMinY) and [ conditional(PlayerY - 1 = BlockY) and conditional(PlayerX = BlockX) ] and conditional(BlockY > GridMinY) and conditional(PlayerHoldingShield = 0).
event(PushBlockDown) - PlayerY(PlayerY + 1), BlockY(BlockY + 1) : 
	conditional(PlayerY < GridMaxY) and [ conditional(PlayerY + 1 = BlockY) and conditional(PlayerX = BlockX) ] and conditional(BlockY < GridMaxY) and conditional(PlayerHoldingShield = 0).



// Pickingup shield
event(PickupShield) - PlayerHoldingShield(0) : conditional(PlayerX = ShieldPosX) & conditional(PlayerY = ShieldPosY).


// These aren't important. 
UseGrid.
UsePlayerPos.

bind(RightArrow, MoveRight,).
bind(LeftArrow, MoveLeft,).
bind(DownArrow, PushBlockDown, MoveDown,).
bind(UpArrow, PushBlockUp, MoveUp,).

bind(Q, PickupShield,).