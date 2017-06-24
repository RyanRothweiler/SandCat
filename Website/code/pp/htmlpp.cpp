#include <iostream>
#include <windows.h>

#include <time.h>
#include <stdlib.h>

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

struct post {
	char *HTMLBody;
	string Date;
	string Title;
	string Sample;
};

void AddChar(char Character, char* Array, int32* Index)
{
	Array[*Index] = Character;
	(*Index)++;
}

void AddString(string String, char* Array, int32* ArrayIndex)
{
	int32 Len = StringLength(String);
	for (int32 Index = 0; Index < Len; Index++) {
		AddChar(String.CharArray[Index], Array, ArrayIndex);
	}
}

void main ()
{

	WIN32_FIND_DATA FindData = {};
	string BasePath = "../site/posts/";
	HANDLE FindHandle = FindFirstFile((BasePath + "*").CharArray, &FindData);

	post Posts[100];
	int32 NextPost = 0;

	// The total amount of mem we need for the final js file.

	int32 PostIndex = 1;
	printf("---------------------------------- \n");
	do {

		string FullPath = BasePath + FindData.cFileName;
		HANDLE FileHandle = CreateFile(FullPath.CharArray, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		LARGE_INTEGER FileSize;
		FileSize.LowPart = FindData.nFileSizeLow;
		FileSize.HighPart = FindData.nFileSizeHigh;
		// LONGLONG FileSize = FileSize.QuadPart;

		if (FileHandle != NULL && FileSize.QuadPart > 0) {
			printf("Processing ");
			printf(FullPath.CharArray);
			printf("\n");

			post * PostCreating = &Posts[NextPost];
			NextPost++;

			PostCreating->HTMLBody = (char*)malloc(FileSize.QuadPart);
			ZeroMemory(PostCreating->HTMLBody, FileSize.QuadPart);

			LPDWORD BytesRead = {};
			DWORD BytesToRead = FileSize.QuadPart;
			bool Success = ReadFile(FileHandle, PostCreating->HTMLBody, BytesToRead, BytesRead, NULL);

			// Get post title
			{
				int32 Index = 0;

				char* Title = (char*)malloc(MAX_STRING_SIZE);
				ZeroMemory(Title, MAX_STRING_SIZE);

				int32 TitleIndex = 0;
				while (PostCreating->HTMLBody[Index] != '\n') {
					Title[TitleIndex] = PostCreating->HTMLBody[Index];
					TitleIndex++;

					PostCreating->HTMLBody[Index] = ' ';
					Index++;
				}
				PostCreating->HTMLBody[Index] = ' ';

				PostCreating->Title = Title;
			}

			// Get post date
			{
				int32 Index = 0;

				char* Title = (char*)malloc(MAX_STRING_SIZE);
				ZeroMemory(Title, MAX_STRING_SIZE);

				int32 TitleIndex = 0;
				while (PostCreating->HTMLBody[Index] != '\n') {

					if (PostCreating->HTMLBody[Index] != ' ') {
						Title[TitleIndex] = PostCreating->HTMLBody[Index];
						TitleIndex++;
					}

					PostCreating->HTMLBody[Index] = ' ';
					Index++;
				}

				PostCreating->Date = Title;
			}

			// PostCreating->HTMLBody = (char*)malloc()
			PostCreating->Sample = "Sample goes here. Sample goes here. Sample goes here. Sample goes here. Sample goes here.";

			string PostIndexStr = PostIndex;
			PostIndex++;

			printf("	");
			printf(PostIndexStr.CharArray);
			printf("\n");
			printf("		Title - ");
			printf(PostCreating->Title.CharArray);
			printf("\n");
			printf("		Date - ");
			printf(PostCreating->Date.CharArray);
			printf("\n");
			printf("---------------------------------- \n");
		}

	} while (FindNextFile(FindHandle, &FindData));


	// Now place all this info into the js file

	HANDLE JSFile = CreateFile("../site/processor_base.js", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	int32 JSFileSize = GetFileSize(JSFile, NULL);
	char* JSData = (char*)malloc(JSFileSize);
	LPDWORD BytesRead = {};
	bool Success = ReadFile(JSFile, JSData, JSFileSize, BytesRead, NULL);

	// Create the titles string
	int32 TitlesLength = 5000;
	int32 TitlesCount = 0;
	char* Titles = (char*)malloc(TitlesLength);
	ZeroMemory(Titles, TitlesLength);

	AddString("var NewsTitles = [", Titles, &TitlesCount);
	for (int32 Index = 0; Index < NextPost; Index++) {
		AddChar('\'', Titles, &TitlesCount);
		AddString(Posts[Index].Title, Titles, &TitlesCount);

		AddChar('\'', Titles, &TitlesCount);
		if (Index != NextPost - 1) {
			AddChar(',', Titles, &TitlesCount);
		}
	}
	AddString("];", Titles, &TitlesCount);


	// Create the dates string
	int32 DatesLength = 5000;
	int32 DatesCount = 0;
	char* Dates = (char*)malloc(DatesLength);
	ZeroMemory(Dates, DatesLength);

	AddString("var NewsDates = [", Dates, &DatesCount);
	for (int32 Index = 0; Index < NextPost; Index++) {
		AddChar('\'', Dates, &DatesCount);
		AddString(Posts[Index].Date, Dates, &DatesCount);

		AddChar('\'', Dates, &DatesCount);
		if (Index != NextPost - 1) {
			AddChar(',', Dates, &DatesCount);
		}
	}
	AddString("];", Dates, &DatesCount);


	// Create the samples string
	int32 SamplesLength = 5000;
	int32 SamplesCount = 0;
	char* Samples = (char*)malloc(SamplesLength);
	ZeroMemory(Samples, SamplesLength);

	AddString("var NewsSample = [", Samples, &SamplesCount);
	for (int32 Index = 0; Index < NextPost; Index++) {
		AddChar('\'', Samples, &SamplesCount);
		AddString(Posts[Index].Sample, Samples, &SamplesCount);

		AddChar('\'', Samples, &SamplesCount);
		if (Index != NextPost - 1) {
			AddChar(',', Samples, &SamplesCount);
		}
	}
	AddString("];", Samples, &SamplesCount);


	int64 TotalJSSize = 0;
	TotalJSSize += TitlesCount;
	TotalJSSize += DatesCount;
	TotalJSSize += SamplesCount;
	TotalJSSize += JSFileSize;

	char* FinalJSPointer = (char*)malloc(TotalJSSize);
	char* FinalJS = FinalJSPointer;
	memcpy(FinalJSPointer, Titles, TitlesCount);
	FinalJSPointer += TitlesCount;

	memcpy(FinalJSPointer, Dates, DatesCount);
	FinalJSPointer += DatesCount;

	memcpy(FinalJSPointer, Samples, SamplesCount);
	FinalJSPointer += SamplesCount;

	memcpy(FinalJSPointer, JSData, JSFileSize);
	FinalJSPointer += JSFileSize;

	HANDLE FinalJSFile = CreateFile("../site/processor.js", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD BytesWritten = {};
	bool SuccessWrite = WriteFile(FinalJSFile, FinalJS, TotalJSSize, &BytesWritten, NULL);

	printf("Done :)");
}