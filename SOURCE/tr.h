


/*  Transmission Line Topology Calculation and Analysis (TranTopCalc) Library 
    Copyright (C) 1985-2002  Sasan H Ardalan

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    http://www.ccdsp.org
    Raleigh, North Carolina 
*/

/*
 *  Transmission Line Modules
 *  Version  0.0.0 - 
 *
 *
 *  header file  name: tr.h
 *
 *  author: Sasan H. Ardalan
 *  revised by:
 *
 *  revision history:
 *     version	--date-- -by-	---reason---
 *	1.0.0	04/27/88   sha	Initial release
 *
 *  compiled on: VAXstation II/GPX - o/s: microVMS 4.5
 *
 *  description:
 *     header file for transmission line analysis modules 
 *
 */



#include <math.h>
#include <stdio.h>


#define PI 3.1415926
#define MU 4*PI*1.0e-7
#define EPS 1.0e-9/36/PI
#define RHO 1.72e-6 /* ohms-cm copper */
#define SIGMA 58e6  /* mho/m copper */


float TrBei(float x);
float TrBer(float x);
float TrBerp(float x);
float TrBeip(float x);
void TrTheta(float x,float *thetar_P,float *thetai_P,float *phir_P,float *phii_P);
void TrBesselg(float x,float *bei_P,float *ber_P,float *berp_P,float *beip_P);
void TrMbessel(float x,float *mbei,float *mber,float *mberp,float *beip);
void Trc_Propconst(float freq,float r,float l,float c,float g,float wL,double *alpha_P,double *beta_P);


