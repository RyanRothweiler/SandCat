role(Player)

true(PlayerX(2))
true(PlayerY(2))
true(PlayerA(2 + 8))
true(PlayerB(2 - 8))
true(PlayerC(2 / 8))
true(PlayerD(2 * 8))
true(PlayerE(PlayerX + 8))

true(HasGas)
true(CarAtHome)


event(GoToCostco) - false(HasGas), true(CarAtCostco), false(CarAtHome), false(CarAtBlockbuster), false(CarAtHome) : true(HasGas), false(CarAtCostco).
event(GoToBlockbuster) - false(HasGas), true(CarAtBlockbuster), false(CarAtCostco), false(CarAtHome) : true(HasGas), false(CarAtBlockbuster).
event(GoHome) - false(HasGas), true(CarAtHome), false(CarAtCostco), false(CarAtBlockbuster): true(HasGas), false(CarAtHome).
event(GetGas) - true(HasGas) : false(HasGas).

event(MoveUp) - true(PlayerX(PlayerX + 1)).

.