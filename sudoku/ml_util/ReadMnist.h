#pragma once

#include <string>
#include <vector>
#include <opencv2\imgproc\imgproc.hpp>

class ReadMnist{
public:
	ReadMnist(std::string filenameimages, std::string filenamelabels);
	~ReadMnist();

	std::vector<cv::Mat> read_Mnist_images(std::vector<cv::Mat>& imagesData);
	std::vector<cv::Mat> read_Mnist_labels();

	cv::Mat	read_Mnist_images();
	//cv::Mat	read_Mnist_labels();
private:
	int ReverseInt(int i);
	cv::Mat	concatenateMat(std::vector<cv::Mat> &vec, double scal);

	void read_Mnist_Images(std::vector<cv::Mat> &vec);
	void read_Mnist_Label(std::vector<cv::Mat> &mat);

	std::string m_filenameimages;
	std::string m_filenamelabels;
};