#include "SudokuPuzzle.h"
#include "SquaresDetection.h"
#include "DigitRecognition.h"
#include "SudokuSolver.h"

using namespace std;
using namespace cv;

SudokuPuzzle::SudokuPuzzle(std::string filename)
{
	m_sudoku = new SquaresDetection(filename);

	string json = "D:\\projects\\sudoku\\MNIST\\mymodel-symbol.json";
	string params = "D:\\projects\\sudoku\\MNIST\\mymodel-0100.params";

	m_digit_teller = new DigitRecognition(json, params);

	int sudoku_matrix[c_number_of_rows][c_number_of_cols];

	if (m_sudoku->IsDetected())
	{
		for (int i = 0; i < c_number_of_rows; i++)
		{
			for (int j = 0; j < c_number_of_cols; j++)
			{
				int number = 0;
				if (!m_sudoku->IsEmpty(i,j))
				{
					Mat sudokuMat = m_sudoku->GetSquare(i, j);
					bitwise_not(sudokuMat, sudokuMat);
					number = m_digit_teller->PredictNumber(sudokuMat);
				}
				sudoku_matrix[i][j] = number;
			}
		}
	}

	m_solver = new SudokuSolver(sudoku_matrix);
}

SudokuPuzzle::~SudokuPuzzle()
{
	delete m_digit_teller;
	m_digit_teller = nullptr;

	delete m_solver;
	m_solver = nullptr;

	delete m_sudoku;
	m_sudoku = nullptr;
}

void SudokuPuzzle::WritePuzzle()
{
	if (m_sudoku->IsDetected())
	{
		m_solver->WriteSudoku();
	}
}

void SudokuPuzzle::Solve()
{
	if (m_sudoku->IsDetected())
	{
		m_solver->Solve();
	}
}
