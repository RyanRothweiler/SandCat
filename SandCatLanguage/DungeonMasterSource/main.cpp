// windows stuff
#include <iostream>
#include <windows.h>

// my stuff
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
	HANDLE FileHandle = CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ,
	                               NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	int32 FileSizeBytes = GetFileSize(FileHandle, NULL);

	// Make sure we got the file
	Assert(FileSizeBytes != 0);

	char* Prog = (char*)malloc(FileSizeBytes);
	ZeroMemory(Prog, FileSizeBytes);

	DWORD BytesRead;
	if (!ReadFile(FileHandle, (void*)Prog, FileSizeBytes, &BytesRead, NULL)) {
		// Couldn't read the file. Something wrong happened
		Assert(0);
	}

	CloseHandle(FileHandle);
	return (Prog);
}

struct fluent {
	string Name;
	bool32 IsTrue;

	bool32 HasValue;
	real64 Value;

	bool32 Using;
};

struct event {
	string Name;

	fluent ConsquenceFluents[100];
	int32 NextConsqFluent;

	fluent ValidationFluents[100];
	int32 NextValidFluent;
};

enum event_parse_state {
	EventParseState_Name,
	EventParseState_Consquences,
	EventParseState_Validation,
};

enum parser_state {
	ParserState_NoGoal,
	ParserState_GrabbingRole,
	ParserState_GrabbingEvent,
};

struct parser {
	parser_state State;

	char* CharOn;

	char WordOn[100] = {};
	int32 WordOnIndex = 0;
};

void
ResetParserWord(parser* Parser) {
	ZeroMemory(&Parser->WordOn, ARRAY_SIZE(Parser->WordOn, char));
	Parser->WordOnIndex = 0;
}

void
ChangeState(parser* Parser, parser_state NewState) {
	ResetParserWord(Parser);
	Parser->State = NewState;
}

void
AddToParser(parser* Parser) {
	Parser->WordOn[Parser->WordOnIndex] = *Parser->CharOn;
	Parser->WordOnIndex++;
}

enum accum_state {
	accum_add, accum_sub, accum_div, accum_mult, accum_none
};

real64
InfixAccumulate(real64 Accum, char* StringOperand, accum_state Operator,
                fluent* Fluents, int32 FluentsCount) {

	real64 NewOperand = 0.0f;
	if (StringIsInt(StringOperand)) {
		// string is number
		NewOperand = (real64)StringToInt32(StringOperand);
	} else {
		// string is a fluent, we should get the value from the fluent
		for (int32 Index = 0; Index < FluentsCount; Index++) {
			if (Fluents[Index].Name == StringOperand) {
				NewOperand = Fluents[Index].Value;
				goto skip_assert;
			}
		}

		// We did not find that fluent
		Assert(0);
	}

skip_assert:

	if (Operator == accum_add) {
		return (Accum + NewOperand);
	} else if (Operator == accum_sub) {
		return (Accum - NewOperand);
	} else if (Operator == accum_mult) {
		return (Accum * NewOperand);
	} else if (Operator == accum_div) {
		return (Accum / NewOperand);
	} else {
		return (NewOperand);
	}
}

real64
GetValueInFluent(parser* Parser, fluent* Fluents, int32 FluentsCount) {
	ResetParserWord(Parser);
	while (*Parser->CharOn != ')') {
		// NOTE here is where we check to make sure what is in the fluent is infact a number
		if (*Parser->CharOn != '(') {
			AddToParser(Parser);
		}
		Parser->CharOn++;
	}

	bool32 IsNum = StringIsInt(Parser->WordOn);
	if (IsNum) {
		// Is just a value
		return ((real64)StringToInt32(Parser->WordOn));
	} else {
		// Is probably an equation

		real64 Accumulator = 0;
		accum_state AccumState = accum_none;

		char Word[100] = {};
		int32 WordIndex = 0;

		for (int32 Index = 0; Index < Parser->WordOnIndex; Index++) {

			if (Parser->WordOn[Index] == ' ')  {
				continue;
			}

			if (Parser->WordOn[Index] == '+') {
				Accumulator = InfixAccumulate(Accumulator, Word, AccumState, Fluents, FluentsCount);
				AccumState = accum_add;

				ZeroMemory(Word, 100);
				WordIndex = 0;
			} else if (Parser->WordOn[Index] == '-') {
				Accumulator = InfixAccumulate(Accumulator, Word, AccumState, Fluents, FluentsCount);
				AccumState = accum_sub;

				ZeroMemory(Word, 100);
				WordIndex = 0;
			} else if (Parser->WordOn[Index] == '*') {
				Accumulator = InfixAccumulate(Accumulator, Word, AccumState, Fluents, FluentsCount);
				AccumState = accum_mult;

				ZeroMemory(Word, 100);
				WordIndex = 0;
			} else if (Parser->WordOn[Index] == '/') {
				Accumulator = InfixAccumulate(Accumulator, Word, AccumState, Fluents, FluentsCount);
				AccumState = accum_div;

				ZeroMemory(Word, 100);
				WordIndex = 0;
			} else {
				Word[WordIndex] = Parser->WordOn[Index];
				WordIndex++;
			}
		}

		Accumulator = InfixAccumulate(Accumulator, Word, AccumState, Fluents, FluentsCount);
		return (Accumulator);
	}
}

fluent
GrabFluent(parser* Parser, fluent* Fluents, int32 FluentsCount) {
	fluent FinalFluent = {};

	FinalFluent.Using = true;
	FinalFluent.IsTrue = (Parser->WordOn == TrueLiteral);

	ResetParserWord(Parser);

	// Consume the '('
	Parser->CharOn++;

	for (;;) {
		char ThisChar = *Parser->CharOn;

		// Throw away these characters
		if (ThisChar == ',' || ThisChar == ' ' || ThisChar == '-') {
			Parser->CharOn++;
			continue;
		}

		if (ThisChar == ')') {
			FinalFluent.Name = Parser->WordOn;
			break;
		} else if (ThisChar == '(') {
			// This means the fluent has a value
			FinalFluent.Name = Parser->WordOn;
			FinalFluent.HasValue = true;
			FinalFluent.Value = GetValueInFluent(Parser, Fluents, FluentsCount);
			goto end;
		} else {
			AddToParser(Parser);
		}

		Parser->CharOn++;
	}

end:
	ResetParserWord(Parser);
	return (FinalFluent);
}

bool32
EventValid(event* Event, fluent* Fluents, int32 FluentsCount) {

	/* VALID WHEN
		If fluent is true, then there must be a true fluent in the world.
		If fluent is false, then either the existing fluent in the world must be false, or no such fluent exists in the world

	*/

	for (int32 Index = 0; Index < Event->NextValidFluent; Index++) {
		fluent FluentTesting = Event->ValidationFluents[Index];

		bool32 FoundFluent = false;
		for (int32 FIndex = 0; FIndex < FluentsCount; FIndex++) {
			if (FluentTesting.Name == Fluents[FIndex].Name) {
				FoundFluent = true;

				// Fluent is false, and it's found in the world, so then the world fluent must also be false, else not valid event
				if (!FluentTesting.IsTrue && Fluents[FIndex].IsTrue) {
					return (false);
				}

				// Fluent testing is true, but the world fluent is false, so not valid event
				if (FluentTesting.IsTrue && !Fluents[FIndex].IsTrue) {
					return (false);
				}
			}
		}

		// Fluent is true, but did not find it in the world, so not valid event.
		if (!FoundFluent && FluentTesting.IsTrue) {
			return (false);
		}
	}

	return (true);
}

void
PrintHeader(string Str) {
	Print("---- " + Str + " ----");
}

void
PrintOptions(fluent* Fluents, int32 FluentsCount, event* Events, int32 EventsCount) {
	// Print State
	{
		PrintHeader("STATE");
		for (int32 Index = 0; Index < FluentsCount; Index++) {
			string StrIndex = Index;
			string TruthString = {};
			Fluents[Index].IsTrue ? TruthString = "TRUE" : TruthString = "FALSE";

			string ValueString = "";
			if (Fluents[Index].HasValue) {
				ValueString = Fluents[Index].Value;
			}

			Print(Fluents[Index].Name + " " + TruthString + " " + ValueString);
		}
		Print("\n");
	}

	// Print Actions
	{
		PrintHeader("POSSIBLE ACTIONS");
		for (int32 Index = 0; Index < EventsCount; Index++) {
			if (EventValid(&Events[Index], Fluents, FluentsCount)) {
				string StrIndex = Index;
				Print(StrIndex + ") " + Events[Index].Name);
			}
		}
		// Print("----------------------- ");
	}

	Print("\n \n----------------------- ");
	Print("Enter action number you wish.");
	Print("");
}

void main(int argc, char const **argv) {

	// These are the 'state' of the game
	string RoleName = {};

	int32 MaxFluents = 100;
	fluent* Fluents = (fluent*)malloc(MaxFluents * sizeof(fluent));
	int32 NextFluent = 0;

	int32 MaxEvents = 100;
	event* Events = (event*)malloc(MaxEvents * sizeof(event));
	event* NextEvent = Events;
	int32 EventsCount = 0;

	// This is the parsing part. Load initial data from script.
	{
		parser Parser = {};
		ChangeState(&Parser, ParserState_NoGoal);

		Parser.CharOn = LoadProg("T:/Games/PlayerGrid.sc");

		event_parse_state EventParseState = EventParseState_Name;

		for (;;) {
			char ThisChar = *Parser.CharOn;
			if (Parser.State == ParserState_NoGoal && ThisChar == '.') {
				// end of program
				break;
			}

			// discard unwanted characters
			if (ThisChar == '\r' || ThisChar == '\n' || ThisChar == '-' || ThisChar == ' ' || ThisChar == ',') {
				Parser.CharOn++;
				continue;
			}

			if (Parser.State == ParserState_GrabbingRole) {
				if (ThisChar == ')') {
					// Role was closed
					RoleName = Parser.WordOn;

					ChangeState(&Parser, ParserState_NoGoal);
				} else {
					AddToParser(&Parser);
				}
			} else if (Parser.State == ParserState_GrabbingEvent) {
				// event(GoToCostco) - true(FoodAtCostco) true(CarAtHome) : false(HasGas), true(HasFood), false(FoodAtCostco) false(CarAtHome) true(CarAtCostco).
				// event(NAME) - CONSQUENCE FLUENTS : VALIDATION FLUENTS.

				if (EventParseState == EventParseState_Name && ThisChar == ')') {
					NextEvent[0] = {};
					NextEvent->Name = Parser.WordOn;
					EventsCount++;

					ResetParserWord(&Parser);
					EventParseState = EventParseState_Consquences;
				} else if (EventParseState == EventParseState_Consquences || EventParseState == EventParseState_Validation) {
					if (Parser.WordOn == TrueLiteral || Parser.WordOn == FalseLiteral) {
						fluent Fluent = GrabFluent(&Parser, Fluents, NextFluent);

						if (EventParseState == EventParseState_Consquences) {
							NextEvent->ConsquenceFluents[NextEvent->NextConsqFluent] = Fluent;
							NextEvent->NextConsqFluent++;
						} else if (EventParseState == EventParseState_Validation) {
							NextEvent->ValidationFluents[NextEvent->NextValidFluent] = Fluent;
							NextEvent->NextValidFluent++;
						}

						// Parser.CharOn++;
					} else if (ThisChar == ':') {
						EventParseState = EventParseState_Validation;
					} else if (ThisChar == '.') {
						ChangeState(&Parser, ParserState_NoGoal);
						NextEvent++;
					} else {
						AddToParser(&Parser);
					}
				} else {
					AddToParser(&Parser);
				}
			} else if (Parser.State == ParserState_NoGoal) {

				// we only want to look for things that could possibly be a reserved word
				if (ThisChar != ')') {
					AddToParser(&Parser);
				}

				if (Parser.WordOn == RoleLiteral) {
					Parser.CharOn++;
					ChangeState(&Parser, ParserState_GrabbingRole);
				} else if (Parser.WordOn == TrueLiteral || Parser.WordOn == FalseLiteral) {
					Parser.CharOn++;
					Fluents[NextFluent] = GrabFluent(&Parser, Fluents, NextFluent);
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
		PrintOptions(Fluents, NextFluent, Events, EventsCount);

		bool32 Running = true;
		while (Running) {
			char Input[100];
			scanf("%99s", &Input);
			char Selection = Input[0];
			system("cls");

			string SelStr = Selection;
			int32 SelInt = StringToInt32(SelStr);
			if (SelInt < EventsCount) {
				// Print("Did Event -> " + Events[SelInt].Name + "\n \n");
				event* EventDoing = &Events[SelInt];

				for (int32 Index = 0; Index < EventDoing->NextConsqFluent; Index++) {
					fluent NewFluent = EventDoing->ConsquenceFluents[Index];
					bool32 FoundFluent = false;

					// Change that fluent state if it exists
					for (int32 FIndex = 0; FIndex < NextFluent; FIndex++) {
						if (NewFluent.Name == Fluents[FIndex].Name) {
							Fluents[FIndex].IsTrue = NewFluent.IsTrue;
							FoundFluent = true;
							break;
						}
					}

					// We need to make that fluent if it wasn't found
					if (!FoundFluent) {
						Fluents[NextFluent] = NewFluent;
						NextFluent++;
					}
				}
			} else {
				Print("Not a valid action");
			}

			PrintOptions(Fluents, NextFluent, Events, EventsCount);
		}
	}

	Print("Closing");
	return;
}
