

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

#include <math.h>
#include <stdio.h>

#include "cx.h"
#include "random.h"
#include "trantopcalc.h"
#include "trantopcalc_tcl.h"
#include <kiss_fft.h>
#include <tcl.h>

#define RAND_LARGE 0x7fffffff
#define DEC

doubleVector_t *Dsp_AllocateDoubleVector(int n) {
  doubleVector_t *vector;
  double *vec;

  vec = (double *)calloc(n, sizeof(double));
  vector = (doubleVector_t *)calloc(1, sizeof(doubleVector_t));
  if (!vec || !vector) {
    fprintf(stderr, "Could not allocate Double Vector \n");
    return (NULL);
  }
  vector->length = n;
  vector->vector_P = vec;
  vector->type = TRC_TCL_TYPE_REAL_VECTOR;

  return (vector);
}

doubleVector_t *Dsp_AllocateVector(int length) {
  int i, j;
  doubleVector_t *theVec_P;
  double *vec_P;

  theVec_P = (doubleVector_t *)calloc(1, sizeof(doubleVector_t));
  vec_P = (double *)calloc(length, sizeof(double));
  if (vec_P == NULL || theVec_P == NULL) {
    fprintf(stderr, "Allocate complex vector could not allocate space \n");
    return (NULL);
  }

  theVec_P->length = length;
  theVec_P->transpose = 0;
  theVec_P->vector_P = vec_P;
  theVec_P->type = TRC_TCL_TYPE_REAL_VECTOR;

  return (theVec_P);
}

cxVector_t *Dsp_AllocateCxVector(int length)

{
  int i, j;
  cxVector_t *theVec_P;
  cx_t *vec_P;

  theVec_P = (cxVector_t *)calloc(1, sizeof(cxVector_t));
  vec_P = (cx_t *)calloc(length, sizeof(cx_t));
  if (vec_P == NULL || theVec_P == NULL) {
    fprintf(stderr, "Allocate complex vector could not allocate space \n");
    return (NULL);
  }

  theVec_P->length = length;
  theVec_P->transpose = 0;
  theVec_P->vector_P = vec_P;
  theVec_P->type = TRC_TCL_TYPE_COMPLEX_VECTOR;

  return (theVec_P);
}

int Dsp_GenGaussVector(doubleVector_t *vec_P, float std, float mean,
                       long seedIn) {

  int i, j, ok, n;
  int numin;
  int count = 0;
  int trouble;
  double s, t, u, v, w, x;
  float y1, y2;
  long seed;
  int pts;

  // if(points % 2)
  //	pts=points-1;
  // else
  //	pts=points;

  seed = seedIn;
#ifdef RANDOM_64_BIT
  init_genrand64((unsigned long long)seed);
#endif

#ifdef RANDOM_32_BIT
  // init_genrand((unsigned  long) seed);

  unsigned long init[4] = {0x123, 0x234, 0x345, 0x456}, length = 4;
  init_by_array(init, length);
#endif

  for (i = 0; i < vec_P->length; i++) {

    /****************************************************************/
    /* 		gauss						*/
    /* code written by Prayson Pate					*/
    /* This code generates two random variables that are normally 	*/
    /* distributed with mean 0 and variance 1 i.e N(0,1).	 	*/
    /* The polar method is used to generate normally distributed    */
    /* samples from a sequence that is uniform on (-1,1).  The      */
    /* resulting distribution is described exactly by N(0,1).       */
    /* This method is based	on the inverse distribution function.   */
    /****************************************************************/
    trouble = 0;
    do {
      if (++trouble > 100) {
        fprintf(stderr, "gauss: problem with random number\
			 generator\n");
        return (2);
      }
      /*
       * get two random numbers in the interval (-1,1)
       */
#ifdef RANDOM_64_BIT
      s = genrand64_real1();
#endif
#ifdef RANDOM_32_BIT
      s = genrand_real2();
#endif
      // fprintf(stderr,"GAUSS: s=%f\n", s);

      u = -1.0 + 2.0 * (s);

      //   t = random();
      //   v = -1.0 + 2.0*(t/max);

#ifdef RANDOM_64_BIT
      t = genrand64_real1();
#endif
#ifdef RANDOM_32_BIT
      t = genrand_real2();
#endif

      v = -1.0 + 2.0 * t;
      w = u * u + v * v;
      /*
       * is point (u,v) in the unit circle?
       */
    } while (w >= 1.0 || w == 0.0);

    x = sqrt((-2.0 * log(w)) / w);
    /*
     * find two independent values of y
     */
    y1 = std * u * x;
    // y2 = std * v * x;

    vec_P->vector_P[i] = y1 + mean;

    // y_P[i]=y2+mean;

    /****************** End of Gauss Code ****************************/

  } /* end for i*/
  return (0);
}

int Dsp_GenGaussComplexVector(cxVector_t *vec_P, float std, float mean,
                              long seedIn) {

  int i, j, ok, n;
  int numin;
  int count = 0;
  int trouble;
  double s, t, u, v, w, x;
  float y1, y2;
  long seed;
  int pts;

  // if(points % 2)
  //	pts=points-1;
  // else
  //	pts=points;

  seed = seedIn;
#ifdef RANDOM_64_BIT
  init_genrand64((unsigned long long)seed);
#endif

#ifdef RANDOM_32_BIT
  // init_genrand((unsigned  long) seed);

  unsigned long init[4] = {0x123, 0x234, 0x345, 0x456}, length = 4;
  init_by_array(init, length);
#endif

  for (i = 0; i < vec_P->length; i++) {

    /****************************************************************/
    /* 		gauss						*/
    /* code written by Prayson Pate					*/
    /* This code generates two random variables that are normally 	*/
    /* distributed with mean 0 and variance 1 i.e N(0,1).	 	*/
    /* The polar method is used to generate normally distributed    */
    /* samples from a sequence that is uniform on (-1,1).  The      */
    /* resulting distribution is described exactly by N(0,1).       */
    /* This method is based	on the inverse distribution function.   */
    /****************************************************************/
    trouble = 0;
    do {
      if (++trouble > 100) {
        fprintf(stderr, "gauss: problem with random number\
			 generator\n");
        return (2);
      }
      /*
       * get two random numbers in the interval (-1,1)
       */
#ifdef RANDOM_64_BIT
      s = genrand64_real1();
#endif
#ifdef RANDOM_32_BIT
      s = genrand_real2();
#endif
      // fprintf(stderr,"GAUSS: s=%f\n", s);

      u = -1.0 + 2.0 * (s);

      //   t = random();
      //   v = -1.0 + 2.0*(t/max);

#ifdef RANDOM_64_BIT
      t = genrand64_real1();
#endif
#ifdef RANDOM_32_BIT
      t = genrand_real2();
#endif
      v = -1.0 + 2.0 * t;
      w = u * u + v * v;
      /*
       * is point (u,v) in the unit circle?
       */
    } while (w >= 1.0 || w == 0.0);

    x = sqrt((-2.0 * log(w)) / w);
    /*
     * find two independent values of y
     */
    y1 = std * u * x;
    y2 = std * v * x;

    vec_P->vector_P[i].re = y1 + mean;
    vec_P->vector_P[i].im = y2 + mean;
    // y_P[i]=y2+mean;

    /****************** End of Gauss Code ****************************/

  } /* end for i*/
  return (0);
}

int Dsp_DotRealReal(doubleVector_t *x_P, doubleVector_t *y_P, double *z) {
  int n;
  int i;
  double sum;

  n = x_P->length;
  sum = 0;
  for (i = 0; i < n; i++) {
    sum = sum + x_P->vector_P[i] * y_P->vector_P[i];
  }

  *z = sum;
}

/*
       multiplication
       z.re = x.re * y.re - x.im * y.im;
        z.im = x.re * y.im + x.im * y.re;
                below we take conjugate of x
*/
int Dsp_DotComplexComplex(cxVector_t *x_P, cxVector_t *y_P, cxScalar_t *z_P) {
  int n;
  int i;
  cx_t sum;

  n = x_P->length;
  sum.re = 0;
  sum.im = 0;

  for (i = 0; i < n; i++) {
    sum.re = sum.re + x_P->vector_P[i].re * y_P->vector_P[i].re +
             x_P->vector_P[i].im * y_P->vector_P[i].im;
    sum.im = sum.im + x_P->vector_P[i].re * y_P->vector_P[i].im -
             x_P->vector_P[i].im * y_P->vector_P[i].re;
  }

  z_P->value.re = sum.re;
  z_P->value.im = sum.im;
}

/*
 * vector   FFT
 */
cxVector_t *Dsp_VectorFFT(doubleVector_t *vec_P) {
  float *x_A, *y_A;
  int i, j;
  float tmp;
  float xx, yy;
  float temp1, temp2;
  cxVector_t *fft_P;
  kiss_fft_cfg cfg;

  kiss_fft_cpx **mat_PP;
  kiss_fft_cpx *cxoutbuff;
  kiss_fft_cpx *capFFT_P;
  kiss_fft_cpx temp;
  int k;
  float tempReal;
  float tempImag;
  int fftexp;
  int fftl;
  int fftwidth;
  float norm;
  float real, imag, mag;

  fftwidth = vec_P->length;
  /*
   * compute the power of 2 number of fft points
   */
  fftexp = (int)(log((float)fftwidth) / log(2.0) + 0.5);
  fftl = 1 << fftexp;
  if (fftl > fftwidth) {
    fftl = fftl / 2;
    fftexp -= 1;
  }
  norm = fftl;
  norm = 1.0 / norm;

  cfg = kiss_fft_alloc(fftl, 0, NULL, NULL);

  fprintf(stderr, "fft width and height  %d %d  \n", vec_P->length);
  fprintf(stderr, "fft fftl and exp   %d %d \n", fftl, fftexp);

  capFFT_P = (kiss_fft_cpx *)calloc(fftl, sizeof(kiss_fft_cpx));
  if (capFFT_P == NULL)
    return (NULL);
  cxoutbuff = (kiss_fft_cpx *)calloc(fftl, sizeof(kiss_fft_cpx));
  if (cxoutbuff == NULL)
    return (NULL);
  fft_P = Dsp_AllocateCxVector(fftl);
  if (fft_P == NULL)
    return (NULL);
  for (k = 0; k < fftl; k++) {
    capFFT_P[k].r = vec_P->vector_P[k];
    capFFT_P[k].i = 0.0;
  }

  kiss_fft(cfg, capFFT_P, cxoutbuff);
  for (k = 0; k < fftl; k++) {
    fft_P->vector_P[k].re = cxoutbuff[k].r;
    fft_P->vector_P[k].im = cxoutbuff[k].i;
  }

  free(capFFT_P);
  free(cxoutbuff);

  return (fft_P);
}

/*
 * vector  inverse  FFT
 */
doubleVector_t *Dsp_VectorInverseFFT(cxVector_t *vec_P) {
  float *x_A, *y_A;
  int i, j;
  float tmp;
  float xx, yy;
  float temp1, temp2;
  doubleVector_t *ifft_P;
  kiss_fft_cfg cfg;

  kiss_fft_cpx **mat_PP;
  kiss_fft_cpx *cxoutbuff;
  kiss_fft_cpx *capFFT_P;
  kiss_fft_cpx temp;
  int k;
  float tempReal;
  float tempImag;
  int fftexp;
  int fftl;
  int fftwidth;
  float norm;
  float real, imag, mag;

  fftwidth = vec_P->length;
  /*
   * compute the power of 2 number of fft points
   */
  fftexp = (int)(log((float)fftwidth) / log(2.0) + 0.5);
  fftl = 1 << fftexp;
  if (fftl > fftwidth) {
    fftl = fftl / 2;
    fftexp -= 1;
  }
  norm = fftl;
  norm = 1.0 / norm;

  cfg = kiss_fft_alloc(fftl, 1, NULL, NULL);

  fprintf(stderr, "fft width and height  %d %d  \n", vec_P->length);
  fprintf(stderr, "fft fftl and exp   %d %d \n", fftl, fftexp);

  capFFT_P = (kiss_fft_cpx *)calloc(fftl, sizeof(kiss_fft_cpx));
  if (capFFT_P == NULL)
    return (NULL);
  cxoutbuff = (kiss_fft_cpx *)calloc(fftl, sizeof(kiss_fft_cpx));
  if (cxoutbuff == NULL)
    return (NULL);
  ifft_P = Dsp_AllocateDoubleVector(fftl);
  if (ifft_P == NULL)
    return (NULL);
  for (k = 0; k < fftl; k++) {
    capFFT_P[k].r = vec_P->vector_P[k].re;
    capFFT_P[k].i = vec_P->vector_P[k].im;
  }

  kiss_fft(cfg, capFFT_P, cxoutbuff);
  for (k = 0; k < fftl; k++) {
    ifft_P->vector_P[k] = cxoutbuff[k].r;
  }

  free(capFFT_P);
  free(cxoutbuff);

  return (ifft_P);
}

int Dsp_SAXPYReal(doubleVector_t *vx_P, doubleVector_t *vy_P, double alpha) {

  int i, j;
  int n;

  n = vx_P->length;
  for (i = 0; i < n; i++) {
    vy_P->vector_P[i] = vy_P->vector_P[i] + alpha * vx_P->vector_P[i];
  }
}

int Dsp_SAXPYComplex(cxVector_t *vx_P, cxVector_t *vy_P, cxScalar_t *alpha) {

  int i, j;
  int n;

  n = vx_P->length;
  for (i = 0; i < n; i++) {
    vy_P->vector_P[i].re = vy_P->vector_P[i].re +
                           alpha->value.re * vx_P->vector_P[i].re -
                           alpha->value.im * vx_P->vector_P[i].im;
    vy_P->vector_P[i].im = vy_P->vector_P[i].im +
                           alpha->value.re * vx_P->vector_P[i].im +
                           alpha->value.im * vx_P->vector_P[i].re;
  }
}

int Dsp_ConvolveReal(doubleVector_t *vx_P, doubleVector_t *vf_P,
                     doubleVector_t *vy_P) {
  int i, j, k;
  int m;
  int n;
  int p;
  double sum;
  doubleVector_t *vtdl_P;
  double tmp2;
  double tmp1;
  int N;
  double *x_P;
  double *h_P;

  vtdl_P = Dsp_AllocateDoubleVector(vf_P->length);
  if (vtdl_P == NULL) {
    fprintf(stderr, "Could not allocate space\n");
    return (1);
  }
  x_P = vtdl_P->vector_P;
  h_P = vf_P->vector_P;

  n = vx_P->length;
  N = vf_P->length;

  for (i = 0; i < n; i++) {
    tmp2 = vx_P->vector_P[i];
    for (k = 0; k < N; k++) {
      tmp1 = x_P[k];
      x_P[k] = tmp2;
      tmp2 = tmp1;
    }
    /*
     * Compute inner product
     */
    sum = 0.0;
    for (k = 0; k < N; k++) {
      sum += x_P[k] * h_P[k];
    }
    vy_P->vector_P[i] = sum;
  }

  free(x_P);
  free(vtdl_P);
}

int Dsp_ConvolveComplex(cxVector_t *vx_P, cxVector_t *vf_P, cxVector_t *vy_P) {
  int i, j, k;
  int m;
  int n;
  int p;
  cx_t sum;
  cx_t tmp2;
  cx_t tmp1;
  int N;
  cx_t *x_P;
  cx_t *h_P;
  cxVector_t *vtdl_P;

  vtdl_P = Dsp_AllocateCxVector(vf_P->length);
  if (vtdl_P == NULL) {
    fprintf(stderr, "Could not allocate space\n");
    return (1);
  }
  x_P = vtdl_P->vector_P;
  h_P = vf_P->vector_P;

  n = vx_P->length;
  N = vf_P->length;

  for (i = 0; i < n; i++) {
    tmp2 = vx_P->vector_P[i];
    for (k = 0; k < N; k++) {
      tmp1 = x_P[k];
      x_P[k] = tmp2;
      tmp2 = tmp1;
    }
    /*
     * Compute inner product
     */
    sum.re = 0.0;
    sum.im = 0.0;
    for (k = 0; k < N; k++) {

      sum.re += x_P[k].re * h_P[k].re + x_P[k].im * h_P[k].im;
      sum.im += x_P[k].im * h_P[k].re - x_P[k].re * h_P[k].im;
    }
    vy_P->vector_P[i] = sum;
  }

  free(x_P);
  free(vtdl_P);
}
