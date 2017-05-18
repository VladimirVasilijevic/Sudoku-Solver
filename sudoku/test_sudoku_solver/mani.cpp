#include <iostream>
#include <memory>
#include "ISudokuSolver.h"
#include "SudokuSolver.h"

using namespace std;

static int const N = 9;
static int const M = 3;

void WriteSudoku(int matrix[N][N])
{
	for (int i = 0; i < N; i++)
	{
		if (i % M == 0)
		{
			cout << "+-------+-------+-------+" << endl;
		}
		for (int j = 0; j < N; j++)
		{
			if (j % M == 0)
			{
				cout << "| ";
			}
			cout << matrix[i][j] << " ";
		}
		cout << "| " << endl;
	}
	cout << "+-------+-------+-------+" << endl;
}

void CopyMatrix(int(&sudoku_matrix)[c_number_of_rows][c_number_of_cols], Matrix & destination)
{
	for (int i = 0; i < c_number_of_rows; i++)
	{
		for (int j = 0; j < c_number_of_cols; j++)
		{
			destination[i][j] = sudoku_matrix[i][j];
		}
	}
}

int main()
{
	Matrix sudoku_matrix;
	sudoku_matrix.resize(c_number_of_rows, vector<int>(c_number_of_cols));
	int matrix[N][N] = {	{ 8, 0, 0, 0, 0, 0, 0, 0, 5 },
								{ 0, 2, 0, 9, 0, 0, 0, 0, 8 },
								{ 0, 0, 0, 8, 6, 7, 0, 1, 0 },
								{ 0, 1, 0, 7, 0, 0, 3, 0, 0 },
								{ 0, 0, 2, 0, 8, 0, 9, 0, 0 },
								{ 0, 0, 4, 0, 0, 6, 0, 8, 0 },
								{ 2, 4, 6, 3, 5, 0, 0, 0, 0 },
								{ 5, 0, 0, 6, 0, 4, 0, 2, 0 },
								{ 1, 0, 0, 0, 7, 0, 0, 0, 6 } 
							  };

	WriteSudoku(matrix);
	cout << "Sudoku" << endl;
	CopyMatrix(matrix, sudoku_matrix);
	std::unique_ptr<ISudokuSolver> sudoku (make_unique<SudokuSolver>(sudoku_matrix));
	bool Is_solved = sudoku->Solve();

	if (Is_solved)
	{
		sudoku->WriteSudoku();
	}
	else
	{
		cout << "Sudoku nije resen!" << endl;
	}
}