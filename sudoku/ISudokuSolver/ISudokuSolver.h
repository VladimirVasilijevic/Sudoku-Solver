#pragma once

class ISudokuSolver {
public:
	virtual bool Solve()  = 0;
	virtual ~ISudokuSolver() {}
	virtual void WriteSudoku() = 0;
	
};