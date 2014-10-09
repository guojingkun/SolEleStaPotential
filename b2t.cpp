#include <iostream>
#include <cstdlib>
#include <fstream>
#include <iomanip>
using namespace std;

const int Nx = 800, Ny = 800, Nz = 800;

int main()
{
	int i, j, k;
	int GetZHi=Nz/2+1, GetZLo=Nz/2;
	ofstream* fileOut = NULL;
	FILE *fileIn;
	fileIn=fopen("FData.dat","rb");
	
	char filename[30] = "a_000";
	double dat;
	for(k = 0; k < GetZLo; k++)
	{
		fseek(fileIn, 1L*sizeof(double)*Nx*Ny,SEEK_CUR);
	}
	
	for(k = GetZLo; k <= GetZHi; k++)
	{
		filename[4] = k % 10 + '0';
		filename[3] = (k / 10) % 10 + '0';
		filename[2] = (k / 100) % 10 + '0';
		fileOut = new ofstream(filename);
		for(i = 0; i < Nx; i++)
		{
			for(j = 0; j < Ny; j++)
			{
				fread(&dat,sizeof(double), 1, fileIn);
				(*fileOut) << setprecision(4) << fixed << dat << " ";
//				fseek(fileIn, 1L*sizeof(double),SEEK_CUR);
			}
			(*fileOut) << endl;
		}
		fileOut->close();
	}
	fseek(fileIn,0,0);
	fclose(fileIn);
	return 0;
}
