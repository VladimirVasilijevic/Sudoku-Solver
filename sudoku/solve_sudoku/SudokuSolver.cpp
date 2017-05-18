#include "SudokuSolver.h"
#include <iostream>>

using namespace std;

const int c_rows_of_blocks = 3;
const int c_cols_of_blocks = 3;
const int c_max_field_value = 9;
const int c_min_field_value = 1;
const int c_unique = 1;
const int c_reset_value = 0; 
const int c_isEmpty = 0; 
const int c_isSingular = 1;

SudokuField StartWithEmptyField()
{
	SudokuField field;
	field.m_row = 0;
	field.m_col = 0;
	field.m_value = 0;
	return field;
}

void CopyMatrix(const Matrix & source, Matrix & destination)
{
	for (int i = 0; i < c_number_of_rows; i++)
	{
		for (int j = 0; j < c_number_of_cols; j++)
		{
			destination[i][j] = source[i][j];
		}
	}
}

void SaveMatrix(const Matrix & source, std::vector<Matrix> & destination)
{
	destination.emplace_back(source);
}


Matrix PopMatrix(std::vector<Matrix> & destination)
{
	Matrix pop_matrix = destination.back();
	destination.pop_back();
	return pop_matrix;
}

void SaveField(const SudokuField& field, std::vector<SudokuField>& saved_fields)
{
	saved_fields.emplace_back(field);
}

SudokuField PopField(std::vector<SudokuField>& saved_fields)
{
	SudokuField field = saved_fields.back();
	saved_fields.pop_back();
	return field;
}

bool IsFieldEmpty(SudokuField field)
{
	return field.m_value == c_isEmpty;
}

SudokuSolver::SudokuSolver()
{
	m_matrix.resize(c_number_of_rows, vector<int>(c_number_of_cols));
}

SudokuSolver::SudokuSolver(const Matrix& sudoku_matrix)
{
	m_matrix.resize(c_number_of_rows, vector<int>(c_number_of_cols));
	CopyMatrix(sudoku_matrix, m_matrix);
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

void SudokuSolver::LoadMatrix(const Matrix & source)
{
	CopyMatrix(source, m_matrix);
}

SudokuField SudokuSolver::FindEmptyFieldAndGuessValue(SudokuField startField, int startValue)
{
	SudokuField field = StartWithEmptyField();

	for (int i = startField.m_row; i < c_number_of_rows; i++)
	{
		int j = (i == startField.m_row) ? startField.m_col : 0;
		for (; j < c_number_of_cols; j++)
		{
			if (m_matrix[i][j] == 0)
			{
				int value = (i == startField.m_row) ? startValue : c_min_field_value;
				for (; value <= c_max_field_value; value++)
				{
					if (IsCompatible(i, j, value))
					{
						field.m_row = i;
						field.m_col = j;
						field.m_value = value;
						return field;
					}
				}
			}
		}
	}
	return field;
}

bool SudokuSolver::IsCompatible(int row, int col, int value)
{
	return !CheckRow(row, value) && !CheckCol(col, value) && !CheckBlock(row, col, value);
}

bool SudokuSolver::CheckRow(int row, int value)
{
	for (int i = 0; i < c_number_of_rows; i++)
	{
		if (m_matrix[row][i] == value)
		{
			return true;
		}
	}
	return false;
}

bool SudokuSolver::CheckCol(int col, int value)
{
	for (int i = 0; i < c_number_of_cols; i++)
	{
		if (m_matrix[i][col] == value)
		{
			return true;
		}
	}
	return false;
}

bool SudokuSolver::CheckBlock(int row, int col, int value)
{
	int block = Block(row, col);
	int block_row = block / c_rows_of_blocks;
	int block_col = block % c_cols_of_blocks;
	for (int row = block_row * c_rows_of_blocks; row < (block_row + 1) * c_rows_of_blocks; row++)
	{
		for (int col = block_col * c_cols_of_blocks; col < (block_col + 1) * c_cols_of_blocks; col++)
		{
			if (m_matrix[row][col] == value)
			{
				return true;
			}
		}
	}
	return false;
}

bool SudokuSolver::IsSingular(int row, int col, int value)
{
	return (CountCompatibleFieldsInBlock(Block(row, col), value) == c_isSingular) 
		|| (CountCompatibleFieldsInRow(row, value) == c_isSingular) 
		|| (CountCompatibleFieldsInCol(col, value) == c_isSingular);
}

int SudokuSolver::CountCompatibleFieldsInBlock(int block, int value)
{
	int count_of_fit_field = 0;
	int block_row = block / c_rows_of_blocks;
	int block_col = block % c_cols_of_blocks;
	for (int row = block_row * c_rows_of_blocks; row < (block_row + 1) * c_rows_of_blocks; row++)
	{
		for (int col = block_col * c_cols_of_blocks; col < (block_col + 1) * c_cols_of_blocks; col++)
		{
			if (m_matrix[row][col] == c_isEmpty && IsCompatible(row, col, value))
			{
				count_of_fit_field++;
			}
		}
	}
	return count_of_fit_field;
}

int SudokuSolver::CountCompatibleFieldsInRow(int row, int value)
{
	int count_of_fit_field = 0;
	for (int i = 0; i < c_number_of_cols; i++)
	{
		if (m_matrix[row][i] == 0 && IsCompatible(row, i, value))
		{
			count_of_fit_field++;
		}
	}
	return count_of_fit_field;
}

int SudokuSolver::CountCompatibleFieldsInCol(int col, int value)
{
	int count_of_fit_field = 0;
	for (int i = 0; i < c_number_of_rows; i++)
	{
		if (m_matrix[i][col] == 0 && IsCompatible(i, col, value))
		{
			count_of_fit_field++;
		}
	}
	return count_of_fit_field;
}

bool SudokuSolver::IsOnlyFitValue(int row, int col, int value)
{
	for (int possible_value = c_min_field_value; possible_value <= c_max_field_value; possible_value++)
	{
		if (possible_value != value && IsCompatible(row, col, possible_value))
		{
			return false;
		}
	}
	return IsCompatible(row, col, value);
}

bool SudokuSolver::IsUniqueSolution(int row, int col, int value)
{
	return IsCompatible(row, col, value) && IsSingular(row, col, value);
}

void SudokuSolver::AssignFieldValue(int row, int col, int value)
{
	m_matrix[row][col] = value;
}

int SudokuSolver::Block(int row, int col)
{
	return (row / c_rows_of_blocks) * c_cols_of_blocks + col / c_cols_of_blocks;
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
					if (IsUniqueSolution(i, j, value) || IsOnlyFitValue(i, j, value))
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
	std::vector<Matrix> saved_matrixs;
	std::vector<SudokuField> saved_fields;
	SudokuField selected_field;
	SudokuField start_field = StartWithEmptyField();

	selected_field = FindEmptyFieldAndGuessValue(start_field, c_min_field_value);
	while (!IsFieldEmpty(selected_field))
	{
		SaveField(selected_field, saved_fields);
		SaveMatrix(m_matrix, saved_matrixs);
		do
		{
			AssignFieldValue(selected_field.m_row, selected_field.m_col, selected_field.m_value);
			if (SolveStraightforward())
			{
				return true;
			}
			selected_field = FindEmptyFieldAndGuessValue(selected_field, c_min_field_value);
		} while (!IsFieldEmpty(selected_field));

		m_matrix = PopMatrix(saved_matrixs);
		selected_field = PopField(saved_fields);

		selected_field = FindEmptyFieldAndGuessValue(selected_field, selected_field.m_value + 1);
	}

	return false;
}

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