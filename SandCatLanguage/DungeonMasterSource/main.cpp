// windows stuff
#include <iostream>
#include <windows.h>

// my stuff
#include "RyansTypes.cpp"
#include "String.cpp"

// Remove this is you don't want to compile any opengl stuff
// #define USING_OPENGL

#ifdef USING_OPENGL
#include "Math.cpp"
#include "color.cpp"
#include "vector2.cpp"
#include "Rect.cpp"
#include "opengl.cpp"
#endif

// Literals
string RoleLiteral = "role";
string TrueLiteral = "true";
string FalseLiteral = "false";
string EventLiteral = "event";
string CommentLiteral = "//";
string ConditionalLiteral = "conditional";
string BindLiteral = "bind";

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

	/* If the fluent has arithmetic as a fluent value, then it is stored here.
		This is is only used when the fluent is part of an event events.
		The initial fluents are evaluated when they're parsed.
		Event fluents are evaluated when they're triggered, so they need to be saved and 'parsed' when they're triggered.
	*/
	bool32 HasArithmetic;
	string Arithmetic;

	bool32 Using;
};

namespace Comparison {
enum Enum {
	LessThan, GreaterThan, EqualTo,
	LessThanOrEqual, GreaterThanOrEqual, NotEqual,
};
}

enum class logic_symbols {
	Default, And, Or, Not, OpenScope, CloseScope, Operand,
};

struct conditional {
	string FirstFluent;
	string SecondFluent;
	Comparison::Enum Conditional;
};

struct event {
	string Name;

	// NOTE these next two are used as queues. Zero is the front.
	logic_symbols LogicOperators[100];
	int32 LogicStackOpsCount;
	conditional Conditionals[100];
	int32 ConditionalsCount;

	fluent ConsquenceFluents[100];
	int32 NextConsqFluent;
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
	ParserState_IgnoringComment,
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

//NOTE the line Parser->CharOn++; always follows this, we can just put that line in here.
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

fluent*
FindFluent(string Name, fluent* Fluents, int32 FluentsCount) {
	for (int32 Index = 0; Index < FluentsCount; Index++) {
		if (Name == Fluents[Index].Name) {
			return (&Fluents[Index]);
		}
	}

	// Could not find that fluent, don't assert becuase sometimes it's normal not to find the fluent.
	return (NULL);
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

struct fluent_value_return {
	real64 Value;
	bool32 Valid;
};

fluent_value_return
EvaluateFluentValue(char* FluentString, fluent* Fluents, int32 FluentsCount) {

	fluent_value_return Value = {};

	char InsideFluent[100] = {};
	int32 InsideIndex = 0;

	while (*FluentString != NULL && *FluentString != ')') {
		// NOTE here is where we check to make sure what is in the fluent is infact a number
		if (*FluentString != '(') {
			InsideFluent[InsideIndex] = *FluentString;
			InsideIndex++;
		}
		FluentString++;
	}

	bool32 IsNum = StringIsInt(InsideFluent);
	if (IsNum) {
		// Is just a value
		Value.Valid = true;
		Value.Value = (real64)StringToInt32(InsideFluent);
		return (Value);
	} else {
		// Is probably an equation

		real64 Accumulator = 0;
		accum_state AccumState = accum_none;

		char Word[100] = {};
		int32 WordIndex = 0;

		for (int32 Index = 0; Index < InsideIndex; Index++) {

			if (InsideFluent[Index] == ' ')  {
				continue;
			}

			if (InsideFluent[Index] == '+') {
				Accumulator = InfixAccumulate(Accumulator, Word, AccumState, Fluents, FluentsCount);
				AccumState = accum_add;

				ZeroMemory(Word, 100);
				WordIndex = 0;
			} else if (InsideFluent[Index] == '-') {
				Accumulator = InfixAccumulate(Accumulator, Word, AccumState, Fluents, FluentsCount);
				AccumState = accum_sub;

				ZeroMemory(Word, 100);
				WordIndex = 0;
			} else if (InsideFluent[Index] == '*') {
				Accumulator = InfixAccumulate(Accumulator, Word, AccumState, Fluents, FluentsCount);
				AccumState = accum_mult;

				ZeroMemory(Word, 100);
				WordIndex = 0;
			} else if (InsideFluent[Index] == '/') {
				Accumulator = InfixAccumulate(Accumulator, Word, AccumState, Fluents, FluentsCount);
				AccumState = accum_div;

				ZeroMemory(Word, 100);
				WordIndex = 0;
			} else {
				Word[WordIndex] = InsideFluent[Index];
				WordIndex++;
			}
		}

		if (StringIsInt(Word)) {
			Accumulator = InfixAccumulate(Accumulator, Word, AccumState, Fluents, FluentsCount);
		} else {
			fluent* FluentTesting = FindFluent(Word, Fluents, FluentsCount);
			if (FluentTesting != NULL) {
				Accumulator = InfixAccumulate(Accumulator, Word, AccumState, Fluents, FluentsCount);
			} else {
				Value.Valid = false;
				return (Value);
			}
		}

		Value.Value = Accumulator;
		Value.Valid = true;
		return (Value);
	}
}

// This gets the fluent from the parser
fluent
GrabFluent(parser* Parser, fluent* Fluents, int32 FluentsCount, bool32 EvaluateArithmetic) {
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
			if (EvaluateArithmetic) {
				FinalFluent.HasValue = true;

				fluent_value_return Val = EvaluateFluentValue(Parser->CharOn, Fluents, FluentsCount);
				Assert(Val.Valid);
				FinalFluent.Value = Val.Value;
			} else {
				ResetParserWord(Parser);

				// Consume the '('
				Parser->CharOn++;

				while (*Parser->CharOn != ')') {
					AddToParser(Parser);
					Parser->CharOn++;
				}

				// Consume the ')'
				Parser->CharOn++;

				FinalFluent.HasArithmetic = true;
				FinalFluent.Arithmetic = Parser->WordOn;
			}
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
GetConditionalBool(conditional* Cond, fluent* Fluents, int32 FluentsCount) {
	// check if both fluents exist, if not then the event is not valid
	// if (FindFluent(Cond->FirstFluent.CharArray, Fluents, FluentsCount) == NULL || FindFluent(Cond->FirstFluent.CharArray, Fluents, FluentsCount) == NULL) {
	// TODO print something out here. This might not be intended behavior
	// return (false);
	// }

	fluent_value_return FirstVal = EvaluateFluentValue(Cond->FirstFluent.CharArray, Fluents, FluentsCount);
	fluent_value_return SecondVal = EvaluateFluentValue(Cond->SecondFluent.CharArray, Fluents, FluentsCount);

	if (!FirstVal.Valid || !SecondVal.Valid) {
		return (false);
	}

	real64 FirstValue = FirstVal.Value;
	real64 SecondValue = SecondVal.Value;

	if (Cond->Conditional == Comparison::GreaterThan && !(FirstValue > SecondValue)) {
		return (false);
	} else if (Cond->Conditional == Comparison::LessThan && !(FirstValue < SecondValue)) {
		return (false);
	} else if (Cond->Conditional == Comparison::EqualTo && !(FirstValue == SecondValue)) {
		return (false);
	} else if (Cond->Conditional == Comparison::NotEqual && !(FirstValue != SecondValue)) {
		return (false);
	} else if (Cond->Conditional == Comparison::GreaterThanOrEqual && !(FirstValue >= SecondValue)) {
		return (false);
	} else if (Cond->Conditional == Comparison::LessThanOrEqual && !(FirstValue <= SecondValue)) {
		return (false);
	}

	return (true);
}

// NOTE we need to hold to logic operands and conditional counters since AND, OR, OPEN and CLOSE SCOPE are logic operators
bool32
EvaluateBoolean(event* Event, int32* NextConditional, int32* NextLogicOp, fluent* Fluents, int32 FluentsCount) {

	bool32 AccumBool = true;

	// This state must be held separate from the logicO
	logic_symbols CurrLogicOp = logic_symbols::Default;

	bool32 NegateNext = false;

	while (*NextLogicOp < Event->LogicStackOpsCount) {
		logic_symbols NextSymbol = Event->LogicOperators[*NextLogicOp];
		(*NextLogicOp)++;

		if (NextSymbol == logic_symbols::And || NextSymbol == logic_symbols::Or) {
			CurrLogicOp = NextSymbol;
		} else if (NextSymbol == logic_symbols::Not) {
			NegateNext = true;
		} else if (NextSymbol == logic_symbols::OpenScope) {
			bool32 NextVal = EvaluateBoolean(Event, NextConditional, NextLogicOp, Fluents, FluentsCount);

			if (NegateNext) {
				NegateNext = false;
				NextVal = !NextVal;
			}

			if (CurrLogicOp == logic_symbols::And) {
				AccumBool = AccumBool && NextVal;
			} else if (CurrLogicOp == logic_symbols::Or) {
				AccumBool = AccumBool || NextVal;
			} else if (CurrLogicOp == logic_symbols::Default) {
				AccumBool = NextVal;
			}
		} else if (NextSymbol == logic_symbols::Operand) {
			bool32 NextVal = GetConditionalBool(&Event->Conditionals[*NextConditional], Fluents, FluentsCount);
			(*NextConditional)++;

			if (NegateNext) {
				NegateNext = false;
				NextVal = !NextVal;
			}

			if (CurrLogicOp == logic_symbols::And) {
				AccumBool = AccumBool && NextVal;
			} else if (CurrLogicOp == logic_symbols::Or) {
				AccumBool = AccumBool || NextVal;
			} else if (CurrLogicOp == logic_symbols::Default) {
				AccumBool = NextVal;
			}
		} else if (NextSymbol == logic_symbols::CloseScope) {
			return (AccumBool);
		} else {
			// There was a problem. Something didn't stack correctly
			Assert(0);
		}
	}

	return (AccumBool);
}

bool32
EventValid(event* Event, fluent* Fluents, int32 FluentsCount) {
	if (Event->ConditionalsCount > 0) {
		int32 NextConditional = 0;
		int32 NextLogic = 0;
		return (EvaluateBoolean(Event, &NextConditional, &NextLogic, Fluents, FluentsCount));
	} else {
		// If there are no conditionals then event is always valid
		return (true);
	}
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

struct event_bind {
	string EventName;
	string KeyName;
};

// these are the rules of the game.
struct game_def {
	fluent* Fluents;
	int32 FluentsCount;

	event* Events;
	int32 EventsCount;

	event_bind* BoundEvents;
	int32 BoundCount;
};

game_def
LoadGameDefinition() {
	game_def GameDefinition = {};

	int32 MaxFluents = 100;
	GameDefinition.Fluents = (fluent*)malloc(MaxFluents * sizeof(fluent));

	int32 MaxEvents = 100;
	GameDefinition.Events = (event*)malloc(MaxEvents * sizeof(event));
	event* NextEvent = GameDefinition.Events;

	GameDefinition.BoundEvents = (event_bind*)malloc(100 * sizeof(event_bind));

	parser Parser = {};
	ChangeState(&Parser, ParserState_NoGoal);

	// Parser.CharOn = LoadProg("T:/Games/ShieldPush/ShieldPush.sc");
	Parser.CharOn = LoadProg("T:/Games/Xedoc.sc");

	event_parse_state EventParseState = EventParseState_Name;

	for (;;) {
		char ThisChar = *Parser.CharOn;
		if (Parser.State == ParserState_NoGoal && ThisChar == '.') {
			// end of program
			break;
		}

		// discard unwanted characters
		if (ThisChar == '\r' || ThisChar == '\n' || ThisChar == '-' || ThisChar == ' ' || ThisChar == ',' || ThisChar == '\t') {
			Parser.CharOn++;

			if (Parser.State == ParserState_IgnoringComment && ThisChar == '\n') {
				ChangeState(&Parser, ParserState_NoGoal);
			}

			/* This is somewhat of a catch all. Sometimes the fluents are indeed parsed but the characters are not consumed (because worrying about that is annoying).
				This resets everything so the next line can be parsed correctly.
			*/
			if (ThisChar == '\r' || ThisChar == '\n') {
				ChangeState(&Parser, ParserState_NoGoal);
				// ResetParserWord(&Parser);
			}
			continue;
		}

		if (Parser.State == ParserState_GrabbingRole) {
			if (ThisChar == ')') {
				// Role was closed
				// RoleName = Parser.WordOn;

				// TODO do this, need to add it back in when it's needed
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
				GameDefinition.EventsCount++;

				ResetParserWord(&Parser);
				EventParseState = EventParseState_Consquences;
			} else if (EventParseState == EventParseState_Consquences || EventParseState == EventParseState_Validation) {
				if (Parser.WordOn == TrueLiteral || Parser.WordOn == FalseLiteral) {
					fluent Fluent = GrabFluent(&Parser, GameDefinition.Fluents, GameDefinition.FluentsCount, false);

					if (EventParseState == EventParseState_Consquences) {

						NextEvent->ConsquenceFluents[NextEvent->NextConsqFluent] = Fluent;
						NextEvent->NextConsqFluent++;
					} else if (EventParseState == EventParseState_Validation) {
						Assert(0);
						// This isn't supported anymore. Should always use conditionals

						// NextEvent->ValidationFluents[NextEvent->NextValidFluent] = Fluent;
						// NextEvent->NextValidFluent++;
					}

					// Parser.CharOn++;
				} else if (Parser.WordOn == ConditionalLiteral) {
					conditional* NextCond = &NextEvent->Conditionals[NextEvent->ConditionalsCount];
					NextEvent->ConditionalsCount++;

					NextEvent->LogicOperators[NextEvent->LogicStackOpsCount] = logic_symbols::Operand;
					NextEvent->LogicStackOpsCount++;

					// Get the first operand
					{
						// consume the '('
						Parser.CharOn++;

						ResetParserWord(&Parser);
						while (*Parser.CharOn != '!' && *Parser.CharOn != '<' && *Parser.CharOn != '>' && *Parser.CharOn != '=') {
							if (*Parser.CharOn != ' ') {
								AddToParser(&Parser);
							}
							Parser.CharOn++;
						}

						NextCond->FirstFluent = Parser.WordOn;
						AddToParser(&Parser);
					}

					// get the comparison
					{
						ResetParserWord(&Parser);

						// consume the first space
						if (*Parser.CharOn == ' ') {
							Parser.CharOn++;
						}

						while (*Parser.CharOn != ' ') {
							AddToParser(&Parser);
							Parser.CharOn++;
						}

						if (Parser.WordOn == (string)">") {
							NextCond->Conditional = Comparison::GreaterThan;
						} else if (Parser.WordOn == (string)"<") {
							NextCond->Conditional = Comparison::LessThan;
						} else if (Parser.WordOn == (string)"=") {
							NextCond->Conditional = Comparison::EqualTo;
						} else if (Parser.WordOn == (string)">=") {
							NextCond->Conditional = Comparison::GreaterThanOrEqual;
						} else if (Parser.WordOn == (string)"<=") {
							NextCond->Conditional = Comparison::LessThanOrEqual;
						} else if (Parser.WordOn == (string)"!=") {
							NextCond->Conditional = Comparison::NotEqual;
						}

						// consume the next space
						Parser.CharOn++;
					}

					// Get the second operand
					{
						ResetParserWord(&Parser);
						while (*Parser.CharOn != ')') {
							AddToParser(&Parser);
							Parser.CharOn++;
						}

						NextCond->SecondFluent = Parser.WordOn;
						AddToParser(&Parser);
					}

					// consume the last ')'
					Parser.CharOn++;
					ResetParserWord(&Parser);

					if (*Parser.CharOn == '.') {
						ChangeState(&Parser, ParserState_NoGoal);
						NextEvent++;
					}
				} else if (ThisChar == ':') {
					EventParseState = EventParseState_Validation;
				} else if (ThisChar == '.') {
					ChangeState(&Parser, ParserState_NoGoal);
					NextEvent++;
				} else if (ThisChar == '!') {
					NextEvent->LogicOperators[NextEvent->LogicStackOpsCount] = logic_symbols::Not;
					NextEvent->LogicStackOpsCount++;
				} else if (ThisChar == '[') {
					NextEvent->LogicOperators[NextEvent->LogicStackOpsCount] = logic_symbols::OpenScope;
					NextEvent->LogicStackOpsCount++;
				} else if (ThisChar == ']') {
					NextEvent->LogicOperators[NextEvent->LogicStackOpsCount] = logic_symbols::CloseScope;
					NextEvent->LogicStackOpsCount++;
				} else if (ThisChar == '&') {
					NextEvent->LogicOperators[NextEvent->LogicStackOpsCount] = logic_symbols::And;
					NextEvent->LogicStackOpsCount++;
				} else if (ThisChar == '|') {
					NextEvent->LogicOperators[NextEvent->LogicStackOpsCount] = logic_symbols::Or;
					NextEvent->LogicStackOpsCount++;
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

			// NOTE this could easily be consolidated
			if (Parser.WordOn == RoleLiteral) {
				Parser.CharOn++;
				ChangeState(&Parser, ParserState_GrabbingRole);
			} else if (Parser.WordOn == TrueLiteral || Parser.WordOn == FalseLiteral) {
				Parser.CharOn++;
				GameDefinition.Fluents[GameDefinition.FluentsCount] = GrabFluent(&Parser, GameDefinition.Fluents, GameDefinition.FluentsCount, true);
				GameDefinition.FluentsCount++;
			} else if (Parser.WordOn == EventLiteral) {
				Parser.CharOn++;
				EventParseState = EventParseState_Name;
				ChangeState(&Parser, ParserState_GrabbingEvent);
			} else if (Parser.WordOn == BindLiteral) {
				Parser.CharOn++;
				ResetParserWord(&Parser);

				while (*Parser.CharOn != ' ') {
					if (*Parser.CharOn != '(') {
						AddToParser(&Parser);
					}
					Parser.CharOn++;

				}
				GameDefinition.BoundEvents[GameDefinition.BoundCount].EventName = Parser.WordOn;
				ResetParserWord(&Parser);

				while (*Parser.CharOn != ')') {
					if (*Parser.CharOn != ' ') {
						AddToParser(&Parser);
					}
					Parser.CharOn++;
				}
				GameDefinition.BoundEvents[GameDefinition.BoundCount].KeyName = Parser.WordOn;
				ResetParserWord(&Parser);

				GameDefinition.BoundCount++;
			} else if (Parser.WordOn == CommentLiteral) {
				Parser.CharOn++;
				ChangeState(&Parser, ParserState_IgnoringComment);
			}
		}

		Parser.CharOn++;
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
UpdateControls(controls* Controls, HWND Window) {
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
DoEvent(event* EventDoing, game_def* GameDef) {
	if (EventValid(EventDoing, GameDef->Fluents, GameDef->FluentsCount)) {
		for (int32 Index = 0; Index < EventDoing->NextConsqFluent; Index++) {
			fluent NewFluent = EventDoing->ConsquenceFluents[Index];

			// Change that fluent state if it exists
			fluent* F = FindFluent(NewFluent.Name, GameDef->Fluents, GameDef->FluentsCount);
			if (F != NULL) {
				fluent_value_return Val = EvaluateFluentValue(NewFluent.Arithmetic.CharArray, GameDef->Fluents, GameDef->FluentsCount);
				Assert(Val.Valid);
				F->Value = Val.Value;
			} else {
				GameDef->Fluents[GameDef->FluentsCount] = NewFluent;
				GameDef->FluentsCount++;
			}
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
					if (FindFluent("UseGrid", GameDefinition.Fluents, GameDefinition.FluentsCount) != NULL) {
						vector2 GridTopLeft = vector2{50.0f, 50.0f};
						real64 GridCellSize = 50;
						int32 GridCellCount  = FindFluent("GridMaxX", GameDefinition.Fluents, GameDefinition.FluentsCount)->Value;


						// square grid outline
						{
							real32 Grey = 0.8f;
							real64 SquareSide = GridCellSize * GridCellCount;
							gl_square GridOutline = MakeGLSquare(GridTopLeft + vector2{SquareSide / 2.0f, SquareSide / 2.0f}, SquareSide + GridCellSize, color{Grey, Grey, Grey, 1.0f});
							PushRenderSquare(&RenderObjects[0], GridOutline);
						}

						if (FindFluent("BlockX", GameDefinition.Fluents, GameDefinition.FluentsCount) != NULL) {
							vector2 BlockPos = {FindFluent("BlockX", GameDefinition.Fluents, GameDefinition.FluentsCount)->Value,
							                    FindFluent("BlockY", GameDefinition.Fluents, GameDefinition.FluentsCount)->Value
							                   };

							vector2 ScreenPos = (BlockPos * vector2{GridCellSize, GridCellSize}) + GridTopLeft;
							gl_square GridOutline = MakeGLSquare(ScreenPos, GridCellSize, COLOR_BLACK);
							PushRenderSquare(&RenderObjects[0], GridOutline);
						}

						if (FindFluent("PlayerHoldingShield", GameDefinition.Fluents, GameDefinition.FluentsCount) == NULL) {
							vector2 ShieldPos = {FindFluent("ShieldPosX", GameDefinition.Fluents, GameDefinition.FluentsCount)->Value,
							                     FindFluent("ShieldPosY", GameDefinition.Fluents, GameDefinition.FluentsCount)->Value
							                    };

							vector2 Center = (ShieldPos * vector2{GridCellSize, GridCellSize}) + GridTopLeft;

							rect ShieldRect = {};
							ShieldRect.TopLeft = Center - vector2{(GridCellSize / 2.0f) - 5.0f, (GridCellSize / 2.0f) + 5.0f};
							ShieldRect.BottomRight = Center + vector2{(GridCellSize / 2.0f) - 5.0f, (GridCellSize / 2.0f) + 5.0f};

							gl_square GridOutline = MakeGLRectangle(ShieldRect, COLOR_BLUE);
							PushRenderSquare(&RenderObjects[0], GridOutline);
						}

						if (FindFluent("UsePlayerPos", GameDefinition.Fluents, GameDefinition.FluentsCount) != NULL) {

							vector2 PlayerPos = {FindFluent("PlayerX", GameDefinition.Fluents, GameDefinition.FluentsCount)->Value,
							                     FindFluent("PlayerY", GameDefinition.Fluents, GameDefinition.FluentsCount)->Value
							                    };

							vector2 PlayerScreenPos = (PlayerPos * vector2{GridCellSize, GridCellSize}) + GridTopLeft;
							gl_square GridOutline = MakeGLSquare(PlayerScreenPos, GridCellSize, COLOR_RED);
							PushRenderSquare(&RenderObjects[0], GridOutline);

							// player controls
							{
								if (CanDoAction) {
									for (int32 Index = 0; Index < GameDefinition.BoundCount; Index++) {
										string KeyWanting = GameDefinition.BoundEvents[Index].KeyName;
										string EventWanting = GameDefinition.BoundEvents[Index].EventName;
										for (int32 ContIndex = 0; ContIndex < ARRAY_SIZE(Controls.AllControls, button); ContIndex++) {
											if (Controls.AllControls[ContIndex].IsDown && KeyWanting == Controls.AllControls[ContIndex].Name) {

												// find the event to do
												for (int32 EventIndex = 0; EventIndex < GameDefinition.EventsCount; EventIndex++) {
													if (GameDefinition.Events[EventIndex].Name == EventWanting) {
														DoEvent(&GameDefinition.Events[EventIndex], &GameDefinition);
														CanDoAction = false;
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

								// can only do another action once the frst input has been released
							}
						}
					}
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
		PrintOptions(GameDefinition.Fluents, GameDefinition.FluentsCount, GameDefinition.Events, GameDefinition.EventsCount);

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
				DoEvent(&GameDefinition.Events[SelInt], &GameDefinition);
			} else {
				Print("Not a valid action");
			}

			PrintOptions(GameDefinition.Fluents, GameDefinition.FluentsCount, GameDefinition.Events, GameDefinition.EventsCount);
		}

		Print("Closing");
		return;
	}
}
#endif