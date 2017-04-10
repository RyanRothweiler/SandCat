
! WithinMap if {Pos} Pos^X < MapBounds^X and Pos^X > 0 and not MapBouts^X = 100.
PlayerPos, MapBounds : [X(0), Y(0)].


GridWidth(10).
PlayerX(GridWidth).
PlayerA(GridWidth + 1).
PlayerB(GridWidth+1).
PlayerC(GridWidth+1 / 2).
PlayerD(2 * GridWidth).
PlayerE(2* GridWidth).

! action NAME is STATE_CHANGING when STATE_CONDITIONS.
action(MoveUp)
	PlayerX(PlayerX - 1) 
	when PlayerX > 0.