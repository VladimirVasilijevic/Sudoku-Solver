#include "ReadMnist.h"
#include <fstream>
#include <cmath>

using namespace cv;
using namespace std;
ReadMnist::~ReadMnist()
{}
ReadMnist::ReadMnist(std::string filenameimages, std::string filenamelabels)
	: m_filenameimages(filenameimages)
	, m_filenamelabels(filenamelabels)
{}

vector<cv::Mat>	ReadMnist::read_Mnist_images(std::vector<cv::Mat>& imagesData)
{
	/*std::vector<cv::Mat> imagesData;*/
	read_Mnist_Images(imagesData);
	return imagesData;
}
vector<cv::Mat>	ReadMnist::read_Mnist_labels()
{
	std::vector<cv::Mat> labelsData;
	read_Mnist_Label(labelsData);
	return labelsData;
}

Mat	ReadMnist::read_Mnist_images()
{
	std::vector<cv::Mat> imagesData;
	read_Mnist_Images(imagesData);
	return concatenateMat(imagesData, 255.0);
}
//Mat	ReadMnist::read_Mnist_labels()
//{
//	std::vector<cv::Mat> labelsData;
//	read_Mnist_Label(labelsData);
//	return concatenateMat(labelsData, 1.0);
//}

void ReadMnist::read_Mnist_Images(vector<cv::Mat> &vec){
	ifstream file(m_filenameimages, ios::binary);
	if (file.is_open())
	{
		int magic_number = 0;
		int number_of_images = 0;
		int n_rows = 0;
		int n_cols = 0;
		file.read((char*)&magic_number, sizeof(magic_number));
		magic_number = ReverseInt(magic_number);
		file.read((char*)&number_of_images, sizeof(number_of_images));
		number_of_images = ReverseInt(number_of_images);
		file.read((char*)&n_rows, sizeof(n_rows));
		n_rows = ReverseInt(n_rows);
		file.read((char*)&n_cols, sizeof(n_cols));
		n_cols = ReverseInt(n_cols);
		for (int i = 0; i < number_of_images; ++i)
		{
			cv::Mat tp = Mat::zeros(n_rows, n_cols, CV_8UC1);
			for (int r = 0; r < n_rows; ++r)
			{
				for (int c = 0; c < n_cols; ++c)
				{
					unsigned char temp = 0;
					file.read((char*)&temp, sizeof(temp));
					tp.at<uchar>(r, c) = (int)temp;
				}
			}
			vec.push_back(tp);
		}
	}
}

void ReadMnist::read_Mnist_Label(vector<cv::Mat> &mat)
{
	ifstream file(m_filenamelabels, ios::binary);
	if (file.is_open()){
		int magic_number = 0;
		int number_of_images = 0;
		int n_rows = 1;
		int n_cols = 10;

		file.read((char*)&magic_number, sizeof(magic_number));
		magic_number = ReverseInt(magic_number);

		file.read((char*)&number_of_images, sizeof(number_of_images));
		number_of_images = ReverseInt(number_of_images);

		for (int i = 0; i < number_of_images; ++i)
		{
			cv::Mat tp = Mat::zeros(n_rows, n_cols, CV_8UC1);
			unsigned char temp = 0;
			file.read((char*)&temp, sizeof(temp));
			tp.at<uchar>(0, (int)temp) = 1;
			mat.push_back(tp);
		}
	}
}


Mat	ReadMnist::concatenateMat(vector<Mat> &vec, double scal){
	// isto za sve slike
	int height = vec[0].rows;
	int width = vec[0].cols;
	Mat res = Mat::zeros(height * width, static_cast<int>(vec.size()), CV_32FC1);
	for (int i = 0; i < vec.size(); i++){
		Mat img(height, width, CV_32FC1);
		vec[i].convertTo(img, CV_32FC1);
		// reshape(int cn, int rows=0), cn is num of channels.
		Mat ptmat = img.reshape(0, height * width);
		Rect roi = cv::Rect(i, 0, ptmat.cols, ptmat.rows);
		Mat subView = res(roi);//ucitava matricu dimenzija roi-a
		ptmat.copyTo(subView);//menja se i res posto se subView pravi samo novi header za iste podatke
	}
	divide(res, scal, res);
	return res.t();
}

int ReadMnist::ReverseInt(int i)
{
	unsigned char ch1, ch2, ch3, ch4;
	ch1 = i & 255;
	ch2 = (i >> 8) & 255;
	ch3 = (i >> 16) & 255;
	ch4 = (i >> 24) & 255;
	return((int)ch1 << 24) + ((int)ch2 << 16) + ((int)ch3 << 8) + ch4;
}