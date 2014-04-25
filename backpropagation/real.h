/*----------------------------------------------------------------------------*
 * 
 *	file: real.h
 * 
 *	function: define Real number as either float or double 
 *	author:   Yoonsuck Choe <yschoe@cs.utexas.edu>
 *	creation: Sat Jul  4 06:32:34 CDT 1998
 * 
 *	$Log: real.h,v $
 *	Revision 1.3  1999/02/09 07:42:25  yschoe
 *	some compatibility routines added -- must clean up
 *
 *	Revision 1.2  1998/09/04 03:36:37  yschoe
 *	fixed all warnings generated by -Wall
 *
 *	Revision 1.1  1998/07/23 01:07:05  yschoe
 *	jul22 stable version (except netlayer)
 *
 *
 *----------------------------------------------------------------------------*/
#ifndef __REAL_H
#define __REAL_H

#include <math.h>
#include <stdlib.h>

using namespace std;

/* define Real as either float or double */
typedef float 	REAL;

#ifndef MAX
#define MAX(x,y)	(((x)>(y))?(x):(y))
#endif

#ifndef MIN
#define MIN(x,y)	(((x)>(y))?(y):(x))
#endif

#define REAL_EQ(x,y)	(fabs((x)-(y))<0.00000000001)
#define DEG_TO_RAD(x)	((x)*M_PI/180.0)

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define DONTCARE	0

#ifdef ANSI
extern double drand48();
#endif

#endif
