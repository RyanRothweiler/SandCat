// -----------------------------------------------------------------------------
// Windows
// -----------------------------------------------------------------------------
#include <iostream>
#include <windows.h>

#include <time.h>
#include <stdlib.h>

// Only for error handling
#include <setjmp.h>
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Type Things
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

#define ID_LINE __LINE__
#define ID_FILE __FILE__
#define ID_METHOD __func__
#define COUNTER __COUNTER__

// If this is true, the program will crash instead of returning an error.
static bool CrashDontError = true;

#define RetAssert(Expression) if (!(Expression)) {return(__LINE__);}
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Random
// -----------------------------------------------------------------------------
bool32 RandInitialized = false;

real64
RandomRangeFloat(real64 Bottom, real64 Top)
{
	if (!RandInitialized) {
		RandInitialized = true;
		srand(time(NULL));
	}

	Assert(Bottom < Top);
	real64 Result = 0;

	uint32 RandomMax = 1000;
	uint32 RandomInt = (10 * rand() % RandomMax);
	real64 RandomScalar = (real32)RandomInt / (real32)RandomMax;

	real64 ScaledNum = (real64)((Top - Bottom) * RandomScalar);
	Result = ScaledNum + Bottom;

	return (Result);
}

// Bottom is inclusive top is exclusive
int64
RandomRangeInt(int32 Bottom, int32 Top)
{
	real64 Result = RandomRangeFloat((real32)Bottom, (real32)Top);
	return ((int64)Result);
}
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// String Things
// -----------------------------------------------------------------------------
#define MAX_STRING_SIZE 100

int32
DigitCount(int64 Input)
{
	int32 Count = 0;
	while (Input != 0) {
		Input = Input / 10;
		Count++;
	}

	return (Count);
}

void
IntToCharArray(int64 Input, char *Output, uint32 Offset)
{
	char *OutputPointer = Output + DigitCount(Input) - 1 + Offset;

	if (Input == 0) {
		*Output = '0';
	}

	int32 Count = 1;
	while (Input != 0) {
		if (Count % 4 == 0) {
			// *OutputPointer = ',';
			// OutputPointer--;
		} else {
			uint8 LastDigit = Input % 10;
			*OutputPointer = '0' + LastDigit;
			OutputPointer--;

			Input = Input / 10;
		}

		Count++;
	}
}

void
IntToCharArray(int64 Input, char *Output)
{
	IntToCharArray(Input, Output, 0);
}

int32
CharArrayLength(char *String)
{
	// NOTE why add one?
	// bool addOne = false;
	int Count = 0;
	while (*String++) {
		// addOne = true;
		Count++;
	}
	// if (addOne)
	// {
	// Count++;
	// }
	return (Count);
}

void
ConcatCharArrays(char *SourceA, char *SourceB, char *Destination)
{
	int32 SourceALength = CharArrayLength(SourceA);
	int32 SourceBLength = CharArrayLength(SourceB);

	for (int32 Index = 0;
	        Index < SourceALength;
	        Index++) {
		*Destination++ = *SourceA++;
	}

	for (int32 Index = 0;
	        Index < SourceBLength;
	        Index++) {
		*Destination++ = *SourceB++;
	}

	*Destination++ = 0;
}


struct string {
	char CharArray[MAX_STRING_SIZE] = {};

	string()
	{

	}

	string (char Input)
	{
		CharArray[0] = Input;
	}

	string(char *Input)
	{
		uint32 StringCount = CharArrayLength(Input);
		for (uint32 Index = 0;
		        Index < StringCount;
		        Index++) {
			CharArray[Index] = Input[Index];
		}
		CharArray[StringCount] = '\0';
	}

	// TODO can we compress these?
	string(int64 Input)
	{
		if (Input < 0) {
			CharArray[0] = '-';
			IntToCharArray(Input * -1, CharArray, 1);
		} else {
			IntToCharArray(Input, CharArray);
		}
	}

	string(int32 Input)
	{
		if (Input < 0) {
			CharArray[0] = '-';
			IntToCharArray(Input * -1, CharArray, 1);
		} else {
			IntToCharArray(Input, CharArray);
		}
	}


	string(uint32 Input)
	{
		if (Input < 0) {
			CharArray[0] = '-';
			IntToCharArray(Input * -1, CharArray, 1);
		} else {
			IntToCharArray(Input, CharArray);
		}
	}

	string (real64 Input)
	{
		uint32 PreDecimalCount = DigitCount((int32)Input);

		real64 MovedDecimal = 100 * Input;
		char Dummy[MAX_STRING_SIZE] = {};
		if (Input < 0) {
			PreDecimalCount++;
			IntToCharArray((int64)(-MovedDecimal), Dummy);
		} else {
			IntToCharArray((int64)MovedDecimal, Dummy);
		}

		uint32 ArrayIndex = 0;

		if (Input < 0.0f) {
			CharArray[0] = '-';
			ArrayIndex++;
		}

		for (uint32 Index = 0; Index < MAX_STRING_SIZE - 1; Index++) {
			if (ArrayIndex == PreDecimalCount) {
				CharArray[ArrayIndex] = '.';
				ArrayIndex++;
			}

			CharArray[ArrayIndex] = Dummy[Index];
			ArrayIndex++;
		}
	}
};

const string EmptyString = string{};

string
IntToString(int64 Input)
{
	string NewString = Input;
	return (NewString);
}

int32
StringLength(string String)
{
	return (CharArrayLength(String.CharArray));
}

string
operator + (string A, string B)
{
	string Output = {};
	ConcatCharArrays(A.CharArray, B.CharArray, Output.CharArray);
	return (Output);
}

bool
operator == (string A, string B)
{
	int32 ALength = StringLength(A);
	int32 BLength = StringLength(B);

	if (ALength == BLength) {
		for (int charIndex = 0;
		        charIndex < ALength;
		        charIndex++) {
			if (A.CharArray[charIndex] != B.CharArray[charIndex]) {
				return (false);
			}
		}

		return (true);
	}
	return (false);
}

bool
operator != (string A, string B)
{
	return (!(A == B));
}

void
ConcatIntChar(int64 IntInput, char *CharInput,
              char *CharOutput)
{
	char IntInputAsChar[256] = {};
	IntToCharArray(IntInput, IntInputAsChar);
	ConcatCharArrays(IntInputAsChar, CharInput, CharOutput);
}


string
CopyString(string OrigString)
{
	string FinalString = {};

	uint32 StringCount = StringLength(OrigString);
	for (uint32 Index = 0;
	        Index < StringCount;
	        Index++) {
		FinalString.CharArray[Index] = OrigString.CharArray[Index];
	}

	return (FinalString);
}

void
ClearString(string *String)
{
	for (int CharIndex = 0;
	        CharIndex < MAX_STRING_SIZE;
	        CharIndex++) {
		String->CharArray[CharIndex] = 0x00000000;
	}
}

bool32
CharIsInt(char Char)
{
	return (Char == '0' || Char == '1' || Char == '2' || Char == '3' || Char == '4' ||
	        Char == '5' || Char == '6' || Char == '7' || Char == '8' || Char == '9');
}

bool32
StringIsInt(string String)
{
	uint32 StringCount = CharArrayLength(String.CharArray);
	for (uint32 CharIndex = 0; CharIndex < StringCount; CharIndex++) {
		char NextCharacter = String.CharArray[CharIndex];
		if (!CharIsInt(NextCharacter)) {
			return (false);
		}

	}

	return (true);
}

int32
StringToInt32(string String)
{
	int32 FinalNumber = 0;

	uint32 StringCount = CharArrayLength(String.CharArray);
	for (uint32 CharIndex = 0; CharIndex < StringCount; CharIndex++) {
		FinalNumber = FinalNumber * 10;

		char NextCharacter = String.CharArray[CharIndex];
		switch (NextCharacter) {
		case ('0'): {
			FinalNumber += 0;
		}
		break;

		case ('1'): {
			FinalNumber += 1;
		}
		break;

		case ('2'): {
			FinalNumber += 2;
		}
		break;

		case ('3'): {
			FinalNumber += 3;
		}
		break;

		case ('4'): {
			FinalNumber += 4;
		}
		break;

		case ('5'): {
			FinalNumber += 5;
		}
		break;

		case ('6'): {
			FinalNumber += 6;
		}
		break;

		case ('7'): {
			FinalNumber += 7;
		}
		break;

		case ('8'): {
			FinalNumber += 8;
		}
		break;

		case ('9'): {
			FinalNumber += 9;
		}
		break;

		default: {
			// String is not entirely a number
			Assert(0);
		}
		break;
		}
	}

	return (FinalNumber);
}

void StringCopy(char* Source, char* Dest)
{
	for (int32 Index = 0; Index < StringLength(Source); Index++) {
		Dest[Index] = Source[Index];
	}
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


enum class prog_state {
	compiletime, runtime
};

prog_state GlobalProgState;
string GlobalErrorDesc;
jmp_buf CompiletimeJumpBuffer;
jmp_buf RuntimeJumpBuffer;

string
BuildErrorString(int32 Line, string Message)
{
	string NumString = Line;
	return ("(" + NumString + ") : " + Message);
}

void
ThrowError(int32 Line, string Message)
{
	GlobalErrorDesc = BuildErrorString(Line, Message);

	// If we're compiling the exe, then simply crash if there was an error
#ifdef DLL
	Assert(0);
#endif

	if (GlobalProgState == prog_state::compiletime) {
		longjmp(CompiletimeJumpBuffer, 10);
	} else {
		longjmp(RuntimeJumpBuffer, 10);
	}
}

char DotNotationChar = '\'';


// -----------------------------------------------------------------------------
// Parser Things
// -----------------------------------------------------------------------------
struct parser {
	char* CharOn;

	char WordOn[100] = {};
	int32 WordOnIndex = 0;
};

void
ResetParserWord(parser* Parser)
{
	ZeroMemory(&Parser->WordOn, ARRAY_SIZE(Parser->WordOn, char));
	Parser->WordOnIndex = 0;
}
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct arena {
	void *Head;
	void *Next;
	int32 SizeBytes;
};

void* ArenaAllocate(arena* Arena, int32 Size)
{
	void* Mem = Arena->Next;

	// Don't allocate more than we can
	if ((uint8*)Arena->Next + Size > (uint8*)Arena->Head + Arena->SizeBytes) {
		Assert(0);
	}

	Arena->Next = (uint8*)Arena->Next + Size;
	return (Mem);
}

struct event_bind {
	string EventNames[100];
	int32 NextEventName;

	string KeyName;
};


enum class token_type {
	// general
	none, entity, period, id, number, comma, colon, playerAction, bind,

	// random
	random, to,

	// event
	event, usingg,

	// action
	does, iff,

	// math symbols
	sub, add, div, mult,

	// equality symbols
	lessThan, equalTo, greaterThan, lessThanOrEqual, greaterThanOrEqual,

	// grouping
	openParen, closeParen, openSquare, closeSquare, openCurly, closedCurly,

	// This is the traditional oop dot notation operator
	dot,

	// boolean
	andd, orr, nott,
};

struct token_info {
	token_type Type;
	string Name;
	int32 LineNumber;
};

struct token_stack {
	token_info* Tokens;
	int32 TokensSize;
	int32 TokensCount;
};

struct array {
	string Name;
	// The names of the objcets in this array
	string Array[100];
	int32 ArrayCount;
};

// This type has been streatched, should reall break it down into smaller pieces.
struct fluent {
	string Name;
	real64 Value;

	/* If the fluent has arithmetic as a fluent value, then it is stored here.
		This is is only used when the fluent is part of an event.
		The initial fluents are evaluated when they're parsed.
		Event fluents are evaluated when they're triggered, so they need to be saved and 'parsed' when they're triggered.
	*/
	token_info Arithmetic[20];
	int32 ArithCount;
};

void
Print(string Output)
{
	Output = Output + "\n";
	printf(Output.CharArray);
}

void
PrintHeader(string Str)
{
	Print("---- " + Str + " ----");
}


void
PrintFluent(string InitialPrint, fluent* Fluent)
{
	string ValueString = "";
	ValueString = Fluent->Value;
	Print(InitialPrint + Fluent->Name + " " + " " + ValueString);
}

struct entity {
	// For a decleration, this is the name. For an instance, this is also the name.
	string Name;

	fluent Fluents[50];
	int32 FluentsCount;
};

struct does_if {
	token_info DoesTokens[100];
	int32 DoesTokensCount;

	token_info IfTokens[50];
	int32 IfTokensCount;
};

// These are PlayerAction.
struct event {
	string Name;

	does_if DoesIf[100];
	int32 DoesIfCount;
};

// These are Events
struct method {
	string Name;

	string Usings[100];
	int32 UsingsCount;

	does_if DoesIf[30];
	int32 DoesIfCount;
};

// these are the rules of the game.
struct game_def {
	fluent* Fluents;
	int32 FluentsCount;

	array* Arrays;
	int32 ArraysCount;

	event* Events;
	int32 EventsCount;

	event_bind* BoundEvents;
	int32 BoundCount;

	// These are instances
	entity* InstancedEntities;
	int32 InstancedEntitiesCount;

	method* Methods;
	int32 MethodsCount;
};

fluent*
FindFluentInList(string Name, fluent* Fluents, int32 FluentsCount)
{
	for (int32 Index = 0; Index < FluentsCount; Index++) {
		if (Name == Fluents[Index].Name) {
			return (&Fluents[Index]);
		}
	}

	// Could not find that fluent, don't assert becuase sometimes it's normal not to find the fluent.
	return (NULL);
}


entity*
FindEntity(string Name, entity* Entities, int32 EntitiesCount)
{
	for (int32 Index = 0; Index < EntitiesCount; Index++) {
		if (Name == Entities[Index].Name) {
			return (&Entities[Index]);
		}
	}

	return (NULL);
}

#define EntityNotFoundPrefix "Could not find entity of name "
#define FluentNotFoundPrefix "Could not find fluent of name "
#define MethodNotFoundPrefix "Could not find method of name "
#define ExpectedPeriodPrefix "Expected period at end of line."
#define InvalidStatementStarterPrefix "Token which starts statment, found not starting a statement. (Did you forget a period?)"

struct fluent_search_return {
	int32 TokensConsumed;
	fluent* Fluent;
};

// This handles dot notation, searching through everything to find the right fluent.
fluent_search_return
FluentSearch(token_info* Tokens, int32 ListStart, fluent* Fluents, int32 FluentsCount, entity* Entities, int32 EntitiesCount)
{

	fluent_search_return Ret = {};

	if (Tokens[ListStart].Type == token_type::id &&
	        Tokens[ListStart + 1].Type == token_type::dot &&
	        Tokens[ListStart + 2].Type == token_type::id) {
		// pattern of ID DOT ID which means dot notation
		Ret.TokensConsumed = 3;
		entity* Entity = FindEntity(Tokens[ListStart].Name, Entities, EntitiesCount);
		if (Entity == NULL) {
			// Could not find that entity
			ThrowError(Tokens[ListStart].LineNumber, EntityNotFoundPrefix + Tokens[ListStart].Name);
		}
		fluent* Fluent = FindFluentInList(Tokens[ListStart + 2].Name, Entity->Fluents, Entity->FluentsCount);
		if (Fluent == NULL) {
			// Could not find that fluent in that entity
			ThrowError(Tokens[ListStart].LineNumber, "Fluent " + Tokens[ListStart + 2].Name + " not found in entity " + Tokens[ListStart].Name);
		}
		Ret.Fluent = Fluent;
	} else {
		//  didn't find dot notation, just going to search in the world fluents
		Ret.TokensConsumed = 1;
		fluent* Fluent = FindFluentInList(Tokens[ListStart].Name, Fluents, FluentsCount);
		if (Fluent == NULL) {
			// Could not find that fluent
			ThrowError(Tokens[ListStart].LineNumber, FluentNotFoundPrefix + Tokens[ListStart].Name);
		}
		Ret.Fluent = Fluent;
	}

	return (Ret);
}

string
BuildArrayFluentID(string ArrayName, int32 index)
{
	return ("ARRAY_" + ArrayName + "_" + index);
}

// This converts a string into an array of tokens. Used entirley for the display layer
fluent_search_return
FluentSearch(string TokenString, fluent* Fluents, int32 FluentsCount, entity* Entities, int32 EntitiesCount)
{
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

real64
Accumulate(int32 Accum, token_type Operation, int32 NewOperand)
{
	if (Operation == token_type::add) {
		Accum = Accum + NewOperand;
	} else if (Operation == token_type::sub) {
		Accum = Accum - NewOperand;
	} else if (Operation == token_type::mult) {
		Accum = Accum * NewOperand;
	} else if (Operation == token_type::div) {
		Accum = Accum / NewOperand;
	} else {
		Accum = NewOperand;
	}

	return (Accum);
}

fluent*
GetFluentInArray(string EntityName, string FluentName, int32 ArrayIndex, int32 LineNum,
                 entity* Entities, int32 EntitiesCount)
{
	string NameWanting = BuildArrayFluentID(EntityName, ArrayIndex);
	entity* EntityEditing = FindEntity(NameWanting, Entities, EntitiesCount);
	if (EntityEditing == NULL) {
		ThrowError(LineNum, "Could not find entity of name " + NameWanting);
	}

	fluent* FluentEditing = FindFluentInList(FluentName, EntityEditing->Fluents, EntityEditing->FluentsCount);
	if (FluentEditing == NULL) {
		ThrowError(LineNum, "Could not find fluent of name " + FluentName);
	}

	return (FluentEditing);
}

real64
InfixAccumulate(token_info* Tokens, int32 TokenIndexStart,
                fluent* Fluents, int32 FluentsCount,
                entity* Entities, int32 EntitiesCount)
{
	token_type AccumState = token_type::none;

	int32 Accum = 0.0f;

	int32 AccumIndex = TokenIndexStart;
	while (Tokens[AccumIndex].Type != token_type::period &&
	        Tokens[AccumIndex].Type != token_type::none &&
	        Tokens[AccumIndex].Type != token_type::closedCurly &&
	        Tokens[AccumIndex].Type != token_type::closeSquare &&
	        Tokens[AccumIndex].Type != token_type::lessThan &&
	        Tokens[AccumIndex].Type != token_type::lessThanOrEqual &&
	        Tokens[AccumIndex].Type != token_type::greaterThan &&
	        Tokens[AccumIndex].Type != token_type::greaterThanOrEqual &&
	        Tokens[AccumIndex].Type != token_type::equalTo &&
	        Tokens[AccumIndex].Type != token_type::andd &&
	        Tokens[AccumIndex].Type != token_type::nott &&
	        Tokens[AccumIndex].Type != token_type::orr &&
	        Tokens[AccumIndex].Type != token_type::to &&
	        Tokens[AccumIndex].Type != token_type::comma) {
		token_type NewTokenType = Tokens[AccumIndex].Type;
		if (NewTokenType == token_type::id || NewTokenType == token_type::number) {

			real64 NewOperand = 0.0f;
			if (Tokens[AccumIndex + 1].Type == token_type::dot) {
				// Dot notation

				entity* Event = FindEntity(Tokens[AccumIndex].Name, Entities, EntitiesCount);
				if (Event == NULL) {
					ThrowError(Tokens[AccumIndex].LineNumber, EntityNotFoundPrefix + Tokens[AccumIndex].Name);
				}
				fluent* Fluent = FindFluentInList(Tokens[AccumIndex + 2].Name, Event->Fluents, Event->FluentsCount);
				if (Fluent == NULL) {
					ThrowError(Tokens[AccumIndex].LineNumber, FluentNotFoundPrefix + Tokens[AccumIndex + 2].Name);
				}

				NewOperand = Fluent->Value;
				AccumIndex += 2;
			} else if (Tokens[AccumIndex + 1].Type == token_type::openSquare) {
				// Fluent in an array

				// Get the number of tokens until the closing square
				int32 CountUntilClose = 0;
				while (Tokens[AccumIndex + CountUntilClose - 1].Type != token_type::closeSquare) {
					CountUntilClose++;
				}

				string EntityName = Tokens[AccumIndex].Name;
				string FluentName = Tokens[AccumIndex + CountUntilClose + 1].Name;
				int32 TokenLineNum = Tokens[AccumIndex].LineNumber;

				int32 ArrayIndex = InfixAccumulate(Tokens, AccumIndex + 2, Fluents, FluentsCount, Entities, EntitiesCount);
				fluent* FluentEditing = GetFluentInArray(EntityName, FluentName, ArrayIndex, TokenLineNum, Entities, EntitiesCount);

				NewOperand = FluentEditing->Value;
				AccumIndex += 1 + CountUntilClose;

				if (false) {
					string EntityName = Tokens[AccumIndex].Name;
					string FluentName = Tokens[AccumIndex + 5].Name;
					int32 ArrayIndex = StringToInt32(Tokens[AccumIndex + 2].Name);
					int32 TokenLineNum = Tokens[AccumIndex].LineNumber;
					NewOperand = GetFluentInArray(EntityName, FluentName, ArrayIndex, TokenLineNum, Entities, EntitiesCount)->Value;
					AccumIndex += 5;
				}
			} else {
				// Straight regular fluent

				if (Tokens[AccumIndex].Type == token_type::id) {
					// find that fluent
					fluent* Fluent = FindFluentInList(Tokens[AccumIndex].Name, Fluents, FluentsCount);
					if (Fluent == NULL) {
						// Report error here. That fluent name does not exist.
						ThrowError(Tokens[AccumIndex].LineNumber, FluentNotFoundPrefix + Tokens[AccumIndex].Name);
					}

					NewOperand = Fluent->Value;
				} else if (Tokens[AccumIndex].Type == token_type::number) {
					NewOperand = StringToInt32(Tokens[AccumIndex].Name);
				} else {
					ThrowError(Tokens[AccumIndex].LineNumber, "Cannot get the value of a token which is neither a number or id.");
				}
			}

			Accum = Accumulate(Accum, AccumState, NewOperand);

		} else if (NewTokenType == token_type::sub ||
		           NewTokenType == token_type::add ||
		           NewTokenType == token_type::mult ||
		           NewTokenType == token_type::div) {
			AccumState = NewTokenType;
		} else if (NewTokenType == token_type::random) {
			// random

			int32 Min = (int32)InfixAccumulate(Tokens, AccumIndex + 1, Fluents, FluentsCount, Entities, EntitiesCount);
			while (Tokens[AccumIndex].Type != token_type::to) {
				AccumIndex++;
			}
			AccumIndex++;
			int32 Max = (int32)InfixAccumulate(Tokens, AccumIndex, Fluents, FluentsCount, Entities, EntitiesCount);

			int32 RandNum = RandomRangeInt(Min, Max + 1);

			Accum = Accumulate(Accum, AccumState, RandNum);

			while (Tokens[AccumIndex].Type != token_type::comma &&
			        Tokens[AccumIndex].Type != token_type::period &&
			        Tokens[AccumIndex].Type != token_type::does &&
			        Tokens[AccumIndex].Type != token_type::iff &&
			        Tokens[AccumIndex].Type != token_type::none) {
				AccumIndex++;
			}

			if (Tokens[AccumIndex].Type == token_type::period) {
				AccumIndex--;
			}
		} else {
			// Something wrong
			ThrowError(Tokens[AccumIndex].LineNumber, "Unexpected token type.");
		}

		AccumIndex++;
	}

	return (Accum);
}


// // This is only used in Evaluate Boolean to get the operands
// real64
// BoolGetValue(token_info* Tokens, int32* NextLogicOp,
//              fluent* Fluents, int32 FluentsCount,
//              entity* Entities, int32 EntitiesCount)
// {
// 	real64 Val = 0.0f;

// 	if (StringIsInt(Tokens[*NextLogicOp].Name)) {
// 		// This is just a number
// 		Val = StringToInt32(Tokens[*NextLogicOp].Name);
// 		(*NextLogicOp)++;
// 	} else if (Tokens[*NextLogicOp].Type == token_type::id &&
// 	           Tokens[*NextLogicOp + 1].Type != token_type::openSquare) {
// 		// This is dot notation
// 		fluent_search_return Ret = FluentSearch(Tokens, *NextLogicOp,
// 		                                        Fluents, FluentsCount, Entities, EntitiesCount);
// 		(*NextLogicOp) += Ret.TokensConsumed;
// 		Val = Ret.Fluent->Value;
// 	} else if (Tokens[*NextLogicOp + 1].Type == token_type::openSquare) {
// 		// This is an array, they always have dot notation
// 		string ArrayName = Tokens[*NextLogicOp].Name;
// 		(*NextLogicOp) += 2;

// 		real64 ArrayIndex = InfixAccumulate(Tokens, *NextLogicOp, Fluents, FluentsCount, Entities, EntitiesCount);

// 		// Move past the array index
// 		while (Tokens[*NextLogicOp].Type != token_type::dot) {
// 			(*NextLogicOp)++;
// 		}
// 		(*NextLogicOp) += 2;

// 		string FluentName = Tokens[*NextLogicOp].Name;

// 		Val = GetFluentInArray(ArrayName, FluentName, ArrayIndex, Tokens[*NextLogicOp].LineNumber, Entities, EntitiesCount)->Value;
// 	} else {
// 		ThrowError(Tokens[*NextLogicOp].LineNumber, "Invalid token pattern in an if statment");
// 	}

// 	return (Val);
// }

bool32
EvaluateBoolean(token_info* ConditionalTokens, int32 ConditionalsCount,
                int32* NextLogicOp,
                fluent* Fluents, int32 FluentsCount,
                entity* Entities, int32 EntitiesCount)
{
	bool32 AccumBool = true;
	bool32 NegateNext = false;

	// This state must be held separate from the logicO
	token_type CurrLogicOp = token_type::none;

	while (*NextLogicOp < ConditionalsCount) {
		token_info* NextToken = &ConditionalTokens[*NextLogicOp];
		(*NextLogicOp)++;

		if (NextToken->Type == token_type::andd || NextToken->Type == token_type::orr) {
			CurrLogicOp = NextToken->Type;
		} else if (NextToken->Type == token_type::nott) {
			NegateNext = true;
		} else if (NextToken->Type == token_type::openParen) {
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
			real64 FirstVal = InfixAccumulate(ConditionalTokens, *NextLogicOp, Fluents, FluentsCount, Entities, EntitiesCount);

			while (ConditionalTokens[*NextLogicOp].Type != token_type::greaterThan &&
			        ConditionalTokens[*NextLogicOp].Type != token_type::greaterThanOrEqual &&
			        ConditionalTokens[*NextLogicOp].Type != token_type::equalTo &&
			        ConditionalTokens[*NextLogicOp].Type != token_type::lessThanOrEqual &&
			        ConditionalTokens[*NextLogicOp].Type != token_type::lessThan) {
				(*NextLogicOp)++;
			}

			// Get operation
			token_type Op = ConditionalTokens[*NextLogicOp].Type;
			(*NextLogicOp)++;

			// Second Value
			real64 SecondVal = InfixAccumulate(ConditionalTokens, *NextLogicOp, Fluents, FluentsCount, Entities, EntitiesCount);

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
				ThrowError(ConditionalTokens[*NextLogicOp].LineNumber, "A non-comparison token found where a comparison one should be.");
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

			// Move onto the next conditional
			while (ConditionalTokens[*NextLogicOp].Type != token_type::none &&
			        ConditionalTokens[*NextLogicOp].Type != token_type::andd &&
			        ConditionalTokens[*NextLogicOp].Type != token_type::nott &&
			        ConditionalTokens[*NextLogicOp].Type != token_type::orr &&
			        ConditionalTokens[*NextLogicOp].Type != token_type::openParen) {
				(*NextLogicOp)++;
			}
		} else if (NextToken->Type == token_type::closeSquare) {
			return (AccumBool);
		} else {
			// There was a problem. Something didn't stack correctly
			ThrowError(NextToken->LineNumber, "There was a problem. Something unexpected happened.");
		}
	}

	return (AccumBool);
}

bool32
IfTokensValid(token_info* IfTokens, int32 TokensCount, game_def* GameDef)
{
	if (TokensCount > 0) {
		int32 NextLogic = 0;
		return (EvaluateBoolean(IfTokens, TokensCount, &NextLogic,
		                        GameDef->Fluents, GameDef->FluentsCount,
		                        GameDef->InstancedEntities, GameDef->InstancedEntitiesCount));
	} else {
		// If there are no conditionals then event is always valid
		return (true);
	}
}

void
PrintOptions(game_def* Def)
{
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

			Print(Entity->Name);
			for (int32 FIndex = 0; FIndex < Entity->FluentsCount; FIndex++) {
				PrintFluent("\t", &Entity->Fluents[FIndex]);
			}
		}

		// print arrays
		for (int32 Index = 0; Index < Def->ArraysCount; Index++) {
			array* Array = &Def->Arrays[Index];

			printf(Array->Name.CharArray);
			printf("[");
			for (int32 AIndex = 0; AIndex < Array->ArrayCount; AIndex++) {
				if (AIndex != 0) {
					printf(", ");
				}

				printf(Array->Array[AIndex].CharArray);
			}
			printf("] \n");
		}

		Print("\n");
	}

	// Print Actions
	{
		PrintHeader("POSSIBLE ACTIONS");
		for (int32 Index = 0; Index < Def->EventsCount; Index++) {
			string StrIndex = Index;
			Print(StrIndex + ") " + Def->Events[Index].Name);
		}
		// Print("----------------------- ");
	}

	Print("\n \n----------------------- ");
	Print("Enter action number you wish.");
	Print("");
}

event*
FindEvent(string Name, event* Events, int32 EventsCount)
{
	for (int32 Index = 0; Index < EventsCount; Index++) {
		if (Name == Events[Index].Name) {
			return (&Events[Index]);
		}
	}

	// Could not find that fluent, don't assert becuase sometimes it's normal not to find the fluent.
	return (NULL);
}

void
AddToken(parser* Parser, token_stack* TokenStack, token_type Type, string Name, int32 LineNum)
{

	if (Type == token_type::id) {
		int32 x = 0;
	}

	TokenStack->Tokens[TokenStack->TokensCount].Type = Type;
	TokenStack->Tokens[TokenStack->TokensCount].Name = Name;
	TokenStack->Tokens[TokenStack->TokensCount].LineNumber = LineNum;
	TokenStack->TokensCount++;

	Assert(TokenStack->TokensCount < TokenStack->TokensSize);

	ResetParserWord(Parser);
}

void
CheckNumberOrID(parser* Parser, token_stack* Tokens, int32 LineNum)
{
	if (Parser->WordOnIndex != 0) {
		if (StringIsInt(Parser->WordOn)) {
			AddToken(Parser, Tokens, token_type::number, Parser->WordOn, LineNum);
		} else {
			AddToken(Parser, Tokens, token_type::id, Parser->WordOn, LineNum);
		}
	}
}


// returns if the given token is a token which appears at the beginning of a statment.
bool
IsStartingToken(token_type Type)
{
	if (Type == token_type::event || Type == token_type::playerAction || Type == token_type::bind || Type == token_type::entity) {
		return (true);
	}
	return (false);
}

fluent
GrabFluent(token_info* Tokens, int32 StackStart, bool32 Evaluate,
           fluent* Fluents, int32 FluentsCount, entity* Entities, int32 EntitiesCount)
{
	fluent FluentAdding = {};
	FluentAdding.Name = Tokens[StackStart].Name;

	// This skips over the ID = part, and gets to the statement itself
	int32 AccumIndex = StackStart + 2;

	if (Evaluate) {
		FluentAdding.Value = InfixAccumulate(Tokens, AccumIndex, Fluents, FluentsCount, Entities, EntitiesCount);
	} else {
		while (Tokens[AccumIndex].Type != token_type::period) {
			FluentAdding.Arithmetic[FluentAdding.ArithCount] = Tokens[AccumIndex];
			FluentAdding.ArithCount++;
			AccumIndex++;
		}
	}

	return (FluentAdding);
}

struct does_if_return {
	does_if DoesIf;
	int32 TokensConsumed;
};

does_if_return
GetDoesIf(token_stack* Tokens, int32 TI)
{
	does_if_return Ret = {};
	does_if* DoesIf = &Ret.DoesIf;

	int32 TIStart = TI;

	// Grab the does tokens
	{
		if (Tokens->Tokens[TI].Type == token_type::does) {
			TI++;
		}

		while (Tokens->Tokens[TI].Type != token_type::iff && Tokens->Tokens[TI].Type != token_type::period && Tokens->Tokens[TI].Type != token_type::does) {

			if (IsStartingToken(Tokens->Tokens[TI].Type)) {
				ThrowError(Tokens->Tokens[TI].LineNumber, InvalidStatementStarterPrefix);
			}

			DoesIf->DoesTokens[DoesIf->DoesTokensCount] = Tokens->Tokens[TI];
			DoesIf->DoesTokensCount++;
			TI++;
		}
	}

	// only get the conditionals if they exist
	if (Tokens->Tokens[TI].Type == token_type::iff) {
		// Move past the if
		TI++;

		// Grab the conditionals
		{
			while (Tokens->Tokens[TI].Type != token_type::period && Tokens->Tokens[TI].Type != token_type::does) {

				if (IsStartingToken(Tokens->Tokens[TI].Type)) {
					ThrowError(Tokens->Tokens[TI].LineNumber, InvalidStatementStarterPrefix);
					return (Ret);
				}

				DoesIf->IfTokens[DoesIf->IfTokensCount] = Tokens->Tokens[TI];
				DoesIf->IfTokensCount++;
				TI++;
			}
		}
	}

	Ret.TokensConsumed = TI - TIStart;
	return (Ret);
}

void GrabEntityFluents(entity* EntityFilling, token_info* Tokens, int32 TokensStart, game_def* GameDefinition)
{
	int32 TI = TokensStart;

	// Move past the opening bracket
	TI += 2;

	// Fill the entities with the right fluents
	while (true) {

		// Get the next fluent, and set all the instanced entities
		fluent NextFluent = GrabFluent(Tokens, TI, true, GameDefinition->Fluents, GameDefinition->FluentsCount,
		                               GameDefinition->InstancedEntities, GameDefinition->InstancedEntitiesCount);

		EntityFilling->Fluents[EntityFilling->FluentsCount] = NextFluent;
		EntityFilling->FluentsCount++;

		// Move TI until we hit the next fluent
		while (Tokens[TI].Type != token_type::comma &&
		        Tokens[TI].Type != token_type::closedCurly &&
		        Tokens[TI].Type != token_type::period) {
			TI++;
		}

		// If we at the end of the entity, then break out
		if (Tokens[TI].Type == token_type::closedCurly) {
			break;
		} else {
			// move past the comma
			TI++;
		}
	}
}

struct param {
	token_info Tokens[10];
	int32 TokenCount;
};

struct token_rep_ret {
	token_info ReplacedTokens[100];
	int32 ReplacedTokensIndex;
};

token_rep_ret
TokenReplacement(param* ParamReals, int32 ParamRealsCount,
                 token_info* TokensReplacing, int32 TokensReplacingCount,
                 string* MethodUsings, int32 MethodUsingsCount)
{
	token_rep_ret Ret = {};

	for (int32 Index = 0; Index < TokensReplacingCount; Index++) {
		Assert(Ret.ReplacedTokensIndex < 100);

		// Check if we need to keep the original, or use the tokens passed in
		bool32 DoReplacement = false;

		// Check for replacement
		int32 ReplacementIndex = 0;
		for (int32 ParamIndex = 0; ParamIndex < MethodUsingsCount; ParamIndex++) {
			if (TokensReplacing[Index].Name == MethodUsings[ParamIndex]) {
				ReplacementIndex = ParamIndex;
				DoReplacement = true;
				break;
			}
		}

		if (DoReplacement) {
			// Instead of adding the original, use the passed in tokens.
			param* P = &ParamReals[ReplacementIndex];
			for (int32 Index = 0; Index < P->TokenCount; Index++) {
				Ret.ReplacedTokens[Ret.ReplacedTokensIndex] = P->Tokens[Index];
				Ret.ReplacedTokensIndex++;
			}
		} else {
			// Else keep the original token.
			Ret.ReplacedTokens[Ret.ReplacedTokensIndex] = TokensReplacing[Index];
			Ret.ReplacedTokensIndex++;
		}
	}

	return (Ret);
}

// TokenChnageState State. Used to hold the state of a TokenChangeState iteration. So we don't recursively call it.
struct tcs_state {
	token_info Tokens[50];
	int32 TokensCount;
	int32 TokenIndex;
};

void
TokensChangeState(token_info* Tokens, int32 TokensCount, game_def * GameDef)
{
	int32 StackSize = 100;
	int32 StackTop = 0;
	tcs_state* StateStack = (tcs_state*)malloc(sizeof(tcs_state) * StackSize);
	ZeroMemory(StateStack, sizeof(tcs_state) * StackSize);

	memcpy(StateStack[0].Tokens, Tokens, sizeof(token_info) * (TokensCount + 1));
	StateStack[0].TokensCount = TokensCount;


	while (StackTop >= 0) {
		tcs_state* StateUsing = &StateStack[StackTop];

		while (StateUsing->TokenIndex < StateUsing->TokensCount) {
			if (StateUsing->Tokens[StateUsing->TokenIndex].Type == token_type::id &&
			        StateUsing->Tokens[StateUsing->TokenIndex + 1].Type == token_type::dot &&
			        StateUsing->Tokens[StateUsing->TokenIndex + 2].Type == token_type::id) {
				// this is dot notation

				string EntityName = StateUsing->Tokens[StateUsing->TokenIndex].Name;
				string FluentName = StateUsing->Tokens[StateUsing->TokenIndex + 2].Name;

				entity* Entity = FindEntity(EntityName, GameDef->InstancedEntities, GameDef->InstancedEntitiesCount);
				if (Entity == NULL) {
					// That entity name does not exist. Report error.
					ThrowError(StateUsing->Tokens[StateUsing->TokenIndex].LineNumber, EntityNotFoundPrefix + EntityName);
				}

				fluent* Fluent = FindFluentInList(FluentName, Entity->Fluents, Entity->FluentsCount);
				if (Fluent != NULL) {
					// modify the existing one
					Fluent->Value  = GrabFluent(StateUsing->Tokens, StateUsing->TokenIndex + 2, true,
					                            GameDef->Fluents, GameDef->FluentsCount,
					                            GameDef->InstancedEntities, GameDef->InstancedEntitiesCount).Value;
				} else {
					// add a new one
					Entity->Fluents[Entity->FluentsCount] = GrabFluent(StateUsing->Tokens, StateUsing->TokenIndex + 2, true,
					                                        Entity->Fluents, Entity->FluentsCount,
					                                        GameDef->InstancedEntities, GameDef->InstancedEntitiesCount);
					Entity->FluentsCount++;
				}

				// Move StateUsing->TokenIndex
				while (StateUsing->Tokens[StateUsing->TokenIndex].Type != token_type::comma &&
				        StateUsing->Tokens[StateUsing->TokenIndex].Type != token_type::iff &&
				        StateUsing->TokenIndex < StateUsing->TokensCount) {
					StateUsing->TokenIndex++;
				}
				StateUsing->TokenIndex++;

				continue;
			} else if (StateUsing->Tokens[StateUsing->TokenIndex].Type == token_type::id &&
			           StateUsing->Tokens[StateUsing->TokenIndex + 1].Type == token_type::equalTo) {
				// this is a regular one

				// Change that fluent state if it exists
				fluent* F = FindFluentInList(StateUsing->Tokens[StateUsing->TokenIndex].Name, GameDef->Fluents, GameDef->FluentsCount);
				if (F != NULL) {
					*F = GrabFluent(StateUsing->Tokens, StateUsing->TokenIndex, true,
					                GameDef->Fluents, GameDef->FluentsCount,
					                GameDef->InstancedEntities, GameDef->InstancedEntitiesCount);
				} else {
					GameDef->Fluents[GameDef->FluentsCount] = GrabFluent(StateUsing->Tokens, StateUsing->TokenIndex, true,
					        GameDef->Fluents, GameDef->FluentsCount,
					        GameDef->InstancedEntities, GameDef->InstancedEntitiesCount);
					GameDef->FluentsCount++;
				}

				// Move StateUsing->TokenIndex until next comma
				while (StateUsing->Tokens[StateUsing->TokenIndex].Type != token_type::comma  &&
				        StateUsing->Tokens[StateUsing->TokenIndex].Type != token_type::iff &&
				        StateUsing->TokenIndex < StateUsing->TokensCount) {
					StateUsing->TokenIndex++;
				}
				StateUsing->TokenIndex++;

				continue;
			} else if (StateUsing->Tokens[StateUsing->TokenIndex].Type == token_type::id &&
			           StateUsing->Tokens[StateUsing->TokenIndex + 1].Type == token_type::openSquare) {
				// array assignment by index. These always have dot notation

				int32 IndexEditing = (int32)InfixAccumulate(StateUsing->Tokens, StateUsing->TokenIndex + 2,
				                     GameDef->Fluents, GameDef->FluentsCount,
				                     GameDef->InstancedEntities, GameDef->InstancedEntitiesCount);

				array* ArrayEditing = {};
				for (int32 Index = 0; Index < GameDef->ArraysCount; Index++) {
					if (GameDef->Arrays[Index].Name == StateUsing->Tokens[StateUsing->TokenIndex].Name) {
						ArrayEditing = &GameDef->Arrays[Index];
						break;
					}
				}

				if (ArrayEditing == NULL) {
					ThrowError(StateUsing->Tokens[StateUsing->TokenIndex].LineNumber, "Array of name " + StateUsing->Tokens[StateUsing->TokenIndex].Name + " does not exist.");
				}

				// Get the number of StateUsing->Tokens until the closing square
				int32 CountUntilClose = 0;
				while (StateUsing->Tokens[StateUsing->TokenIndex + CountUntilClose - 1].Type != token_type::closeSquare) {
					CountUntilClose++;
				}

				if (StateUsing->Tokens[StateUsing->TokenIndex + CountUntilClose].Type == token_type::dot) {
					string EntityName = StateUsing->Tokens[StateUsing->TokenIndex].Name;
					string FluentName = StateUsing->Tokens[StateUsing->TokenIndex + CountUntilClose + 1].Name;
					int32 TokenLineNum = StateUsing->Tokens[StateUsing->TokenIndex].LineNumber;

					int32 ArrayIndex = InfixAccumulate(StateUsing->Tokens, StateUsing->TokenIndex + 2,
					                                   GameDef->Fluents, GameDef->FluentsCount,
					                                   GameDef->InstancedEntities, GameDef->InstancedEntitiesCount);

					fluent* FluentEditing = GetFluentInArray(EntityName, FluentName, ArrayIndex, TokenLineNum, GameDef->InstancedEntities, GameDef->InstancedEntitiesCount);

					FluentEditing->Value = InfixAccumulate(StateUsing->Tokens, StateUsing->TokenIndex + CountUntilClose + 3,
					                                       GameDef->Fluents, GameDef->FluentsCount,
					                                       GameDef->InstancedEntities, GameDef->InstancedEntitiesCount);
				} else if (StateUsing->Tokens[StateUsing->TokenIndex + CountUntilClose].Type == token_type::equalTo) {
					// We're putting a new fluent into the array
					ArrayEditing->Array[IndexEditing] = StateUsing->Tokens[StateUsing->TokenIndex + CountUntilClose + 5].Name;
				} else {
					ThrowError(StateUsing->Tokens[StateUsing->TokenIndex].LineNumber, "Unexpected token after array.");
				}

				// Move until the period
				while (StateUsing->Tokens[StateUsing->TokenIndex].Type != token_type::period &&
				        StateUsing->Tokens[StateUsing->TokenIndex].Type != token_type::none &&
				        StateUsing->Tokens[StateUsing->TokenIndex].Type != token_type::comma) {
					StateUsing->TokenIndex++;
				}

				if (StateUsing->Tokens[StateUsing->TokenIndex].Type == token_type::comma) {
					StateUsing->TokenIndex++;
				}

			} else if (StateUsing->Tokens[StateUsing->TokenIndex].Type == token_type::id &&
			           StateUsing->Tokens[StateUsing->TokenIndex + 1].Type == token_type::openParen) {
				// This is a method

				// Find the method to do
				method* MethodDoing = {};
				for (int32 Index = 0; Index < GameDef->MethodsCount; Index++) {
					if (GameDef->Methods[Index].Name == StateUsing->Tokens[StateUsing->TokenIndex].Name) {
						MethodDoing = &GameDef->Methods[Index];
						break;
					}
				}
				if (MethodDoing == NULL) {
					// Could not find that method. Error here
					ThrowError(StateUsing->Tokens[StateUsing->TokenIndex].LineNumber, MethodNotFoundPrefix + StateUsing->Tokens[StateUsing->TokenIndex].Name);
				}

				Assert(MethodDoing->UsingsCount < 50);

				// The real params are the literal strings which are being passed in. Replace the method parameters with these literals.
				int32 ParamCountMax = 20;
				param ParamReals[20] = {};
				int32 ParamRealsCount = 0;
				ZeroMemory(ParamReals, ParamCountMax * sizeof(param));

				// Move past id and open curly
				StateUsing->TokenIndex += 2;

				// Fill the real params
				{
					while (StateUsing->Tokens[StateUsing->TokenIndex].Type != token_type::closeParen &&
					        StateUsing->Tokens[StateUsing->TokenIndex].Type != token_type::none &&
					        StateUsing->Tokens[StateUsing->TokenIndex].Type != token_type::comma) {
						param* P = &ParamReals[ParamRealsCount];
						while (StateUsing->Tokens[StateUsing->TokenIndex].Type != token_type::comma && StateUsing->Tokens[StateUsing->TokenIndex].Type != token_type::closeParen) {
							P->Tokens[P->TokenCount] = StateUsing->Tokens[StateUsing->TokenIndex];
							P->TokenCount++;
							StateUsing->TokenIndex++;
						}

						StateUsing->TokenIndex++;
						ParamRealsCount++;
						Assert(ParamRealsCount < ParamCountMax);
					}
				}

				// Move token index past the curly
				while (StateUsing->Tokens[StateUsing->TokenIndex].Type != token_type::closeParen &&
				        StateUsing->Tokens[StateUsing->TokenIndex].Type != token_type::comma &&
				        StateUsing->Tokens[StateUsing->TokenIndex].Type != token_type::none) {
					StateUsing->TokenIndex++;
				}
				StateUsing->TokenIndex++;

				// Move past comma, only if it's there
				if (StateUsing->Tokens[StateUsing->TokenIndex].Type == token_type::comma) {
					StateUsing->TokenIndex++;
				}

				// Execute method last

				for (int32 DoesIfIndex = 0; DoesIfIndex < MethodDoing->DoesIfCount; DoesIfIndex++) {
					does_if* DI = &MethodDoing->DoesIf[DoesIfIndex];

					// first check if the method is valid
					token_rep_ret ReplacedIfs =  TokenReplacement(ParamReals, ParamRealsCount,
					                             DI->IfTokens, DI->IfTokensCount,
					                             MethodDoing->Usings, MethodDoing->UsingsCount);

					int32 NextLogicOp = 0;
					bool32 Ret = EvaluateBoolean(ReplacedIfs.ReplacedTokens, ReplacedIfs.ReplacedTokensIndex, &NextLogicOp,
					                             GameDef->Fluents, GameDef->FluentsCount,
					                             GameDef->InstancedEntities, GameDef->InstancedEntitiesCount);
					if (Ret) {
						// Event is valid, so do it
						token_info ReplacedDoesTokens[100];
						int32 ReplacedDoesTokensIndex = 0;

						token_rep_ret ReplacedDoes =  TokenReplacement(ParamReals, ParamRealsCount,
						                              DI->DoesTokens, DI->DoesTokensCount,
						                              MethodDoing->Usings, MethodDoing->UsingsCount);

						// Push new state onto the stack, and start from that state.
						// TokensChangeState(ReplacedDoes.ReplacedTokens, ReplacedDoes.ReplacedTokensIndex, GameDef);

						StackTop++;
						memcpy(StateStack[StackTop].Tokens, ReplacedDoes.ReplacedTokens, sizeof(token_info) * (ReplacedDoes.ReplacedTokensIndex + 1));
						// StateStack[StackTop].Tokens = ReplacedDoes.ReplacedTokens;
						StateStack[StackTop].TokensCount = ReplacedDoes.ReplacedTokensIndex;

						StateUsing = &StateStack[StackTop];
						StateUsing->TokenIndex = 0;
					}
				}

				continue;
			} else {
				// This method changes state when given tokens. We could not find a pattern of those tokens which made sense.
				ThrowError(StateUsing->Tokens[StateUsing->TokenIndex].LineNumber, "Invalid token type in a does statement.");
			}
		}

		// Pop off the stack, and continue with the next token change state.
		StackTop--;
	}
}

bool32 firstLoad = true;
string LoadGameDefinition(char* RulesData, int32 RulesLength, game_def* GameDefinition)
{
	GlobalProgState = prog_state::compiletime;
	if (setjmp(CompiletimeJumpBuffer) == 10) {
		// This is an error. So return the error val;
		return (GlobalErrorDesc);
	}

	if (RulesLength == 0) {
		ThrowError(0, "Rules file is empty.");
	}

	// TODO at some point make sure we don't exceede the maximum of any of these
	int32 MaxCount = 100;

	if (firstLoad) {
		firstLoad = false;

		GameDefinition->Fluents = (fluent*)malloc(MaxCount * sizeof(fluent));
		GameDefinition->Events = (event*)malloc(MaxCount * sizeof(event));
		GameDefinition->BoundEvents = (event_bind*)malloc(MaxCount * sizeof(event_bind));
		GameDefinition->InstancedEntities = (entity*)malloc(MaxCount * sizeof(entity));
		GameDefinition->Methods = (method*)malloc(MaxCount * sizeof(method));
		GameDefinition->Arrays = (array*)malloc(MaxCount * sizeof(array));

	}

	ZeroMemory(GameDefinition->Fluents, sizeof(fluent) * MaxCount);
	ZeroMemory(GameDefinition->Events, sizeof(event) * MaxCount);
	ZeroMemory(GameDefinition->BoundEvents, sizeof(event_bind) * MaxCount);
	ZeroMemory(GameDefinition->InstancedEntities, sizeof(entity) * MaxCount);
	ZeroMemory(GameDefinition->Methods, sizeof(method) * MaxCount);
	ZeroMemory(GameDefinition->Arrays, sizeof(array) * MaxCount);

	GameDefinition->FluentsCount = 0;
	GameDefinition->EventsCount = 0;
	GameDefinition->BoundCount = 0;
	GameDefinition->InstancedEntitiesCount = 0;
	GameDefinition->MethodsCount = 0;
	GameDefinition->ArraysCount = 0;


	// lexer and parser system.
	{
		token_stack Tokens = {};
		Tokens.TokensSize = 5000;
		Tokens.Tokens = (token_info*)malloc(sizeof(token_info) * Tokens.TokensSize);

		// Create all the tokens
		{
			parser Parser = {};
			ResetParserWord(&Parser);

			int32 CharactersCount = RulesLength;
			Parser.CharOn = RulesData;

			bool32 InCommentMode = false;

			string ActionLiteral = "PlayerAction";
			string MethodLiteral = "Event";
			string UsingLiteral = "using";
			string DoesLiteral = "does";
			string IfLiteral = "if";
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
			string RandomLiteral = "random";
			string ToLiteral = "to";

			int32 CurrLineNum =  1;

			for (int32 Index = 0; Index < CharactersCount; Index++) {

				char ThisChar = *Parser.CharOn;

				if (ThisChar == NULL) {
					break;
				}

				if (ThisChar == '\n') {
					CurrLineNum++;
				}

				// ignore comments
				if (InCommentMode) {
					if (ThisChar == '\n') {
						InCommentMode = false;
					}
					Parser.CharOn++;
					continue;
				}

#define ADDTOKEN(token) AddToken(&Parser, &Tokens, token, Parser.WordOn, CurrLineNum);

				//NOTE order of these ifs is very important
				if (ThisChar == NULL) {
					// end of program
					break;
				} else if (Parser.WordOn == ToLiteral) {
					// CheckNumberOrID(&Parser, &Tokens, CurrLineNum);
					ADDTOKEN(token_type::to);
				} else if (Parser.WordOn == RandomLiteral) {
					ADDTOKEN(token_type::random);
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
					ADDTOKEN(token_type::event);
				} else if (Parser.WordOn == AndLiteral) {
					ADDTOKEN(token_type::andd);
				} else if (Parser.WordOn == ActionLiteral) {
					ADDTOKEN(token_type::playerAction);
				} else if (Parser.WordOn == GreaterThanOrEqualLiteral) {
					ADDTOKEN(token_type::greaterThanOrEqual);
				} else if (Parser.WordOn == LessThanOrEqualLiteral) {
					ADDTOKEN(token_type::lessThanOrEqual);
				} else if (Parser.WordOnIndex != 0 && StringIsInt(Parser.WordOn) && !CharIsInt(ThisChar) && ThisChar == ')') {
					ADDTOKEN(token_type::number);
				}

				if (Parser.WordOnIndex != 0 && (ThisChar == ' ' || ThisChar == '.')) {
					CheckNumberOrID(&Parser, &Tokens, CurrLineNum);
				}

				if (ThisChar == '.') {
					ADDTOKEN(token_type::period);
				} else if (ThisChar == ':') {
					CheckNumberOrID(&Parser, &Tokens, CurrLineNum);
					ADDTOKEN(token_type::colon);
				} else if (ThisChar == ',') {
					CheckNumberOrID(&Parser, &Tokens, CurrLineNum);
					ADDTOKEN(token_type::comma);
				} else if (ThisChar == '#') {
					InCommentMode = true;
				} else if (ThisChar == '=') {
					ADDTOKEN(token_type::equalTo);
				} else if (ThisChar == '<') {
					ADDTOKEN(token_type::lessThan);
				} else if (ThisChar == '>') {
					ADDTOKEN(token_type::greaterThan);
				} else if (ThisChar == '-') {
					CheckNumberOrID(&Parser, &Tokens, CurrLineNum);
					ADDTOKEN(token_type::sub);
				} else if (ThisChar == '+') {
					CheckNumberOrID(&Parser, &Tokens, CurrLineNum);
					ADDTOKEN(token_type::add);
				} else if (ThisChar == '/') {
					CheckNumberOrID(&Parser, &Tokens, CurrLineNum);
					ADDTOKEN(token_type::div);
				} else if (ThisChar == '*') {
					CheckNumberOrID(&Parser, &Tokens, CurrLineNum);
					ADDTOKEN(token_type::mult);
				} else if (ThisChar == ')') {
					CheckNumberOrID(&Parser, &Tokens, CurrLineNum);
					ADDTOKEN(token_type::closeParen);
				} else if (ThisChar == '(') {
					CheckNumberOrID(&Parser, &Tokens, CurrLineNum);
					ADDTOKEN(token_type::openParen);
				} else if (ThisChar == ']') {
					CheckNumberOrID(&Parser, &Tokens, CurrLineNum);
					ADDTOKEN(token_type::closeSquare);
				} else if (ThisChar == '[') {
					CheckNumberOrID(&Parser, &Tokens, CurrLineNum);
					ADDTOKEN(token_type::openSquare);
				} else if (ThisChar == '}') {
					CheckNumberOrID(&Parser, &Tokens, CurrLineNum);
					ADDTOKEN(token_type::closedCurly);
				} else if (ThisChar == '{') {
					CheckNumberOrID(&Parser, &Tokens, CurrLineNum);
					ADDTOKEN(token_type::openCurly);
				} else if (ThisChar == DotNotationChar) {
					CheckNumberOrID(&Parser, &Tokens, CurrLineNum);
					ADDTOKEN(token_type::dot);
				} else if (ThisChar == '\r') {
					CheckNumberOrID(&Parser, &Tokens, CurrLineNum);
				} else if (*Parser.CharOn != ' ' && ThisChar != '\r' && ThisChar != '\t' && ThisChar != '\n') {
					Parser.WordOn[Parser.WordOnIndex] = *Parser.CharOn;
					Parser.WordOnIndex++;
				}

				Parser.CharOn++;
			}
		}

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

				// Do some error checking while parsing the statement.
				{
					int32 StatementEnd = Index - 1;

					// Checking that global fluents end with a period
					if (Tokens.Tokens[StatementStart].Type == token_type::id &&
					        Tokens.Tokens[StatementStart + 1].Type == token_type::openParen &&
					        Tokens.Tokens[StatementEnd].Type == token_type::closeParen &&
					        Tokens.Tokens[StatementEnd + 1].Type != token_type::period) {
						free(Tokens.Tokens);
						ThrowError(Tokens.Tokens[StatementEnd].LineNumber, ExpectedPeriodPrefix);
					}

					// Checking that entity fluents end with a period.
					if (Tokens.Tokens[StatementStart].Type == token_type::id &&
					        Tokens.Tokens[StatementStart + 1].Type == token_type::dot &&
					        Tokens.Tokens[StatementEnd].Type == token_type::closeParen &&
					        Tokens.Tokens[StatementEnd + 1].Type != token_type::period) {
						free(Tokens.Tokens);
						ThrowError(Tokens.Tokens[StatementEnd].LineNumber, ExpectedPeriodPrefix);
					}

					// Making sure methods have a using or does
					if (Tokens.Tokens[StatementStart].Type == token_type::event &&
					        Tokens.Tokens[StatementStart + 1].Type == token_type::id &&
					        Tokens.Tokens[StatementStart + 2].Type != token_type::usingg &&
					        Tokens.Tokens[StatementStart + 2].Type != token_type::does) {
						free(Tokens.Tokens);
						ThrowError(Tokens.Tokens[StatementStart].LineNumber, "Method id must be followed by a using or a does. Methods follow does-if pattern.");
					}
				}

				// Find a pattern for the tokens that we have
				if (NextToken.Type == token_type::period) {
					int32 StatementEnd = Index - 1;

					if (Tokens.Tokens[StatementStart].Type == token_type::id &&
					        Tokens.Tokens[StatementStart + 1].Type == token_type::equalTo &&
					        Tokens.Tokens[StatementStart + 2].Type != token_type::playerAction &&
					        Tokens.Tokens[StatementStart + 2].Type != token_type::event &&
					        Tokens.Tokens[StatementStart + 2].Type != token_type::openSquare &&
					        Tokens.Tokens[StatementEnd + 1].Type == token_type::period) {
						// Fluent with value statement

						GameDefinition->Fluents[GameDefinition->FluentsCount] =
						    GrabFluent(Tokens.Tokens, StatementStart, true,
						               GameDefinition->Fluents, GameDefinition->FluentsCount,
						               GameDefinition->InstancedEntities, GameDefinition->InstancedEntitiesCount);
						GameDefinition->FluentsCount++;

						RESET
					} else if (Tokens.Tokens[StatementStart].Type == token_type::id &&
					           Tokens.Tokens[StatementStart + 1].Type == token_type::equalTo &&
					           Tokens.Tokens[StatementStart + 2].Type == token_type::openSquare &&
					           Tokens.Tokens[StatementEnd].Type == token_type::closedCurly) {
						// array which creates the objects it holds.

						array* NextArray = &GameDefinition->Arrays[GameDefinition->ArraysCount];
						GameDefinition->ArraysCount++;
						Assert(GameDefinition->ArraysCount < 100);

						NextArray->Name = Tokens.Tokens[StatementStart].Name;
						int32 ArrayLength = StringToInt32(Tokens.Tokens[StatementStart + 3].Name);

						/*
						This is the entity that we're duplicating and putting into the array.
						Basically the entity is duplicated, given a mangled name, and placed into the array.
						*/
						entity BaseEntity = {};
						GrabEntityFluents(&BaseEntity, Tokens.Tokens, StatementStart + 4, GameDefinition);

						for (int32 Index  = 0; Index < ArrayLength; Index++) {
							GameDefinition->InstancedEntities[GameDefinition->InstancedEntitiesCount] = BaseEntity;
							GameDefinition->InstancedEntities[GameDefinition->InstancedEntitiesCount].Name = BuildArrayFluentID(NextArray->Name, Index);

							NextArray->Array[NextArray->ArrayCount] = GameDefinition->InstancedEntities[GameDefinition->InstancedEntitiesCount].Name;

							NextArray->ArrayCount++;
							GameDefinition->InstancedEntitiesCount++;
						}

						RESET
					} else if (Tokens.Tokens[StatementStart].Type == token_type::id &&
					           Tokens.Tokens[StatementStart + 1].Type == token_type::equalTo &&
					           Tokens.Tokens[StatementStart + 2].Type == token_type::openSquare &&
					           Tokens.Tokens[StatementEnd].Type == token_type::closeSquare) {
						// array which puts already declared things inside it.

						array* NextArray = &GameDefinition->Arrays[GameDefinition->ArraysCount];
						GameDefinition->ArraysCount++;
						Assert(GameDefinition->ArraysCount < 100);

						NextArray->Name = Tokens.Tokens[StatementStart].Name;

						int32 TI = StatementStart + 3;
						while (Tokens.Tokens[TI].Type != token_type::period && Tokens.Tokens[TI].Type != token_type::closeSquare) {

							token_info* NextToken = &Tokens.Tokens[TI];
							if (NextToken->Type == token_type::id) {
								NextArray->Array[NextArray->ArrayCount] = NextToken->Name;
								NextArray->ArrayCount++;
							} else {
								return (BuildErrorString(NextToken->LineNumber, "Invalid token type in an array."));
							}

							// Move to next fluent
							while (Tokens.Tokens[TI].Type != token_type::period &&
							        Tokens.Tokens[TI].Type != token_type::closeSquare &&
							        Tokens.Tokens[TI].Type != token_type::comma) {
								TI++;
							}

							TI++;
						}

						RESET
					} else if (Tokens.Tokens[StatementStart].Type == token_type::id &&
					           Tokens.Tokens[StatementStart + 1].Type == token_type::dot) {
						// This is setting or creating a fluent within an instance

						string EntityName = Tokens.Tokens[StatementStart].Name;
						string FluentName = Tokens.Tokens[StatementStart + 2].Name;

						entity* Entity = FindEntity(EntityName, GameDefinition->InstancedEntities, GameDefinition->InstancedEntitiesCount);
						if (Entity == NULL) {
							// That entity name does not exist. Report error.
							free(Tokens.Tokens);
							ThrowError(Tokens.Tokens[StatementStart].LineNumber, EntityNotFoundPrefix + EntityName);
						}

						fluent* Fluent = FindFluentInList(FluentName, Entity->Fluents, Entity->FluentsCount);
						if (Fluent != NULL) {
							// modify the existing one

							Fluent->Value = GrabFluent(Tokens.Tokens, StatementStart + 2, true,
							                           GameDefinition->Fluents, GameDefinition->FluentsCount,
							                           GameDefinition->InstancedEntities, GameDefinition->InstancedEntitiesCount).Value;
						} else {
							free(Tokens.Tokens);
							ThrowError(Tokens.Tokens[StatementStart].LineNumber, FluentNotFoundPrefix + FluentName);
						}

						RESET
					} else if (Tokens.Tokens[StatementStart].Type == token_type::id &&
					           Tokens.Tokens[StatementStart + 1].Type == token_type::openSquare) {
						// array assignment by index. These always have dot notation. This behavior is in TokensChangeState, because this is not a decleration.
						TokensChangeState(Tokens.Tokens + StatementStart, StatementEnd - StatementStart, GameDefinition);
						RESET
					} else if (Tokens.Tokens[StatementStart].Type == token_type::id &&
					           Tokens.Tokens[StatementStart + 1].Type == token_type::period) {
						// Fluent without value

						fluent *FluentAdding = &GameDefinition->Fluents[GameDefinition->FluentsCount];
						FluentAdding->Name = Tokens.Tokens[StatementStart].Name;
						FluentAdding->Value = 0.0f;

						GameDefinition->FluentsCount++;

						RESET
					} else if (Tokens.Tokens[StatementStart].Type == token_type::id &&
					           Tokens.Tokens[StatementStart + 1].Type == token_type::equalTo &&
					           Tokens.Tokens[StatementStart + 2].Type == token_type::playerAction) {
						// Action / event

						event* Event = &GameDefinition->Events[GameDefinition->EventsCount];
						GameDefinition->EventsCount++;
						Event->Name = Tokens.Tokens[StatementStart].Name;

						int32 TI = StatementStart + 3;

						// Get all the does-ifs
						while (Tokens.Tokens[TI].Type != token_type::period) {
							does_if_return Ret = GetDoesIf(&Tokens, TI);
							TI += Ret.TokensConsumed;

							Event->DoesIf[Event->DoesIfCount] = Ret.DoesIf;
							Event->DoesIfCount++;
						}

						RESET
					} else if (Tokens.Tokens[StatementStart].Type == token_type::id &&
					           Tokens.Tokens[StatementStart + 1].Type == token_type::equalTo &&
					           Tokens.Tokens[StatementStart + 2].Type == token_type::event) {
						// Method

						GameDefinition->Methods[GameDefinition->MethodsCount] = {};

						method* NextMethod = &GameDefinition->Methods[GameDefinition->MethodsCount];
						GameDefinition->MethodsCount++;
						NextMethod->Name = Tokens.Tokens[StatementStart].Name;

						int32 UsingOffset = 4;

						// grab the using strings
						if (Tokens.Tokens[StatementStart + UsingOffset - 1].Type == token_type::usingg) {
							while (Tokens.Tokens[StatementStart + UsingOffset].Type != token_type::does) {

								if (IsStartingToken(Tokens.Tokens[StatementStart + UsingOffset].Type)) {
									free(Tokens.Tokens);
									return (BuildErrorString(Tokens.Tokens[StatementStart + UsingOffset].LineNumber, InvalidStatementStarterPrefix));
								}

								if (Tokens.Tokens[StatementStart + UsingOffset].Type == token_type::id) {
									NextMethod->Usings[NextMethod->UsingsCount] = Tokens.Tokens[StatementStart + UsingOffset].Name;
									NextMethod->UsingsCount++;
								}
								UsingOffset++;
							}

							UsingOffset++;
						}

						// Get all the does-ifs
						while (Tokens.Tokens[StatementStart + UsingOffset].Type != token_type::period) {
							does_if_return Ret = GetDoesIf(&Tokens, StatementStart + UsingOffset);
							UsingOffset += Ret.TokensConsumed;

							NextMethod->DoesIf[NextMethod->DoesIfCount] = Ret.DoesIf;
							NextMethod->DoesIfCount++;
						}

						RESET
					} else if (Tokens.Tokens[StatementStart].Type == token_type::id &&
					           Tokens.Tokens[StatementEnd].Type == token_type::closedCurly) {
						// Entity

						// These hold which entities we're now instancing
						int32 EntityListStart = GameDefinition->InstancedEntitiesCount;
						int32 EntitiyListEnd = GameDefinition->InstancedEntitiesCount;

						int32 TI = StatementStart;
						while (Tokens.Tokens[TI].Type != token_type::equalTo) {
							if (Tokens.Tokens[TI].Type == token_type::id) {
								// Add a new instanced entity
								EntitiyListEnd++;

								GameDefinition->InstancedEntities[GameDefinition->InstancedEntitiesCount].Name = Tokens.Tokens[TI].Name;
								GameDefinition->InstancedEntitiesCount++;

							} else if (Tokens.Tokens[TI].Type == token_type::comma) {
								// just ignore the comma, but it's still valid
							} else {
								// Something unexpected before the colon.
								// For entities, only names can come before the colon.
								free(Tokens.Tokens);
								return (BuildErrorString(Tokens.Tokens[TI].LineNumber, "Unallowed character in entity identifier."));
							}

							TI++;
						}

						// Get the base entity
						entity BaseEntity = {};
						GrabEntityFluents(&BaseEntity, Tokens.Tokens, TI, GameDefinition);

						// Fill the entities with the right fluents
						for (int32 Index = EntityListStart; Index < EntitiyListEnd; Index++) {
							string OldName = GameDefinition->InstancedEntities[Index].Name;
							GameDefinition->InstancedEntities[Index] = BaseEntity;
							GameDefinition->InstancedEntities[Index].Name = OldName;
						}

						RESET
					} else {
						int32 LineNumber = Tokens.Tokens[StatementStart].LineNumber;
						free(Tokens.Tokens);
						return (BuildErrorString(LineNumber, "This statement did not match any of the known patterns."));
					}
				}
			}
		}

		free(Tokens.Tokens);
	}

	return ("x");
}

/* This goes through each does-if, checks if it's valid, and does it if it s.
This is effectivel the 'runtime'. All state changes are done through these events.
Any errors that occur during runtime are returned here.
*/
string
DoEvent(event* Event, game_def* Rules)
{
	GlobalProgState = prog_state::runtime;

	if (setjmp(RuntimeJumpBuffer) == 10) {
		// This is an error. So return the error val;
		return (GlobalErrorDesc);
	}

	if (Event == NULL) {
		ThrowError(0, "Action is null, you probably typed the action name incorrectly.");
	}

	for (int32 Index = 0; Index < Event->DoesIfCount; Index++) {
		does_if* DoesIf = &Event->DoesIf[Index];
		if (IfTokensValid(DoesIf->IfTokens, DoesIf->IfTokensCount, Rules)) {
			TokensChangeState(DoesIf->DoesTokens, DoesIf->DoesTokensCount, Rules);
		}
	}

	return ("x");
}

extern "C"
{
#define EXPORT __declspec(dllexport)
	game_def GlobalRulesDef;

	void EXPORT SC_LoadGame(char* Rules, int32 RuleLen, char* ErrorDescBuffer, int32 ErrorBuffSize)
	{
		ZeroMemory(ErrorDescBuffer, ErrorBuffSize);
		string Error = LoadGameDefinition(Rules, RuleLen, &GlobalRulesDef);
		StringCopy(Error.CharArray, ErrorDescBuffer);
	}

	int32 EXPORT SC_GetMethodsCount()
	{
		return (GlobalRulesDef.MethodsCount);
	}

	// If methodIndex is out of bounds then returns 123456
	int EXPORT SC_GetMethodIndexNameLength(int methodIndex)
	{
		if (methodIndex > GlobalRulesDef.MethodsCount) {
			return (123456);
		}

		return (StringLength(GlobalRulesDef.Methods[methodIndex].Name));
	}

	void EXPORT SC_GetMethodIndexName(int MethodIndex, char* BufferFilling)
	{
		if (MethodIndex > GlobalRulesDef.MethodsCount) {
			string OutString = "INDEX OUT OF BOUNDS";
			StringCopy(OutString.CharArray, BufferFilling);
		} else {
			StringCopy(GlobalRulesDef.Methods[MethodIndex].Name.CharArray, BufferFilling);
		}
	}

	// Returns 1 if it exists, 0 if fluent does not exist.
	int32 EXPORT SC_DoesFluentExist(char* Fluent)
	{
		fluent* F = FindFluentInList(Fluent, GlobalRulesDef.Fluents, GlobalRulesDef.FluentsCount);
		if (F == NULL) {
			return (0);
		} else {
			return (1);
		}
	}

	// Returns 123456 if the fluent doesn't exist. Though 0 doesn't necessarily mean an error
	// Though 123456 doesn't necessarily mean an error, thus probably check that the entity exists before using this.
	float EXPORT SC_GetFluentValue(char* Fluent)
	{
		fluent* F = FindFluentInList(Fluent, GlobalRulesDef.Fluents, GlobalRulesDef.FluentsCount);
		if (F == NULL) {
			return (123456);
		}
		return (F->Value);
	}

	int32 EXPORT SC_DoesEntityExist(char* EntityName)
	{
		if (FindEntity(EntityName, GlobalRulesDef.InstancedEntities, GlobalRulesDef.InstancedEntitiesCount) == NULL) {
			return (0);
		}
		return (1);
	}

	// Returns 123456 if the entity or fluent doesn't exist.
	// Though 123456 doesn't necessarily mean an error, thus probably check that the entity exists before using this.
	float EXPORT SC_GetEntityFluent(char* EntityName, char* FluentName)
	{
		entity* Entity = FindEntity(EntityName, GlobalRulesDef.InstancedEntities, GlobalRulesDef.InstancedEntitiesCount);

		if (Entity == NULL) {
			return (123456);
		}

		fluent* Fluent = FindFluentInList(FluentName, Entity->Fluents, Entity->FluentsCount);
		if (Fluent == NULL) {
			return (123456);
		}

		return (Fluent->Value);
	}


	// Returns 1 if action exists, 0 if action doesn't
	int32 EXPORT SC_DoesActionExist(char* ActionName)
	{
		for (int32 Index = 0; Index < GlobalRulesDef.EventsCount; Index++) {
			if (ActionName == GlobalRulesDef.Events[Index].Name) {
				return (1);
			}
		}
		return (0);
	}

	// Returns 123456 if the action doesn't exist. Returns 1 if the action is valid, and 0 if it's not valid.
	int32 EXPORT SC_ActionIsValid(char* ActionName)
	{
		// TODO this return if the action is ENTIRELY invalid. Might be necessary?
		return (0);


		// event* EventChecking = {};
		// for (int32 Index = 0; Index < GlobalRulesDef.EventsCount; Index++) {
		// 	if (ActionName == GlobalRulesDef.Events[Index].Name) {
		// 		EventChecking = &GlobalRulesDef.Events[Index];
		// 		break;
		// 	}
		// }

		// if (EventChecking == NULL) {
		// 	return (123456);
		// }

		// if (EventValid(EventChecking, &GlobalRulesDef)) {
		// 	return (1);
		// }

		// return (0);
	}

	// Does an action. Assumes the action exists, checks to make sure the action is valid.
	// Fills the error buffer with x if no error, otherwise fills it with the error
	void EXPORT SC_DoAction(char* ActionName, char* ErrorBuffer, int32 ErrorBufferSize)
	{
		ZeroMemory(ErrorBuffer, ErrorBufferSize);

		event* EventChecking = {};
		for (int32 Index = 0; Index < GlobalRulesDef.EventsCount; Index++) {
			if (ActionName == GlobalRulesDef.Events[Index].Name) {
				EventChecking = &GlobalRulesDef.Events[Index];
				break;
			}
		}

		string Ret = DoEvent(EventChecking, &GlobalRulesDef);;
		StringCopy(Ret.CharArray, ErrorBuffer);
	}

	int32 EXPORT SC_GetActionsCount()
	{
		return (GlobalRulesDef.EventsCount);
	}


	// This returns 123456 name if the index is out of bounds
	int32 EXPORT SC_GetActionIndexNameLength(int Index)
	{
		if (Index < GlobalRulesDef.EventsCount) {
			return (StringLength(GlobalRulesDef.Events[Index].Name));
		} else {
			return (123456);
		}
	}

	// This returns "INDEX OUT OF BOUND" name if the index is out of bounds
	// This will cut off the action name if the bufferSize isn't big enough
	void EXPORT SC_GetActionNameFromIndex(int ActionIndex, char* BufferFilling)
	{
		if (ActionIndex > GlobalRulesDef.EventsCount) {
			string OutString = "INDEX OUT OF BOUNDS";
			for (int32 Index = 0; Index < StringLength(OutString); Index++) {
				BufferFilling[Index] = OutString.CharArray[Index];
			}
		} else {
			string actionName = GlobalRulesDef.Events[ActionIndex].Name;
			for (int32 Index = 0; Index < StringLength(actionName); Index++) {
				BufferFilling[Index] = actionName.CharArray[Index];
			}
		}
	}

	int32 EXPORT SC_GetFluentInArray(char* ArrayName, int ArrayIndex, char* FluentName)
	{
		return (GetFluentInArray(ArrayName, FluentName, ArrayIndex, 0,
		                         GlobalRulesDef.InstancedEntities, GlobalRulesDef.InstancedEntitiesCount)->Value);
	}

	bool32 EXPORT SC_ArrayExists(string ArrayName)
	{
		for (int32 Index = 0; Index < GlobalRulesDef.ArraysCount; Index++) {
			if (GlobalRulesDef.Arrays[Index].Name == ArrayName) {
				return (true);
			}
		}
		return (false);
	}
}


void
main(int argc, char const **argv)
{
	while (true) {
		char* Prog = {};
		int32 CharactersCount = 0;
		// Load the program into the parser
		{
			string UnityGameFile = "../SandCat_Unity/Assets/Games/Situps/Situps.txt";
			// string UnityGameFile = "../SandCat_Unity/Assets/Games/Test/Test.txt";
			HANDLE FileHandle = CreateFile(UnityGameFile.CharArray, GENERIC_READ, FILE_SHARE_READ,
			                               NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			int32 FileSizeBytes = GetFileSize(FileHandle, NULL);

			// Make sure we got the file
			Assert(FileSizeBytes != 0);

			Prog = (char*)malloc(FileSizeBytes);
			ZeroMemory(Prog, FileSizeBytes);

			CharactersCount = FileSizeBytes;

			DWORD BytesRead;
			if (!ReadFile(FileHandle, (void*)Prog, FileSizeBytes, &BytesRead, NULL)) {
				// Couldn't read the file. Something wrong happened
				Assert(0);
			}

			CloseHandle(FileHandle);
		}

		game_def GameDefinition = {};
		string Error = LoadGameDefinition(Prog, CharactersCount, &GameDefinition);

		if (Error.CharArray[0] == 'x') {

			// This is the playing part
			{
				PrintOptions(&GameDefinition);

				while (true) {
					char Input[100];
					scanf("%99s", &Input);
					char Selection = Input[0];
					system("cls");

					string SelStr = Selection;
					int32 SelInt = StringToInt32(SelStr);
					if (SelInt < GameDefinition.EventsCount) {
						DoEvent(&GameDefinition.Events[SelInt], &GameDefinition);
					} else {
						Print("Not a valid action");
					}

					PrintOptions(&GameDefinition);
				}

				return;
			}
		} else {
			Print(Error);
			Print("\n");
			Print("Press anything to reload the rules.");

			char Input[100];
			scanf("%99s", &Input);
			system("cls");

			free(Prog);
		}
	}
}