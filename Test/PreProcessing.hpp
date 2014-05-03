#ifndef PREPROCESSING_HPP
#define PPEPROCESSING_HPP

#include <opencv\cv.h>
#include <list>
#include "EdgeSegment.h"
#include "EllipticalArc.h"
#include "Point.h"
/**Canny based edge detection based on the OpenCv Implementation
input_image		Input image
edge_image	edge image
lowThreshold	lower threashold for hysteresis procedure
highThreshold	higher threashold for hysteresis procedure
kernelSize		size for sobel operator*/
int edgeDetection(cv::Mat input_image,cv::Mat edge_image,int lowThreshold,int highThreshold,int kernelSize);

/*Edge linking of a thinned edge image
return	number of found segments
Based on Peter Kovesis edgeLinkig

Copyright (c) 2006-2013 Peter Kovesi
% Centre for Exploration Targeting
% The University of Western Australia
% peter.kovesi at uwa edu au
%
% Permission is hereby granted, free of charge, to any person obtaining a copy
% of this software and associated documentation files (the "Software"), to deal
% in the Software without restriction, subject to the following conditions:
%
% The above copyright notice and this permission notice shall be included in
% all copies or substantial portions of the Software.
%
% The Software is provided "as is", without warranty of any kind.
*/
int edgeLinking(cv::Mat *edge_image,std::list<Point*> *endPoints,std::list<EdgeSegment*> *segments);

/*Finds end points and junctions in the provied edge image
  Based on Peter Kovesis findEndsJunctions*/
int findEnds(std::list<Point*>*,cv::Mat*);

/*Test whether the segment should be splitted at P inregard to the length condition
param
L1		point left to P
R1		point right to P
return
true	segment must be splitted at P
false	else*/
bool lengthCond(Point* L1, Point *P, Point *R1);

/*Test whether the segment should be splitted at P in regard to the curvature condition
param
L2		point before L1
L1		point before P
P		point before R1
R1		point after P
return
true	segment should be splitted at P
fase	else
*/
bool curvatureCond(Point *L2, Point *L1, Point *P, Point *R1);

/*Test whetherthe segment should be splitted at P in regard to the curvature condition
param
L2		point before L1
L1		point before P
P		point before R1
R1		point after P
R2		point after R1
return
true	segment should be splitted at P
fase	else
*/
bool angleCond(Point *L2, Point *L1, Point *P, Point *R1, Point *R2);

/*Curve segmentation based on the approach in "A real-time ellipse detection based on edge grouping" by Thanh Minh Nguyen, Siddhant Ahuja and Q. M. Jonathan Wu*/
int curveSegmentation(std::list<EdgeSegment*> *edgeSegs, std::list<EdgeSegment*> *curveSegs);

/*Curve grouping based on the approach in "A real-time ellipse detection based on edge grouping" by Thanh Minh Nguyen, Siddhant Ahuja and Q. M. Jonathan Wu*/
int curveGrouping(std::list<EdgeSegment*> *curveSegs, std::set<EllipticalArc*> *arcs);

#endif