#ifndef PREPROCESSING_HPP
#define PPEPROCESSING_HPP

#include <opencv\cv.h>
#include <list>
#include "EdgeSegment.h"
/**Canny based edge detection based on the OpenCv Implementation
input_image		Input image
edge_image	edge image
lowThreshold	lower threashold for hysteresis procedure
highThreshold	higher threashold for hysteresis procedure
kernelSize		size for sobel operator*/
int edgeDetection(cv::Mat input_image,cv::Mat edge_image,int lowThreshold,int highThreshold,int kernelSize);

/*Edge segmentation off a thinned edge list*/
int edgeSegmentation(cv::Mat edge_image,std::list<EdgeSegment> *segments);

/*Finds end points and junctions in the provied edge image
  Based on Peter Kovesis findEndsJunctions*/
int findEndsJunctions(std::list<Point*>*,cv::Mat);

#endif