entity(Vector2) [
	X(0).
	Y(0).
]

instance(Vector2, Player).
instance(Vector2, MapBounds).

MapBounds^X(10).
MapBounds^Y(10).

Player^X(5).

Player^Y(5).
Player^Y = 5.
Player^Y is Player^Y - 1
PlayerY 5.



//Subject is InMapBounds if Subject^X > 0 and Subject^X < MapBounds.X.

event(MovePlayerLeft) - Player^X(Player^X - 1) : conditional(Player^X > 0).
event(MovePlayerRight) - Player^X(Player^X + 1) : conditional(Player^X < MapBounds^X).