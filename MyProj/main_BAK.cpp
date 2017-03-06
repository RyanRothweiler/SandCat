#include <iostream>
#include <windows.h>
#include "clingo/c-api/clingo.h"
#include "RyansTypes.cpp"

void Print(char* Output) {
	std::cout << Output << std::endl;
}

void Print(const char* Output) {
	std::cout << Output << std::endl;
}

void PrintError() {
	const char* error_message;
	if (!(error_message = clingo_error_message())) { error_message = "error"; }
	Print(error_message);
}

bool on_model(clingo_model_t *model, void *data, bool *goon) {
	(void)data;
	bool ret = true;
	clingo_symbol_t *atoms = NULL;
	size_t atoms_n;
	clingo_symbol_t const *it, *ie;
	char *str = NULL;
	size_t str_n = 0;

	// determine the number of (shown) symbols in the model
	if (!clingo_model_symbols_size(model, clingo_show_type_shown, &atoms_n)) { goto error; }

	// allocate required memory to hold all the symbols
	if (!(atoms = (clingo_symbol_t*)malloc(sizeof(*atoms) * atoms_n))) {
		clingo_set_error(clingo_error_bad_alloc, "could not allocate memory for atoms");
		goto error;
	}

	// retrieve the symbols in the model
	if (!clingo_model_symbols(model, clingo_show_type_shown, atoms, atoms_n)) { goto error; }

	// printf("Model:");

	for (it = atoms, ie = atoms + atoms_n; it != ie; ++it) {
		size_t n;
		char *str_new;

		// determine size of the string representation of the next symbol in the model
		if (!clingo_symbol_to_string_size(*it, &n)) { goto error; }

		bool isPositive = false;
		clingo_symbol_is_positive(clingo_symbol_t symbol, &isPositive);

		if (str_n < n) {
			// allocate required memory to hold the symbol's string
			if (!(str_new = (char*)realloc(str, sizeof(*str) * n))) {
				clingo_set_error(clingo_error_bad_alloc, "could not allocate memory for symbol's string");
				goto error;
			}

			str = str_new;
			str_n = n;
		}

		// retrieve the symbol's string
		if (!clingo_symbol_to_string(*it, str, n)) { goto error; }

		printf(" %s \n", str);
	}

	printf("\n");
	*goon = true;
	goto out;

error:
	ret = false;

out:
	if (atoms) { free(atoms); }
	if (str)   { free(str); }

	return ret;
}

char* LoadProg(char* FileName) {
	OFSTRUCT FileInfo = {};
	HFILE FileHandle = OpenFile(FileName, &FileInfo, OF_READ);
	Assert(FileHandle);

	char* Prog = (char*)malloc(FileInfo.cBytes);
	ZeroMemory(Prog, FileInfo.cBytes);

	DWORD BytesRead;
	if (!ReadFile((HANDLE)FileHandle, (void*)Prog, FileInfo.cBytes, &BytesRead, NULL)) {
		// Couldn't read the file. Something wrong happene.d
		Assert(0);
	}

	return (Prog);
}

clingo_control_t *ClingoControl = NULL;

void main(int argc, char const **argv) {
	Print("Starting");


	// create a control object and pass command line arguments
	if (!clingo_control_new(argv + 1, argc - 1, NULL, NULL, 20, &ClingoControl) != 0) { PrintError(); }


	if (!clingo_control_load(ClingoControl, "TestProg.lp")) { PrintError(); }

	// ground it
	clingo_part_t GroundParts[] = {{ "base", NULL, 0 }};
	if (!clingo_control_ground(ClingoControl, GroundParts, 1, NULL, NULL)) { PrintError(); }

	bool32 running = true;
	while (running) {
		printf("----------------------- \n");
		printf("1 - Show State \n");
		printf("2 - Quit \n");
		printf("3 - Clear state and reload \n");
		printf("4 - Cleanup and solve \n");
		printf("\n");

		char Input[100];
		scanf("%99s", &Input);
		char Selection = Input[0];

		if (Selection == '1') {
			// solve it
			clingo_solve_result_bitset_t solve_ret;
			if (!clingo_control_solve(ClingoControl, on_model, NULL, NULL, 0, &solve_ret)) { PrintError(); }
		} else if (Selection == '2') {
			running = false;
		} else if (Selection == '3') {

			// destroy and create new clingo control
			if (ClingoControl) { clingo_control_free(ClingoControl); }
			if (!clingo_control_new(argv + 1, argc - 1, NULL, NULL, 20, &ClingoControl) != 0) { PrintError(); }

			if (!clingo_control_load(ClingoControl, "TestProg.lp")) { PrintError(); }

			clingo_part_t GroundParts[] = {{ "base", NULL, 0 }};
			if (!clingo_control_ground(ClingoControl, GroundParts, 1, NULL, NULL)) { PrintError(); }

			clingo_solve_result_bitset_t solve_ret;
			if (!clingo_control_solve(ClingoControl, on_model, NULL, NULL, 0, &solve_ret)) { PrintError(); }
		} else if (Selection == '4') {
			clingo_control_cleanup(ClingoControl);

			clingo_part_t GroundParts[] = {{ "base", NULL, 0 }};
			if (!clingo_control_ground(ClingoControl, GroundParts, 1, NULL, NULL)) { PrintError(); }

			clingo_solve_result_bitset_t solve_ret;
			if (!clingo_control_solve(ClingoControl, on_model, NULL, NULL, 0, &solve_ret)) { PrintError(); }
		}

		// printf(Input);
	}

	// free the clingo control
	if (ClingoControl) { clingo_control_free(ClingoControl); }

	return;
}