
/*  Transmission Line Topology Calculation and Analysis (TransTopCalc TTC)
 * Library Copyright (C) 1985-2017  Sasan H Ardalan
 *
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 2.1 of the License, or (at your option) any later version.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with this library; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    http://www.SiliconDsp.com
 *    Silicon DSP  Corporation
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
 *    Mathematical routines for transmission line analysis
 *
 */

#include "tr.h"

/******************************************************/
/* The following functions are valid for -8<x<8       */
/* Reference:                                         */
/* Milton Abramowitz and Irene Stegun, "Handbook of   */
/* Mathematical Functions," Dover Publications,       */
/* New York, 1965                                     */
/* pages 384-385                                      */
/******************************************************/
/* Written by Sasan H. Ardalan                        */
/* Center for Communcations and Signal Processing     */
/* Dept. of Electrical and Computer Enginmeering      */
/* NC State University                                */
/* Raleigh, NC 27695-7914                             */
/******************************************************/

float TrBei(float x) {

  float xx, xx2, xx4, xx6, xx10, xx14, xx18, xx22, xx26;
  float sum;
  sum = 0.0;
  xx = x / 8;
  xx2 = xx * xx;
  xx4 = xx2 * xx2;
  xx6 = xx4 * xx2;
  xx10 = xx6 * xx4;
  xx14 = xx10 * xx4;
  xx18 = xx14 * xx4;
  xx22 = xx18 * xx4;
  xx26 = xx22 * xx4;
  sum = sum + 16 * xx2 - 113.77777774 * xx6 + 72.81777742 * xx10 -
        10.56765 * xx14;
  sum = sum + 0.52185615 * xx18 - 0.01103667 * xx22 + 0.00011346 * xx26;
  return (sum);
}

float TrBer(float x) {

  float xx4, xx8, xx12, xx16, xx20, xx24, xx28;
  float sum;

  xx4 = x / 8;
  xx4 = xx4 * xx4 * xx4 * xx4;
  xx8 = xx4 * xx4;
  xx12 = xx8 * xx4;
  xx16 = xx12 * xx4;
  xx20 = xx16 * xx4;
  xx24 = xx20 * xx4;
  xx28 = xx24 * xx4;
  sum = 1.0 - 64.0 * xx4 + 113.77777774 * xx8 - 32.36345652 * xx12 +
        2.64191397 * xx16;
  sum = sum - 0.08349609 * xx20 + 0.00122552 * xx24 - 0.00000901 * xx28;
  return (sum);
}

float TrBerp(float x)

{

  float xx, xx2, xx4, xx6, xx10, xx14, xx18, xx22, xx26;
  float sum;
  sum = 0.0;
  xx = x / 8;
  xx2 = xx * xx;
  xx4 = xx2 * xx2;
  xx6 = xx4 * xx2;
  xx10 = xx6 * xx4;
  xx14 = xx10 * xx4;
  xx18 = xx14 * xx4;
  xx22 = xx18 * xx4;
  xx26 = xx22 * xx4;
  sum = sum - 4.0 * xx2 + 14.22222222 * xx6 - 6.06814810 * xx10 +
        0.66047849 * xx14;
  sum = sum - 0.02609253 * xx18 + 0.00045957 * xx22 - 0.00000394 * xx26;
  sum = sum * x;
  return (sum);
}

float TrBeip(float x)

{

  float xx4, xx8, xx12, xx16, xx20, xx24, xx28;
  float sum;

  xx4 = x / 8;
  xx4 = xx4 * xx4 * xx4 * xx4;
  xx8 = xx4 * xx4;
  xx12 = xx8 * xx4;
  xx16 = xx12 * xx4;
  xx20 = xx16 * xx4;
  xx24 = xx20 * xx4;
  sum = 0.5 - 10.66666666 * xx4 + 11.37777772 * xx8 - 2.31167514 * xx12 +
        0.14677204 * xx16;
  sum = sum - 0.00379386 * xx20 + 0.00004609 * xx24;
  sum = sum * x;
  return (sum);
}

/*********************************************************/
/* The following routine computes the real and imaginary */
/*  part of TrTheta() and phi() per page 385 of the        */
/* Handbook of Mathematical Functions, by Abramowitz and */
/* Stegun, Dover Publications, New York.                 */
/*********************************************************/
void TrTheta(float x, float *thetar_P, float *thetai_P, float *phir_P,
             float *phii_P)

{

  float xx, xx2, xx3, xx4, xx5, xx6;
  xx = 8.0 / x;
  xx2 = xx * xx;
  xx3 = xx2 * xx;
  xx4 = xx3 * xx;
  xx5 = xx4 * xx;
  xx6 = xx5 * xx;
  *thetar_P = 0.0110486 * xx - 0.0000906 * xx3 - 0.0000252 * xx4 -
              0.0000034 * xx5 + 0.0000006 * xx6;
  *thetai_P = -0.3926991 - 0.0110485 * xx - 0.0009765 * xx2 - 0.0000901 * xx3 +
              0.0000051 * xx5 + 0.0000019 * xx6;
  *phir_P = 0.7071068 - 0.0625001 * xx - 0.0013813 * xx2 + 0.0000005 * xx3 +
            0.0000346 * xx4 + 0.0000117 * xx5 + 0.0000016 * xx6;
  *phii_P = 0.7071068 - 0.0000001 * xx + 0.0013811 * xx2 + 0.0002452 * xx3 +
            0.0000338 * xx4 - 0.0000024 * xx5 - 0.0000032 * xx6;
}

/*********************************************************/
/* Compute TrBer(), TrBerp(), TrBei(), and TrBeip()      */
/* The results passed by parameter list for simultaneous */
/* computation for efficiency.                           */
/**********************************************/
/*  The following valid for 8<x< infinity     */
/*  overflow at x> 100 with single and double */
/*  unless G format floating point is used.   */
/**********************************************/

void TrBesselg(float x, float *bei_P, float *ber_P, float *berp_P,
               float *beip_P)

{

  float alpha, beta;
  float alpham, betam;
  float tmp1, tmp2;
  float xm;
  float stpx;
  float thetar, thetai, phir, phii;
  float thetarm, thetaim, phirm, phiim;
  float alphar, alphai;
  float alpharm, alphaim;

  xm = -x;
  stpx = pow(2.0 * PI * x, 0.5);

  TrTheta(x, &thetar, &thetai, &phir, &phii);
  TrTheta(xm, &thetarm, &thetaim, &phirm, &phiim);
  alpha = pow(2.718281828, x * 0.7071068 + thetar);
  alpham = pow(2.718281828, xm * 0.7071068 + thetarm);
  beta = x * 0.7071068 + thetai;
  betam = xm * 0.7071068 + thetaim;

  alphar = alpha * cos(beta);
  alphaim = alpham * sin(betam);
  *ber_P = alphar - alphaim;
  *ber_P = *ber_P / stpx;

  alphai = alpha * sin(beta);
  alpharm = alpham * cos(betam);
  *bei_P = alphai + alpharm;
  *bei_P = *bei_P / stpx;

  *berp_P = alphar * phir - alphai * phii + alphaim * phirm + alpharm * phiim;
  *berp_P = *berp_P / stpx;

  *beip_P = alphai * phir + alphar * phii - alpharm * phirm + alphaim * phiim;
  *beip_P = *beip_P / stpx;
}
/*********************************************************/
/* Compute mber(), mberp(), mbei(), and mbeip()          */
/* The results passed by parameter list for simultaneous */
/* computation for efficiency.                           */
/* NOTE: These are modified functions. In the limit as   */
/* x becomes large all functions will have a common      */
/* factor exp{x*0.70710678+thetar(x)}/sqrt(2PI*x)        */
/* The modified functions do not contain this factor as  */
/* it is cancelled out when ratios of the functions are  */
/* used.                                                 */
/*********************************************************/
/**********************************************/
/*  The following valid for 50<x< infinity    */
/**********************************************/

void TrMbessel(float x, float *mbei, float *mber, float *mberp, float *mbeip)

{

  float alpha, beta;
  float alpham, betam;
  float tmp1, tmp2;
  float xm;
  float stpx;
  float thetar, thetai, phir, phii;
  float thetarm, thetaim, phirm, phiim;
  float alphar, alphai;
  float alpharm, alphaim;
  float cos_beta, sin_beta;

  xm = -x;

  TrTheta(x, &thetar, &thetai, &phir, &phii);
  TrTheta(xm, &thetarm, &thetaim, &phirm, &phiim);

  beta = x * 0.7071068 + thetai;
  cos_beta = cos(beta);
  sin_beta = sin(beta);
  *mber = cos_beta;

  *mbei = sin_beta;

  *mberp = cos_beta * phir - sin_beta * phii;

  *mbeip = sin_beta * phir + cos_beta * phii;
}
