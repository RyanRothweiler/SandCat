#include <iostream>
#include <windows.h>
#include "RyansTypes.cpp"
#include "String.cpp"

// Literals
string RoleLiteral = "role";
string TrueLiteral = "true";
string FalseLiteral = "false";
string EventLiteral = "event";

void Print(string Output) {
	Output = Output + "\n";
	printf(Output.CharArray);
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

struct fluent {
	string FluentName;
	bool32 IsTrue;

	fluent* NestedFluent;
	bool32 IsNested;

	// If this fluent has been named. This is janky and should be done a different way.
	bool32 Completed;
};

struct event {
	string EventName;
};

enum event_parse_state {
	EventParseState_Name,
	EventParseState_Consquences,
	EventParseState_Validation,
};

enum parser_state {
	ParserState_NoGoal,
	ParserState_GrabbingRole,
	ParserState_GrabbingFluent,
	ParserState_GrabbingEvent,
};

struct parser {
	parser_state State;

	char* CharOn;

	char WordOn[100] = {};
	int32 WordOnIndex = 0;
};

void ResetParserWord(parser* Parser) {
	ZeroMemory(&Parser->WordOn, ARRAY_SIZE(Parser->WordOn, char));
	Parser->WordOnIndex = 0;
}

void ChangeState(parser* Parser, parser_state NewState) {
	ResetParserWord(Parser);
	Parser->State = NewState;
}

void AddToParser(parser* Parser, char NewChar) {
	Parser->WordOn[Parser->WordOnIndex] = NewChar;
	Parser->WordOnIndex++;
}


fluent GrabFluent(parser* Parser) {
	fluent FinalFluent = {};
	FinalFluent.IsTrue = (Parser->WordOn == TrueLiteral);

	ResetParserWord(Parser);

	for (;;) {
		char ThisChar = *Parser->CharOn;

		if (ThisChar == ')') {
			FinalFluent.FluentName = Parser->WordOn;
			FinalFluent.Completed = true;
			break;
		} else if (ThisChar == '(') {
			// TODO maybe this?? A feature we might want later.

			// // Nest the fluents
			// FLuent.FluentName = Parser.WordOn;
			// FLuent.NestedFluent = &Fluents[NextFluent + 1];
			// FLuent.Completed = true;
			// FLuent.IsNested = false;

			// Fluents[NextFluent + 1].IsNested = true;

			// NextFluent++;
			// ResetParserWord(&Parser);
		} else {
			AddToParser(Parser, ThisChar);
		}

		Parser->CharOn++;
	}

	ResetParserWord(Parser);
	return (FinalFluent);
}

void main(int argc, char const **argv) {

	// These are the 'state' of the game
	string RoleName = {};

	int32 MaxFluents = 100;
	fluent* Fluents = (fluent*)malloc(MaxFluents * sizeof(fluent));
	int32 NextFluent = 0;

	int32 MaxEvents = 100;
	event* Events = (event*)malloc(MaxEvents * sizeof(event));
	int32 NextEvent = 0;

	// This is the parsing part. Load initial data from script.
	{
		parser Parser = {};
		ChangeState(&Parser, ParserState_NoGoal);

		Parser.CharOn = LoadProg("../Games/Gas.sc");

		event_parse_state EventParseState = EventParseState_Name;

		for (;;) {
			char ThisChar = *Parser.CharOn;
			if (ThisChar == '.') {
				// end of program
				break;
			}

			// discard unwanted characters
			if (ThisChar == '\r') {
				Parser.CharOn++;
				continue;
			} else if (ThisChar == '\n') {
				Parser.CharOn++;
				continue;
			}

			if (Parser.State == ParserState_GrabbingRole) {
				if (ThisChar == ')') {
					// Role was closed
					RoleName = Parser.WordOn;

					ChangeState(&Parser, ParserState_NoGoal);
				} else  {
					AddToParser(&Parser, ThisChar);
				}
			} else if (Parser.State == ParserState_GrabbingFluent) {
			} else if (Parser.State == ParserState_GrabbingEvent) {
				// // event(GoToStore(false(HasGas), false(FoodAtCostco), true(FoodAtHome)) : HasGas, FoodAtCostco)
				// if (ThisChar == '(') {
				// 	if (EventParseState == EventParseState_Name) {
				// 		Events[NextEvent].Name = Parse.WordOn;
				// 		ResetparserWord(&Parser);
				// 		NextChar++;
				// 		EventParseState = EventParseState_Consquences;
				// 	} else if (EventParseState == EventParseState_Consquences) {
				// 		EventParseState = EventParseState_Validation;
				// 	} else if (EventParseState == EventParseState_Validation) {

				// 	}

				// } else if (ThisChar == ',' && EventParseState == EventParseState_Consquences) {
				// 	Events[NextEvent].
				// } else {
				// 	AddToParser(&Parser, ThisChar);
				// }
			} else if (Parser.State == ParserState_NoGoal) {

				// we only want to look for things that could possibly be a reserved word
				if (ThisChar != ')') {
					AddToParser(&Parser, ThisChar);
				}

				if (Parser.WordOn == RoleLiteral) {
					Parser.CharOn++;
					ChangeState(&Parser, ParserState_GrabbingRole);
				} else if (Parser.WordOn == TrueLiteral || Parser.WordOn == FalseLiteral) {
					Parser.CharOn++;
					Fluents[NextFluent] = GrabFluent(&Parser);
					NextFluent++;
					Assert(NextFluent < MaxFluents);
				} else if (Parser.WordOn == EventLiteral) {
					Parser.CharOn++;
					EventParseState = EventParseState_Name;
					ChangeState(&Parser, ParserState_GrabbingEvent);
				}
			}

			Parser.CharOn++;
		}
	}

	// This is the playing part
	{
		bool32 Running = true;
		while (Running) {
			Print("----------------------- ");
			Print("1 - Show State ");
			Print("2 - Show Actions");
			Print("9 - Quit ");
			Print("");

			char Input[100];
			scanf("%99s", &Input);
			char Selection = Input[0];

			if (Selection == '1') {
				// Print State
				Print("\n ---- STATE ----");
				for (int32 Index = 0; Index < NextFluent; Index++) {
					string StrIndex = Index;
					string TruthString = {};
					Fluents[Index].IsTrue ? TruthString = "TRUE" : TruthString = "FALSE";
					Print(StrIndex + ") " + Fluents[Index].FluentName + " " + TruthString);
				}
			} else if (Selection == '2') {
				Print("ACTIONS");
			}
			else if (Selection == '9') {
				Running = false;
			}
		}
	}


	Print("Closing");
	return;
}
