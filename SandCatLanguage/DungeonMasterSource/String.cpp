#ifndef STRING_CPP
#define STRING_CPP


#define MAX_STRING_SIZE 100

// TODO need a real64 to string conversion

int32
DigitCount(int64 Input)
{
	int32 Count = 0;
	while (Input != 0)
	{
		Input = Input / 10;
		Count++;
	}

	return (Count);
}

void
IntToCharArray(int64 Input, char *Output, uint32 Offset)
{
	char *OutputPointer = Output + DigitCount(Input) - 1 + Offset;

	if (Input == 0)
	{
		*Output = '0';
	}

	int32 Count = 1;
	while (Input != 0)
	{
		if (Count % 4 == 0)
		{
			// *OutputPointer = ',';
			// OutputPointer--;
		}
		else
		{
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
	while (*String++)
	{
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
	        Index++)
	{
		*Destination++ = *SourceA++;
	}

	for (int32 Index = 0;
	        Index < SourceBLength;
	        Index++)
	{
		*Destination++ = *SourceB++;
	}

	*Destination++ = 0;
}


struct string
{
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
		        Index++)
		{
			CharArray[Index] = Input[Index];
		}
		CharArray[StringCount] = '\0';
	}

	// TODO can we compress these?
	string(int64 Input)
	{
		if (Input < 0)
		{
			CharArray[0] = '-';
			IntToCharArray(Input * -1, CharArray, 1);
		}
		else
		{
			IntToCharArray(Input, CharArray);
		}
	}

	string(int32 Input)
	{
		if (Input < 0)
		{
			CharArray[0] = '-';
			IntToCharArray(Input * -1, CharArray, 1);
		}
		else
		{
			IntToCharArray(Input, CharArray);
		}
	}


	string(uint32 Input)
	{
		if (Input < 0)
		{
			CharArray[0] = '-';
			IntToCharArray(Input * -1, CharArray, 1);
		}
		else
		{
			IntToCharArray(Input, CharArray);
		}
	}

	string (real64 Input)
	{
		uint32 PreDecimalCount = DigitCount((int32)Input);

		real64 MovedDecimal = 100 * Input;
		char Dummy[MAX_STRING_SIZE] = {};
		if (Input < 0)
		{
			PreDecimalCount++;
			IntToCharArray((int64)(-MovedDecimal), Dummy);
		}
		else
		{
			IntToCharArray((int64)MovedDecimal, Dummy);
		}

		uint32 ArrayIndex = 0;

		if (Input < 0.0f)
		{
			CharArray[0] = '-';
			ArrayIndex++;
		}

		for (uint32 Index = 0; Index < MAX_STRING_SIZE - 1; Index++)
		{
			if (ArrayIndex == PreDecimalCount)
			{
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

	if (ALength == BLength)
	{
		for (int charIndex = 0;
		        charIndex < ALength;
		        charIndex++)
		{
			if (A.CharArray[charIndex] != B.CharArray[charIndex])
			{
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
	        Index++)
	{
		FinalString.CharArray[Index] = OrigString.CharArray[Index];
	}

	return (FinalString);
}

void
ClearString(string *String)
{
	for (int CharIndex = 0;
	        CharIndex < MAX_STRING_SIZE;
	        CharIndex++)
	{
		String->CharArray[CharIndex] = 0x00000000;
	}
}

struct split_info
{
	void *Strings;
	uint32 StringsCount;
};

// split_info
// SplitString(string StringSplitting, char SplitChar, bool32 KeepSplitChar, void *MemoryUsing)
// {
// 	split_info SplitInfo = {};

// 	uint32 WordLength = StringLength(StringSplitting);

// 	uint32 SplitStartIndex = 0;
// 	uint32 SplitCheckIndex = 1;

// 	uint32 CurrentArraySize = 50;
// 	void *Array = ArenaAllocate(Memory, sizeof(string) * CurrentArraySize);
// 	string *SplitStrings = (string *)Array;
// 	uint32 CurrentStringsIndex = 0;

// 	SplitInfo.StringsCount = 0;
// 	SplitInfo.Strings = SplitStrings;

// 	while (SplitCheckIndex < WordLength)
// 	{
// 		if (StringSplitting.CharArray[SplitCheckIndex] == SplitChar)
// 		{
// 			ClearString(&SplitStrings[CurrentStringsIndex]);
// 			char *CharArray = SplitStrings[CurrentStringsIndex].CharArray;

// 			for (uint32 Index = SplitStartIndex;
// 			        Index < SplitCheckIndex;
// 			        Index++)
// 			{
// 				uint32 StringFillingIndex = Index - SplitStartIndex;
// 				SplitStrings[CurrentStringsIndex].CharArray[StringFillingIndex] = StringSplitting.CharArray[Index];
// 			}

// 			CurrentStringsIndex++;

// 			// TODO make a system which doubles the size when we want an array larger thatn the one delclared here.
// 			// do such a thing here instead of asserting this
// 			Assert(CurrentStringsIndex < CurrentArraySize);

// 			SplitInfo.StringsCount++;

// 			SplitStartIndex = SplitCheckIndex;
// 			if (!KeepSplitChar)
// 			{
// 				SplitStartIndex++;
// 			}

// 			SplitCheckIndex++;
// 		}
// 		else
// 		{
// 			SplitCheckIndex++;
// 		}
// 	}

// 	// add the last string
// 	SplitStrings[CurrentStringsIndex] = string{};
// 	char *CharArray = SplitStrings[CurrentStringsIndex].CharArray;

// 	for (uint32 Index = SplitStartIndex;
// 	        Index < SplitCheckIndex;
// 	        Index++)
// 	{
// 		uint32 StringFillingIndex = Index - SplitStartIndex;
// 		SplitStrings[CurrentStringsIndex].CharArray[StringFillingIndex] = StringSplitting.CharArray[Index];
// 	}
// 	CharArray[SplitCheckIndex + 1] = '\0';

// 	CurrentStringsIndex++;
// 	SplitInfo.StringsCount++;

// 	return (SplitInfo);
// }

bool32
StringIsInt(string String) {
	uint32 StringCount = CharArrayLength(String.CharArray);
	for (uint32 CharIndex = 0; CharIndex < StringCount; CharIndex++)
	{
		char NextCharacter = String.CharArray[CharIndex];
		if (NextCharacter != '0' && NextCharacter != '1' && NextCharacter != '2' && NextCharacter != '3' &&
		        NextCharacter != '4' && NextCharacter != '5' && NextCharacter != '6' && NextCharacter != '7' &&
		        NextCharacter != '8' && NextCharacter != '9') {
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
	for (uint32 CharIndex = 0; CharIndex < StringCount; CharIndex++)
	{
		FinalNumber = FinalNumber * 10;

		char NextCharacter = String.CharArray[CharIndex];
		switch (NextCharacter)
		{
		case ('0'):
		{
			FinalNumber += 0;
		}
		break;

		case ('1'):
		{
			FinalNumber += 1;
		}
		break;

		case ('2'):
		{
			FinalNumber += 2;
		}
		break;

		case ('3'):
		{
			FinalNumber += 3;
		}
		break;

		case ('4'):
		{
			FinalNumber += 4;
		}
		break;

		case ('5'):
		{
			FinalNumber += 5;
		}
		break;

		case ('6'):
		{
			FinalNumber += 6;
		}
		break;

		case ('7'):
		{
			FinalNumber += 7;
		}
		break;

		case ('8'):
		{
			FinalNumber += 8;
		}
		break;

		case ('9'):
		{
			FinalNumber += 9;
		}
		break;

		default:
		{
			// String is not entirely a number
			Assert(0);
		}
		break;
		}
	}

	return (FinalNumber);
}

#if UNIT_TESTING
void
StringUnitTests()
{
	string First = {};
	string Second = {};

	First = "ABC124!+-`,./";
	Second = "ABC124!+-`,./";
	Assert(StringLength(First) == 13);
	Assert(First == First);
	Assert(First == Second);
	Assert(First == "ABC124!+-`,./");

	First = "Donald Trump";
	Second = "President";
	Assert(First != Second);

	Assert(StringLength(First) == 12);

	ClearString(&First);
	int length = StringLength(First);
	Assert(StringLength(First) == 0);

	/* TODO need unit tests for
		- SplitString
		- CopyString
		- ConcatIntChar
		- DigitCount
		- IntToCharArray (two methods_)
		*/
}
#endif


#endif