#pragma once

#include "ISudokuSolver.h"


int const c_number_of_rows = 9;
int const c_number_of_cols = 9;

class SudokuSolver:public ISudokuSolver {
public:
	SudokuSolver(int (&sudoku_matrix)[c_number_of_rows][c_number_of_cols]);
	bool Solve()  override;
	
	void WriteSudoku() override;
	~SudokuSolver() {}
private:
	void LoadField(int(&sudoku_matrix)[c_number_of_rows][c_number_of_cols]);

	void SaveMatrix(int(&sudoku_matrix)[c_number_of_rows][c_number_of_cols]);
	void ResetMatrix(int(&sudoku_matrix)[c_number_of_rows][c_number_of_cols]);

	void FillCheckArrays();

	bool CheckFieldValue(int row, int col, int value);
	bool CheckRow(int row, int value);
	bool CheckCol(int col, int value);
	bool CheckBlock(int row, int col, int value);

	bool IsSingular(int row, int col, int value);
	bool IsBlockSingular(int block, int value);
	bool IsRowSingular(int row, int value);
	bool IsColSingular(int col, int value);
	bool IsOnlyFitValue(int row, int col, int value);

	bool IsValueAdequate(int row, int col, int value);

	void AssignCheckValues(int row, int col, int value);
	void AssignFieldValue(int row, int col, int value);

	int Block(int row, int col);

	void InitCheckArrays();

	bool CheckOutFields();

	bool SolveStraightforward();
	bool SolveBacktrack();
	bool IsSolved();

	
	//-----------------------
	int m_matrix[c_number_of_rows][c_number_of_cols];

	//check_arrays
	bool m_rows[c_number_of_rows][c_number_of_cols];
	bool m_cols[c_number_of_rows][c_number_of_cols];
	bool m_blocks[c_number_of_rows][c_number_of_cols];

	
};