#include "SudokuField.h"

SudokuField::SudokuField() : m_row(0), m_col(0), m_value(0)
{}

bool SudokuField::IsEmpty()
{
	return m_value == 0;
}

SudokuField & SudokuField::operator=(const SudokuField & source)
{
	if (this != &source)
	{ 
		this->m_row = source.m_row;
		this->m_col = source.m_col;
		this->m_value = source.m_value;
	}
	return *this;
}

void SudokuField::SetField(int row, int col, int value)
{
	m_row = row;
	m_col = col;
	m_value = value;
}

int SudokuField::Row()
{
	return m_row;
}

int SudokuField::Col()
{
	return m_col;
}

int SudokuField::Value()
{
	return m_value;
}
