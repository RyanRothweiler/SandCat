! Action Spaces
CoalMine, WheatField, GoldMine: [Current(2), Addition(5), Maximum(10), SpaceTaken(0)].

CoalMine'Addition(2).
WheatField'Addition(3).
GoldMine'Addition(1).

! Player info
PlayerCoal(0).
PlayerWheat(0).
PlayerGold(0).

PlayerDwarvesHome(2).
PlayerDwarvesTotal(PlayerDwarvesHome).



method RefillSpace
	using	ThingOne
	does	ThingOne'Current(ThingOne'Current + ThingOne'Addition)
	if		ThingOne'Current + ThingOne'Addition < ThingOne'Maximum.

method RefillBoard
	does RefillSpace{CoalMine}, RefillSpace{WheatField}, RefillSpace{GoldMine}, CoalMine'SpaceTaken(0), WheatField'SpaceTaken(0), GoldMine'SpaceTaken(0).

method UseSpace
	using	Space, PlayerResource
	does	PlayerDwarvesHome(PlayerDwarvesHome - 1), PlayerResource(PlayerResource + Space'Current), Space'Current(0), Space'SpaceTaken(1).




! ----------- Coal Placement
action(PlaceDwarf_Coal)
	does 	UseSpace{CoalMine, PlayerCoal} 
	if 		CoalMine'SpaceTaken = 0 and PlayerDwarvesHome > 1.

action(PlaceFinalDwarf_Coal)
	does 	UseSpace{CoalMine, PlayerCoal}, PlayerDwarvesHome(PlayerDwarvesTotal), RefillBoard{}
	if 		CoalMine'SpaceTaken = 0 and PlayerDwarvesHome = 1.



! ----------- Wheat Placement
action(PlaceDwarf_Wheat)
	does 	UseSpace{WheatField, PlayerWheat}
	if 		WheatField'SpaceTaken = 0 and PlayerDwarvesHome > 1.

action(PlaceFinalDwarf_Wheat)
	does 	UseSpace{WheatField, PlayerWheat}, PlayerDwarvesHome(PlayerDwarvesTotal), RefillBoard{}
	if 		WheatField'SpaceTaken = 0 and PlayerDwarvesHome = 1.



! ----------- Gold Placement
action(PlaceDwarf_GoldMine)
	does 	UseSpace{GoldMine, PlayerGold}
	if 		GoldMine'SpaceTaken = 0 and PlayerDwarvesHome > 1.

action(PlaceFinalDwarf_Gold)
	does 	UseSpace{GoldMine, PlayerGold}, PlayerDwarvesHome(PlayerDwarvesTotal), RefillBoard{}
	if 		GoldMine'SpaceTaken = 0 and PlayerDwarvesHome = 1.