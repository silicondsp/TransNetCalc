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

/*********************************************************************/
/*                                                                   */
/* NAME:  cn_netimp.c                                                */
/*                                                                   */
/* ACTION:  calculates the response of a transmission line           */
/*          network to a multiple frequency excitation;              */
/*          the input impedence of the network is calculated         */
/*          and then the voltage and current for each node           */
/*	    Generates the impulse response                           */
/*                                                                   */
/*                                                                   */
/* NOTE:  network topology must be entered as specified in           */
/*        users manual                                               */
/*                                                                   */
/* AUTHOR:  Sasan Ardalan 			                     */
/*          North Carolina State University                          */
/*                                                                   */
/* DATE:  Sept 24, 1986                                              */
/*                                                                   */
/*********************************************************************/

#include "cx.h"
#include "kiss_fft.h"
#include "trantopcalc.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define IMPULSE 0

#define WIND_RECTANGULAR 0
#define WIND_HANNING 1
#define WIND_KAISER 2

#define INVERSE_FFT 1
#define FORWARD_FFT 0

extern char *cs_consoleBuffer;

int Trc_NetImp(char *netFileName, char *nodeName, int frimpFlag, float frmin,
               float frdel, float frmax, int nfft, float sourceE,
               float sourceImp)

{
  struct node *root_P;
  struct node *current_P;
  FILE *net_F;  /* pointer to file with network */
  FILE *imp;    /* impulse response */
  FILE *fresp;  /* frequency response */
  FILE *inpImp; /* Input Impedance response */
  int invalid;  /* flag */
  int go_plot;  /* flag */
  float freq;   /* frequency */
  float deltaT;
  int npts; /* number of points */
  int i;
  complex eg,   /* voltage source */
      zs,       /* source impedence */
      v_in,     /* voltage of parent node */
      totalImp, /* impedence at top of tree */
      totalCur; /* current_P at top of tree */
  char buf[256];

  float vreal, vimag, vmag, df;
  float *xi_A, *xr_A;
  kiss_fft_cpx *xx_A;
  kiss_fft_cpx *ff_A;
  kiss_fft_cpx *xxi_A;
  kiss_fft_cfg cfg;
  complex *zInput_A;
  complex dummy;
  int k, npts2;
  int window;
  float norm;
  char fname[256], fileimp[256], filefreq[256], fileImped[256];

  fprintf(stdout,
          "netFileName=%s nodeName=%s frimpFlag=%d frmin=%f frdel=%f frmax=%f "
          "nfft=%d fsourceE=%f sourceImp=%f\n",
          netFileName, nodeName, frimpFlag, frmin, frdel, frmax, nfft, sourceE,
          sourceImp);

  /*
   * create the network as a recursive tree data structure
   */
  if ((net_F = fopen(netFileName, "r")) == NULL) {
    Trc_Message("Cannot find file !");
    return (1);
  }
#if 1
  root_P = Trc_RecallTree(net_F);
  if (!root_P)
    return (0);
#endif
  fclose(net_F);

  window = 0;

  npts = 1;
  npts <<= nfft;
  npts2 = npts / 2;

  fprintf(stdout, "nfft , npts, npts2 %d %d %d \n", nfft, npts, npts2);

  eg.re = sourceE;
  eg.im = 0.0;
  zs.re = sourceImp;
  zs.im = 0.0;
  if (frimpFlag == IMPULSE) {
    /*
     * calculate frequency interval
     */
    df = frmax / npts;
    freq = 0.01;
  }
#if 1
  if (strcmp(nodeName, "src") != 0) {
    current_P = Trc_SearchTreeName(root_P, nodeName);
    if (current_P == NULL) {
      Trc_Message("Sorry could not find node in impulse response calculation");
      return (1);
    }
  } else {
    CsInfo("For the source the current is calculated");
  }
#endif

  xi_A = (float *)calloc(npts, sizeof(float));
  xr_A = (float *)calloc(npts, sizeof(float));
  zInput_A = (complex *)calloc(npts, sizeof(complex));

  xx_A = (kiss_fft_cpx *)calloc(npts, sizeof(kiss_fft_cpx));
  xxi_A = (kiss_fft_cpx *)calloc(npts, sizeof(kiss_fft_cpx));
  ff_A = (kiss_fft_cpx *)calloc(npts, sizeof(kiss_fft_cpx));

  if (!xi_A || !xr_A || !zInput_A || !xx_A || !xxi_A || !ff_A) {
    Trc_Message("Could not allocate space in Impulse Response Calculation");
    return (0);
  }
  cfg = kiss_fft_alloc(npts, INVERSE_FFT, NULL, NULL);
#if 1
  CsInfo("Calculating Impulse Response");
  for (i = 0; i <= npts2; i++) {

    if (frimpFlag == IMPULSE) {
      freq = df * i;
      if (i == 0)
        freq = 0.00001;
    } else
      freq = frmin + i * (frmax - frmin) / (float)(npts2);

    /*
     * Calculate characteristic impedance and propogation constant
     * for each node
     */
    Trc_TreeCharic(root_P, freq);

    /*
     * calculate impedences
     */
    totalImp = Trc_CalcImp(root_P, root_P->zo, root_P->gamma);
    zInput_A[i].re = totalImp.re;
    zInput_A[i].im = totalImp.im;

    /*
     * calculate currents and voltages
     */
    totalCur = Trc_InitCur(root_P, root_P->gamma, root_P->zo, eg, zs,
                           root_P->zLoad, root_P->length, dummy);
    v_in.re = 0.0;
    v_in.im = 0.0;
    Trc_CalcCurrent(root_P, totalCur, v_in, root_P->zo, root_P->gamma);

    /*
     * output data at each node
     */
    if (strcmp(nodeName, "src") != 0) {

      current_P = Trc_SearchTreeName(root_P, nodeName);
      vreal = current_P->voltage.re;
      vimag = current_P->voltage.im;

      Trc_GetNodeVoltage(root_P, nodeName, &vreal, &vimag);
#if 0
		   GetNodeCurrent(root_P, nodeName,  &vreal, &vimag);
#endif
    } else {
      vreal = totalCur.re;
      vimag = totalCur.im;
    }

    xr_A[i] = vreal;
    xi_A[i] = vimag;
  }
#endif
  CsInfo("Impulse/Frequency  response calculated");

  for (i = 1; i < npts2; i++) {
    k = npts - i;
    xr_A[k] = xr_A[i];
    xi_A[k] = -xi_A[i];
  }
  xi_A[npts2] = 0.0;
  for (i = 0; i < npts; i++) {
    xx_A[i].r = xr_A[i];
    xx_A[i].i = xi_A[i];
    ff_A[i] = xx_A[i];
  }

  if (frimpFlag == IMPULSE) {

    kiss_fft(cfg, xx_A, xxi_A);
    // cxfft(xx_A,&nfft);
  }

  deltaT = 1.0 / (frdel * npts);
  strcpy(fileimp, nodeName);
  strcpy(filefreq, nodeName);
  strcpy(fileImped, nodeName);
  strcat(fileimp, ".imp");
  strcat(filefreq, ".fre");
  strcat(fileImped, ".zin");
  if (frimpFlag == IMPULSE) {
    imp = fopen(fileimp, "w");
    if (imp) {
      norm = 1.0 / (float)npts;
      for (i = 0; i < npts; i++) {

        fprintf(imp, "%e\n", xxi_A[i].r * norm);
      }
    }
  }
  fresp = fopen(filefreq, "w");
  inpImp = fopen(fileImped, "w");

  if (fresp)
    for (i = 0; i < npts2; i++)
      fprintf(fresp, "%e %e \n", ff_A[i].r, ff_A[i].i);
  if (inpImp)
    for (i = 0; i < npts2; i++)
      fprintf(inpImp, "%e %e \n", zInput_A[i].re, zInput_A[i].im);

  CsInfo("Stored Files");
  if (fresp)
    fclose(fresp);
  if (frimpFlag == IMPULSE)
    if (imp)
      fclose(imp);
  if (inpImp)
    fclose(inpImp);

  free(xr_A);
  free(xi_A);
  free(ff_A);
  free(xx_A);
  free(xxi_A);
  free(zInput_A);

  return (0);
}
