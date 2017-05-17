#pragma once
#include <vector>

typedef std::vector<std::vector<int>> Matrix;

class ISudokuSolver {
public:
	virtual bool Solve()  = 0;
	virtual ~ISudokuSolver() {}
	virtual void WriteSudoku() = 0;
	virtual void LoadMatrix(const Matrix& source) = 0;
};