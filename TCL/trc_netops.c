/*  Transmission Line Topology Calculation and Analysis (TranTopCalc) Command
   Tool Copyright (C) 1985-2006  Sasan H Ardalan

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

#include <math.h>
#include <stdio.h>

#include "cx.h"
#include "trantopcalc.h"
#include "trantopcalc_tcl.h"
#include <kiss_fft.h>
#include <tcl.h>

#define RAND_LARGE 0x7fffffff
#define DEC

int Trc_NetImpVec(char *netFileName, char *nodeName, float frmin, float frdel,
                  float frmax, int nfft, float sourceE, float sourceImp,
                  doubleVector_t *imp_P, cxVector_t *zInput_P,
                  cxVector_t *fresp_P)

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
          "nodeName=%s  frmin=%f frdel=%f frmax=%f nfft=%d fsourceE=%f "
          "sourceImp=%f\n",
          nodeName, frmin, frdel, frmax, nfft, sourceE, sourceImp);

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
  if (imp_P) {
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

    if (imp_P) {
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

  if (imp_P) {

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
  if (imp_P) {
    imp = fopen(fileimp, "w");
    if (imp) {
      norm = 1.0 / (float)npts;
      for (i = 0; i < npts; i++) {

        fprintf(imp, "%e\n", xxi_A[i].r * norm);
        imp_P->vector_P[i] = xxi_A[i].r * norm;
      }
    }
  }
  fresp = fopen(filefreq, "w");
  inpImp = fopen(fileImped, "w");

  if (fresp)
    for (i = 0; i < npts2; i++) {
      fprintf(fresp, "%e %e \n", ff_A[i].r, ff_A[i].i);
      if (fresp_P) {
        fresp_P->vector_P[i].re = ff_A[i].r;
        fresp_P->vector_P[i].im = ff_A[i].i;
      }
    }
  if (inpImp) {
    for (i = 0; i < npts2; i++) {
      fprintf(inpImp, "%e %e \n", zInput_A[i].re, zInput_A[i].im);
      if (zInput_P) {
        zInput_P->vector_P[i].re = zInput_A[i].re;
        zInput_P->vector_P[i].im = zInput_A[i].im;
      }
    }
  }
  CsInfo("Stored Files");
  if (fresp)
    fclose(fresp);

  if (imp_P)
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

/*
 * Compute current along length of node transmission line
 */

void Trc_CalcCurrentLengthNode(struct node *node_P, int points,
                               doubleVector_t *y_P) {
  char loadString[256];
  char subTitle[256];
  char title[256];

  char yLabel[80];
  char vswrString[80];
  float dx;
  float length;

  int i;
  complex val;

  double a, b;

  complex refCurrent;
  complex refVoltage;
  short currentFlag = 0;
  float rflcMag;
  complex rflc;

  refCurrent.re = 1.0;
  refCurrent.im = 0.0;

  refVoltage.re = 1.0;
  refVoltage.im = 0.0;

  dx = node_P->length / (float)points;

  currentFlag = 0;
  fprintf(stdout, "node v=%f %f\n", node_P->voltage.re, node_P->voltage.im);

  for (i = 0; i <= points; i++) {
    length = dx * i;

    if (node_P->zLoad.re == 0.0 && node_P->zLoad.im == 0.0 &&
        !node_P->infinite) {
      val = Trc_TrLineCur(node_P, node_P->inCurrent, length);
      currentFlag = TRUE;
    } else {

      val = Trc_TrLineVoltage(node_P, node_P->voltage, length);
    }
    //		  current=TrLineVolt(node_P, refVoltage, length);
    a = val.re;
    b = val.im;

    y_P->vector_P[i] = sqrt(a * a + b * b);
  }

  rflc = Trc_TrCalcReflectionCoeff(node_P);

  a = rflc.re;
  b = rflc.im;

  rflcMag = sqrt(a * a + b * b);

  if (rflcMag == 1.0) {
    sprintf(vswrString, "Infinite");
  } else {
    sprintf(vswrString, "%.2f", (1 + rflcMag) / (1 - rflcMag));
  }

  CsInfo(vswrString);
  if (currentFlag) {
    strcpy(yLabel, "Current");

  } else
    strcpy(yLabel, "Voltage");
  CsInfo(yLabel);

  sprintf(title, "%s  VSWR=%s", node_P->name, vswrString);
  if (node_P->infinite)
    sprintf(loadString, "open");
  else
    sprintf(loadString, "(%.2f:%.2f)", node_P->zLoad.re, node_P->zLoad.im);
  CsInfo(loadString);

  CsInfo(title);
  sprintf(subTitle, "Load %s Z0(%.2f:%.2f) Length=%f ", loadString,
          node_P->zo.re, node_P->zo.im, node_P->length);
  CsInfo(subTitle);

  fprintf(stdout, "\n%s\n", title);
  fprintf(stdout, "%s\n", subTitle);

  //	curve_P=IIP_PlotCurve(x_P,y_P,points,title,subTitle,
  //	              	"Length Towards Generator",yLabel,0);
}
