role(John)

fluent(JohnPositionX(5))
fluent(JohnPositionY(5))

fluent(GridMin(0))
fluent(GridMax(10))

event(MoveRight(JohnPosition(JohnPositionX + 1)) : JohnPositionX + 1 < GridMax)
event(MoveLeft(JohnPosition(JohnPositionX - 1)) : JohnPositionX - 1 > GridMin)

sees(John, MoveRight)
sees(John, MoveLeft)
sees(John, JohnPositionX)