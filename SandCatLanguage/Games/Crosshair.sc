// Initial state
GridMaxX(10).
GridMinX(0).
GridMaxY(10).
GridMinY(0).

PlayerX(2).
PlayerY(3).

V_BlockX(3).
V_BlockY(3).

H_BlockX(8).
H_BlockY(8).

ShieldPosX(5).
ShieldPosY(7).


// Moving player. Can't move into the block. 
event(MoveUp) - PlayerY(PlayerY - 1) : 
	conditional(PlayerY > GridMinY) and not [ conditional(PlayerY - 1 = V_BlockY) and conditional(PlayerX = V_BlockX) ] and not [ conditional(PlayerY - 1 = H_BlockY) and conditional(PlayerX = H_BlockX) ].
event(MoveDown) - PlayerY(PlayerY + 1) : 
	conditional(PlayerY < GridMaxY) and not [ conditional(PlayerY + 1 = V_BlockY) and conditional(PlayerX = V_BlockX) ] and not [ conditional(PlayerY + 1 = H_BlockY) and conditional(PlayerX = H_BlockX) ].
event(MoveLeft) - PlayerX(PlayerX - 1) : 
	conditional(PlayerX > GridMinX) and not [ conditional(PlayerX - 1 = V_BlockX) and conditional(PlayerY = V_BlockY) ] and not [ conditional(PlayerX - 1 = H_BlockX) and conditional(PlayerY = H_BlockY) ].
event(MoveRight) - PlayerX(PlayerX + 1) : 
	conditional(PlayerX < GridMaxX) and not [ conditional(PlayerX + 1 = V_BlockX) and conditional(PlayerY = V_BlockY) ] and not [ conditional(PlayerX + 1 = H_BlockX) and conditional(PlayerY = H_BlockY) ].

// Pushing the block.

event(PushBlockUp) - PlayerY(PlayerY - 1), V_BlockY(V_BlockY - 1) : 
	conditional(PlayerY > GridMinY) and conditional(V_BlockY > GridMinY) and conditional(PlayerHoldingShield = 0).

event(PushBlockDown) - PlayerY(PlayerY + 1), V_BlockY(V_BlockY + 1) : 
	conditional(PlayerY < GridMaxY) and conditional(V_BlockY < GridMaxY) and conditional(PlayerHoldingShield = 0).

event(PushBlockLeft) - PlayerX(PlayerX - 1), H_BlockX(H_BlockX - 1) : 
	conditional(PlayerY > GridMinY) and conditional(H_BlockY > GridMinY) and conditional(PlayerHoldingShield = 0).

event(PushBlockRight) - PlayerX(PlayerX + 1), H_BlockX(H_BlockX + 1) : 
	conditional(PlayerY < GridMaxY) and conditional(H_BlockY < GridMaxY) and conditional(PlayerHoldingShield = 0).


// Pickingup shield
event(PickupShield) - PlayerHoldingShield(0) : conditional(PlayerX = ShieldPosX) & conditional(PlayerY = ShieldPosY).


// These aren't important. 
UseGrid.
UsePlayerPos.

bind(RightArrow, PushBlockRight, MoveRight,).
bind(LeftArrow, PushBlockLeft, MoveLeft,).
bind(DownArrow, PushBlockDown, MoveDown,).
bind(UpArrow, PushBlockUp, MoveUp,).

bind(Q, PickupShield,).