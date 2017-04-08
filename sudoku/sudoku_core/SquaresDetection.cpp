#include "SquaresDetection.h"
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#include <cmath>
#include <vector>

static int const  c_number_of_rows = 28;
static int const  c_number_of_cols = 28; 
static int const  c_threshold_edge = 255/2;
static int const  c_threshold_maxVal = 1;
static int const  c_number_of_sudoku_cols = 9;
static double const  c_percent = 0.25;
static int const  c_edge_for_histogram = 4;

SquaresDetection::SquaresDetection(std::string filename)
{
	cv::Mat src = cv::imread(filename);
	m_image = src.clone();
	m_allFields = getImageSquares();
}

SquaresDetection::SquaresDetection(cv::Mat image)
{
	m_image = image.clone();
	m_allFields = getImageSquares();
}

bool SquaresDetection::IsDetected()
{
	return !m_allFields.empty();
}

cv::Mat SquaresDetection::GetSquare(int row, int col) const
{
	if (m_allFields.empty())
	{
		return cv::Mat();
	}
	return m_allFields[row * c_number_of_sudoku_cols + col];
}

bool SquaresDetection::IsEmpty(int row, int col)
{
	cv::Mat square = GetSquare(row, col).clone();
	cv::resize(square, square, cv::Size(c_number_of_rows, c_number_of_cols));
	if (square.channels() != 1)
	{
		cv::cvtColor(square, square, CV_BGR2GRAY);
	}
	cv::threshold(square, square, c_threshold_edge, c_threshold_maxVal, CV_THRESH_BINARY_INV);

	std::vector<float> histogram(c_number_of_cols, 0);
	for (int j = (c_number_of_cols * c_percent); j < (c_number_of_cols * (1 - c_percent)); j++)
	{
		for (int k = 0; k < c_number_of_rows; k++)
		{
			histogram[j] += square.at<uchar>(k, j);
		}
		if (histogram[j] > c_edge_for_histogram)
		{
			return false;
		}
	}
	return true;
}

void SquaresDetection::imagePreProcessing(cv::Mat& image)
{
	GaussianBlur(image, image, cv::Size(11, 11), 0, 0);
	cv::cvtColor(image, image, CV_BGR2GRAY);

	erode(image, image, cv::MORPH_RECT);
	dilate(image, image, cv::MORPH_RECT);
	// Use Canny instead of threshold to catch squares with gradient shading
	cv::Canny(image, image, 0, 100, 3);
}

cv::Rect SquaresDetection::getBiggestRectangle()
{
	cv::Rect biggestRectangle;

	cv::Mat image = m_image.clone();
	imagePreProcessing(image);
	
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(image, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	//cv::findContours(image.clone(), contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	double max_rectangle_size = 0;
	for (int i = 0; i < contours.size(); i++)
	{
		std::vector<cv::Point> approx(contours[i].size()); // Allocate enough memory for approxPolyDP.

														   // Approximate contour with accuracy proportional
														   // to the contour perimeter
		const double contour_perimeter = cv::arcLength(cv::Mat(contours[i]), true) * 0.01;
		cv::approxPolyDP(cv::Mat(contours[i]), approx, contour_perimeter, true);
		//cv::approxPolyDP(contours[i], approx, cv::arcLength(cv::Mat(contours[i]), true)*0.01, true);

		// Skip small or non-convex objects 
		double rectangle_size = std::fabs(cv::contourArea(contours[i]));
		//std::fabs(cv::contourArea(contours[i])) < 100 ||
		if (!cv::isContourConvex(approx))
		{
			continue;
		}
		else if (IsRectangle(approx) && rectangle_size > max_rectangle_size)
		{
			max_rectangle_size = rectangle_size;
			cv::Point topLeft = topLeftCorner(approx);
			int height = SquaresHeight(approx);
			int width = SquaresWidth(approx);
			biggestRectangle = cv::Rect(topLeft.x, topLeft.y, height, width);
		}
	}
	return biggestRectangle;
}

std::vector<cv::Mat> SquaresDetection::getImageSquares()
{
	std::vector<cv::Mat> imageSquares;
	cv::Rect biggestRectangle = getBiggestRectangle();
	
	cv::Point topLeft;
	topLeft.x = biggestRectangle.x;
	topLeft.y = biggestRectangle.y;
	int height = biggestRectangle.height / 9;
	int width = biggestRectangle.width / 9;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
			{
				cv::Rect field = cv::Rect(topLeft.x + height * j, topLeft.y + width * i, height, width);
				cv::Mat field_image = m_image(field);
				imageSquares.push_back(field_image);
			}
	}
	return imageSquares;
}

int SquaresDetection::SquaresHeight(std::vector<cv::Point> contours)
{
	int top_x = std::min(std::min(contours[0].x, contours[1].x), std::min(contours[2].x, contours[3].x));
	int bottom_x = std::max(std::max(contours[0].x, contours[1].x), std::max(contours[2].x, contours[3].x));
	return bottom_x - top_x;
}

int SquaresDetection::SquaresWidth(std::vector<cv::Point> contours)
{
	int left_y = std::min(std::min(contours[0].y, contours[1].y), std::min(contours[2].y, contours[3].y));
	int right_y = std::max(std::max(contours[0].y, contours[1].y), std::max(contours[2].y, contours[3].y));
	return right_y - left_y;
}

cv::Point SquaresDetection::topLeftCorner(std::vector<cv::Point> contours)
{
	cv::Point topLeft;
	topLeft.x = std::min(std::min(contours[0].x, contours[1].x), std::min(contours[2].x, contours[3].x));
	topLeft.y = std::min(std::min(contours[0].y, contours[1].y), std::min(contours[2].y, contours[3].y));
	return topLeft;
}

double SquaresDetection::angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

bool SquaresDetection::IsRectangle(std::vector<cv::Point> contours)
{
	if (contours.size() == 4)
	{
		// Number of vertices of polygonal curve
		int vtc = contours.size();

		// Get the cosines of all corners
		std::vector<double> cos;
		for (int j = 2; j < vtc + 1; j++)
			cos.push_back(angle(contours[j%vtc], contours[j - 2], contours[j - 1]));

		// Sort ascending the cosine values
		std::sort(cos.begin(), cos.end());

		// Get the lowest and the highest cosine
		double mincos = cos.front();
		double maxcos = cos.back();

		// Use the degrees obtained above and the number of vertices
		// to determine the shape of the contour
		return (vtc == 4 && mincos >= -0.1 && maxcos <= 0.3);
	}
	return false;
}