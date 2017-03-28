// Phases of the game
// 0 - Refill board
// 1 - Place dwarves

//Phase(0).

// This declares the actions members and initial state
entity(Action) [
	Current = 0.
	Accum = 2.
]

instance(Action GoldMine).
GoldMine^Accum = 5.
GoldMine^Current = 1.

instance(Action WeakForest).
WeakForest^Accum = 2.

instance(Action StrongForest).
StrongForest^Accum = 3.

//event(FillBoard) - Action^Accum = Action^Accum + Action^Current : conditional(Phase = 0)