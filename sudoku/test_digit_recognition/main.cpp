#include "DigitRecognition.h"

#include <opencv2/opencv.hpp>

#include <fstream>


using namespace cv;
using namespace std;

static int const  c_width = 28;
static int const  c_height = 28;
static int const  c_channels = 1;

static int const  c_image_width = 20;
static int const  c_image_height = 20;


static int const  c_num_channels = 3;
static cv::Size const c_input_geometry = cv::Size(c_width, c_height);


int main(int argc, char** argv)
{
	string json = "D:\\projects\\sudoku\\MNIST\\mymodel-symbol.json";
	string params = "D:\\projects\\sudoku\\MNIST\\mymodel-0100.params";

	ofstream fileOutput(R"(D:\\projects\\sudoku\\MNIST\\digits.txt)", ios::out);
	DigitRecognition net(json, params);

	Mat input = imread("D:\\projects\\sudoku\\MNIST\\digits.png");
	int correct = 0;
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 50; j++)
		{
			Mat dst = input
			(
				Rect(i * c_image_width, j * c_image_height, c_image_width, c_image_height)
			).clone();
			Mat number;
			resize(dst, number, c_input_geometry);
			int predict_number = net.PredictNumber(number);
			if (predict_number == j / 5)
			{
				correct++;
			}
			fileOutput << net.PredictNumber(number) << " ";
			
		}
		fileOutput << endl;
	}
	cout << "procenat tacnih = " << correct * 100 / 5000 << "%";
	return 0;
}