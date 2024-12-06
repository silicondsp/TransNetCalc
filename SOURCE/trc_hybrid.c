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

/* cn_hybrid.c */
/********************************************************************/
/* This program creates an echo path impulse response, given an
 * input file of complex line impedance data.  The analog model of
 * the hybrid balance network is taken from:
 * 	Falconer, "Timing Jitter Effects...Part II,"
 * 	IEEE Trans. on Comm. Vol COM-33, No.8, Aug 1985.
 *          _________
 *   _______| R1=143 |_________________________
 *      |   |________|                    |
 *      |   ________     ____________     |
 *      |___| R=643 |____| C=.055mmF |____|
 *          |_______|    |___________|
 *
 * The echo transfer function is defined:
 *	         	Z(line) - Z(balance)
 *     		E(jw) = --------------------
 *	         	Z(line) + Z(balance)
 *
 * The impedance input file is expected in CAPNET form:
 * 	real/imaginary impedance pair for each frequency point 0 - fs/2
 * The impulse response output file is:
 * 	real data, at spacing 1/fs
 */
/******************************************************************/
#include <math.h>
#include <stdio.h>
#define R1 143
#define R 643
#define C .055e-6
#define PI 3.141592653589793
#define PI2 9.869604404

int Trn_Hybrid(char *zinFileName, float fs, int npts, float *echoPathImp_P)

// zinFileName; 	 name of file containing input impedance
// npts;		 log2(number of freq. pts to evaluate)
// fs;		 sampling freq., Hz
// *echoPathImp_P;	 echo path freq resp., real/imag alternate

{

  int i, j;
  float *zlr_P;  /* line real imedance array */
  float *zli_P;  /* line imaginary imedance array */
  float *zbr_P;  /* balance real imedance array */
  float *zbi_P;  /* balance imaginary imedance array */
  float a, b, c; /* constant holders */
  float denom;   /* denominator */
  float omega;   /* angular freq. */
  float wstep;   /* angular freq. increment */
  float magsql;  /* magnitude squared, line impedance */
  float magsqb;  /* magnitude squared, balance impedance */
  FILE *fp;      /* file pointer for file containing input impedance */

  if ((fp = fopen(zinFileName, "r")) == NULL) {
    fprintf(stderr, "Hybrid: can't open file %s\n", zinFileName);
    return (1);
  }

  npts = 1 << npts;

  fprintf(stderr, "Echo Impulse Response:\n");
  fprintf(stderr, "\tsample spacing = %e (sec)\n\t          span = %e (sec)\n",
          1.0 / fs, npts / fs);

  /* Allocate arrays */
  zbr_P = (float *)calloc(npts / 2, sizeof(float));
  zbi_P = (float *)calloc(npts / 2, sizeof(float));
  zlr_P = (float *)calloc(npts / 2, sizeof(float));
  zli_P = (float *)calloc(npts / 2, sizeof(float));
  for (i = 0; i < npts / 2; i++) {
    if ((fscanf(fp, "%f%f", &zlr_P[i], &zli_P[i])) != 2) {
      fprintf(stderr, "Hybrid: impedance file too small!\n");
      return (1);
    }
  }

  /*
   * Network constants
   */
  a = C * C * R * R1 * (R + R1);
  b = C * C * (R + R1) * (R + R1);
  c = C * R1 * R1;

  wstep = fs / npts * 2 * PI; /* radian frequency increment */
  for (i = 0; i < npts / 2; i++) {
    omega = i * wstep;
    denom = 1. + omega * omega * b;
    zbr_P[i] = (R1 + omega * omega * a) / denom;
    zbi_P[i] = -omega * c / denom;
  }
  /*
   * Compute echo freq. response
   */
  for (i = 0; i < npts / 2; i++) {
    magsql = zlr_P[i] * zlr_P[i] + zli_P[i] * zli_P[i];
    magsqb = zbr_P[i] * zbr_P[i] + zbi_P[i] * zbi_P[i];
    denom = magsql + magsqb + 2 * (zlr_P[i] * zbr_P[i] + zli_P[i] * zbi_P[i]);
    echoPathImp_P[2 * i] = (magsql - magsqb) / denom;
    /* I put minus sign in imaginary part of echoPathImp_P in order to do
    inverse FT, rfti() */
    echoPathImp_P[2 * i + 1] =
        -2 * (zli_P[i] * zbr_P[i] - zlr_P[i] * zbi_P[i]) / denom;
  }
  /*
   * Compute echo impulse response with inverse fft
   */
  echoPathImp_P[1] = 0;
  rfti(echoPathImp_P, npts);

  free(zbr_P);
  free(zbi_P);
  free(zlr_P);
  free(zli_P);
  return (0);
}
