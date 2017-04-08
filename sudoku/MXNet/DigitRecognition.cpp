#include "DigitRecognition.h"
#include "BufferFile.h"

#include "opencv2/imgproc.hpp"

#include <assert.h>

using namespace std;
using namespace cv;

static int const  c_width = 28;
static int const  c_height = 28;
static int const  c_channels = 1;

static int const  c_num_channels = 3;
static cv::Size const c_input_geometry = cv::Size(c_width, c_height);

static const int c_outputLayerSize = 10;
static const int c_outputLayerIndex = 0;

DigitRecognition::DigitRecognition(std::string json_file, std::string param_file)
	: m_json_file(json_file)
	, m_param_file(param_file)
	, m_predictor(0)
{
	InitPredictor(m_predictor);
}

DigitRecognition::~DigitRecognition()
{
	MXPredFree(m_predictor);
}

void DigitRecognition::InitPredictor(PredictorHandle &predictor)
{
	// Init predictor parameters
	string json_file = m_json_file;
	string param_file = m_param_file;

	BufferFile json_data(json_file);
	BufferFile param_data(param_file);

	int dev_type = 1; // 1:CPU, 2:GPU
	int dev_id = 0;
	mx_uint num_input = 1;
	const char* input_keys[] = { "data" };
	const mx_uint input_shape_indptr[] = { 0,4 };
	const mx_uint input_shape_data[] = 
	{ 
		1,
		static_cast<mx_uint>(c_channels),
		static_cast<mx_uint>(c_width),
		static_cast<mx_uint>(c_height) 
	};

	// Create predictor
	int status = MXPredCreate
	(
		(const char*)json_data.GetBuffer(),
		(const char*)param_data.GetBuffer(),
		static_cast<size_t>(param_data.GetLength()),
		dev_type,
		dev_id,
		num_input,
		input_keys,
		input_shape_indptr,
		input_shape_data,
		&predictor
	);
	assert(predictor);
}

int DigitRecognition::PredictNumber(Mat image_of_number)
{
	int check;
	Mat netImage = Preprocess(image_of_number, c_num_channels, c_input_geometry);
	//-- Set Input Image
	check = MXPredSetInput(m_predictor, "data", (float *)netImage.data, c_width*c_height * c_channels);
	assert(!check);
	// Run forward
	check = MXPredForward(m_predictor);
	assert(!check);
	// Get output array from the class layer
	float *output = new float[c_outputLayerSize];
	check = MXPredGetOutput(m_predictor, c_outputLayerIndex, &output[0], c_outputLayerSize);
	assert(!check);

	//index of max in output
	int number = MaxInArray(output);

	// Free memory
	delete[] output;

	return number;
}

// Convert the input image to feed the network

Mat DigitRecognition::Preprocess(const cv::Mat& img, int num_channels, cv::Size input_geometry)
{
	Mat sample;

	// Convet color space
	if (img.channels() == 3 && num_channels == 1)
		cvtColor(img, sample, CV_BGR2GRAY);
	else if (img.channels() == 4 && num_channels == 1)
		cvtColor(img, sample, CV_BGRA2GRAY);
	else if (img.channels() == 4 && num_channels == 3)
		cvtColor(img, sample, CV_BGRA2BGR);
	else if (img.channels() == 1 && num_channels == 3)
		cvtColor(img, sample, CV_GRAY2BGR);
	else sample = img;

	// Resize image
	Mat sample_resized;
	resize(sample, sample_resized, input_geometry);

	Mat sample_float;
	if (num_channels == 3)
		sample_resized.convertTo(sample_float, CV_32FC3);
	else
		sample_resized.convertTo(sample_float, CV_32FC1);

	// Convert image from BGR,BGR,BGR,... into RRR...+GGG...+BBB...
	vector<Mat> spl;
	split(sample_float, spl);

	Mat output(spl[2]);
	output.push_back(spl[1]);
	output.push_back(spl[0]);

	// Return normalized image
	return output;
}

int DigitRecognition::MaxInArray(const float *array)
{
	int index = 0;
	float max = array[0];
	for (int i = 0; i < c_outputLayerSize; i++)
	{
		if (max < array[i])
		{
			max = array[i];
			index = i;
		}
	}
	return index;
}