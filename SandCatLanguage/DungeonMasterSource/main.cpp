#include <iostream>
#include <windows.h>
#include "RyansTypes.cpp"
#include "String.cpp"

void Print(char* Output) {
	printf(Output);
}

char* LoadProg(char* FileName) {
	OFSTRUCT FileInfo = {};
	HFILE FileHandle = OpenFile(FileName, &FileInfo, OF_READ);

	// Make sure we got the file handle
	Assert(FileHandle != -1);

	char* Prog = (char*)malloc(FileInfo.cBytes);
	ZeroMemory(Prog, FileInfo.cBytes);

	DWORD BytesRead;
	if (!ReadFile((HANDLE)FileHandle, (void*)Prog, FileInfo.cBytes, &BytesRead, NULL)) {
		// Couldn't read the file. Something wrong happened
		Assert(0);
	}

	return (Prog);
}

enum parser_state {
	ParserState_NoGoal, ParserState_GrabbingRole,
};

void main(int argc, char const **argv) {
	Print("Starting");

	char* GameProg = LoadProg("../Games/TestProg.sc");

	// Load initial data from game.
	{
		// Literals
		string RoleLiteral = "role";

		char* NextChar = GameProg;

		char WordOn[100] = {};
		int32 WordOnIndex = 0;

		string RoleName = {};

		parser_state ParserState = ParserState_NoGoal;

		for (;;) {
			char ThisChar = *NextChar;

			if (ParserState == ParserState_GrabbingRole) {
				if (ThisChar == '(') {
					// Skip this character, role was opened.
				} else if (ThisChar == ')') {
					// Role was closed
					RoleName = WordOn;
				} else  {
					WordOn[WordOnIndex] = ThisChar;
					WordOnIndex++;
				}
			} else if (ParserState == ParserState_NoGoal) {
				WordOn[WordOnIndex] = ThisChar;
				WordOnIndex++;

				if (ThisChar == '.') {
					// end of program
					break;
				}

				if (WordOn == RoleLiteral) {
					ZeroMemory(&WordOn, 100);
					WordOnIndex = 0;
					ParserState = ParserState_GrabbingRole;
				}
			}

			NextChar++;
		}
	}

	return;
}
