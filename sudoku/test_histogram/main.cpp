#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <chrono>
#include <filesystem>
#include <fstream>

#include "ReadMnist.h"
#include "SquaresDetection.h"

using namespace std::tr2::sys;
using namespace cv;
using namespace std;

void writeHistogram(const string& output, const vector<float>& histogram)
{
	// c++11 tr2
	path imageOutput(output);
	create_directories(imageOutput.parent_path());
	ofstream fileOutput(imageOutput, ios::out);
	//assert(confusion.rows == confusion.cols);
	const int verticals = static_cast<int>(histogram.size());
	fileOutput << "C";
	for (int i = 0; i < verticals; ++i)
	{
		fileOutput << "\t" << i;
	}
	fileOutput << endl;
	for (int i = 0; i < verticals; ++i)
	{
		fileOutput << "\t" << histogram[i];
	}
}

void make_histogram(vector<float>& histogram, const int number_of_images, const int threshold_type, const vector<cv::Mat> images)
{
	int image_dimension = 28;
	const double threshold = 255 / 4;
	for (int i = 0; i < number_of_images; i++)
	{
		cv::Mat img = cv::Mat(images[i].rows, images[i].cols, CV_32FC1);
		images[i].copyTo(img); 
		cv::resize(img, img, cv::Size(image_dimension, image_dimension));
		if (img.channels() != 1)
		{
			cv::cvtColor(img, img, CV_BGR2GRAY);
		}
		cv::threshold(img, img, threshold, 1, threshold_type);
		for (int j = 0; j < image_dimension; j++)
		{
			for (int k = 0; k < image_dimension; k++)
			{
				histogram[j] += img.at<uchar>(k, j);
			}
		}
	}

	for (int i = 0; i < image_dimension; i++)
	{
		histogram[i] /= number_of_images;
	}
}

int main()
{
	string filenamelabelsTest = "D:\\projects\\sudoku\\MNIST\\t10k-labels.idx1-ubyte";
	string filenameimagesTest = "D:\\projects\\sudoku\\MNIST\\t10k-images.idx3-ubyte";

	string filenamelabels = "D:\\projects\\sudoku\\MNIST\\train-labels.idx1-ubyte";
	string filenameimages = "D:\\projects\\sudoku\\MNIST\\train-images.idx3-ubyte";



	ReadMnist MnistTest(filenameimagesTest, filenamelabelsTest);

	//read MNIST iamge into OpenCV Mat vector
	vector<cv::Mat> TestData;
	MnistTest.read_Mnist_images(TestData);

	//read MNIST label into double vector
	vector<cv::Mat> labelsTest = MnistTest.read_Mnist_labels();

	//histogram for MNIST
	const int number_of_MNIST_images = 10000;
	const int image_dimension = 28;
	vector<float> histogram_for_numbers(image_dimension, 0);
	make_histogram(histogram_for_numbers, number_of_MNIST_images, CV_THRESH_BINARY, TestData);

	//histogram for empty sudoku
	SquaresDetection empty_sudoku("D:\\projects\\sudoku\\TestImages\\Untitled.png");
	vector<cv::Mat> empty_fields = empty_sudoku.getImageSquares();
	const int number_of_sudoku_fields_images = 9 * 9;
	vector<float> histogram_for_empty_fields(image_dimension, 0);
	make_histogram(histogram_for_empty_fields, number_of_sudoku_fields_images, CV_THRESH_BINARY_INV, empty_fields);

	//write histograms
	writeHistogram(R"(D:\\projects\\sudoku\\histogram_for_numbers.txt)", histogram_for_numbers);
	writeHistogram(R"(D:\\projects\\sudoku\\histogram_for_empty_fields.txt)", histogram_for_empty_fields);
}