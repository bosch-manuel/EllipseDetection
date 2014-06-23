#ifndef DEFINES
#define DEFINES
#include <fstream>

//#define SOURCE_IMAGE "..\\6Ellipsen.jpg"
//#define SOURCE_IMAGE "..\\strassenschilder2.png"
#define SOURCE_IMAGE "..\\Bild3.jpg"

#define TRUE 1
#define FALSE 0
#define PI 3.14159265359
//#define DEBUG
//#define DEBUG_MAIN
#define EDGE_DEBUG "..\\EdgeLink_Debug.txt"
#define FINDENDS_DEBUG "..\\FindEnds_Debug.txt"
#define CURVE_SEG_DEBUG "..\\CurveSeg_Debug.txt"
#define CURVE_GRP_DEBUG "..\\CurveGrp_Debug.txt"
#define EVAL_CURVE_DEBUG "..\\EvalCurvature.txt"
#define EDGEPOINT 255
#define ENDPOINT 200
//#define DEBUG_MAXLINEDEV
//#define DEBUG_LINESEG
//#define DEBUG_SHOW_LINESEGMENTEDEDGES
//#define DEBUG_SHOW_EDGESEGS
//#define DEBUG_SHOW_EDGES
//#define DEBUG_EVAL_CURVE

/*Line segmentation*/
#define MIN_LENGTH 10
#define D_TOL 3
//#define DRAW_POINTS

/*Curve segmentation*/
#define IMPROVED_SEGMENTATION
#define DEBUB_CURVE_SEG
#define LTH 4 /* if LTH* length of line 1 > length of line 2 or the other way round an edge segment will be splitted at their connection point P   */
#define TH 0.4363//25  //.2618//15  .1745 //10  .5236//30	.698 /*40 max dif between angles of neighboring line segments (in radian)  */
#define NP 3 /*a curve segment must contain more than NP pixels otherwise it will be deleted*/
#define A_TOL -.07 //-4 deg /*if A_TOL < a2-a1 < 0, segment will not be splitted at point P in regard to curvature condition */
#define B_MIN 1.745 /*angles between the neighbouring lines in a curve segments must all be greater than B_MIN (in radian)*/
#define B_MAX 3.054 /*angles between the neighbouring lines in a curve segments must all be smaller than B_MAX (in radian)*/
//#define SINGLESTEPCURVE

///*Curve grouping*/
////#define DEBUB_CURVE_GRP
//#define D0 10/*max distance between neighbouring curve segments*/
//#define END_END 1
//#define END_BEGIN 2
//#define BEGIN_END 3
//#define BEGIN_BEGIN 4

/*Ellipse calculation*/
//#define DEBUG_CALC
#define DRAW_ELLIPSES
#define DEBUG_ELLIP_DIST
#define TH_E 20 /*an ellipse is a valid if the avarage distance of all points from the ellipse is smaller than TH_E*/

/*RANSAC*/
#define TH_DIST 10 //a pixel mathes the calculated ellipse if its distance is <= TH_DIST
#define N_T	//number of tries
#define TH_ACCEPT 0.6	// an curve segment matches an ellipse if: (num of matching pixels)/(num of segment pixels) >= TH_ACCEPT
#endif