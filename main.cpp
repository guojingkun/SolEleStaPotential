/*
 * Successive Overrelaxation (SOR)
 * guojk leizj
 * 
 * g++ std=c++11 -pthread main.cpp -Ofast
 */

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <stdio.h>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <unistd.h>

#include "mypara.h"
#include "Point.h"
#include "boundary.h"
//#include "BoundModify.cpp"

using namespace std;

CPoint ***Point;
time_t time0;

bool P = 0, run = 1;
mutex m;

class CSeg
{
public:
	double err;
	int NxBeg;
	int NxEnd;
	int LoopProgressed;
	CSeg()
	{
		LoopProgressed = 0;
		err = 0;
	}
}cseg[ThreadNum];    //The space is divided in to "ThreadNum" parts. Each part would be calculated by one thread.

void CreateSpace()    //To allocate enough RAM
{
	int i, j;
	Point = new CPoint**[Nx];
	for(i = 0; i < Nx; i++)
	{
		Point[i] = new CPoint*[Ny];
		for(j = 0; j < Ny; j++)
		{
			Point[i][j] = new CPoint[Nz];
		}
	}
}

void calculate(int td)    //To calculate the potential inside the space
{
	printf("thread %d\n",td);
	int i, j, k, Head = cseg[td].NxBeg, Tail = cseg[td].NxEnd;
	double err, residual, residual_charge, tmp;
	int LoopPorgressed = 0;
	while(run)
	{
		err = 0;
		i = Head+1;
		
		for(i = Head; i <= Tail; i++)
		{
			for(j = 0; j < Ny; j++)
			{
				for(k = 1; k < Nz; k++)
				{
					if(Point[i][j][k].potentialFix)
						continue;
					//SOR
					residual = (Point[i+1][j][k].potential+Point[i-1][j][k].potential+Point[i][j+1][k].potential+Point[i][j-1][k].potential+Point[i][j][k+1].potential+Point[i][j][k-1].potential - 6*Point[i][j][k].potential);
					Point[i][j][k].r = rho * (pow(E,-electron*Point[i][j][k].potential/(kb*T)) - pow(E,electron*Point[i][j][k].potential/(kb*T)));
					residual_charge = - omega/6.0*delta*delta * Point[i][j][k].r /dieleConst;
					if(residual_charge > 0.028)
					{
						residual_charge = 0.028;
					}
					else if(residual_charge < -0.028)
					{
						residual_charge = -0.028;
					}
					tmp = Point[i][j][k].potential;
					Point[i][j][k].potential = Point[i][j][k].potential + omega / 6.0 * residual - residual_charge;
					if(abs(Point[i][j][k].potential-tmp) > err)    //an estimate of the error
					{
						err = abs(Point[i][j][k].potential-tmp);
					}
				}
			}
		}

		LoopPorgressed++;
		if(err > cseg[td].err)
			cseg[td].err = err;
		if(LoopPorgressed % 100 == 0)
			printf("Td %d  LP %d ; error %e  %ld\n", td, LoopPorgressed, cseg[td].err, time(0)-time0);
	}
	cout << "L: " << LoopPorgressed << endl;
}

void commander()    //to determin whether the program can stop, and chane the omega used in SOR method
{
	printf("thread Commander begins\n");

	int i, CountAccept = 0;
	double MaxError = 0;
//	omega = 2/(1+Pi/Nx);
	double BoundE = 0;
	bool IsBoundMod = 1;
	int WAIT = 20;
	while(1)
	{
		sleep(WAIT);
		MaxError = 0;

		for(i = 0; i < ThreadNum; i++)
		{
			if(cseg[i].err > MaxError)
				MaxError = cseg[i].err;
		}
		
		m.lock();
		for(i = 0; i < ThreadNum; i++)
			cseg[i].err = 0;
		m.unlock();
//		if(c%8==0)
		printf("err: %e %e\n",MaxError, omega);

		if(MaxError < MAXERR)
		{
//			BoundE = ModifyBound(Point);
			BoundE = 0;
			if(BoundE < BOUNDERR)
			{
				CountAccept++;
				printf("CountAccept %d; BE %e\n", CountAccept, BoundE);
			}
			else
			{
				printf("BE %e\n", BoundE);
			}
			if(CountAccept == 1)
			{
				MAXERR = MAXERR2;    //smaller error
				WAIT = 20;
			}
			if(CountAccept > 5)
			{
				omega = 1;
				break;
			}
			continue;
		}
		CountAccept = 0;
	}
	printf("WAIT ANOTHER 60 SECONDS TO TERMINATE\n");
	sleep(60);
	run = 0;
}

void OutputToFile()
{
	cout << "Output To File" << endl;

	int i, j, k;
	FILE *filedata, *filer;
	filedata = fopen("FData.dat","wb");
	for(k = 0; k < Nz; k++)
	{
		for(i = 0; i < Nx; i++)
		{
			for(j = 0; j < Ny; j++)
			{
				fwrite(&(Point[i][j][k].potential), sizeof(double), 1, filedata);
			}
		}
	}
	filer = fopen("FDr.dat","wb");
	for(k = 0; k < Nz; k++)
	{
		for(i = 0; i < Nx; i++)
		{
			for(j = 0; j < Ny; j++)
			{
				fwrite(&(Point[i][j][k].r), sizeof(double), 1, filer);
			}
		}
	}
	ofstream fileLog("fLog.txt");
	fileLog << "size: " << Nx << " " << Ny << " " << Nz << endl;
}

void AllocateThread()
{
	int i;
	thread *thd[ThreadNum+1];
	int tdcount[100];
	double curLoc = 0, Gap = (Nx-2)/ThreadNum;
	for(i = 0; i < ThreadNum; i++)
	{
		cseg[i].NxBeg = (int)curLoc+1;
		curLoc+=Gap;
		cseg[i].NxEnd = (int)curLoc;
		tdcount[i] = i;
		thd[i] = new thread(calculate,tdcount[i]);
	}
	cseg[ThreadNum-1].NxEnd = Nx-1;
	thd[ThreadNum] = new thread(commander);
	
	//write down the allocation of each thread
	ofstream fallolog("fallolog.txt");
	for(i = 0; i < ThreadNum; i++)
	{
		fallolog << i << " " << cseg[i].NxBeg << " " << cseg[i].NxEnd << endl;
	}
	
	for(i = 0; i <= ThreadNum; i++)
	{
		thd[i]->join();
	}
}

int main()
{
	CreateSpace();
	BoundaryCondition(Point);
	time0 = time(0);
	AllocateThread();
	cout << "t: " << time(0) - time0 << endl;
	printf("aaa\n");
	OutputToFile();
	return 0;
}
