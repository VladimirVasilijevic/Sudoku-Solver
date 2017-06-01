#include "SudokuPuzzle.h"
#include <string>

int main(int argc, char** argv)
{
	const std::string output(argv[1]);

	SudokuPuzzle puzzle(output);
	puzzle.WritePuzzle();
	puzzle.Solve();
	puzzle.WritePuzzle();
}