#pragma once
#include <string>
#include <memory>

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


	std::unique_ptr<DigitRecognition> m_digit_teller;
	std::unique_ptr<ISudokuSolver> m_solver;
	std::unique_ptr<SquaresDetection> m_squaresDetector;
};