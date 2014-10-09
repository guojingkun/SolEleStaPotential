/*
 * guojk leizj
 */


#ifndef POINT_H
#define POINT_H

#include "mypara.h"

#pragma pack(1)

class CPoint
{
public:
	double potential;    //unit: voltage
	bool potentialFix;    //1 for boundary, or somewhere with fixed potential
	double r;
//	bool rFix;
	CPoint()
	{
		potential = 0;
		potentialFix = 0;
//		rFix = 0;
		r = 0;
	}
};

#endif
