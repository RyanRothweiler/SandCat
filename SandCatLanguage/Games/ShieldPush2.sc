GridMax, PlayerPos, BlockPos, ShieldPos : [x(0), y(0)].

GridMax'x(6).
GridMax'y(6).

PlayerPos'x(GridMax'x / 2).
PlayerPos'y(GridMax'y / 2).

BlockPos'x(3).
BlockPos'y(3).

! ShieldPos'x(5).
! ShieldPos'y(7).

! conditional method
! OnSameSpace{SubjectOne, SubjectTwo} if SubjectOne'x = SubjectTwo'x and SubjectOne'y = SubjectTwo'y.

action(MoveUp)
	does	PlayerPos'y(PlayerPos'y - 1)
	if		PlayerPos'y > 0.

action(MoveDown) 
	does	PlayerPos'y(PlayerPos'y + 1)
	if		PlayerPos'y < GridMax'y.

action(MoveRight) 
	does	PlayerPos'x(PlayerPos'x + 1) 
	if		PlayerPos'x < GridMax'x.

action(MoveLeft) 
	does	PlayerPos'x(PlayerPos'x - 1) 
	if		PlayerPos'x > 0.

! Key bindings
bind(RightArrow, MoveRight).
bind(LeftArrow, MoveLeft).
bind(DownArrow, MoveDown).
bind(UpArrow, MoveUp).