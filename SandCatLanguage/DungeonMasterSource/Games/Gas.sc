role(Player)

true(MovieAtBlockbuster)
true(HasGas)
true(FoodAtCostco)
true(CarAtHome)

false(FalseTest)
true(TrueTest)

event(GoToCostco) - true(FoodAtCostco) true(CarAtHome) : false(HasGas), true(HasFood), false(FoodAtCostco) false(CarAtHome) true(CarAtCostco)

.