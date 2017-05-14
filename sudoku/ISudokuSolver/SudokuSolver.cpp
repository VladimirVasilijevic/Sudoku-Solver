#include "SudokuSolver.h"
#include <iostream>>

using namespace std;

const int c_rows_of_blocks = 3;
const int c_cols_of_blocks = 3;
const int c_max_field_value = 9;
const int c_min_field_value = 1;
const int c_unique = 1;

SudokuSolver::SudokuSolver(int(&sudoku_matrix)[c_number_of_rows][c_number_of_cols])
{
	FillCheckArrays();
	LoadField(sudoku_matrix);
	InitCheckArrays();
}

bool SudokuSolver::Solve() 
{
	SolveStraightforward();

	if (!IsSolved())
	{
//		SolveBacktrack();
	}
	return IsSolved();
}

void SudokuSolver::LoadField(int(&sudoku_matrix)[c_number_of_rows][c_number_of_cols])
{
	for (int i = 0; i < c_number_of_rows; i++)
	{
		for (int j = 0; j < c_number_of_cols; j++)
		{
			m_matrix[i][j] = sudoku_matrix[i][j];
		}
	}
}

void SudokuSolver::FillCheckArrays()
{
	for (int i = 0; i < c_number_of_rows; i++)
	{
		for (int j = 0; j < c_number_of_cols; j++)
		{
			m_rows[i][j] = false;
			m_cols[i][j] = false;
			m_blocks[i][j] = false;
		}
	}
}

bool SudokuSolver::CheckFieldValue(int row, int col, int value)
{
	return !CheckRow(row, value) && !CheckCol(col, value) && !CheckBlock(row, col, value);
}

bool SudokuSolver::CheckRow(int row, int value)
{
	return m_rows[row][value - 1];
}

bool SudokuSolver::CheckCol(int col, int value)
{
	return m_cols[col][value - 1];
}

bool SudokuSolver::CheckBlock(int row, int col, int value)
{
	return m_blocks[Block(row, col)][value - 1];
}

bool SudokuSolver::IsSingular(int row, int col, int value)
{
	return IsBlockSingular(Block(row, col), value) || IsRowSingular(row, value) || IsColSingular(col, value);
}

bool SudokuSolver::IsBlockSingular(int block, int value)
{
	int count_of_fit_field = 0;
	int block_row = block / c_rows_of_blocks;
	int block_col = block % c_cols_of_blocks;
	for (int row = block_row * c_rows_of_blocks; row < (block_row + 1) * c_rows_of_blocks; row++)
	{
		for (int col = block_col * c_cols_of_blocks; col < (block_col + 1) * c_cols_of_blocks; col++)
		{
			if (m_matrix[row][col] == 0 && CheckFieldValue(row, col, value))
			{
				count_of_fit_field++;
			}
		}
	}
	return count_of_fit_field == c_unique;
}

bool SudokuSolver::IsRowSingular(int row, int value)
{
	int count_of_fit_field = 0;
	for (int i = 0; i < c_number_of_cols; i++)
	{
		if (m_matrix[row][i] == 0 && CheckFieldValue(row, i, value))
		{
			count_of_fit_field++;
		}
	}
	return count_of_fit_field == c_unique;
}

bool SudokuSolver::IsColSingular(int col, int value)
{
	int count_of_fit_field = 0;
	for (int i = 0; i < c_number_of_rows; i++)
	{
		if (m_matrix[i][col] == 0 && CheckFieldValue(i, col, value))
		{
			count_of_fit_field++;
		}
	}
	return count_of_fit_field == c_unique;
}

bool SudokuSolver::IsValueAdequate(int row, int col, int value)
{
	return CheckFieldValue(row, col, value) && IsSingular(row, col, value);
}

void SudokuSolver::AssignCheckValues(int row, int col, int value)
{
	m_rows[row][value - 1] = true;
	m_cols[col][value - 1] = true;
	m_blocks[Block(row, col)][value - 1] = true;
}

void SudokuSolver::AssignFieldValue(int row, int col, int value)
{
	AssignCheckValues(row, col, value);
	m_matrix[row][col] = value;
}

int SudokuSolver::Block(int row, int col)
{
	return (row / c_rows_of_blocks) * c_cols_of_blocks + col / c_cols_of_blocks;
}

void SudokuSolver::InitCheckArrays()
{
	for (int i = 0; i < c_number_of_rows; i++)
	{
		for (int j = 0; j < c_number_of_cols; j++)
		{
			if (m_matrix[i][j] != 0)
			{
				AssignCheckValues(i, j, m_matrix[i][j]);
			}
		}
	}
}

bool SudokuSolver::CheckOutFields()
{
	bool changes = false;
	for (int i = 0; i < c_number_of_rows; i++)
	{
		for (int j = 0; j < c_number_of_cols; j++)
		{
			if (m_matrix[i][j] == 0)
			{
				
				for (int value = c_min_field_value ; value <= c_max_field_value && !changes; value++)
				{
					if (IsValueAdequate(i, j, value))
					{
						AssignFieldValue(i, j, value);
						changes = true;
					}
				}
			}
		}
	}
	return changes;
}

bool SudokuSolver::SolveStraightforward()
{
	bool changes = true;
	while (changes)
	{
		changes = CheckOutFields();
	}
	return IsSolved();
}

bool SudokuSolver::IsSolved()
{
	bool solved = false;
	for (int i = 0; i < c_number_of_rows; i++)
	{
		for (int j = 0; j < c_number_of_cols; j++)
		{
			solved = m_matrix[i][j] != 0;
			if (!solved)
			{
				return false;
			}
		}
	}
	return true;
}

void SudokuSolver::WriteSudoku()
{
	for (int i = 0; i < c_number_of_rows; i++)
	{
		if (i % c_rows_of_blocks == 0)
		{
			cout << "+-------+-------+-------+" << endl;
		}
		for (int j = 0; j < c_number_of_rows; j++)
		{
			if (j % c_rows_of_blocks == 0)
			{
				cout << "| ";
			}
			cout << m_matrix[i][j] << " ";
		}
		cout << "| " << endl;
	}
	cout << "+-------+-------+-------+" << endl;
}