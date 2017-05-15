#pragma once

class SudokuField
{
public:
	SudokuField();

	bool IsEmpty();

	SudokuField& operator=(const SudokuField& source);

	void SetField(int row, int col, int value);

	int Row();
	int Col();
	int Value();
private:
	int m_row;
	int m_col;
	int m_value;

};
