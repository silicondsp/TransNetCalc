
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
#include "graphnet.h"
#include <math.h>
#include <stdio.h>
#define IMPULSE 0

#define WIND_RECTANGULAR 0
#define WIND_HANNING 1
#define WIND_KAISER 2

extern struct node *RecallTree();

int Cn_NetImp(char *netFileName, float nodeName, float frimpFlag, float frmin,
              float frdel, float frmax, int nfft, float sourceE,
              float sourceImp)

{

  struct node *root_P;
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
  char buf[64];
  complex CalcImp(), InitCur();
  float vreal, vimag, vmag, df;
  float *xi_A, *xr_A, *xx_A, *ff_A;
  complex *zInput_A;
  int k, npts2;
  int window;
  char fname[80], fileimp[80], filefreq[80], fileImped[80];
  struct node *current_P;
  struct node *SearchTreeName();

  /*
   * create the network as a recursive tree data structure
   */
  if ((net_F = fopen(netFileName, "r")) == NULL) {
    Message("Cannot find file !");
    return (1);
  }
  root_P = RecallTree(net_F);
  fclose(net_F);

  window = 0;

  npts = 1;
  npts <<= nfft;
  npts2 = npts / 2;

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
  if (strcmp(nodeName, "src") != 0) {
    current_P = SearchTreeName(root_P, nodeName);
    if (current_P == NULL) {
      fprintf(stderr, "Sorry could not find node \n");
      return (1);
    }
  } else {
    Prompt("For the source the current is calculated");
  }

  xi_A = (float *)calloc(npts, sizeof(float));
  xr_A = (float *)calloc(npts, sizeof(float));
  zInput_A = (complex *)calloc(npts, sizeof(complex));
  xx_A = (float *)calloc(2 * npts, sizeof(float));
  ff_A = (float *)calloc(2 * npts, sizeof(float));

  Message("Calculating Impulse Response");
  for (i = 0; i <= npts2; i++) {

    if (frimpFlag == IMPULSE) {
      freq = df * i;
      if (i == 0)
        freq = 0.00001;
    } else
      freq = frmin + i * frdel;

    /*
     * Calculate characteristic impedance and propogation constant
     * for each node
     */
    TreeCharic(root_P, freq);

    /*
     * calculate impedences
     */
    totalImp = CalcImp(root_P, root_P->zo, root_P->gamma);
    zInput_A[i].re = totalImp.re;
    zInput_A[i].im = totalImp.im;
    /*
     * calculate currents and voltages
     */
    totalCur = InitCur(root_P, root_P->gamma, root_P->zo, eg, zs, root_P->zLoad,
                       root_P->length);
    v_in.re = 0.0;
    v_in.im = 0.0;
    CalcCurrent(root_P, totalCur, v_in, root_P->zo, root_P->gamma);

    /*
     * output data at each node
     */
    if (strcmp(nodeName, "src") != 0) {
      GetNodeVoltage(root_P, nodeName, &vreal, &vimag);
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
  Prompt("Impulse/Frequency  response calculated");

  for (i = 1; i < npts2; i++) {
    k = npts - i;
    xr_A[k] = xr_A[i];
    xi_A[k] = -xi_A[i];
  }
  xi_A[npts2] = 0.0;
  for (i = 0; i < npts; i++) {
    xx_A[2 * i] = xr_A[i];
    xx_A[2 * i + 1] = xi_A[i];
  }
  for (i = 0; i < 2 * npts; i++) {
    ff_A[i] = xx_A[i];
  }

  cxfft(xx_A, &nfft);

  deltaT = 1.0 / (frdel * npts);
  strcpy(fileimp, nodeName);
  strcpy(filefreq, nodeName);
  strcpy(fileImped, nodeName);
  strcat(fileimp, ".imp");
  strcat(filefreq, ".fre");
  strcat(fileImped, ".zin");
  imp = fopen(fileimp, "w");
  fresp = fopen(filefreq, "w");
  inpImp = fopen(fileImped, "w");
  for (i = 0; i < npts; i++) {
    fprintf(imp, "%e\n", xx_A[2 * i] / (float)npts);
  }
  for (i = 0; i < npts2; i++)
    fprintf(fresp, " %e %e \n", ff_A[2 * i], ff_A[2 * i + 1]);
  for (i = 0; i < npts2; i++)
    fprintf(inpImp, " %e %e \n", zInput_A[i].re, zInput_A[i].im);
  fclose(fresp);
  fclose(imp);
  fclose(inpImp);

  free(xr_A);
  free(xi_A);
  free(ff_A);
  free(xx_A);
  free(zInput_A);

  fprintf(stderr, "nfft , npts, npts2 %d %d %d \n", nfft, npts, npts2);
  return (0);
}
