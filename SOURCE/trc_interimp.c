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
/* NAME:  netimp.c                                                  */
/*                                                                   */
/* ACTION:  calculates the response of a transmission line           */
/*          network to a multiple frequency excitation;                */
/*          the input impedence of the network is calculated         */
/*          and then the voltage and current_P for each node           */
/*	    Generates and plots the impulse response                 */
/*                                                                   */
/* INPUT:  file containing network topology                          */
/*                                                                   */
/* OUTPUT:  for each node in the topology                            */
/*	     node name                                               */
/*           length                                                  */
/*           characteristic impedence                                */
/*           propagation constant                                    */
/*           left branch name                                        */
/*           right branch name                                       */
/*           impedence                                               */
/*           left branch impedence                                   */
/*           right branch impedence                                  */
/*	     incoming current_P                                        */
/*           left branch current_P                                     */
/*           right branch current_P                                    */
/*           voltage                                                 */
/*	    voltage magnitude vs distance points between nodes       */
/*          current_P magnitude vs distance points between nodes       */
/*                                                                   */
/* NOTE:  network topology must be entered as specified in           */
/*        users manual                                               */
/*        linked with transfr.c,  complex.c                          */
/*        includes <stdio.h>, <math.h>, "defs.h"                     */
/*                                                                   */
/* AUTHOR:  Sasan Ardalan and Susan Alexander ,                      */
/*          North Carolina State University                          */
/*                                                                   */
/* DATE:  Sept 24, 1986                                              */
/*                                                                   */
/*********************************************************************/

#include "cx.h"
#include "trantopcalc.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#define OVER 'x'
#define IMPULSE 0

#define WIND_RECTANGULAR 0
#define WIND_HANNING 1
#define WIND_KAISER 2

int Trc_Menu(char *selection_A[]) {
  int select;
  int i;

  i = 0;
  while (strcmp(selection_A[i], "") != 0) {
    fprintf(stderr, "%s\n", selection_A[i]);
    i++;
  }
  scanf("%d", &select);
  return (select);
}

void Trc_NetImpCalc(struct node *root_P, char topologyFileName[]) {
  FILE *topfp;  /* topology */
  FILE *imp;    /* impulse response */
  FILE *fresp;  /* frequency response */
  FILE *inpImp; /* Input Impedance response */
  int invalid;  /* flag */
  int go_plot;  /* flag */
  float freq;   /* frequency */
  float frmax;  /* maximum frequency */
  float deltaT;
  int npts; /* number of points */
  int i;
  complex eg,   /* voltage source */
      zs,       /* source impedence */
      v_in,     /* voltage of parent node */
      totalImp, /* impedence at top of tree */
      totalCur; /* current_P at top of tree */

  complex dummy;
  char buf[64];
  float vreal, vimag, vmag, df;
  float xplot_A[1024], yplot_A[1024];
  int num_curves = 1;
  float xi_A[1024], xr_A[1024], xx_A[2048], ff_A[2048];
  float wind_A[1024];
  complex zInput_A[1024];
  int k, nfft, npts2;
  float frmin, frdel;
  int frimpFlag;
  char fname[80], fileimp[80], filefreq[80], fileImped[80];
  char nodeName[80];
  int DONE;
  float sfrd;
  short int key;
  struct node *current_P;
  int action;
  static char *selection_A[] = {"(1) Another Node", "(2) Exit            ", ""};
  static char *selFreqImp_A[] = {"(0) Impulse Response",
                                 "(1) Frequency Data   ",
                                 "(2) Exit            ", ""};
  static char *window_A[] = {"(0) Rectangular", "(1) Hanning", "(2) Kaiser",
                             ""};

  topfp = stdin;

  frimpFlag = Trc_Menu(selFreqImp_A);
  if (frimpFlag == 2)
    return;

  if (frimpFlag == IMPULSE) {
    Trc_Prompt("Enter sampling rate  [Hz]: ");
    scanf("%f", &frmax);
    Trc_Prompt("Enter number of points nfft exp: ");
    scanf("%d", &nfft);
  } else {
    Trc_Prompt("Enter beginning frequency, Hz");
    scanf("%f", &frmin);
    Trc_Prompt("Enter frequency interval, Hz");
    scanf("%f", &frdel);
    Trc_Prompt("Enter m for Number of Points n=2**m");
    scanf("%d", &nfft);
  }

  npts = 1;
  npts <<= nfft;
  npts2 = npts / 2;

  Trc_Prompt("Enter Generator Voltage");
  scanf("%f", &sfrd);
  eg.re = sfrd;
  eg.im = 0.0;
  Trc_Prompt("Enter Source Impedance");
  scanf("%f", &sfrd);
  zs.re = sfrd;
  zs.im = 0.0;
  if (frimpFlag == IMPULSE) {
    /*
     * calculate frequency interval
     */
    df = frmax / npts;
    freq = 0.01;
  }
  DONE = FALSE;
  while (DONE == FALSE) {
    Trc_Prompt("Enter a node name  ");
    scanf("%s", nodeName);
    if (strcmp(nodeName, "src") != 0) {
      current_P = Trc_SearchTreeName(root_P, nodeName);
      if (current_P == NULL) {
        fprintf(stderr, "Sorry could not find node \n");
        continue;
      }
    } else {
      Trc_Prompt("For the source the current is calculated");
    }

    printf("SOURCE VOLTAGE=%f IMPEDANCE =%f fmin=%f fd=%f fmax=%f \n", eg.re,
           zs.re, frmin, df, frmax);

    Trc_NetImp(topologyFileName, nodeName, frimpFlag, frmin, df, frmax, nfft,
               eg.re, zs.re);

#if 000 // SHA
    Trc_Message("Calculating Impulse Response");
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
#if 0
		   Trc_GetNodeCurrent(root_P, nodeName,  &vreal, &vimag);
		   Trc_GetNodeVoltage(root_P, nodeName,  &vreal, &vimag);
#endif
        current_P = Trc_SearchTreeName(root_P, nodeName);
        vreal = current_P->voltage.re;
        vimag = current_P->voltage.im;
      } else {
        vreal = totalCur.re;
        vimag = totalCur.im;
      }

      xr_A[i] = vreal;
      xi_A[i] = vimag;
#if 0
		vmag=vreal*vreal+vimag*vimag;
		yplot_A[i]=10.0*log10(vmag);
		xplot_A[i]=i;
#endif
    }
    /*
            GraphPlot(xplot,yplot,&npts,&num_curves,
                    "Frequency Response","Frequency","Responce dB");
    */
    Trc_Prompt("Impulse/Frequency  response calculated");

    /*
     * window the frequency response
     */
    switch (Trc_Menu(window_A)) {
    case WIND_RECTANGULAR:
      for (i = 0; i <= npts2; i++) {
        wind_A[i] = 1.0;
      }
      break;
    case WIND_HANNING:
      for (i = 0; i <= npts2; i++) {
        wind_A[i] = 0.5 * (1.0 + cos(2 * i * PI / (float)npts));
      }
      break;
    case WIND_KAISER:
      for (i = 0; i <= npts2; i++) {
        wind_A[i] = 1.0;
      }
      break;
    }
    for (i = 0; i <= npts2; i++) {
      xr_A[i] = xr_A[i] * wind_A[i];
      xi_A[i] = xi_A[i] * wind_A[i];
    }
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
    for (i = 0; i < npts; i++) {
      yplot_A[i] = xx_A[2 * i] / (float)npts;
    }
    deltaT = 1.0 / (frdel * npts);
    Trc_BuildDistance(root_P, xplot_A, deltaT, frmin, npts);
/*
        GraphPlot(xplot,yplot,&npts,&num_curves,
                "Impulse Response","Time","Response");
*/
#if 0
		Trc_Prompt("Enter file name (no attribute)\n");
		scanf("%s",fname);
#endif
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
      fprintf(imp, "%e  %e \n", xplot_A[i], yplot_A[i]);
    }
    for (i = 0; i < npts2; i++)
      fprintf(fresp, " %e %e \n", ff_A[2 * i], ff_A[2 * i + 1]);
    for (i = 0; i < npts2; i++)
      fprintf(inpImp, " %e %e \n", zInput_A[i].re, zInput_A[i].im);
    fclose(fresp);
    fclose(imp);
    fclose(inpImp);

#endif
    action = Trc_Menu(selection_A);
    if (action == 2)
      DONE = TRUE;
  }

  fclose(topfp);
}
