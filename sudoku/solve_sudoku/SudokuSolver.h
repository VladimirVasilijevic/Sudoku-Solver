#pragma once

#include <vector>

#include "ISudokuSolver.h"
#include "SudokuField.h"

typedef std::vector<std::vector<int>> Matrix;

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

	//za rekurzivno resenje
	void SaveMatrix(Matrix &sudoku_matrix);
	void ResetMatrix(Matrix sudoku_matrix);

	//resenje bez rekurzije
	void SaveMatrix();
	void ResetMatrix();

	void SaveField(SudokuField field);
	void ResetField(SudokuField &field);

	SudokuField FindEmptyFieldAndGuessValue(SudokuField startField, int startValue);

	void FillCheckArrays();
	void ResetCheckArrays(int row, int col);

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
	Matrix m_matrix;

	//check_arrays
	bool m_rows[c_number_of_rows][c_number_of_cols];
	bool m_cols[c_number_of_rows][c_number_of_cols];
	bool m_blocks[c_number_of_rows][c_number_of_cols];

	std::vector<Matrix> m_saved_matrixs;
	std::vector<SudokuField> m_saved_fields;

	
};