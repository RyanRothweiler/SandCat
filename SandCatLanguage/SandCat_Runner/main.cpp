// windows stuff
#include <iostream>
#include <windows.h>


// -----------------------------------------------------------------------------
// My type stuff
// -----------------------------------------------------------------------------
int8_t		 		typedef 	int8;
int16_t		 		typedef 	int16;
int32_t		 		typedef 	int32;
int64_t		 		typedef 	int64;
unsigned char 		typedef 	uint8;
unsigned short 		typedef 	uint16;
unsigned int 		typedef 	uint32;
unsigned long long 	typedef 	uint64;
int32		 		typedef 	bool32;
float		 		typedef 	real32;
double		 		typedef 	real64;

#define ARRAY_SIZE(Array, Type) sizeof(Array) / sizeof(Type)
#define Assert(Expression) if (!(Expression)) {*(int *)0 = 0;}
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------



// my stuff
#include "String.cpp"

// Remove this is you don't want to compile any opengl stuff
// #define USING_OPENGL

#ifdef USING_OPENGL
#include "Math.cpp"
#include "color.cpp"
#include "vector2.cpp"
#include "Rect.cpp"
#include "opengl.cpp"

#include "EditorUI.h"
#include "EditorUI.cpp"
#include "stb_truetype.h"
#endif

char DotNotationChar = '\'';

void Print(string Output) {
	Output = Output + "\n";
	printf(Output.CharArray);
}

struct prog_data {
	char* Characters;
	int32 CharactersCount;
};

prog_data
LoadProg(char* FileName) {
	HANDLE FileHandle = CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ,
	                               NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	int32 FileSizeBytes = GetFileSize(FileHandle, NULL);

	// Make sure we got the file
	Assert(FileSizeBytes != 0);

	prog_data ProgData = {};
	ProgData.Characters = (char*)malloc(FileSizeBytes);
	ZeroMemory(ProgData.Characters, FileSizeBytes);

	ProgData.CharactersCount = FileSizeBytes;

	DWORD BytesRead;
	if (!ReadFile(FileHandle, (void*)ProgData.Characters, FileSizeBytes, &BytesRead, NULL)) {
		// Couldn't read the file. Something wrong happened
		Assert(0);
	}

	CloseHandle(FileHandle);
	return (ProgData);
}

struct parser {
	char* CharOn;

	char WordOn[100] = {};
	int32 WordOnIndex = 0;
};

void
ResetParserWord(parser* Parser) {
	ZeroMemory(&Parser->WordOn, ARRAY_SIZE(Parser->WordOn, char));
	Parser->WordOnIndex = 0;
}

struct event_bind {
	string EventNames[100];
	int32 NextEventName;

	string KeyName;
};

void
PrintHeader(string Str) {
	Print("---- " + Str + " ----");
}

struct window_info
{
	// NOTE these are virtual, used to create a consistent look across multiple platforms
	uint32 Width;
	uint32 Height;

	// NOTE these are ACTUAl width in pixels
	uint32 PixelWidth;
	uint32 PixelHeight;
};

static bool32 GlobalRunning = true;
static real64 GlobalScrollPos = 0.0f;
static real64 PrevGlobalScrollPos;

// This moves parser, stopping at anything other than white space
void
ConsumeWhiteSpace(parser* Parser) {
	while (true) {
		char ThisChar = *Parser->CharOn;
		if (ThisChar != '\r' && ThisChar != '\n' && ThisChar != ' ' && ThisChar != '\t') {
			break;
		}
		Parser->CharOn++;
	}
}

enum class token_type {
	// general
	none, entity, period, id, number, comma, colon, action, bind,

	// method
	method, usingg,

	// action
	does, iff,

	// math symbols
	sub, add, div, mult,

	// equality symbols
	lessThan, equalTo, greaterThan, lessThanOrEqual, greaterThanOrEqual,

	// grouping
	openParen, closeParen, openBracket, closeBracket, openCurly, closedCurly,

	// This is the traditional oop dot notation operator
	dot,

	// boolean
	andd, orr, nott,
};

struct token_info {
	token_type Type;
	string Name;
};

struct token_stack {
	token_info Tokens[500];
	int32 TokensCount;
};

// This type has been streatched, should reall break it down into smaller pieces.
struct fluent {
	string Name;
	bool32 IsTrue;

	bool32 HasValue;
	real64 Value;

	/* If the fluent has arithmetic as a fluent value, then it is stored here.
		This is is only used when the fluent is part of an event events.
		The initial fluents are evaluated when they're parsed.
		Event fluents are evaluated when they're triggered, so they need to be saved and 'parsed' when they're triggered.
	*/
	token_info Arithmetic[20];
	int32 ArithCount;
};

void
PrintFluent(string InitialPrint, fluent* Fluent) {
	string ValueString = "";
	if (Fluent->HasValue) {
		ValueString = Fluent->Value;
	}

	Print(InitialPrint + Fluent->Name + " " + " " + ValueString);
}

struct entity {
	// For a decleration, this is the name. For an instance, this is also the name.
	string Name;
	string TypeName;

	fluent Fluents[100];
	int32 FluentsCount;
};

struct event {
	string Name;

	// NOTE these next two are used as queues. Zero is the front.
	token_info ConditionalTokens[100];
	int32 ConditionalsCount;

	token_info ConsquenceTokens[100];
	int32 NextConsqFluent;
};

struct method {
	string Name;

	string Usings[100];
	int32 UsingsCount;

	token_info DoesTokens[100];
	int32 DoesTokensCount;

	token_info IfTokens[100];
	int32 IfTokensCount;
};

// these are the rules of the game.
struct game_def {
	fluent* Fluents;
	int32 FluentsCount;

	event* Events;
	int32 EventsCount;

	event_bind* BoundEvents;
	int32 BoundCount;

	// These are instances
	entity* InstancedEntities;
	int32 InstancedEntitiesCount;

	// These are just the declarations
	entity* Entities;
	int32 EntitiesCount;

	method* Methods;
	int32 MethodsCount;
};

fluent*
FindFluentInList(string Name, fluent* Fluents, int32 FluentsCount) {
	for (int32 Index = 0; Index < FluentsCount; Index++) {
		if (Name == Fluents[Index].Name) {
			return (&Fluents[Index]);
		}
	}

	// Could not find that fluent, don't assert becuase sometimes it's normal not to find the fluent.
	return (NULL);
}


entity*
FindEntity(string Name, entity* Entities, int32 EntitiesCount) {
	for (int32 Index = 0; Index < EntitiesCount; Index++) {
		if (Name == Entities[Index].Name) {
			return (&Entities[Index]);
		}
	}

	Assert(0);
	return (NULL);
}

struct fluent_search_return {
	int32 TokensConsumed;
	fluent* Fluent;
};

// This handles dot notation, searching through everything to find the right fluent.
fluent_search_return
FluentSearch(token_info* Tokens, int32 ListStart, fluent* Fluents, int32 FluentsCount, entity* Entities, int32 EntitiesCount) {

	fluent_search_return Ret = {};

	if (Tokens[ListStart].Type == token_type::id &&
	        Tokens[ListStart + 1].Type == token_type::dot &&
	        Tokens[ListStart + 2].Type == token_type::id) {
		// pattern of ID DOT ID which means dot notation
		Ret.TokensConsumed = 3;
		entity* Entity = FindEntity(Tokens[ListStart].Name, Entities, EntitiesCount);
		if (Entity == NULL) {
			// Could not find that entity
			Assert(0);
		}
		fluent* Fluent = FindFluentInList(Tokens[ListStart + 2].Name, Entity->Fluents, Entity->FluentsCount);
		if (Fluent == NULL) {
			// Could not find that fluent in that entity
			Assert(0);
		}
		Ret.Fluent = Fluent;
	} else {
		//  didn't find dot notation, just going to search in the world fluents
		Ret.TokensConsumed = 1;
		fluent* Fluent = FindFluentInList(Tokens[ListStart].Name, Fluents, FluentsCount);
		if (Fluent == NULL) {
			// Could not find that fluent
			Assert(0);
		}
		Ret.Fluent = Fluent;
	}

	return (Ret);
}

// This converts a string into an array of tokens. Used entirley for the display layer
fluent_search_return
FluentSearch(string TokenString, fluent* Fluents, int32 FluentsCount, entity* Entities, int32 EntitiesCount) {
	token_info Tokens[5];
	int32 TokensCount = 0;

	char Word[100];
	ZeroMemory(&Word, ARRAY_SIZE(Word, char));
	int32 WordIndex = 0;

	for (int32 StringIndex = 0; StringIndex < StringLength(TokenString); StringIndex++) {
		if (TokenString.CharArray[StringIndex] == DotNotationChar) {
			Tokens[TokensCount].Type = token_type::id;
			Tokens[TokensCount].Name = Word;
			TokensCount++;

			ZeroMemory(&Word, ARRAY_SIZE(Word, char));
			WordIndex = 0;

			Tokens[TokensCount].Type = token_type::dot;
			TokensCount++;
		} else {
			Word[WordIndex] = TokenString.CharArray[StringIndex];
			WordIndex++;
		}
	}

	Tokens[TokensCount].Type = token_type::id;
	Tokens[TokensCount].Name = Word;

	return (FluentSearch(&Tokens[0], 0, Fluents, FluentsCount, Entities, EntitiesCount));
}

bool32
EvaluateBoolean(token_info* ConditionalTokens, int32 ConditionalsCount,
                int32* NextLogicOp,
                fluent* Fluents, int32 FluentsCount,
                entity* Entities, int32 EntitiesCount) {

	bool32 AccumBool = true;

	// This state must be held separate from the logicO
	token_type CurrLogicOp = token_type::none;

	bool32 NegateNext = false;

	while (*NextLogicOp < ConditionalsCount) {
		token_info* NextToken = &ConditionalTokens[*NextLogicOp];
		(*NextLogicOp)++;

		if (NextToken->Type == token_type::andd || NextToken->Type == token_type::orr) {
			CurrLogicOp = NextToken->Type;
		} else if (NextToken->Type == token_type::nott) {
			NegateNext = true;
		} else if (NextToken->Type == token_type::openBracket) {
			bool32 NextVal = EvaluateBoolean(ConditionalTokens, ConditionalsCount, NextLogicOp, Fluents, FluentsCount, Entities, EntitiesCount);

			if (NegateNext) {
				NegateNext = false;
				NextVal = !NextVal;
			}

			if (CurrLogicOp == token_type::andd) {
				AccumBool = AccumBool && NextVal;
			} else if (CurrLogicOp == token_type::orr) {
				AccumBool = AccumBool || NextVal;
			} else if (CurrLogicOp == token_type::none) {
				AccumBool = NextVal;
			}
		} else if (NextToken->Type == token_type::id || NextToken->Type == token_type::number) {
			// we found an id,

			// bool32 NextVal = GetConditionalBool(&Event->Conditionals[*NextConditional], Fluents, FluentsCount, Entities, EntitiesCount);
			bool32 NextVal = true;

			// evaluate the conditional
			(*NextLogicOp)--;

			// First Value
			real64 FirstVal = 0.0f;
			string FirstName = ConditionalTokens[*NextLogicOp].Name;
			if (StringIsInt(FirstName)) {
				FirstVal = StringToInt32(FirstName);
				(*NextLogicOp)++;
			} else {
				fluent_search_return Ret = FluentSearch(ConditionalTokens, *NextLogicOp,
				                                        Fluents, FluentsCount, Entities, EntitiesCount);
				(*NextLogicOp) += Ret.TokensConsumed;
				FirstVal = Ret.Fluent->Value;
			}

			// Check for operations. This needs to be significantly more robust.
			if (ConditionalTokens[*NextLogicOp].Type == token_type::sub ||
			        ConditionalTokens[*NextLogicOp].Type == token_type::add ||
			        ConditionalTokens[*NextLogicOp].Type == token_type::mult ||
			        ConditionalTokens[*NextLogicOp].Type == token_type::div) {

				token_type Operation = ConditionalTokens[*NextLogicOp].Type;
				(*NextLogicOp)++;

				int32 Accum = 0;
				if (StringIsInt(ConditionalTokens[*NextLogicOp].Name)) {
					Accum = StringToInt32(ConditionalTokens[*NextLogicOp].Name);
					(*NextLogicOp)++;
				} else {
					fluent_search_return Ret = FluentSearch(ConditionalTokens, *NextLogicOp,
					                                        Fluents, FluentsCount, Entities, EntitiesCount);
					(*NextLogicOp) += Ret.TokensConsumed;
					Accum = Ret.Fluent->Value;
				}

				if (Operation == token_type::sub) {
					FirstVal -= Accum;
				} else if (Operation == token_type::add) {
					FirstVal += Accum;
				} else if (Operation == token_type::div) {
					FirstVal /= Accum;
				} else if (Operation == token_type::mult) {
					FirstVal *= Accum;
				}
			}

			// Get operation
			token_type Op = ConditionalTokens[*NextLogicOp].Type;
			(*NextLogicOp)++;

			// Second Value
			real64 SecondVal = 0.0f;
			string SecondName = ConditionalTokens[*NextLogicOp].Name;
			if (StringIsInt(SecondName)) {
				SecondVal = StringToInt32(SecondName);
				(*NextLogicOp)++;
			} else {
				fluent_search_return Ret = FluentSearch(ConditionalTokens, *NextLogicOp,
				                                        Fluents, FluentsCount, Entities, EntitiesCount);
				(*NextLogicOp) += Ret.TokensConsumed;
				SecondVal = Ret.Fluent->Value;
			}

			if (Op == token_type::greaterThan) {
				NextVal = FirstVal > SecondVal;
			} else if (Op == token_type::lessThan) {
				NextVal = FirstVal < SecondVal;
			} else if (Op == token_type::lessThanOrEqual) {
				NextVal = FirstVal <= SecondVal;
			} else if (Op == token_type::greaterThanOrEqual) {
				NextVal = FirstVal >= SecondVal;
			} else if (Op == token_type::equalTo) {
				NextVal = FirstVal == SecondVal;
			} else {
				// That was not a valid logic operation. Throw an error.
				Assert(0);
			}

			if (NegateNext) {
				NegateNext = false;
				NextVal = !NextVal;
			}

			// accumualte the value of the conditional
			if (CurrLogicOp == token_type::andd) {
				AccumBool = AccumBool && NextVal;
			} else if (CurrLogicOp == token_type::orr) {
				AccumBool = AccumBool || NextVal;
			} else if (CurrLogicOp == token_type::none) {
				AccumBool = NextVal;
			}
		} else if (NextToken->Type == token_type::closeBracket) {
			return (AccumBool);
		} else {
			// There was a problem. Something didn't stack correctly
			Assert(0);
		}
	}

	return (AccumBool);
}

bool32
EventValid(token_info* ConditionalTokens, int32 ConditionalTokensCount, game_def* GameDef) {
	if (ConditionalTokensCount > 0) {
		int32 NextLogic = 0;
		return (EvaluateBoolean(ConditionalTokens, ConditionalTokensCount, &NextLogic,
		                        GameDef->Fluents, GameDef->FluentsCount,
		                        GameDef->InstancedEntities, GameDef->InstancedEntitiesCount));
	} else {
		// If there are no conditionals then event is always valid
		return (true);
	}
}

void
PrintOptions(game_def* Def) {
	// Print State
	{
		PrintHeader("STATE");
		// print raw fluents
		for (int32 Index = 0; Index < Def->FluentsCount; Index++) {
			PrintFluent("", &Def->Fluents[Index]);
		}
		// print instances
		for (int32 Index = 0; Index < Def->InstancedEntitiesCount; Index++) {
			entity* Entity = &Def->InstancedEntities[Index];

			string TypeName = "";
			if (Entity->TypeName != "") {
				TypeName = " is a " + Entity->TypeName;
			}

			Print(Entity->Name + TypeName);
			for (int32 FIndex = 0; FIndex < Entity->FluentsCount; FIndex++) {
				PrintFluent("\t", &Entity->Fluents[FIndex]);
			}
		}
		Print("\n");
	}

	// Print Actions
	{
		PrintHeader("POSSIBLE ACTIONS");
		for (int32 Index = 0; Index < Def->EventsCount; Index++) {
			if (EventValid(Def->Events[Index].ConditionalTokens, Def->Events[Index].ConditionalsCount, Def)) {
				string StrIndex = Index;
				Print(StrIndex + ") " + Def->Events[Index].Name);
			}
		}
		// Print("----------------------- ");
	}

	Print("\n \n----------------------- ");
	Print("Enter action number you wish.");
	Print("");
}

event*
FindEvent(string Name, event* Events, int32 EventsCount) {
	for (int32 Index = 0; Index < EventsCount; Index++) {
		if (Name == Events[Index].Name) {
			return (&Events[Index]);
		}
	}

	// Could not find that fluent, don't assert becuase sometimes it's normal not to find the fluent.
	return (NULL);
}

void
AddToken(parser* Parser, token_stack* TokenStack, token_type Type, string Name) {

	if (Type == token_type::id) {
		int32 x = 0;
	}

	TokenStack->Tokens[TokenStack->TokensCount].Type = Type;
	TokenStack->Tokens[TokenStack->TokensCount].Name = Name;
	TokenStack->TokensCount++;

	ResetParserWord(Parser);
}

void
CheckNumberOrID(parser* Parser, token_stack* Tokens) {
	if (Parser->WordOnIndex != 0) {
		if (StringIsInt(Parser->WordOn)) {
			AddToken(Parser, Tokens, token_type::number, Parser->WordOn);
		} else {
			AddToken(Parser, Tokens, token_type::id, Parser->WordOn);
		}
	}
}

token_stack
CreateTokens() {
	token_stack Tokens = {};

	parser Parser = {};
	ResetParserWord(&Parser);

	// Parser.CharOn = LoadProg("T:/Games/ShieldPush/ShieldPush.sc");
	prog_data ProgData = LoadProg("T:/Games/Cavern2.sc");
	Parser.CharOn = ProgData.Characters;

	bool32 InCommentMode = false;

	string ActionLiteral = "action";
	string MethodLiteral = "method";
	string UsingLiteral = "using";
	string DoesLiteral = "does";
	string IfLiteral = "if";
	string CommentLiteral = "//";
	string BindLiteral = "bind";
	string EntityLiteral = "entity";
	string InstanceLiteral = "instance";
	string IsLiteral = "is";
	string WhenLiteral = "when";
	string GreaterThanOrEqualLiteral = ">=";
	string LessThanOrEqualLiteral = "<=";
	string AndLiteral = "and";
	string OrLiteral = "or";
	string NotLiteral = "not";

	for (int32 Index = 0; Index < ProgData.CharactersCount; Index++) {

		char ThisChar = *Parser.CharOn;

		if (ThisChar == NULL) {
			break;
		}

		// ignore comments
		if (InCommentMode) {
			if (ThisChar == '\n') {
				InCommentMode = false;
			}
			Parser.CharOn++;
			continue;
		}

#define ADDTOKEN(token) AddToken(&Parser, &Tokens, token, Parser.WordOn);

		//NOTE order of these ifs is very important
		if (ThisChar == NULL) {
			// end of program
			break;
		} else if (Parser.WordOn == NotLiteral) {
			ADDTOKEN(token_type::nott);
		} else if (Parser.WordOn == OrLiteral) {
			ADDTOKEN(token_type::orr);
		} else if (Parser.WordOn == DoesLiteral) {
			ADDTOKEN(token_type::does);
		} else if (Parser.WordOn == IfLiteral) {
			ADDTOKEN(token_type::iff);
		} else if (Parser.WordOn == BindLiteral) {
			ADDTOKEN(token_type::bind);
		} else if (Parser.WordOn == UsingLiteral) {
			ADDTOKEN(token_type::usingg);
		} else if (Parser.WordOn == MethodLiteral) {
			ADDTOKEN(token_type::method);
		} else if (Parser.WordOn == AndLiteral) {
			ADDTOKEN(token_type::andd);
		} else if (Parser.WordOn == ActionLiteral) {
			ADDTOKEN(token_type::action);
		} else if (Parser.WordOn == GreaterThanOrEqualLiteral) {
			ADDTOKEN(token_type::greaterThanOrEqual);
		} else if (Parser.WordOn == LessThanOrEqualLiteral) {
			ADDTOKEN(token_type::lessThanOrEqual);
		} else if (Parser.WordOnIndex != 0 && StringIsInt(Parser.WordOn) && !CharIsInt(ThisChar) && ThisChar == ')') {
			ADDTOKEN(token_type::number);
		}

		if (Parser.WordOnIndex != 0 && (ThisChar == ' ' || ThisChar == '.') && Parser.WordOn != CommentLiteral) {
			CheckNumberOrID(&Parser, &Tokens);
		}

		if (ThisChar == '.') {
			ADDTOKEN(token_type::period);
		} else if (ThisChar == ':') {
			CheckNumberOrID(&Parser, &Tokens);
			ADDTOKEN(token_type::colon);
		} else if (ThisChar == ',') {
			CheckNumberOrID(&Parser, &Tokens);
			ADDTOKEN(token_type::comma);
		} else if (ThisChar == '!') {
			InCommentMode = true;
		} else if (ThisChar == '=') {
			ADDTOKEN(token_type::equalTo);
		} else if (ThisChar == '<') {
			ADDTOKEN(token_type::lessThan);
		} else if (ThisChar == '>') {
			ADDTOKEN(token_type::greaterThan);
		} else if (ThisChar == '-') {
			CheckNumberOrID(&Parser, &Tokens);
			ADDTOKEN(token_type::sub);
		} else if (ThisChar == '+') {
			CheckNumberOrID(&Parser, &Tokens);
			ADDTOKEN(token_type::add);
		} else if (ThisChar == '/') {
			CheckNumberOrID(&Parser, &Tokens);
			ADDTOKEN(token_type::div);
		} else if (ThisChar == '*') {
			CheckNumberOrID(&Parser, &Tokens);
			ADDTOKEN(token_type::mult);
		} else if (ThisChar == ')') {
			CheckNumberOrID(&Parser, &Tokens);
			ADDTOKEN(token_type::closeParen);
		} else if (ThisChar == '(') {
			CheckNumberOrID(&Parser, &Tokens);
			ADDTOKEN(token_type::openParen);
		} else if (ThisChar == ']') {
			CheckNumberOrID(&Parser, &Tokens);
			ADDTOKEN(token_type::closeBracket);
		} else if (ThisChar == '[') {
			CheckNumberOrID(&Parser, &Tokens);
			ADDTOKEN(token_type::openBracket);
		} else if (ThisChar == '}') {
			CheckNumberOrID(&Parser, &Tokens);
			ADDTOKEN(token_type::closedCurly);
		} else if (ThisChar == '{') {
			CheckNumberOrID(&Parser, &Tokens);
			ADDTOKEN(token_type::openCurly);
		} else if (ThisChar == DotNotationChar) {
			CheckNumberOrID(&Parser, &Tokens);
			ADDTOKEN(token_type::dot);
		} else if (ThisChar == '\r') {
			CheckNumberOrID(&Parser, &Tokens);
		} else if (*Parser.CharOn != ' ' && ThisChar != '\r' && ThisChar != '\t' && ThisChar != '\n') {
			Parser.WordOn[Parser.WordOnIndex] = *Parser.CharOn;
			Parser.WordOnIndex++;
		}

		Parser.CharOn++;
	}

	return (Tokens);
}

real64
GetTokenValue(token_info Token, fluent* Fluents, int32 FluentsCount) {
	if (Token.Type == token_type::id) {
		// find that fluent
		fluent* Fluent = FindFluentInList(Token.Name, Fluents, FluentsCount);
		if (Fluent == NULL) {
			// Report error here. That fluent name does not exist.
			Assert(0);
		}
		return (Fluent->Value);

	} else if (Token.Type == token_type::number) {
		return (StringToInt32(Token.Name));
	} else {
		Assert(0);
		// Some kind of error, something unexpected.
	}

	// Shouldn't get down here
	Assert(0);
	return (0);
}

real64
InfixAccumulate(token_info* Tokens, int32 TokenIndexStart,
                fluent* Fluents, int32 FluentsCount,
                entity* Entities, int32 EntitiesCount) {
	token_type AccumState = token_type::none;

	int32 Accum = 0.0f;

	int32 AccumIndex = TokenIndexStart;
	while (Tokens[AccumIndex].Type != token_type::closeParen && Tokens[AccumIndex].Type != token_type::none) {
		token_type NewTokenType = Tokens[AccumIndex].Type;
		if (NewTokenType == token_type::id || NewTokenType == token_type::number) {

			real64 NewOperand = 0.0f;
			if (Tokens[AccumIndex + 1].Type == token_type::dot) {
				entity* Event = FindEntity(Tokens[AccumIndex].Name, Entities, EntitiesCount);
				if (Event == NULL) {
					// Error here. Could not find that event name.
					Assert(0);
				}
				fluent* Fluent = FindFluentInList(Tokens[AccumIndex + 2].Name, Event->Fluents, Event->FluentsCount);
				if (Fluent == NULL) {
					// Error here. Could not find that fluent in the event.
					Assert(0);
				}

				NewOperand = Fluent->Value;
				AccumIndex += 2;
			} else {
				NewOperand = GetTokenValue(Tokens[AccumIndex], Fluents, FluentsCount);
			}

			if (AccumState == token_type::add) {
				Accum = Accum + NewOperand;
			} else if (AccumState == token_type::sub) {
				Accum = Accum - NewOperand;
			} else if (AccumState == token_type::mult) {
				Accum = Accum * NewOperand;
			} else if (AccumState == token_type::div) {
				Accum = Accum / NewOperand;
			} else {
				Accum = NewOperand;
			}
		} else if (NewTokenType == token_type::sub ||
		           NewTokenType == token_type::add ||
		           NewTokenType == token_type::mult ||
		           NewTokenType == token_type::div) {
			AccumState = NewTokenType;
		} else {
			// Something wrong
			Assert(0);
		}

		AccumIndex++;
	}

	return (Accum);
}

fluent
GrabFluent(token_info* Tokens, int32 StackStart, bool32 Evaluate,
           fluent* Fluents, int32 FluentsCount, entity* Entities, int32 EntitiesCount) {
	fluent FluentAdding = {};
	FluentAdding.HasValue = true;
	FluentAdding.Name = Tokens[StackStart].Name;

	int32 AccumIndex = StackStart + 2;

	if (Evaluate) {
		FluentAdding.Value = InfixAccumulate(Tokens, AccumIndex, Fluents, FluentsCount, Entities, EntitiesCount);
	} else {
		while (Tokens[AccumIndex].Type != token_type::closeParen) {
			FluentAdding.Arithmetic[FluentAdding.ArithCount] = Tokens[AccumIndex];
			FluentAdding.ArithCount++;
			AccumIndex++;
		}
	}

	return (FluentAdding);
}

game_def
LoadGameDefinition() {
	game_def GameDefinition = {};

	// TODO at some point make sure we don't exceede the maximum of any of these
	int32 MaxCount = 100;
	GameDefinition.Fluents = (fluent*)malloc(MaxCount * sizeof(fluent));
	GameDefinition.Events = (event*)malloc(MaxCount * sizeof(event));
	GameDefinition.BoundEvents = (event_bind*)malloc(MaxCount * sizeof(event_bind));
	GameDefinition.Entities = (entity*)malloc(MaxCount * sizeof(entity));
	GameDefinition.InstancedEntities = (entity*)malloc(MaxCount * sizeof(entity));
	GameDefinition.Methods = (method*)malloc(MaxCount * sizeof(method));

	ZeroMemory(GameDefinition.Fluents, sizeof(fluent) * MaxCount);
	ZeroMemory(GameDefinition.Events, sizeof(event) * MaxCount);
	ZeroMemory(GameDefinition.BoundEvents, sizeof(event_bind) * MaxCount);
	ZeroMemory(GameDefinition.Entities, sizeof(entity) * MaxCount);
	ZeroMemory(GameDefinition.InstancedEntities, sizeof(entity) * MaxCount);
	ZeroMemory(GameDefinition.Methods, sizeof(method) * MaxCount);

	// lexer and parser system.
	{
		// Create all the tokens
		token_stack Tokens = CreateTokens();

		// go throught the tokens. Filling the data structures with the stuff.
		{
			// a statement is everything before a period.
			int32 StatementStart = 0;
			bool32 HasColon = false;
			bool32 HasDoes = false;

#define RESET StatementStart = Index + 1; HasDoes = false; HasColon = false; continue;

			// This moves along, grabbing statements
			for (int32 Index = 0; Index < Tokens.TokensCount; Index++) {
				token_info NextToken = Tokens.Tokens[Index];

				if (NextToken.Type == token_type::colon) {
					HasColon = true;
				} else if (NextToken.Type == token_type::does) {
					HasDoes = true;
				}

				if (NextToken.Type == token_type::period) {
					int32 StatementEnd = Index - 1;

					if (Tokens.Tokens[StatementStart].Type == token_type::id &&
					        Tokens.Tokens[StatementStart + 1].Type == token_type::openParen &&
					        Tokens.Tokens[StatementEnd].Type == token_type::closeParen) {
						// Fluent with value statement

						GameDefinition.Fluents[GameDefinition.FluentsCount] =
						    GrabFluent(Tokens.Tokens, StatementStart, true,
						               GameDefinition.Fluents, GameDefinition.FluentsCount,
						               GameDefinition.InstancedEntities, GameDefinition.InstancedEntitiesCount);

						GameDefinition.FluentsCount++;
						RESET
					} else if (Tokens.Tokens[StatementStart].Type == token_type::id &&
					           Tokens.Tokens[StatementStart + 1].Type == token_type::dot &&
					           Tokens.Tokens[StatementEnd].Type == token_type::closeParen) {
						// This is setting or creating a fluent within an instance

						string EntityName = Tokens.Tokens[StatementStart].Name;
						string FluentName = Tokens.Tokens[StatementStart + 2].Name;

						entity* Entity = FindEntity(EntityName, GameDefinition.InstancedEntities, GameDefinition.InstancedEntitiesCount);
						if (Entity == NULL) {
							// That entity name does not exist. Report error.
							Assert(0);
						}

						fluent* Fluent = FindFluentInList(FluentName, Entity->Fluents, Entity->FluentsCount);
						if (Fluent != NULL) {
							// modify the existing one
							Fluent->Value = GrabFluent(Tokens.Tokens, StatementStart + 2, true, Entity->Fluents, Entity->FluentsCount,
							                           GameDefinition.InstancedEntities, GameDefinition.InstancedEntitiesCount).Value;
						} else {
							// add a new one
							Entity->Fluents[Entity->FluentsCount] = GrabFluent(Tokens.Tokens, StatementStart + 2, true, Entity->Fluents, Entity->FluentsCount,
							                                        GameDefinition.InstancedEntities, GameDefinition.InstancedEntitiesCount);
							Entity->FluentsCount++;
						}


						RESET
					} else if (Tokens.Tokens[StatementStart].Type == token_type::id &&
					           Tokens.Tokens[StatementStart + 1].Type == token_type::period) {
						// Fluent without value

						fluent *FluentAdding = &GameDefinition.Fluents[GameDefinition.FluentsCount];
						FluentAdding->Name = Tokens.Tokens[StatementStart].Name;
						FluentAdding->HasValue = false;

						GameDefinition.FluentsCount++;

						RESET
					} else if (HasDoes && Tokens.Tokens[StatementStart].Type == token_type::action) {
						// Action / event

						event* Event = &GameDefinition.Events[GameDefinition.EventsCount];
						GameDefinition.EventsCount++;

						Event->Name = Tokens.Tokens[StatementStart + 2].Name;

						int32 TI = StatementStart + 5;
						// Grab the state to change
						{
							while (Tokens.Tokens[TI].Type != token_type::iff && Tokens.Tokens[TI].Type != token_type::period) {
								Event->ConsquenceTokens[Event->NextConsqFluent] = Tokens.Tokens[TI];
								Event->NextConsqFluent++;
								TI++;
							}
						}

						// only get the conditionals if they exist
						if (Tokens.Tokens[TI].Type != token_type::period) {
							// Move past the colon
							TI++;

							// Grab the conditionals
							{
								while (Tokens.Tokens[TI].Type != token_type::period) {
									Event->ConditionalTokens[Event->ConditionalsCount] = Tokens.Tokens[TI];
									Event->ConditionalsCount++;
									TI++;
								}
							}
						}

						RESET
					} else if (Tokens.Tokens[StatementStart].Type == token_type::method) {
						// Method

						GameDefinition.Methods[GameDefinition.MethodsCount] = {};

						method* NextMethod = &GameDefinition.Methods[GameDefinition.MethodsCount];
						GameDefinition.MethodsCount++;
						NextMethod->Name = Tokens.Tokens[StatementStart + 1].Name;

						int32 UsingOffset = 3;

						// grab the using strings
						if (Tokens.Tokens[StatementStart + UsingOffset - 1].Type == token_type::usingg) {
							while (Tokens.Tokens[StatementStart + UsingOffset].Type != token_type::does) {
								if (Tokens.Tokens[StatementStart + UsingOffset].Type == token_type::id) {
									NextMethod->Usings[NextMethod->UsingsCount] = Tokens.Tokens[StatementStart + UsingOffset].Name;
									NextMethod->UsingsCount++;
								}
								UsingOffset++;
							}

							UsingOffset++;
						}


						// grab the does tokens
						while (Tokens.Tokens[StatementStart + UsingOffset].Type != token_type::iff &&
						        Tokens.Tokens[StatementStart + UsingOffset].Type != token_type::period) {
							NextMethod->DoesTokens[NextMethod->DoesTokensCount] = Tokens.Tokens[StatementStart + UsingOffset];
							NextMethod->DoesTokensCount++;
							UsingOffset++;
						}

						// grab the if tokens
						if (Tokens.Tokens[StatementStart + UsingOffset].Type == token_type::iff) {
							UsingOffset++;

							while (Tokens.Tokens[StatementStart + UsingOffset].Type != token_type::period) {
								NextMethod->IfTokens[NextMethod->IfTokensCount] = Tokens.Tokens[StatementStart + UsingOffset];
								NextMethod->IfTokensCount++;
								UsingOffset++;
							}
						}

						RESET
					} else if (Tokens.Tokens[StatementStart].Type == token_type::bind) {
						// Key binding

						int32 TI = StatementStart + 2;
						event_bind* Binding = &GameDefinition.BoundEvents[GameDefinition.BoundCount];
						GameDefinition.BoundCount++;

						// Get key
						Binding->KeyName = Tokens.Tokens[TI].Name;
						TI++;

						// Get events
						while (Tokens.Tokens[TI].Type != token_type::closeParen) {
							TI++;
							Binding->EventNames[Binding->NextEventName] = Tokens.Tokens[TI].Name;
							Binding->NextEventName++;
							TI++;
						}

						RESET
					} else if (HasColon && Tokens.Tokens[StatementEnd].Type == token_type::closeBracket) {
						// Entity

						// These hold which entities we're now instancing
						int32 EntityListStart = GameDefinition.InstancedEntitiesCount;
						int32 EntitiyListEnd = GameDefinition.InstancedEntitiesCount;

						int32 TI = StatementStart;
						while (Tokens.Tokens[TI].Type != token_type::colon) {
							if (Tokens.Tokens[TI].Type == token_type::id) {
								// Add a new instanced entity
								EntitiyListEnd++;

								GameDefinition.InstancedEntities[GameDefinition.InstancedEntitiesCount].Name = Tokens.Tokens[TI].Name;
								GameDefinition.InstancedEntitiesCount++;

							} else if (Tokens.Tokens[TI].Type == token_type::comma) {
								// just ignore the comma, but it's still valid
							} else {
								// Something unexpected before the colon.
								// For entities, only names can come before the colon.
								Assert(0);
							}

							TI++;
						}

						// Get the type name
						string TypeName = "";
						if (Tokens.Tokens[TI + 1].Type == token_type::id) {
							TypeName = Tokens.Tokens[TI + 1].Name;
							TI += 3;
						} else {
							// Move past the opening bracket
							TI += 2;
						}

						// Fill the entities with the right fluents
						while (true) {

							// Get the next fluent, and set all the instanced entities
							fluent NextFluent = GrabFluent(Tokens.Tokens, TI, true, GameDefinition.Fluents, GameDefinition.FluentsCount,
							                               GameDefinition.InstancedEntities, GameDefinition.InstancedEntitiesCount);

							for (int32 Index = EntityListStart; Index < EntitiyListEnd; Index++) {
								GameDefinition.InstancedEntities[Index].TypeName = TypeName;
								GameDefinition.InstancedEntities[Index].Fluents[GameDefinition.InstancedEntities[Index].FluentsCount] = NextFluent;
								GameDefinition.InstancedEntities[Index].FluentsCount++;
							}

							// Move TI until we hit the next fluent
							while (Tokens.Tokens[TI].Type != token_type::comma && Tokens.Tokens[TI].Type != token_type::period) {
								TI++;
							}

							// If we at the end of the entity, then break out
							if (Tokens.Tokens[TI].Type == token_type::period) {
								break;
							} else {
								// move past the comma
								TI++;
							}
						}

						RESET
					}
				}
			}
		}
	}

	return (GameDefinition);
}

LRESULT CALLBACK
Win32MainWindowCallback(HWND Window,
                        UINT Message,
                        WPARAM WParam,
                        LPARAM LParam)
{
	LRESULT Result = 0;

	switch (Message)
	{
	case WM_CLOSE:
	{
		GlobalRunning = false;
		Result = DefWindowProcA(Window, Message, WParam, LParam);
	}
	break;

	case WM_DESTROY:
	{
		GlobalRunning = false;
		Result = DefWindowProcA(Window, Message, WParam, LParam);
	}
	break;

	default:
	{
		Result = DefWindowProcA(Window, Message, WParam, LParam);
	}
	break;
	}

	return (Result);
}

struct button {
	string Name;
	bool32 IsDown;
};

union controls {
	button AllControls[10];

	struct {
		button UpArrow;
		button DownArrow;
		button LeftArrow;
		button RightArrow;
		button W;
		button A;
		button S;
		button D;
		button Q;
		button E;
	} Controls;
};

void
UpdateControls(controls * Controls, HWND Window) {
	MSG WindowMessage = {};
	while (PeekMessage(&WindowMessage, Window,  0, 0, PM_REMOVE))
	{
		switch (WindowMessage.message)
		{

		case (WM_KEYDOWN):
		{
			switch (WindowMessage.wParam)
			{
			case (VK_LEFT): Controls->Controls.LeftArrow.IsDown = true; break;
			case (VK_RIGHT): Controls->Controls.RightArrow.IsDown = true; break;
			case (VK_UP): Controls->Controls.UpArrow.IsDown = true; break;
			case (VK_DOWN): Controls->Controls.DownArrow.IsDown = true; break;
			case (0x57): Controls->Controls.W.IsDown = true; break;
			case (0x41): Controls->Controls.A.IsDown = true; break;
			case (0x53): Controls->Controls.S.IsDown = true; break;
			case (0x44): Controls->Controls.D.IsDown = true; break;
			case (0x45): Controls->Controls.E.IsDown = true; break;
			case (0x51): Controls->Controls.Q.IsDown = true; break;
			}
		}
		break;

		case (WM_KEYUP):
		{
			switch (WindowMessage.wParam)
			{
			case (VK_LEFT): Controls->Controls.LeftArrow.IsDown = false; break;
			case (VK_RIGHT): Controls->Controls.RightArrow.IsDown = false; break;
			case (VK_UP): Controls->Controls.UpArrow.IsDown = false; break;
			case (VK_DOWN): Controls->Controls.DownArrow.IsDown = false; break;
			case (0x57): Controls->Controls.W.IsDown = false; break;
			case (0x41): Controls->Controls.A.IsDown = false; break;
			case (0x53): Controls->Controls.S.IsDown = false; break;
			case (0x44): Controls->Controls.D.IsDown = false; break;
			case (0x45): Controls->Controls.E.IsDown = false; break;
			case (0x51): Controls->Controls.Q.IsDown = false; break;
			}
		}
		break;

		default:
		{
			TranslateMessage(&WindowMessage);
			DispatchMessage(&WindowMessage);
		}
		break;
		}
	}

}

void
TokensChangeState(token_info* Tokens, int32 TokensCount, game_def * GameDef) {
	int32 TokenIndex = 0;
	while (TokenIndex < TokensCount) {

		if (Tokens[TokenIndex].Type == token_type::id &&
		        Tokens[TokenIndex + 1].Type == token_type::dot &&
		        Tokens[TokenIndex + 2].Type == token_type::id) {
			// this is dot notation

			string EntityName = Tokens[TokenIndex].Name;
			string FluentName = Tokens[TokenIndex + 2].Name;

			entity* Entity = FindEntity(EntityName, GameDef->InstancedEntities, GameDef->InstancedEntitiesCount);
			if (Entity == NULL) {
				// That entity name does not exist. Report error.
				Assert(0);
			}

			fluent* Fluent = FindFluentInList(FluentName, Entity->Fluents, Entity->FluentsCount);
			if (Fluent != NULL) {
				// modify the existing one
				Fluent->Value = GrabFluent(Tokens, TokenIndex + 2, true, Entity->Fluents, Entity->FluentsCount,
				                           GameDef->InstancedEntities, GameDef->InstancedEntitiesCount).Value;
			} else {
				// add a new one
				Entity->Fluents[Entity->FluentsCount] = GrabFluent(Tokens, TokenIndex + 2, true,
				                                        Entity->Fluents, Entity->FluentsCount,
				                                        GameDef->InstancedEntities, GameDef->InstancedEntitiesCount);
				Entity->FluentsCount++;
			}

			// Move TokenIndex
			while (Tokens[TokenIndex].Type != token_type::comma &&
			        Tokens[TokenIndex].Type != token_type::iff &&
			        TokenIndex < TokensCount) {
				TokenIndex++;
			}
			TokenIndex++;

			continue;
		} else if (Tokens[TokenIndex].Type == token_type::id &&
		           Tokens[TokenIndex + 1].Type == token_type::openParen) {
			// this is a regular one

			// Change that fluent state if it exists
			fluent* F = FindFluentInList(Tokens[TokenIndex].Name, GameDef->Fluents, GameDef->FluentsCount);
			if (F != NULL) {

				*F = GrabFluent(Tokens, TokenIndex, true,
				                GameDef->Fluents, GameDef->FluentsCount,
				                GameDef->InstancedEntities, GameDef->InstancedEntitiesCount);
			} else {
				GameDef->Fluents[GameDef->FluentsCount] =
				    GrabFluent(Tokens, TokenIndex, true,
				               GameDef->Fluents, GameDef->FluentsCount,
				               GameDef->InstancedEntities, GameDef->InstancedEntitiesCount);
				GameDef->FluentsCount++;
			}

			// Move TokenIndex until next comma
			while (Tokens[TokenIndex].Type != token_type::comma  &&
			        Tokens[TokenIndex].Type != token_type::iff &&
			        TokenIndex < TokensCount) {
				TokenIndex++;
			}
			TokenIndex++;

			continue;
		} else if (Tokens[TokenIndex].Type == token_type::id &&
		           Tokens[TokenIndex + 1].Type == token_type::openCurly) {
			// This is a method

			// Find the method to do
			method* MethodDoing = {};
			for (int32 Index = 0; Index < GameDef->MethodsCount; Index++) {
				if (GameDef->Methods[Index].Name == Tokens[TokenIndex].Name) {
					MethodDoing = &GameDef->Methods[Index];
				}
			}
			if (MethodDoing == NULL) {
				// Could not find that method. Error here
				Assert(0);
			}

			Assert(MethodDoing->UsingsCount < 50);
			string ParamReals[50];
			ZeroMemory(ParamReals, 50 * sizeof(string));
			for (int32 ParamIndex = 0; ParamIndex < MethodDoing->UsingsCount; ParamIndex++) {
				ParamReals[ParamIndex] = Tokens[TokenIndex + 2 + (ParamIndex * 2)].Name;
			}

			// string FirstParamReal = Tokens[TokenIndex + 2].Name;

			token_info ReplacedTokens[100];
			for (int32 Index = 0; Index < MethodDoing->IfTokensCount; Index++) {
				ReplacedTokens[Index] = MethodDoing->IfTokens[Index];

				// Check for replacement
				for (int32 ParamIndex = 0; ParamIndex < MethodDoing->UsingsCount; ParamIndex++) {
					if (ReplacedTokens[Index].Name == MethodDoing->Usings[ParamIndex]) {
						ReplacedTokens[Index].Name = ParamReals[ParamIndex];
						break;
					}
				}
			}

			// first check if the method is valid
			int32 NextLogicOp = 0;
			if (EvaluateBoolean(ReplacedTokens, MethodDoing->IfTokensCount, &NextLogicOp,
			                    GameDef->Fluents, GameDef->FluentsCount,
			                    GameDef->InstancedEntities, GameDef->InstancedEntitiesCount)) {
				// Event is valid, so do it
				token_info ReplacedDoesTokens[100];
				for (int32 Index = 0; Index < MethodDoing->DoesTokensCount; Index++) {
					ReplacedDoesTokens[Index] = MethodDoing->DoesTokens[Index];

					// Check for replacement
					for (int32 ParamIndex = 0; ParamIndex < MethodDoing->UsingsCount; ParamIndex++) {
						if (ReplacedDoesTokens[Index].Name == MethodDoing->Usings[ParamIndex]) {
							ReplacedDoesTokens[Index].Name = ParamReals[ParamIndex];
							break;
						}
					}
				}

				TokensChangeState(ReplacedDoesTokens, MethodDoing->DoesTokensCount, GameDef);
				int32 x = 0;
			}

			// Move token index past the curly
			while (Tokens[TokenIndex].Type != token_type::closedCurly) {
				TokenIndex++;
			}
			TokenIndex++;

			// Move past comma, only if it's there
			if (Tokens[TokenIndex].Type == token_type::comma) {
				TokenIndex++;
			}

			continue;
		} else {
			// This method changes state when given tokens. We could not find a pattern of those tokens which made sense.
			Assert(0);
		}
	}
}

#ifdef USING_OPENGL
int CALLBACK
WinMain(HINSTANCE Instance,
        HINSTANCE PrevInstance,
        LPSTR CommandLine,
        int ShowCode)
{
	// NOTE this is the virtual pixel size. The actual pixel size of the window is different.
	int32 WindowWidth = 1920 * 0.8f;
	int32 WindowHeight = 1080 * 0.8f;

	real32 WindowSizeAdjustment = 100.0f;

	WNDCLASSA WindowClass = {};

	WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc = Win32MainWindowCallback;
	WindowClass.hInstance = Instance;
	WindowClass.hCursor = LoadCursor(0, IDC_ARROW);
	//    WindowClass.hIcon;
	WindowClass.lpszClassName = "TowerWindowClass";
	if (RegisterClassA(&WindowClass))
	{
		HWND Window =
		    CreateWindowExA(
		        0, // WS_EX_TOPMOST|WS_EX_LAYERED,
		        WindowClass.lpszClassName,
		        "SandCat",
		        WS_VISIBLE,
		        // WS_MAXIMIZE | WS_VISIBLE,
		        0, 0,
		        WindowWidth, WindowHeight,
		        0,
		        0,
		        Instance,
		        0);
		if (Window)
		{
			// Init open gl
			HDC DeviceContext = GetDC(Window);

			PIXELFORMATDESCRIPTOR DesiredPixelFormat = {};
			DesiredPixelFormat.nSize = sizeof(DesiredPixelFormat);
			DesiredPixelFormat.nVersion = 1;
			DesiredPixelFormat.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
			DesiredPixelFormat.cColorBits = 24;
			DesiredPixelFormat.cAlphaBits = 8;
			DesiredPixelFormat.cStencilBits = 8;
			DesiredPixelFormat.iLayerType = PFD_MAIN_PLANE;

			int SuggestedPixelFormatIndex = ChoosePixelFormat(DeviceContext, &DesiredPixelFormat);
			PIXELFORMATDESCRIPTOR SuggestedPixelFormat = {};
			DescribePixelFormat(DeviceContext, SuggestedPixelFormatIndex, sizeof(SuggestedPixelFormat), &SuggestedPixelFormat);
			SetPixelFormat(DeviceContext, SuggestedPixelFormatIndex, &SuggestedPixelFormat);

			HGLRC OpenGLRC = wglCreateContext(DeviceContext);
			if (!wglMakeCurrent(DeviceContext, OpenGLRC))
			{
				Assert(0);
				// TODO diagnostic
			}

			ReleaseDC(Window, DeviceContext);

			RECT ScreenSize;
			GetWindowRect(Window, &ScreenSize);
			window_info WindowInfo = {};
			WindowInfo.Width = ScreenSize.right - 15;
			WindowInfo.Height = ScreenSize.bottom - 40;

			// monitor is 105
			// device is 538
			// WindowInfo.Width = (uint32)(VirtualWidth / WindowSizeAdjustment);
			// WindowInfo.Height = (uint32)(VirtualHeight / WindowSizeAdjustment);
			// SetWindowPos(Window, HWND_TOP, 0, 0, WindowInfo.Width, WindowInfo.Height, NULL);

			render_layer RenderObjects[3];

			game_def GameDefinition = LoadGameDefinition();
			controls Controls = {};
			Controls.Controls.UpArrow.Name = "UpArrow";
			Controls.Controls.DownArrow.Name = "DownArrow";
			Controls.Controls.LeftArrow.Name = "LeftArrow";
			Controls.Controls.RightArrow.Name = "RightArrow";
			Controls.Controls.W.Name = "W";
			Controls.Controls.A.Name = "A";
			Controls.Controls.S.Name = "S";
			Controls.Controls.D.Name = "D";
			Controls.Controls.Q.Name = "Q";
			Controls.Controls.E.Name = "E";

			bool32 CanDoAction = true;

			GlobalRunning = true;
			while (GlobalRunning)
			{
				UpdateControls(&Controls, Window);

				// Game stuff

				// reset all the lists
				{
					for (uint32 LayerIndex = 0;  LayerIndex < RenderLayer_Count; LayerIndex++) {
						RenderObjects[LayerIndex].EntitiesCount = 0;
					}
				}

				//all rendering stuff needs to happen in here
				{
					// This is for ShieldPush2.sc
					if (false) {
						vector2 GridTopLeft = vector2{50.0f, 50.0f};
						real64 GridCellSize = 50;
						// int32 GridCellCount  = FluentSearch("F", GameDefinition.Fluents, GameDefinition.FluentsCount,
						// GameDefinition.Entities, GameDefinition.EntitiesCount)->Value;

						int32 GridCellCount = FluentSearch("GridMax'y", GameDefinition.Fluents, GameDefinition.FluentsCount,
						                                   GameDefinition.InstancedEntities, GameDefinition.InstancedEntitiesCount).Fluent->Value;


						// square grid outline
						{
							real32 Grey = 0.8f;
							real64 SquareSide = GridCellSize * GridCellCount;
							gl_square GridOutline = MakeGLSquare(GridTopLeft + vector2{SquareSide / 2.0f, SquareSide / 2.0f}, SquareSide + GridCellSize, color{Grey, Grey, Grey, 1.0f});
							PushRenderSquare(&RenderObjects[0], GridOutline);
						}

						// if (FindFluent("V_BlockX", GameDefinition.Fluents, GameDefinition.FluentsCount,
						// GameDefinition.Entities, GameDefinition.EntitiesCount) != NULL) {
						{
							vector2 BlockPos = {FluentSearch("BlockPos'x", GameDefinition.Fluents, GameDefinition.FluentsCount,
							                                 GameDefinition.InstancedEntities, GameDefinition.InstancedEntitiesCount).Fluent->Value,
							                    FluentSearch("BlockPos'y", GameDefinition.Fluents, GameDefinition.FluentsCount,
							                                 GameDefinition.InstancedEntities, GameDefinition.InstancedEntitiesCount).Fluent->Value
							                   };

							vector2 ScreenPos = (BlockPos * vector2{GridCellSize, GridCellSize}) + GridTopLeft;
							gl_square GridOutline = MakeGLSquare(ScreenPos, GridCellSize, COLOR_BLACK);
							PushRenderSquare(&RenderObjects[0], GridOutline);
						}
						// }

						// if (FindFluent("H_BlockX", GameDefinition.Fluents, GameDefinition.FluentsCount,
						//                GameDefinition.Entities, GameDefinition.EntitiesCount) != NULL) {
						// 	vector2 BlockPos = {FindFluent("H_BlockX", GameDefinition.Fluents, GameDefinition.FluentsCount,
						// 	                               GameDefinition.Entities, GameDefinition.EntitiesCount)->Value,
						// 	                    FindFluent("H_BlockY", GameDefinition.Fluents, GameDefinition.FluentsCount,
						// 	                               GameDefinition.Entities, GameDefinition.EntitiesCount)->Value
						// 	                   };

						// 	vector2 ScreenPos = (BlockPos * vector2{GridCellSize, GridCellSize}) + GridTopLeft;
						// 	gl_square GridOutline = MakeGLSquare(ScreenPos, GridCellSize, COLOR_BLACK);
						// 	PushRenderSquare(&RenderObjects[0], GridOutline);
						// }

						// if (FindFluent("PlayerHoldingShield", GameDefinition.Fluents, GameDefinition.FluentsCount,
						//                GameDefinition.Entities, GameDefinition.EntitiesCount) == NULL) {
						// 	vector2 ShieldPos = {FindFluent("ShieldPosX", GameDefinition.Fluents, GameDefinition.FluentsCount,
						// 	                                GameDefinition.Entities, GameDefinition.EntitiesCount)->Value,
						// 	                     FindFluent("ShieldPosY", GameDefinition.Fluents, GameDefinition.FluentsCount,
						// 	                                GameDefinition.Entities, GameDefinition.EntitiesCount)->Value
						// 	                    };

						// 	vector2 Center = (ShieldPos * vector2{GridCellSize, GridCellSize}) + GridTopLeft;

						// 	rect ShieldRect = {};
						// 	ShieldRect.TopLeft = Center - vector2{(GridCellSize / 2.0f) - 5.0f, (GridCellSize / 2.0f) + 5.0f};
						// 	ShieldRect.BottomRight = Center + vector2{(GridCellSize / 2.0f) - 5.0f, (GridCellSize / 2.0f) + 5.0f};

						// 	gl_square GridOutline = MakeGLRectangle(ShieldRect, COLOR_BLUE);
						// 	PushRenderSquare(&RenderObjects[0], GridOutline);
						// }

						// if (FindFluent("UsePlayerPos", GameDefinition.Fluents, GameDefinition.FluentsCount,
						//                GameDefinition.Entities, GameDefinition.EntitiesCount) != NULL) {


						vector2 PlayerPos = {FluentSearch("PlayerPos'x", GameDefinition.Fluents, GameDefinition.FluentsCount,
						                                  GameDefinition.InstancedEntities, GameDefinition.InstancedEntitiesCount).Fluent->Value,
						                     FluentSearch("PlayerPos'y", GameDefinition.Fluents, GameDefinition.FluentsCount,
						                                  GameDefinition.InstancedEntities, GameDefinition.InstancedEntitiesCount).Fluent->Value
						                    };

						vector2 PlayerScreenPos = (PlayerPos * vector2{GridCellSize, GridCellSize}) + GridTopLeft;
						gl_square GridOutline = MakeGLSquare(PlayerScreenPos, GridCellSize, COLOR_RED);
						PushRenderSquare(&RenderObjects[0], GridOutline);
					}
				}

				// control bindings
				{
					if (CanDoAction) {

						int32 Index = 0;
						for (; Index < GameDefinition.BoundCount; Index++) {
							bool32 BindingDone = false;
							string KeyWanting = GameDefinition.BoundEvents[Index].KeyName;

							for (int32 ContIndex = 0; ContIndex < ARRAY_SIZE(Controls.AllControls, button); ContIndex++) {
								if (Controls.AllControls[ContIndex].IsDown && KeyWanting == Controls.AllControls[ContIndex].Name) {

									for (int32 BEIndex = 0; BEIndex < GameDefinition.BoundEvents[Index].NextEventName; BEIndex++) {
										string EventWanting = GameDefinition.BoundEvents[Index].EventNames[BEIndex];

										// find the event to do
										for (int32 EventIndex = 0; EventIndex < GameDefinition.EventsCount; EventIndex++) {
											if (!BindingDone && GameDefinition.Events[EventIndex].Name == EventWanting) {

												event* EventDoing = &GameDefinition.Events[EventIndex];

												if (EventValid(EventDoing->ConditionalTokens, EventDoing->ConditionalsCount, &GameDefinition)) {
													TokensChangeState(EventDoing->ConsquenceTokens, EventDoing->NextConsqFluent, &GameDefinition);
													CanDoAction = false;
													BindingDone = true;
												}
											}
										}
									}
								}
							}
						}
					} else {
						bool32 AllButtonsUp = true;
						for (int32 Index = 0; Index < ARRAY_SIZE(Controls.AllControls, button); Index++) {
							if (Controls.AllControls[Index].IsDown) {
								AllButtonsUp = false;
							}
						}

						if (AllButtonsUp) {
							CanDoAction = true;
						}
					}

					// 		// can only do another action once the frst input has been released
					// 	}

				}

				// render stuff!
				{
					glMatrixMode(GL_MODELVIEW);
					glLoadIdentity();

					glOrtho(ScreenSize.left, WindowInfo.Width, WindowInfo.Height, ScreenSize.top, 1, -1);

					glEnable(GL_ALPHA);
					glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					glColorMask(1, 1, 1, 1);

					for (int32 LayerIndex = 0;
					        LayerIndex < RenderLayer_Count;
					        LayerIndex++)
					{
						for (uint32 RenderIndex = 0;
						        RenderIndex < (uint32)RenderObjects[LayerIndex].EntitiesCount;
						        RenderIndex++)
						{
							render_entity* Entity = &RenderObjects[LayerIndex].Entities[RenderIndex];
							switch (Entity->Type)
							{
							case RenderEntity_Texture:
							{
								RenderTexture(&Entity->Texture);
								break;
							}

							case RenderEntity_Line:
							{
								RenderLine(&Entity->Line);
								break;
							}

							case RenderEntity_Square:
							{
								RenderSquare(&Entity->Square);
								break;
							}

							case RenderEntity_ColorPoints:
							{
								RenderColorPoints(&Entity->ColorPoints);
								break;
							}
							}
						}
					}

					glFlush();

					HDC DC = GetDC(Window);
					SwapBuffers(DC);
					ReleaseDC(Window, DC);
				}
			}
		}
	}
}

#else

void
main(int argc, char const **argv)
{
	game_def GameDefinition = LoadGameDefinition();

	// This is the playing part
	{
		PrintOptions(&GameDefinition);

		GlobalRunning = true;
		while (GlobalRunning) {
			char Input[100];
			scanf("%99s", &Input);
			char Selection = Input[0];
			system("cls");

			string SelStr = Selection;
			int32 SelInt = StringToInt32(SelStr);
			if (SelInt < GameDefinition.EventsCount) {
				// DO EVENT
				if (EventValid(GameDefinition.Events[SelInt].ConditionalTokens, GameDefinition.Events[SelInt].ConditionalsCount, &GameDefinition)) {
					TokensChangeState(GameDefinition.Events[SelInt].ConsquenceTokens, GameDefinition.Events[SelInt].NextConsqFluent, &GameDefinition);
				}
			} else {
				Print("Not a valid action");
			}

			PrintOptions(&GameDefinition);
		}

		Print("Closing");
		return;
	}
}
#endif