#pragma once

#include <string>
#include "mxnet/c_predict_api.h"
#include <opencv2/opencv.hpp>

class DigitRecognition {
public:
	DigitRecognition(std::string json_file, std::string param_file);
	~DigitRecognition();

	int PredictNumber(cv::Mat image_of_number);
private:
	// Convert the input image to feed the network
	cv::Mat Preprocess(const cv::Mat& img, int num_channels, cv::Size input_geometry);

	void InitPredictor(PredictorHandle &predictor);

	int MaxInArray(const float *array);

	PredictorHandle m_predictor;

	std::string m_json_file;
	std::string m_param_file;
};