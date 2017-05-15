#include <iostream>
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

int main()
{
	int sudoku_matrix[N][N] = {	{ 8, 0, 0, 0, 0, 0, 0, 0, 5 },
								{ 0, 2, 0, 9, 0, 0, 0, 0, 8 },
								{ 0, 0, 0, 8, 6, 7, 0, 1, 0 },
								{ 0, 1, 0, 7, 0, 0, 3, 0, 0 },
								{ 0, 0, 2, 0, 8, 0, 9, 0, 0 },
								{ 0, 0, 4, 0, 0, 6, 0, 8, 0 },
								{ 2, 4, 6, 3, 5, 0, 0, 0, 0 },
								{ 5, 0, 0, 6, 0, 4, 0, 2, 0 },
								{ 1, 0, 0, 0, 7, 0, 0, 0, 6 } 
							  };

	WriteSudoku(sudoku_matrix);
	cout << "Sudoku" << endl;

	ISudokuSolver* sudoku = new SudokuSolver(sudoku_matrix);
	bool Is_solved = sudoku->Solve();

	if (Is_solved)
	{
		sudoku->WriteSudoku();
	}
	else
	{
		cout << "Sudoku nije resen!" << endl;
	}

	delete sudoku;
}