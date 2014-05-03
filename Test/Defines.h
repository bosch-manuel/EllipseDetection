#ifndef DEFINES
#define DEFINES
#include <fstream>

#define TRUE 1
#define FALSE 0
#define PI 3.14159265359
//#define DEBUG
#define DEBUG_MAIN
#define EDGE_DEBUG "..\\EdgeLink_Debug.txt"
#define FINDENDS_DEBUG "..\\FindEnds_Debug.txt"
#define CURVE_SEG_DEBUG "..\\CurveSeg_Debug.txt"
#define CURVE_GRP_DEBUG "..\\CurveGrp_Debug.txt"
#define EDGEPOINT 255
#define ENDPOINT 200
//#define DEBUG_MAXLINEDEV
#define DEBUG_SHOW_LINESEGMENTEDEDGES
#define DEBUG_SHOW_EDGESEGS

/*Line segmentation*/
#define MIN_LENGTH 10
#define D_TOL 4

/*Curve segmentation*/
#define DEBUB_CURVE_SEG
#define LTH 4 /* if LTH* length of line 1 > length of line 2 or the other way round an edge segment will be splitted at their connection point P   */
#define TH 40 /*max dif between angles of neighboring line segments  */
#define NP 4 /*a curve segment must contain more than NP pixels otherwise it will be deleted*/

/*Curve grouping*/
#define DEBUB_CURVE_GRP
#define D0 10/*max distance between neighbouring curve segments*/


#endif