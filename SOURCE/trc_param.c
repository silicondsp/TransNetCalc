
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
 *  module name: tr_param.c
 *      functions:
 *		void Trc_Propconst()
 *		void Trc_CirCond()
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
 *    transmission line analysis modules for
 *    parallel and coaxial cables.
 *
 */

#include "tr.h"

/*****************************************/
/* Calculate propogation constant        */
/* Given r,l,c, and g parameters         */
/* 					 */
/*****************************************/

void Trc_Propconst(float freq, float r, float l, float c, float g, float wL,
                   double *alpha_P, double *beta_P) {

  float theta, phi;
  float gamma;
  float w;
  float wl, wc, t1, t2;

  w = 2.0 * PI * freq;
  wl = w * l + wL;
  wc = w * c;
  t1 = sqrt(r * r + wl * wl);
  t2 = sqrt(g * g + wc * wc);
  gamma = t1 * t2;
  gamma = sqrt(gamma);
  t1 = atan(wl / r);
  if (g == 0)
    t2 = PI / 2;
  else
    t2 = atan(wc / g);
  phi = 0.5 * (t1 + t2);
  *alpha_P = gamma * cos(phi);
  *beta_P = gamma * sin(phi);
}

/******************************************************/
/* Calculate the intrinsic impedance of a circular    */
/* conductor of radius ri at frequency freq.          */
/* Also computes skin depth delta.                    */
/******************************************************/

void Trc_CirCond(float freq, float ri, float *delta_P, float *r_P, float *wL_P)

{

  float x, x1, y, y1, a, z;
  float tmp, u, u1, u2, u3, u4;
  float gbei, gber, gberp, gbeip;
  double rd, wLd, zd;

  /* compute skin depth */
  if (freq != 0.0) {
    tmp = freq * PI * MU * SIGMA;
    tmp = 1.0 / tmp;
    tmp = pow(tmp, 0.5);
    *delta_P = tmp;
    u = ri * 1.414213562 / (*delta_P);
    a = pow(MU * freq / 2.0 / PI / SIGMA, 0.5) / ri;
    if (u < 8.0) {
      x = TrBerp(u);
      x1 = TrBeip(u);
      y = TrBer(u);
      y1 = TrBei(u);
      z = x * x + x1 * x1;
      rd = y * x1 - y1 * x;
      *r_P = rd * a / z;
      wLd = y * x + y1 * x1;
      *wL_P = wLd * a / z;
    } else if (u < 60) {
      /*****************************************/
      /*    u > 8                              */
      /* The following approximations are from */
      /* Abramowitz and Stegun, "Handbook of   */
      /* Mathematical Functions,"              */
      /* Dover Publications, New York, 1969    */
      /* Page 384-85                           */
      /* The routine besselg() computes        */
      /* ber,bei,berp,beip simultaneously      */
      /* and returns them as parameters.       */
      /*****************************************/
      TrBesselg(u, &gbei, &gber, &gberp, &gbeip);
      zd = gberp * gberp + gbeip * gbeip;
      rd = gber * gbeip - gbei * gberp;
      *r_P = a * rd / zd;
      wLd = gber * gberp + gbei * gbeip;
      *wL_P = wLd * a / zd;
    } else {
      /*****************************************/
      /*    u large                              */
      /* The following approximations are from */
      /* Abramowitz and Stegun, "Handbook of   */
      /* Mathematical Functions,"              */
      /* Dover Publications, New York, 1969    */
      /* Page 383.                             */
      /*****************************************/
      float u2, u3, u4;
      u2 = u * u;
      u3 = u2 * u;
      u4 = u3 * u;
      rd = (0.707107 + 0.125 / u + 0.0994 / u2 + 7.61719e-2 / u3 +
            6.47376e-3 / u4);
      z = (1.0 - 0.530330086 / u + 0.140625 / u2 - 0.2071602 / u3 -
           0.302124023 / u4);
      *r_P = a * rd / z;
      wLd = (0.707107 - 0.375 / u - 0.165728 / u2 - 8.78906e-2 / u3 +
             2.71898e-2 / u4);
      *wL_P = wLd * a / z;
    }
  } else { /**** freq = 0.0   ********/
    *r_P = 1.0 / (SIGMA * PI * ri * ri);
    *wL_P = MU / 8.0 / PI;
  }
}
