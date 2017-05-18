#pragma once

#include <vector>

#include "ISudokuSolver.h"
#include "SudokuField.h"

int const c_number_of_rows = 9;
int const c_number_of_cols = 9;


class SudokuSolver:public ISudokuSolver {
public:
	SudokuSolver();
	SudokuSolver(const Matrix& sudoku_matrix);
	bool Solve()  override;
	
	void WriteSudoku() override;
	~SudokuSolver() {}

	void LoadMatrix(const Matrix& source) override;
private:

	SudokuField FindEmptyFieldAndGuessValue(SudokuField startField, int startValue);

	bool IsCompatible(int row, int col, int value);
	bool CheckRow(int row, int value);
	bool CheckCol(int col, int value);
	bool CheckBlock(int row, int col, int value);

	bool IsSingular(int row, int col, int value);
	int CountCompatibleFieldsInBlock(int block, int value);
	int CountCompatibleFieldsInRow(int row, int value);
	int CountCompatibleFieldsInCol(int col, int value);
	bool IsOnlyFitValue(int row, int col, int value);

	bool IsUniqueSolution(int row, int col, int value);

	void AssignFieldValue(int row, int col, int value);

	int Block(int row, int col);
	
	bool CheckOutFields();

	bool SolveStraightforward();
	bool SolveBacktrack();
	bool IsSolved();

	Matrix m_matrix;
};