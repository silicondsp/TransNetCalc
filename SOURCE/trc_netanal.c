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
/* NAME:  netanal.c                                                  */
/*                                                                   */
/* ACTION:  calculates the response of a transmission line           */
/*          network to a multiple frequency excitation;                */
/*          the input impedence of the network is calculated         */
/*          and then the voltage and current_P for each node           */
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
/* AUTHOR:  Susan Alexander , Modified by Sasan Ardalan             */
/*          North Carolina State University                          */
/*                                                                   */
/* DATE:  Sept 24, 1986                                              */
/*                                                                   */
/*********************************************************************/

#include "cx.h"
#include "trantopcalc.h"
#include <math.h>
#include <stdio.h>

#define NOT_OVER 0
#define OVER 'x'
#define FREQ 'f'
#define NUM_COLUMNS 28
#define NUM_LINES 20
#define BEGIN_X 52
#define BEGIN_Y 0

float Trc_freq;

void Trc_NetAnal(struct node *root_P, char topologyFileName[]) {
  struct node *current_P; /* current_P node accessed */
  FILE *topfp;            /* topology */
  FILE *outfile;          /* data output */
  int invalid;            /* flag */
  int go_plot;            /* flag */
  float freq;             /* frequency */
  float frmax;            /* maximum frequency */
  int npts;               /* number of points */
  int i;
  complex eg,   /* voltage source */
      zs,       /* source impedence */
      v_in,     /* voltage of parent node */
      totalImp, /* impedence at top of tree */
      totalCur; /* current_P at top of tree */
  char buf[64];
  char ndbuf[64];
  char *fgets();
  int key; /* key pressed in get_node_info */
  float sx, sy;

  topfp = stdin;

/*
 * open file to output data
 */
#if 0
Trc_Prompt("Filename for output [tt: for stdout]: ");
scanf("%s",buf);
outfile = fopen(buf, "w");
#endif

  Trc_Prompt("Enter  frequency [Hz]: ");
  scanf("%f", &freq);
  Trc_freq = freq;
  Trc_Prompt("Enter Generator Voltage");
  scanf("%f%f", &sx, &sy);
  eg.re = sx;
  eg.im = sy;
  Trc_Prompt("Enter Source Impedance (real and imaginary)");
  scanf("%f%f", &sx, &sy);
  zs.re = sx;
  zs.im = sy;
  key = FREQ;
  while (key != OVER) {
    /*
     * Calculate characteristic impedance and propogation constant
     * for all nodes
     */
    Trc_TreeCharic(root_P, freq);

    /*
     * calculate impedences
     */
    totalImp = Trc_CalcImp(root_P, root_P->zo, root_P->gamma);
    /*
     * calculate currents and voltages
     */
    totalCur = Trc_InitCur(root_P, root_P->gamma, root_P->zo, eg, zs,
                           root_P->zLoad, root_P->length, totalImp);
    v_in.re = 0.0;
    v_in.im = 0.0;
    Trc_CalcCurrent(root_P, totalCur, v_in, root_P->zo, root_P->gamma);
    printf("Total impedance = %e %e \n", totalImp.re, totalImp.im);
    printf("Total input current = %e %e \n", totalCur.re, totalCur.im);
    /******************************************************************/
    /*  Get node from network graph                                   */
    /******************************************************************/
    Trc_GetNodeInfo(root_P, &key);
    /*
     * output data at each node
     */
    if (key == FREQ) {
      Trc_Prompt("Enter  frequency [Hz]: ");
      scanf("%f", &freq);
    }
  }
}
