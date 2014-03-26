#include "PreProcessing.hpp"
#include "opencv2\imgproc\imgproc.hpp"

int edgeDetection(cv::Mat input_image, cv::Mat edge_image, int lowThreshold, int highThreshold, int kernelSize) {
	cv::Canny(input_image, edge_image, lowThreshold, highThreshold, kernelSize);

	return 0;
}