#ifndef LINEARIZEDEDGESEGMENT_H
#define LINEARIZEDEDGESEGMENT_H

#include "EdgeSegment.h"

class LinearizedEdgeSegment
{
public:
	/*Calculates an linearized edge segment which approximates the countour of the provided edge segment with straight lines*/
	LinearizedEdgeSegment(EdgeSegment*);
	~LinearizedEdgeSegment();
};


#endif