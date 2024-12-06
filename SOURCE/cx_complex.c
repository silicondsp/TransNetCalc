
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
 * cx_complex.c
 *
 * A library of functions for complex math.
 *
 * Author:
 * Joseph Nathan Hall
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

#include <math.h>
#include <stdio.h>

#include "cx.h"

cx_t Cx_Complex(float a, float b)

{

  cx_t x;

  x.re = a;
  x.im = b;

  return (x);
}

/*
 * Calculate x + y, x and y complex
 *
 * x = x1 + jx2
 * y = y1 + jy2
 * z = (x1 + y1) + j(x2 + y2)
 */

cx_t Cx_Add(cx_t x, cx_t y)

{

  cx_t z;

  z.re = x.re + y.re;
  z.im = x.im + y.im;
  return (z);
}

/*
 * Calculate x - y, x and y complex
 *
 * x = x1 + jx2
 * y = y1 + jy2
 * z = (x1 - y1) + j(x2 - y2)
 */

cx_t Cx_Sub(cx_t x, cx_t y)

{
  cx_t z;

  z.re = x.re - y.re;
  z.im = x.im - y.im;
  return (z);
}

/*
 * Calculate x * y, x and y complex
 *
 * x = x1 + jx2
 * y = y1 + jy2
 * z = (x1 * y1 - x2 * y2) + j(x1 * y2 + x2 * y1)
 */

cx_t Cx_Mult(cx_t x, cx_t y)

{

  cx_t z;

  z.re = x.re * y.re - x.im * y.im;
  z.im = x.re * y.im + x.im * y.re;
  return (z);
}

/*
 * Calculate x / y, x and y complex
 *
 * x = x1 + jx2
 * y = y1 + jy2
 * z = [(x1 * y1 + x2 * y2)/(y1 * y1 + y2 * y2)] +
 *    j[(x2 * y1 - x1 * y2)/(y1 * y1 + y2 * y2)]
 */

cx_t Cx_Div(cx_t x, cx_t y)

{

  cx_t z;
  register double tmp;

  tmp = y.re * y.re + y.im * y.im;
  if (tmp == 0.0) {
    fprintf(stderr, "error trc, divide by 0.0 in cdiv\n");
    z.re = 1.0e10;
    z.im = 1.0e10;
  } else {
    z.re = (x.re * y.re + x.im * y.im) / tmp;
    z.im = (x.im * y.re - x.re * y.im) / tmp;
  }
  return (z);
}

/*
 * Calculate e raised to the complex power x
 *
 * x = x1 + jx2
 * z = [exp(x1) * cos(x2)] + j[exp(x1) * sin(x2)]
 */

cx_t Cx_Exp(cx_t x)

{

  cx_t z;

  if (x.re < -200) {
    z.re = 0.0;
    z.im = 0.0;
    return (z);
  }
  z.re = exp(x.re) * cos(x.im);
  z.im = exp(x.re) * sin(x.im);
  return (z);
}

/*
 * tanh(x)
 *	  exp(x) - exp(-x)
 *	= ----------------
 *	  exp(x) + exp(-x)
 */

cx_t Cx_Tanh(cx_t x)

{

  cx_t nx;

  nx = x;
  nx.re = -nx.re;
  nx.im = -nx.im;

  return (Cx_Div(Cx_Sub(Cx_Exp(x), Cx_Exp(nx)), Cx_Add(Cx_Exp(x), Cx_Exp(nx))));
}

/*
 * Calculate a square root
 *
 * convert to polar, take the square root of the magnitude and halve theta,
 * then convert back to rectangular
 */

cx_t Cx_Sqrt(cx_t x)

{

  register double m, theta;
  static double pi = 3.14159265358979323;

  if (x.im == 0)
    if (x.re >= 0)
      theta = 0;
    else
      theta = pi;
  else if (x.re == 0)
    if (x.im >= 0)
      theta = pi / 2;
    else
      theta = 3 * pi / 2;
  else if (x.re > 0)
    theta = atan(x.im / x.re);
  else
    theta = atan(x.im / x.re) + pi;

  m = hypot(x.re, x.im);

  theta = theta / 2;
  m = sqrt(m);

  x.re = m * cos(theta);
  x.im = m * sin(theta);

  return (x);
}
