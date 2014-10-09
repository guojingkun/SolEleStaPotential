/*
 * guojk, leizj
 * Verification
 * Surface of a sphere. The potential of it is 1V.
 * R = 50um, r = 45um
 */

#include <iostream>
#include <fstream>
#include <cmath>
#include "Point.h"
#include "mypara.h"
using namespace std;

void CheckBoundaryCondition(CPoint*** Point)
{
	ofstream filef("FP.txt");
	int i, j;
	for(i = 0; i < Nx; i++)
	{
		for(j = 0; j < Ny; j++)
		{
			filef << Point[i][j][Nz/2].potentialFix << " ";
		}
		filef << endl;
	}
}

void BoundaryCondition(CPoint*** Point)
{
	cout << "creating boundary condition" << endl;
	int i, j, k;

	//the surface of the aera
	for(i = 0; i < Nx; i++)
	{
		for(j = 0; j < Ny; j++)
		{
			Point[i][j][0].potential = 0;
			Point[i][j][0].potentialFix = 1;
			Point[i][j][Nz-1].potential = 0;
			Point[i][j][Nz-1].potentialFix = 1;
		}
	}
	for(j = 0; j < Ny; j++)
	{
		for(k = 0; k < Nz; k++)
		{
			Point[0][j][k].potential = 0;
			Point[0][j][k].potentialFix = 1;
			Point[Nx-1][j][k].potential = 0;
			Point[Nx-1][j][k].potentialFix = 1;
		}
	}
	for(i = 0; i < Nx; i++)
	{
		for(k = 0; k < Nz; k++)
		{
			Point[i][0][k].potential = 0;
			Point[i][0][k].potentialFix = 1;
			Point[i][Ny-1][k].potential = 0;
			Point[i][Ny-1][k].potentialFix = 1;
		}
	}

	//inside
	double dist;    //distance from the centre
	double r = 0*delta, R = 20*delta;
	for(i = 0; i < Nx; i++)
	{
		for(j = 0; j < Ny; j++)
		{
			for(k = 0; k < Nz; k++)
			{
				dist = pow((double)(pow(i-CtNx,2.0) + pow(j-CtNy,2.0) + pow(k-CtNz,2.0)), 0.5)*delta;
				if(dist < R)
				{
					Point[i][j][k].potential = 0.1;
					Point[i][j][k].potentialFix = 1;
//					Point[i][j][k].r = 1e2;
//					Point[i][j][k].rFix = 1;
				}
			}
		}
	}
	CheckBoundaryCondition(Point);
	cout << "BC finiehed" << endl;
}
