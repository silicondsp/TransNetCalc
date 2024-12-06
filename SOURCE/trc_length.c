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
/* NAME:  cn_length.c                                                  */
/*                                                                   */
/* ACTION:  calculates the response of a transmission line           */
/*          network to variation in length at a node;                */
/*          the input impedence of the network is calculated         */
/*          and then the voltage and current for each node           */
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
/*                                                                   */
/* AUTHOR:  Sasan Ardalan                                            */
/*          North Carolina State University                          */
/*                                                                   */
/* DATE:  March 10, 1990                                              */
/*                                                                   */
/*********************************************************************/

#include "cx.h"
#include "trantopcalc.h"
#include <math.h>
#include <stdio.h>
#define OVER 'x'
#define LENGTH 0

void Trc_NetLength(struct node *root_P) {
  FILE *topfp;  /* topology */
  FILE *len_F;  /* impulse response */
  FILE *inpImp; /* Input Impedance response */
  int invalid;  /* flag */
  int go_plot;  /* flag */
  float freq;   /* frequency */
  float frmax;  /* maximum frequency */
  int npts;     /* number of points */
  float length;
  int i;
  complex eg,   /* voltage source */
      zs,       /* source impedence */
      v_in,     /* voltage of parent node */
      totalImp, /* impedence at top of tree */
      totalCur; /* current_P at top of tree */
  char buf[64];
  complex CalcImp(), InitCur();
  float vreal, vimag, vmag, df;
  float xplot_A[1024], yplot_A[1024];
  int num_curves = 1;
  complex zInput_A[1024];
  int k, nfft, npts2;
  float lenmin, lendel;
  int lengthFlag;
  char fname[80], fileLength[80], filefreq[80], fileImped[80];
  char nodeName[80];
  int DONE;
  short int key;
  struct node *current_P;

  int action;
  complex dummy;
  static char *selection_A[] = {"(1) Another Node", "(2) Exit            ", ""};
  static char *selFreqImp_A[] = {"(0) Length Response", "(1) Exit            ",
                                 ""};

  topfp = stdin;

  lengthFlag = Trc_Menu(selFreqImp_A);
  if (lengthFlag == 1)
    return;

  Trc_Prompt("Enter Frequency");
  scanf("%f", &freq);

  Trc_Prompt("Enter beginning length");
  scanf("%f", &lenmin);
  Trc_Prompt("Enter line interval, meters");
  scanf("%f", &lendel);
  Trc_Prompt("Enter Number of Points");
  scanf("%d", &npts);

  /*
   * Calculate characteristic impedance and propagation constant
   * for each node
   */
  Trc_TreeCharic(root_P, freq);

  Trc_Prompt("Enter Generator Voltage");
  scanf("%f", &eg.re);
  eg.im = 0.0;
  Trc_Prompt("Enter Source Impedance, real and imaginary");
  scanf("%f %f", &zs.re, &zs.im);
  DONE = FALSE;
  while (DONE == FALSE) {
    Trc_Prompt("Enter a node name  to change length");
    scanf("%s", nodeName);
    current_P = Trc_SearchTreeName(root_P, nodeName);
    if (current_P == NULL) {
      fprintf(stderr, "Sorry could not find node \n");
      continue;
    }
    Trc_Prompt("Enter a node to examine response");
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
    Trc_Message("Calculating Length Response");
    for (i = 0; i <= npts; i++) {

      length = lenmin + i * lendel;
      current_P->length = length;

      /*
       * calculate impedences
       */
      totalImp = Trc_CalcImp(root_P, root_P->zo, root_P->gamma);
      zInput_A[i].re = totalImp.re;
      zInput_A[i].im = totalImp.im;
      /*
       * calculate current_Ps and voltages
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
		   GetNodeCurrent(root_P, nodeName,  &vreal, &vimag);
#endif
        Trc_GetNodeVoltage(root_P, nodeName, &vreal, &vimag);
      } else {
        vreal = totalCur.re;
        vimag = totalCur.im;
      }

#if 0
		xr_A[i]=vreal;
		xi_A[i]=vimag;
#endif
#if 1
      vmag = vreal * vreal + vimag * vimag;
      yplot_A[i] = 10.0 * log10(vmag);
      xplot_A[i] = length;
#endif
    }
    Trc_Prompt("Length  response calculated");
    strcpy(fileLength, nodeName);
    strcpy(filefreq, nodeName);
    strcpy(fileImped, nodeName);
    strcat(fileLength, ".len");
    strcat(filefreq, ".fre");
    strcat(fileImped, ".zin");
    len_F = fopen(fileLength, "w");
    inpImp = fopen(fileImped, "w");
    for (i = 0; i < npts; i++) {
      fprintf(len_F, " %e %e \n", xplot_A[i], yplot_A[i]);
    }
    for (i = 0; i < npts; i++)
      fprintf(inpImp, " %e %e \n", zInput_A[i].re, zInput_A[i].im);
    fclose(len_F);
    fclose(inpImp);
    action = Trc_Menu(selection_A);
    if (action == 2)
      DONE = TRUE;
  }

  fclose(topfp);
}
