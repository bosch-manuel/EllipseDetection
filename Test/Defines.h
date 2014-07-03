#ifndef DEFINES
#define DEFINES
#include <fstream>

//#define SOURCE_IMAGE "..\\6Ellipsen.jpg"
//#define SOURCE_IMAGE "..\\strassenschilder2.png"
/*edge detection*/
#define TH_U 30  //150
#define TH_L 20  //100
#define DEBUG_SHOW_GAUSS

#define TRUE 1
#define FALSE 0
#define PI 3.14159265359
//#define DEBUG
//#define DEBUG_MAIN
#define EDGE_DEBUG "E:\\EdgeLink_Debug.txt"
#define FINDENDS_DEBUG "E:\\FindEnds_Debug.txt"
#define CURVE_SEG_DEBUG "E:\\CurveSeg_Debug.txt"
#define CURVE_GRP_DEBUG "E:\\CurveGrp_Debug.txt"
#define EVAL_CURVE_DEBUG "E:\\Curvature.txt"

/*image paths*/
#define SRC_IMG_PATH "E:\\source.png"
#define EDEGE_IMG_PATH "E:\\edgeImage.png"
#define EDGE_IMG_AFTER_FIND_PATH "E:\\edgeImage_NachFindEnds.png"
#define EDGE_SEG_IMG_PATH "E:\\Segments.png"
#define LINE_SEG_IMG_PATH "E:\\lineSegmentedEdges.png"
#define CURVE_SEG_IMG_PATH "E:\\curveSegments.png"
#define ELLIP_ON_SRC_IMG_PATH "E\\EllipsesOnSource.png"
#define ELLIP_IMG_PATH "E:\\Ellipses.png"
#define PRETTY_EDEGE_IMG_PATH "E:\\PrettyEdgeImage.png"
#define PRETTY_EDEGE_IMG_PATH2 "E:\\PrettyEdgeImage_NachFindEnds.png"

#define EDGEPOINT 255
#define ENDPOINT 200
//#define DEBUG_MAXLINEDEV
//#define DEBUG_LINESEG
//#define DEBUG_SHOW_LINESEGMENTEDEDGES
#define DEBUG_SHOW_EDGESEGS
#define DEBUG_SHOW_EDGES
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
#define D0 10/*max distance between neighbouring curve segments*/
#define END_END 1
#define END_BEGIN 2
#define BEGIN_END 3
#define BEGIN_BEGIN 4

/*Ellipse calculation*/
//#define DIRECT_FIT
//#define DEBUG_CALC
#define DRAW_ELLIPSES
//#define DEBUG_ELLIP_DIST
#define TH_E 20 /*an ellipse is a valid if the avarage distance of all points from the ellipse is smaller than TH_E*/

/*RANSAC*/
//#define DEBUG_RANSAC
//#define DEBUG_RANSAC_SHOW_LAST_ELLIPSE
//#define DEBUG_RANSAC_SHOW_ALL_POSSIBLE_SEGMENTS
//#define DEBUG_RANSAC_SHOW_ALL_FITTING_SEGMENTS
//#define DEBUG_RANSAC_RANDOM_POINTS
//#define DEBUG_COULD_MAKE_ELLIPSE
#define N_POINTS 6	// number of points needed to calculate an ellipse
#define TH_DIST 3 //3 a pixel matches the calculated ellipse if its distance is <= TH_DIST
#define N_T	4//number of tries
#define TH_AVDIST 2
#define TH_FULLFIT 0.8	// an curve segment matches an ellipse as a whole if: (num of matching pixels)/(num of segment pixels) >= TH_FULLFIT
#define TH_PARTIALFIT 0.4 // a curve segment matches an ellipse partly if:(num of matching pixels)/(num of segment pixels) >= TH_PARTIALFIT

#endif