#include "SudokuSolver.h"
#include <iostream>>

using namespace std;

const int c_rows_of_blocks = 3;
const int c_cols_of_blocks = 3;
const int c_max_field_value = 9;
const int c_min_field_value = 1;
const int c_unique = 1;
const int c_reset_value = 0;

SudokuSolver::SudokuSolver(int(&sudoku_matrix)[c_number_of_rows][c_number_of_cols])
{
	m_matrix.resize(c_number_of_rows, vector<int>(c_number_of_cols));
	FillCheckArrays();
	LoadField(sudoku_matrix);
	InitCheckArrays();
}

bool SudokuSolver::Solve() 
{
	SolveStraightforward();
	if (!IsSolved())
	{
		SolveBacktrack();
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

void SudokuSolver::SaveMatrix(Matrix &sudoku_matrix)
{
	for (int i = 0; i < c_number_of_rows; i++)
	{
		for (int j = 0; j < c_number_of_cols; j++)
		{
			sudoku_matrix[i][j] = m_matrix[i][j];
		}
	}
}

void SudokuSolver::ResetMatrix(Matrix sudoku_matrix)
{
	for (int i = 0; i < c_number_of_rows; i++)
	{
		for (int j = 0; j < c_number_of_cols; j++)
		{
			m_matrix[i][j] = sudoku_matrix[i][j];
		}
	}
	FillCheckArrays();
	InitCheckArrays();
}

void SudokuSolver::SaveMatrix()
{
	m_saved_matrixs.push_back(m_matrix);
}

void SudokuSolver::ResetMatrix()
{
	ResetMatrix(m_saved_matrixs.back());
	m_saved_matrixs.pop_back();
}

void SudokuSolver::SaveField(SudokuField field)
{
	m_saved_fields.push_back(field);
}

void SudokuSolver::ResetField(SudokuField &field)
{
	field = m_saved_fields.back();
	m_saved_fields.pop_back();
}

SudokuField SudokuSolver::FindEmptyFieldAndGuessValue(SudokuField startField, int startValue)
{
	SudokuField field;
	for (int i = startField.Row(); i < c_number_of_rows; i++)
	{
		int j = (i == startField.Row()) ? startField.Col() : 0;
		for (; j < c_number_of_cols; j++)
		{
			if (m_matrix[i][j] == 0)
			{
				int value = (i == startField.Row()) ? startValue : c_min_field_value;
				for (; value <= c_max_field_value; value++)
				{
					if (CheckFieldValue(i, j, value))
					{
						field.SetField(i, j, value);
						return field;
					}
				}
			}
		}
	}
	return field;
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

bool SudokuSolver::IsOnlyFitValue(int row, int col, int value)
{
	for (int possible_value = c_min_field_value; possible_value <= c_max_field_value; possible_value++)
	{
		if (possible_value != value && CheckFieldValue(row, col, possible_value))
		{
			return false;
		}
	}
	return CheckFieldValue(row, col, value);
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
					if (IsValueAdequate(i, j, value) || IsOnlyFitValue(i, j, value))
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

bool SudokuSolver::SolveBacktrack()
{
	SudokuField selected_field;
	SudokuField start_field;

	selected_field = FindEmptyFieldAndGuessValue(start_field, c_min_field_value);
	while (!selected_field.IsEmpty())
	{
		SaveField(selected_field);
		SaveMatrix();
		do
		{
			AssignFieldValue(selected_field.Row(), selected_field.Col(), selected_field.Value());
			if (SolveStraightforward())
			{
				return true;
			}
			selected_field = FindEmptyFieldAndGuessValue(selected_field, c_min_field_value);
		} while (!selected_field.IsEmpty());

		ResetMatrix();
		ResetField(selected_field);

		selected_field = FindEmptyFieldAndGuessValue(selected_field, selected_field.Value() + 1);
	}

	return false;
}

//bool SudokuSolver::SolveBacktrack()
//{
//	for (int i= 0; i < c_number_of_rows; i++)
//	{
//		for (int j = 0; j < c_number_of_cols; j++)
//		{
//			if (m_matrix[i][j] == 0)
//			{
//				for (int value = c_min_field_value; value <= c_max_field_value; value++)
//				{
//					if (CheckFieldValue(i, j, value))
//					{
//						Matrix saved_matrix;
//						saved_matrix.resize(c_number_of_rows, vector<int>(c_number_of_cols));
//						SaveMatrix(saved_matrix);
//						AssignFieldValue(i, j, value);
//						if (Solve())
//						{
//							return true;
//						}
//						else
//						{
//							ResetMatrix(saved_matrix);
//						}
//					}
//				}
//			}
//		}
//	}
//	return false;
//}

bool SudokuSolver::IsSolved()
{
	for (int i = 0; i < c_number_of_rows; i++)
	{
		for (int j = 0; j < c_number_of_cols; j++)
		{
			if (m_matrix[i][j] == 0)
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