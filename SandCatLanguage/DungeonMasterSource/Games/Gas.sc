
//NOTE this is old, and won't work without updating the syntax

HasGas.
CarAtHome.

event(GoToCostco) - false(HasGas), true(CarAtCostco), false(CarAtHome), false(CarAtBlockbuster), false(CarAtHome) : true(HasGas), false(CarAtCostco).
event(GoToBlockbuster) - false(HasGas), true(CarAtBlockbuster), false(CarAtCostco), false(CarAtHome) : true(HasGas), false(CarAtBlockbuster).
event(GoHome) - false(HasGas), true(CarAtHome), false(CarAtCostco), false(CarAtBlockbuster): true(HasGas), false(CarAtHome).
event(GetGas) - true(HasGas) : false(HasGas).

.