
role(Player)

true(HasGas)
true(CarAtHome)

event(GoToCostco) - false(HasGas), true(CarAtCostco), false(CarAtHome), false(CarAtBlockbuster), false(CarAtHome) : true(HasGas), false(CarAtCostco).
event(GoToBlockbuster) - false(HasGas), true(CarAtBlockbuster), false(CarAtCostco), false(CarAtHome) : true(HasGas), false(CarAtBlockbuster).
event(GoHome) - false(HasGas), true(CarAtHome), false(CarAtCostco), false(CarAtBlockbuster): true(HasGas), false(CarAtHome).
event(GetGas) - true(HasGas) : false(HasGas).

.