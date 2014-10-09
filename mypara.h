/*
 * guojk
 * ~35G
 */

#ifndef MYPARA_H_
#define MYPARA_H_

#include <cmath>

#define Nx 800
#define Ny 800
#define Nz 800
#define InNx 100
#define InNy 100
#define InNz 100
#define dieleConst 81.5*8.854e-12  // 8.85*10^(-12)*81.5
#define electron 1.602189e-19
#define E 2.718281828459
#define Pi 3.14159265

#define delta 0.5e-9    //the length of each cubic (unit:m)
#define ThreadNum 30    //depends on the computer
double MAXERR = 1e-4;    //if the gap between two calculation, is smaller than MaxErr, I do think that the numercial result is convincing
double BOUNDERR = 1e-4;  //If the error of space boundary is smaller than it, the result is converged.
double MAXERR2 = 8e-5;

const double kb = 1.381e-23;
const double T = 310;    //37C
const double rho = 0.154 * 6.02e23 *10e3 * electron;    //saline: 0.154 mol/L

double omega = 1;
int CtNx = Nx/2;
int CtNy = Ny/2;
int CtNz = Nz/2;
int CtInNx = InNx/2;
int CtInNy = InNy/2;
int CtInNz = InNz/2;
int CntBoundModify = 0;

double EleDist[InNx][InNy][InNz] = {0};
double Q = 0;
double Dip[3] = {0};
double Qr[3][3] = {0};
double delta2 = pow(delta,2.0);

#endif
