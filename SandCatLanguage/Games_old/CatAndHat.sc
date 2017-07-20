event(MakeCat) - true(Cat).
event(MakeHat) - true(Hat).

event(PutHatOnCat) - true(CatOnHat) : conditional(Cat = 0) & conditional(Hat = 0).

.