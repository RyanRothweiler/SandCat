// Initial state and card definitions

true(Card_Bat)
true(Card_Bat_Atk(1))
true(Card_Bat_Hp(5))
true(Card_Bat_Hand(1))
true(Card_Bat_Board(0))

true(Card_Wolf)
true(Card_Wolf_Atk(2))
true(Card_Wolf_Hp(4))
true(Card_Wolf_Hand(1))
true(Card_Wolf_Board(0))

true(Card_RockMonster)
true(Card_RockMonster_Atk(1))
true(Card_RockMonster_Hp(10))
true(Card_RockMonster_Hand(0))
true(Card_Monster_OpposingBoard(1))



//Events

event(PlayWolf) - true(Card_Wolf_Board(1)), true(Card_Wolf_Hand(0)) : conditional(Card_Wolf_Hand = 1).
event(PlayBat) - true(Card_Bat_Board(1)), true(Card_Bat_Hand(0)) : conditional(Card_Bat_Hand = 1).

event(WolfAttackRockMonster) - true(Card_RockMonster_Hp(Card_RockMonster_Hp - Card_Wolf_Atk)), true(Card_Wolf_Hp(Card_Wolf_Hp - Card_RockMonster_Atk)) : conditional(Card_Wolf_Board = 1).
event(BatAttackRockMonster) - true(Card_RockMonster_Hp(Card_RockMonster_Hp - Card_Bat_Atk)), true(Card_Bat_Hp(Car_Bat_Hp - Card_RockMonster_Atk)) : conditional(Card_Bat_Board = 1).
.