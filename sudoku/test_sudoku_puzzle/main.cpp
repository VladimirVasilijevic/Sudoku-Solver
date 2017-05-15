#include "SudokuPuzzle.h"

int main()
{
	SudokuPuzzle puzzle("D:\\projects\\sudoku\\TestImages\\sudoku\\sudoku_test_0.png");
	puzzle.WritePuzzle();
	puzzle.Solve();
	puzzle.WritePuzzle();
}