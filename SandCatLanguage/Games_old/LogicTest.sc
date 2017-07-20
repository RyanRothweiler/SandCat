
// conditional(Cat = 0) is currently longhand to check if cat exists
//event(CatAndHatPossible) - true(CatOnHat) : conditional(Cat = 0) & conditional(Hat = 0).
//event(CatOrHatPossible) - true(HardToSay) : conditional(Cat = 0) | conditional(Hat = 0).

// So. ((Pig and fish.) Or hat) and cat
// true when. Cat and ((hat) or (pig and fish) or (gerbal)) and Shark.
//event(LazyDog) - true(DogIsLazy) : conditional(Cat = 0) & [ conditional(Hat = 0) | conditional(Pig = 0) ] & conditional(Gerbal = 0).
//event(SmartDog) - true(DogIsSmart) : conditional(Cat = 0) & [ conditional(Hat = 0) | [ conditional(Pig = 0) & conditional(Fish = 0) ] ].
//event(PHDDog) - true(DogIsProfessor) : conditional(Cat = 0) & [ conditional(Hat = 0) | [ conditional(Pig = 0) & conditional(Fish = 0) ] | conditional(Gerbal = 0) ] & conditional(Shark = 0).
//event(WorriedDog) - true(WorriedDog) : ! conditional(Cat = 0).
event(AnxiousGod) - true(AnxiousDog) : ! [ conditional(Cat = 0) | ! conditional(Hat = 0) ].

event(MakeDog) - true(Dog).
event(MakeCat) - true(Cat).
event(MakeHat) - true(Hat).
event(MakeFish) - true(Fish).
event(MakeShark) - true(Shark).
event(MakeGerbal) - true(Gerbal).
event(MakePig) - true(Pig).

.