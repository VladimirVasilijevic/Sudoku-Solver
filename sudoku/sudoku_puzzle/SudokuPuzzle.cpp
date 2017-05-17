#include "SudokuPuzzle.h"
#include "SquaresDetection.h"
#include "DigitRecognition.h"
#include "SudokuSolver.h"

using namespace std;
using namespace cv;

const string json = "D:\\projects\\sudoku\\MNIST\\mymodel-symbol.json";
const string params = "D:\\projects\\sudoku\\MNIST\\mymodel-0100.params";

SudokuPuzzle::SudokuPuzzle(std::string filename)
	: m_squaresDetector(new SquaresDetection(filename))
	, m_digit_teller(new DigitRecognition(json, params))
	, m_solver(new SudokuSolver())
{
	Matrix sudoku_matrix;
	sudoku_matrix.resize(c_number_of_rows, vector<int>(c_number_of_cols));

	if (m_squaresDetector->IsDetected())
	{
		for (int i = 0; i < c_number_of_rows; i++)
		{
			for (int j = 0; j < c_number_of_cols; j++)
			{
				int number = 0;
				if (!m_squaresDetector->IsEmpty(i,j))
				{
					Mat sudokuMat = m_squaresDetector->GetSquare(i, j);
					bitwise_not(sudokuMat, sudokuMat);
					number = m_digit_teller->PredictNumber(sudokuMat);
				}
				sudoku_matrix[i][j] = number;
			}
		}
	}

	m_solver->LoadMatrix(sudoku_matrix);
}

SudokuPuzzle::~SudokuPuzzle()
{}

void SudokuPuzzle::WritePuzzle()
{
	if (m_squaresDetector->IsDetected())
	{
		m_solver->WriteSudoku();
	}
}

void SudokuPuzzle::Solve()
{
	if (m_squaresDetector->IsDetected())
	{
		m_solver->Solve();
	}
}
