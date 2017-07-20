
! WithinMap if {Pos} Pos^X < MapBounds^X and Pos^X > 0 and not MapBouts^X = 100.


GridWidth(10).
PlayerX(10).
! PlayerA(GridWidth + 1).
! PlayerB(GridWidth+1).
! PlayerC(GridWidth+1 / 2).
! PlayerD(2 * GridWidth).

! PlayerPos, RatPos, MapBounds : [X(GridWidth), Y(2)].


RatStats, PlayerStats : [atk(10), def(10)].

! Can modify what is already there, or add more
! PlayerStats^atk(20).

! action NAME is STATE_CHANGING when STATE_CONDITIONS.
action(MoveUp) : PlayerX(PlayerX - 1) : PlayerX > 0.

PlayerE(2* GridWidth).
Chicken.
PlayerStats^spd(100).
