#pragma once
#include <string>

class DigitRecognition;
class ISudokuSolver;
class SquaresDetection;

class SudokuPuzzle
{
public:
	SudokuPuzzle(std::string filename);
	~SudokuPuzzle();

	void WritePuzzle();

	void Solve();

private:


	DigitRecognition* m_digit_teller;
	ISudokuSolver* m_solver;
	SquaresDetection* m_sudoku;
};