Wall.
GridSizeX(5).
JohnPosX(GridSizeX + 2).

entity(Action) [
	Current(0).
	Accum(2).
	Limit(5).

	event(Fill)
].

instance(Action,Forest).
Forest^Limit(10).
instance(Action,Mine).
instance(Action,Farm).
instance(Action,River).

//event(FillBoard) - Mine^Current(Mine^Current + Mine^Accum), Farm^Current(Farm^Current + Farm^Accum) : conditional(Wall = 0).
//event(FillBoardAdvanced) - Mine^Current(Mine^Current + Mine^Accum) : conditional(Wall = 0) & conditional(Mine^Current < 10).
//event(EntityFill) - Action^Current(Action^Current + Action^Accum) : conditional(Action^Limit > Action^Current).

//action(PlaceDwarfForest) - do(Action^Fill)