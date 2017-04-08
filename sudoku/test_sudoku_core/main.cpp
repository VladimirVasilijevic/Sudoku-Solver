#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>



#include "SquaresDetection.h"

using namespace cv;
using namespace std;

int main()
{
	SquaresDetection sudoku("D:\\projects\\sudoku\\TestImages\\sudoku0.jpg");
	SquaresDetection no_sudoku("D:\\projects\\sudoku\\TestImages\\no_sudoku.jpg");

	if (no_sudoku.IsDetected())
	{
		cout << "nije sudoku slika \n";
	}
	else
	{
		cout << "detekcija ne valja!!! \n"; 
	}

	//cv::Mat src = cv::imread("D:\\projects\\sudoku\\TestImages\\sudoku0.jpg");
	for (size_t i = 0; i < 9; i++)
	{
		for (size_t j = 0; j < 9; j++)
		{
			Mat sudokuMat = sudoku.GetSquare(i, j);
			cout << sudoku.IsEmpty(i, j) << "\t";
		}
		cout <<  endl;
	}

	//cv::namedWindow("sudoku", CV_WINDOW_AUTOSIZE);
	//cv::imshow("sudoku", src);

	//cv::namedWindow("sudoku 1,1", CV_WINDOW_AUTOSIZE);
	//cv::namedWindow("sudoku 8,8", CV_WINDOW_AUTOSIZE);
	//cv::namedWindow("sudoku 5,2", CV_WINDOW_AUTOSIZE);
	//cv::namedWindow("sudoku 4,6", CV_WINDOW_AUTOSIZE);

	//cv::imshow("sudoku 1,1", sudoku.GetSquare(1, 1));
	//cv::imshow("sudoku 8,8", sudoku.GetSquare(8, 8));
	//cv::imshow("sudoku 5,2", sudoku.GetSquare(5, 2));
	//cv::imshow("sudoku 4,6", sudoku.GetSquare(4, 6));

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			string file_name = string("D:\\projects\\sudoku\\SudokuSquares\\square");
			file_name = file_name + "(" + std::to_string(i) + ", " + std::to_string(j) + ").jpg";
			cv::imwrite(file_name, sudoku.GetSquare(i, j));
		}
	}

	//cv::waitKey(0);
}