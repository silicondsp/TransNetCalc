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

#include "cx.h"
#include "trantopcalc.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define MU 12.566371e-7
#define EPS0 8.854e-12
#define SIG 58.0e6
#define SIGMA 58.0e6
#define SPEED_OF_LIGHT 3.0e8
/*
 * for miles
#define SCALE 1609.35
 */
#define SCALE 1.0
#define INFINITY 1.0e22

/*
 * This routine calculates the characteristic impedance and
 * propogation constant for a given frequency and places them
 * into the tree
 */
void Trc_TreeCharic(struct node *root_P, float freq) {
  complex zo, /* characteristic impedence */
      gamma;  /* propagation constant */
  float r, l, c, g, r0, delta, rad;
  float n, epsr, eps, nu;
  float ri, rs, t;
  float h, d;
  float w;
  float wrgc;
  double tmp;
  float airThickness;
  float midThisckness;
  float baseThickness;
  float stripThickness;
  float stripWidth;
  float stripRho;
  float midEPSRl;
  float midLossTan;
  float baeEPSR;
  float baseLossTan;
  int i;

  if (root_P == (struct node *)NULL)
    return;
  if (root_P->lumped) {

    zo.re = root_P->r;
    zo.im = 2.0 * PI * freq * root_P->l;
    c = root_P->c;
    g = root_P->g;
    if (g == 0.0) {
      gamma.re = 0.0;
      if (c == 0)
        gamma.im = 0.0;
      else
        gamma.im = -1.0 / (2.0 * PI * freq * c);

    } else {
      wrgc = 2.0 * PI * freq * c / g;
      tmp = wrgc * wrgc + 1.0;
      gamma.re = 1.0 / tmp / g;
      gamma.im = -wrgc / g / tmp;
    }

  } else if (strncmp(root_P->typeName, "plasma", 6) == 0) {
    nu = root_P->r;
    n = root_P->l;
    gamma = Trc_CalcPropConstPlasma(nu, n, freq);
    zo = Trc_CalcCharImpPlasma(nu, n, freq);

  } else if (strncmp(root_P->typeName, "coax", 4) == 0) {
    ri = root_P->r;
    rs = root_P->l;
    eps = root_P->c;
    t = root_P->g;
    gamma = Trc_CalcPropConstCoax(ri, rs, eps, t, freq);
    zo = Trc_CalcCharImpCoax(ri, rs, eps, t, freq);

  } else if (strncmp(root_P->typeName, "paral", 5) == 0) {
    ri = root_P->r;
    d = root_P->l;
    eps = root_P->c;
    gamma = Trc_CalcPropConstParall(ri, d, eps, freq);
    zo = Trc_CalcCharImpParall(ri, d, eps, freq);

  } else if (strncmp(root_P->typeName, "wireabg", 7) == 0) {
    ri = root_P->r;
    h = root_P->l;
    eps = root_P->c;
    gamma = Trc_CalcPropConstWireabg(ri, h, eps, freq);
    zo = Trc_CalcCharImpWireabg(ri, h, eps, freq);

  } else if (strncmp(root_P->typeName, "balsh", 5) == 0) {
    ri = root_P->r;
    rs = root_P->l;
    eps = root_P->c;
    h = root_P->g;

    gamma = Trc_CalcPropConstBalShielded(ri, rs, eps, h, freq);
    zo = Trc_CalcCharImpBalShielded(ri, rs, eps, h, freq);

  } else if (strncmp(root_P->typeName, "mstrip", 6) == 0) {

    // fprintf(stderr,"Not supported. Working on open sourcing the code. \n");

    w = root_P->r;
    h = root_P->l;
    eps = root_P->c;
    t = root_P->g;
    gamma = Trc_CalcPropConstMicrostrip(w, h, eps, t, freq);
    zo = Trc_CalcCharImpMicrostrip(w, h, eps, t, freq);

  } else {
    r = root_P->r;
    l = root_P->l;
    c = root_P->c;
    g = root_P->g;
    /*****************************************************
     Take into consideration the skin effect
    *****************************************************/
    r0 = r / SCALE;
    delta = sqrt(PI * SIG * MU * freq);
    delta = 1.0 / delta;
    rad = sqrt(2.0 / PI / SIG / r0);
    if (delta < rad) {
      tmp = SIG * PI * delta * (2.0 * rad - delta);
      r = 2.0 * SCALE / tmp;
    }
#if 0
fprintf(stderr,"name r,l,c,g %s %e %e %e %e  \n",root_P->name,
		root_P->r,root_P->l,root_P->c,root_P->g);
#endif
    zo = Trc_CalcCharImp(r, c, l, g, freq, (double)0.0);
    gamma = Trc_CalcPropConst(r, c, l, g, freq);
  }
  root_P->gamma.re = gamma.re;
  root_P->gamma.im = gamma.im;
  root_P->zo.re = zo.re;
  root_P->zo.im = zo.im;
  Trc_TreeCharic(root_P->left_P, freq);
  Trc_TreeCharic(root_P->right_P, freq);
}

/*
 * The function  outputs the data calculated during program
 * execution.
 */
void Trc_GetNodeVoltage(struct node *root_P, char *nodename, float *vreal,
                        float *vimag) {

  if (root_P == (struct node *)NULL)
    return;

  // sprintf(cs_consoleBuffer,"GetNodeVoltage nodeName=%s requested
  // =%s\n",root_P->name, nodename); PrintConsole();
  if (strcmp(root_P->name, nodename) == 0) {
    //		CsInfo("GetNodeVoltage found");
    *vreal = root_P->voltage.re;
    *vimag = root_P->voltage.im;
  }

  Trc_GetNodeVoltage(root_P->left_P, nodename, vreal, vimag);
  Trc_GetNodeVoltage(root_P->right_P, nodename, vreal, vimag);
}

/*
 * The function  outputs the data calculated during program
 * execution.
 */
void Trc_GetNodeCurrent(struct node *root_P, char *nodename, float *vreal,
                        float *vimag) {

  if (root_P == (struct node *)NULL)
    return;
  if (strcmp(root_P->name, nodename) == 0) {
    *vreal = root_P->inCurrent.re;
    *vimag = root_P->inCurrent.im;
    return;
  }

  Trc_GetNodeCurrent(root_P->left_P, nodename, vreal, vimag);
  Trc_GetNodeCurrent(root_P->right_P, nodename, vreal, vimag);
}

complex Trc_LumpedImp(struct node *root_P) {

  complex zPar;

  /*
   * note gamma is actually ZShunt for lumped element
   * and zo is ZSeries
   */

  if (root_P->infinite)
    zPar = root_P->gamma;
  else if ((root_P->gamma.re == 0.0) && (root_P->gamma.im == 0.0))
    zPar = root_P->zLoad;
  else
    zPar = Trc_ZParallel(root_P->zLoad, root_P->gamma);
  return (Cx_Add(zPar, root_P->zo));
}

/*
 * The function CalcImp is a recursive procedure which
 * calculates the impedences at each node from the
 * termination loads moving back toward the source.
 */
complex Trc_CalcImp(struct node *root_P, complex zo, complex gamma)

{

  complex rcl;

  // sprintf(cs_consoleBuffer,"CalcImp node =%s \n",root_P->name);
  // PrintConsole();

  switch (root_P->type) {
  case NODE_TYPE_LEAF:
    if (root_P->lumped) {
      return (Trc_LumpedImp(root_P));
    } else {
      rcl = Trc_CalcRcl(root_P->zLoad, root_P->zo, root_P->infinite);
      return (Trc_LineImp(rcl, root_P->zo, root_P->gamma, root_P->length));
    }
    break;

  case NODE_TYPE_UNARY:
    root_P->leftImp = Trc_CalcImp(root_P->left_P, root_P->zo, root_P->gamma);
    root_P->zLoad = root_P->leftImp;
    if (root_P->lumped) {
      return (Trc_LumpedImp(root_P));
    } else {
      rcl = Trc_CalcRcl(root_P->zLoad, root_P->zo, root_P->infinite);
      return (Trc_LineImp(rcl, root_P->zo, root_P->gamma, root_P->length));
    }
    break;

  case NODE_TYPE_BINARY:
    root_P->leftImp = Trc_CalcImp(root_P->left_P, root_P->zo, root_P->gamma);
    root_P->rightImp = Trc_CalcImp(root_P->right_P, root_P->zo, root_P->gamma);
    root_P->zLoad = Trc_ZParallel(root_P->leftImp, root_P->rightImp);
    if (root_P->lumped) {
      return (Trc_LumpedImp(root_P));
    } else {
      rcl = Trc_CalcRcl(root_P->zLoad, root_P->zo, root_P->infinite);
      return (Trc_LineImp(rcl, root_P->zo, root_P->gamma, root_P->length));
    }
    break;
  }
}

/*
 * The function ZParallel calculates the impedence at a
 * node from the parallel combination of the left and right
 * impedences.
 */
complex Trc_ZParallel(complex leftImp, complex rightImp)

{

  complex zPar;

  /*
   * ZParallel = (z_left * z_right)/(z_left + z.right)
   */

  zPar = Cx_Div(Cx_Mult(leftImp, rightImp), Cx_Add(leftImp, rightImp));
  return (zPar);
}

/*
 * The function InitCur calculates the current at the
 * input of the network.
 */
complex Trc_InitCur(struct node *root_P, complex gamma, complex zo, complex eg,
                    complex zs, complex zLoad, float length, complex totalImp)

//   root_P;  	/* pointer to node */
//   complex		eg,		/* source */
//   zs,		/* source impedence */
//   zLoad,		/* load impedence */
//   gamma, 		/* propagation constant */
//   zo;		/* characteristic impedence */
//   float		length;		/* length of line */
//   complex		totalImp;	/* total input impedance to network */

{

  complex tmp1, tmp2, tmp3;
  complex rcl, rcs, initCur;

  complex sourceCur;

  if (root_P->lumped) {
    sourceCur = Cx_Div(eg, Cx_Add(zs, totalImp));
    initCur = Trc_LumpedLineCur(root_P, sourceCur);

  } else {

    /*
     *   init_cur = eg/(zo + zs) *
     *    	      [(1 - rcl * exp(-2 * gamma * length))/
     *            (1 - rcl * rcs * exp(-2 * gamma * length))]
     */
    tmp3 = Cx_Div(eg, Cx_Add(zo, zs));
    rcl = Trc_CalcRcl(root_P->zLoad, root_P->zo, root_P->infinite);
    rcs = Trc_CalcRcs(zs, root_P->zo);
    if (length) {
      tmp1.re = -2.0 * gamma.re * length;
      tmp1.im = -2.0 * gamma.im * length;
      tmp1 = Cx_Exp(tmp1);
    } else {
      tmp1.re = 1.0;
      tmp1.im = 0.0;
    }
    tmp1 = Cx_Mult(rcl, tmp1);
    tmp2 = Cx_Mult(rcs, tmp1);
    tmp1.re = 1.0 - tmp1.re;
    tmp1.im = 0.0 - tmp1.im;
    tmp2.re = 1.0 - tmp2.re;
    tmp2.im = 0.0 - tmp2.im;
    initCur = Cx_Mult(tmp3, Cx_Div(tmp1, tmp2));
  }
  return (initCur);
}

/*
 * The function LumpedLineCur calculates the current for a lumped element
 */
complex Trc_LumpedLineCur(struct node *root_P, complex inputCurrent)
//   struct node	*root_P; 		/* pointer to node */
//   complex         inputCurrent;		/* current into node */
{

  complex tmp1, tmp2, puredelay;
  complex incoming;

  /*
   * incoming = inputCurrent *  ZL || ZShunt/ZL
   * note for lumped element ZShunt = gamma
   */

  /*
   * if both c and g are zero then that means they dont exist.
   * So return input current
   */
  if ((root_P->gamma.re == 0.0) && (root_P->gamma.im == 0.0)) {
    return (inputCurrent);
  }

  if (root_P->infinite)
    tmp1 = root_P->gamma;
  else
    tmp1 = Trc_ZParallel(root_P->gamma, root_P->zLoad);
  incoming = Cx_Div(Cx_Mult(inputCurrent, tmp1), root_P->zLoad);
  return (incoming);
}

/*
 * The function LineCur calculates the current as a function
 * of the distance along the line.
 */
complex Trc_LineCur(struct node *root_P, complex inputCurrent, complex zo,
                    complex gamma, complex zLoad, float length, int inf)

//    struct node	*root_P; 		/* pointer to node */
//    complex		zo,		/* characteristic impedence */
//    gamma,		/* propagation constant */
//    inputCurrent,		/* current into node */
//    zLoad;		/* load impedence */
//    float		length;		/* length of line */
//    int		inf;		/* open circuit */
{

  complex tmp1, tmp2, puredelay;
  complex rcl, incoming;

  /*
   * incoming = inputCurrent * exp(- 1 * gamma * length) *
   *            [(1 - rcl) / (1 - rcl * exp(-2 * gamma
   *            length)]
   */

  rcl = Trc_CalcRcl(root_P->zLoad, root_P->zo, root_P->infinite);
  if ((rcl.re == 1.0) && (rcl.im == 0) && (length == 0.0)) {
    incoming.re = 0.0;
    incoming.im = 0.0;
    return (incoming);
  }
  tmp1.re = 1.0 - rcl.re;
  tmp1.im = 0.0 - rcl.im;
  if (length) {
    tmp2.re = -2.0 * gamma.re * length;
    tmp2.im = -2.0 * gamma.im * length;
    tmp2 = Cx_Exp(tmp2);
  } else {
    tmp2.re = 1.0;
    tmp2.im = 0.0;
  }
  tmp2 = Cx_Mult(rcl, tmp2);
  tmp2.re = 1.0 - tmp2.re;
  tmp2.im = 0.0 - tmp2.im;
  tmp1 = Cx_Div(tmp1, tmp2);
  if (length) {
    tmp2.re = -1.0 * gamma.re * length;
    tmp2.im = -1.0 * gamma.im * length;
    tmp2 = Cx_Exp(tmp2);
  } else {
    tmp2.re = 1.0;
    tmp2.im = 0.0;
  }
  incoming = Cx_Mult(Cx_Mult(tmp1, tmp2), inputCurrent);

  return (incoming);
}

/*
 * The function LineCur calculates the current as a function
 * of the distance along the line. Length=0 is at load.
 */
complex Trc_TrLineCur(struct node *root_P, complex inputCurrent, float length)

//   struct node	*root_P; 		/* pointer to node */
//                inputCurrent,		/* current into node */
//   float		length;		/* length of line */
{
  complex tmp1, tmp2, tmp3, puredelay;
  complex rcl, incoming;
  complex egx, egmx;
  complex zL;

  if (root_P->zLoad.re == 0.0 && root_P->zLoad.im == 0.0) {
    zL.re = 0.00001;
    zL.im = 0.0;
  } else
    zL = root_P->zLoad;

  rcl = Trc_CalcRcl(zL, root_P->zo, root_P->infinite);
  if ((rcl.re == 1.0) && (rcl.im == 0) && (length == 0.0)) {
    incoming.re = 0.0;
    incoming.im = 0.0;
    return (incoming);
  }
  tmp1.re = 1.0 - rcl.re;
  tmp1.im = 0.0 - rcl.im;
  if (length) {
    egmx.re = -1.0 * root_P->gamma.re * length;
    egmx.im = -1.0 * root_P->gamma.im * length;
    egmx = Cx_Exp(egmx);
  } else {
    egmx.re = 1.0;
    egmx.im = 0.0;
  }

  if (length) {
    egx.re = 1.0 * root_P->gamma.re * length;
    egx.im = 1.0 * root_P->gamma.im * length;
    egx = Cx_Exp(egx);
  } else {
    egx.re = 1.0;
    egx.im = 0.0;
  }

  tmp2 = Cx_Mult(rcl, egmx);
  tmp2.re = egx.re - tmp2.re;
  tmp2.im = egx.im - tmp2.im;

  tmp3.re = 1.0 + rcl.re;
  tmp3.im = rcl.im;

  tmp1 = Cx_Div(zL, Cx_Mult(root_P->zo, tmp3));

  tmp3 = Cx_Mult(tmp2, tmp1);

  incoming = Cx_Mult(tmp3, inputCurrent);

  return (incoming);
}

/*
 * The function LineCur calculates the voltage as a function
 * of the distance along the line. Length=0 is at load.
 */
complex Trc_TrLineVoltage(struct node *root_P, complex inputVoltage,
                          float length) {
  complex tmp1, tmp2, tmp3, puredelay;
  complex rcl, incoming;

  complex egx, egmx;
  complex zL;

  if (root_P->zLoad.re == 0.0 && root_P->zLoad.im == 0.0 && !root_P->infinite) {
    incoming.re = 0.0;
    incoming.im = 0.0;
    return (incoming);
  } else
    zL = root_P->zLoad;

  rcl = Trc_CalcRcl(zL, root_P->zo, root_P->infinite);
  if ((rcl.re == 1.0) && (rcl.im == 0) && (length == 0.0)) {
    return (inputVoltage);
  }

  tmp1.re = 1.0 - rcl.re;
  tmp1.im = 0.0 - rcl.im;
  if (length) {
    egmx.re = -1.0 * root_P->gamma.re * length;
    egmx.im = -1.0 * root_P->gamma.im * length;
    egmx = Cx_Exp(egmx);
  } else {
    egmx.re = 1.0;
    egmx.im = 0.0;
  }

  if (length) {
    egx.re = 1.0 * root_P->gamma.re * length;
    egx.im = 1.0 * root_P->gamma.im * length;
    egx = Cx_Exp(egx);
  } else {
    egx.re = 1.0;
    egx.im = 0.0;
  }

  tmp2 = Cx_Mult(rcl, egmx);
  tmp2.re = egx.re + tmp2.re;
  tmp2.im = egx.im + tmp2.im;

  tmp3.re = 1.0 + rcl.re;
  tmp3.im = rcl.im;

  tmp1 = Cx_Div(tmp2, tmp3);

  incoming = Cx_Mult(tmp1, inputVoltage);

  return (incoming);
}

/*
 */
complex Trc_TrCalcReflectionCoeff(struct node *root_P) {
  complex rcl, incoming;
  complex CalcRcl();
  complex zL;

  zL = root_P->zLoad;

  rcl = Trc_CalcRcl(zL, root_P->zo, root_P->infinite);

  return (rcl);
}

/*
 * The function LeftCur calculates the current entering
 * the left child of a node.
 */
complex Trc_LeftCur(complex leftImp, complex rightImp, complex inputCurrent)

//  complex leftImp,		/* left impedence */
//	rightImp,		/* right impedence */
// 	inputCurrent;		/* current entering node */
{
  complex tmp;

  /* leftCurrent = inputCurrent[rightImp/(rightImp + leftImp)]
   */
  tmp = Cx_Add(rightImp, leftImp);
  tmp = Cx_Div(rightImp, tmp);
  tmp = Cx_Mult(inputCurrent, tmp);
  return (tmp);
}

/*
 * The function rightCurrent calculates the current entering
 * the right child of a node.
 */
complex Trc_RightCur(complex leftImp, complex rightImp, complex inputCurrent)

// complex leftImp,		/* left impedence */
// 	rightImp,		/* right impedence */
// 	inputCurrent;		/* current entering node */

{
  complex tmp;

  /* rightCurrent = inputCurrent[leftImp/(rightImp + leftImp)]
   */
  tmp = Cx_Add(rightImp, leftImp);
  tmp = Cx_Div(leftImp, tmp);
  tmp = Cx_Mult(inputCurrent, tmp);
  return (tmp);
}

/*
 * The function LineVolt calculates the voltage as a function
 * of the distance along the line.
 */
complex Trc_LineVolt(struct node *root_P, complex inputVoltage)

// struct node	*root_P; 	/* pointer to node */
// complex		inputVoltage;	/* voltage at top of node needed for
//                         * equation to calculate voltage for
//                       * open circuit */
{
  complex tmp1, tmp2, tmp3;
  complex rcl, in_volt;

  //			  fprintf(stdout,"open node v=%f
  //%f\n",inputVoltage.re,inputVoltage.im);

  /*
   * in_volt = inputVoltage * exp(- 1 * gamma * length) *
   *            [(1 + rcl) / (1 + rcl * exp(-2 * gamma
   *            length)]
   */
  rcl = Trc_CalcRcl(root_P->zLoad, root_P->zo, root_P->infinite);
  if (root_P->length) {
    tmp1.re = -1.0 * root_P->gamma.re * root_P->length;
    tmp1.im = -1.0 * root_P->gamma.im * root_P->length;
    tmp1 = Cx_Exp(tmp1);
  } else {
    tmp1.re = 1.0;
    tmp1.im = 0.0;
  }
  tmp1 = Cx_Mult(inputVoltage, tmp1);
  tmp2.re = 1.0 + rcl.re;
  tmp2.im = 0.0 + rcl.im;
  if (root_P->length) {
    tmp3.re = -2.0 * root_P->gamma.re * root_P->length;
    tmp3.im = -2.0 * root_P->gamma.im * root_P->length;
    tmp3 = Cx_Exp(tmp3);
  } else {
    tmp3.re = 1.0;
    tmp3.im = 0.0;
  }
  tmp3 = Cx_Mult(rcl, tmp3);
  tmp3.re = 1.0 + tmp3.re;
  tmp3.im = 0.0 + tmp3.im;
  tmp3 = Cx_Div(tmp2, tmp3);
  in_volt = Cx_Mult(tmp1, tmp3);
  return (in_volt);
}

/*
 * The function CalcRcl calculates the reflection
 * coefficient at the load.
 */
complex Trc_CalcRcl(complex zLoad, complex zo, int inf)

// complex	zLoad,		/* load impedence */
// 	zo;		/* characteristic impedence */
// int 	inf;		/* open circuit */

{
  complex rcl;
  /*
   * rcl = (zLoad - zo)/(zLoad + zo)
   */
  if (inf) {
    rcl.re = 1.0;
    rcl.im = 0.0;
  } else
    rcl = Cx_Div(Cx_Sub(zLoad, zo), Cx_Add(zLoad, zo));
  return (rcl);
}

/*
 * The function CalcRcs calculates the reflection
 * coefficient at the source.
 */
complex Trc_CalcRcs(complex z_source, complex zo)

// complex	z_source,	/* source impedence */

// 	zo;		/* characteristic impedence */

{

  complex rcs;

  /*
   * rcl = (z_source - zo)/(z_source + zo)
   */
  rcs = Cx_Div(Cx_Sub(z_source, zo), Cx_Add(z_source, zo));
  return (rcs);
}

/*
 * The function LineImp calculates the impedence of the line
 * as a direct function of the distance to the load.
 */
complex Trc_LineImp(complex rcl, complex zo, complex gamma, float length)

//   complex	rcl,		/* reflection coefficent at load */
//	zo,		/* characteristic impedence */
//	gamma;		/* propagation constant */
//      float  length;		/* length of line */
{

  complex tmp1, tmp2, zin;

  /*
   * PLASMA SPECIFIC
   */
  if ((gamma.im == 0) && (gamma.re < 0.0)) {
    zin.re = 0.0;
    zin.im = 0.0;
    return (zin);
  }

  if ((length == 0.0) && (rcl.re == 1.0) && (rcl.im == 0.0)) {
    zin.re = INFINITY;
    zin.im = INFINITY;
    return (zin);
  }
  /*
   * line_imp = zo * {[1 + rcl * exp(-2 * gamma * length)]/
   *                  [1 - rcl * exp(-2 * gamma * length)]}
   */
  if (length) {
    tmp1.re = -2.0 * gamma.re * length;
    tmp1.im = -2.0 * gamma.im * length;
    tmp1 = Cx_Exp(tmp1);
  } else {
    tmp1.re = 1.0;
    tmp1.im = 0.0;
  }

  tmp1 = Cx_Mult(rcl, tmp1);
  tmp2.re = 1.0 - tmp1.re;
  tmp2.im = 0.0 - tmp1.im;
  tmp1.re = 1.0 + tmp1.re;
  zin = Cx_Mult(Cx_Div(tmp1, tmp2), zo);
  return (zin);
}

/*
 * The function CalcCurrent is a recursive procedure
 * which calculates the current at each node.
 * Then the voltage at each node is calculated from the
 * current and impedence.
 */
void Trc_CalcCurrent(struct node *root_P, complex inputCurrent,
                     complex inputVoltage, complex zo, complex gamma)

// struct node *root_P;	/* pointer to node */
// complex inputCurrent;		/* current entering node */
// complex zo;		/* characteristic impedence */
// complex gamma; 		/* propagation constant */
// complex inputVoltage;		/* voltage of the parent node */

{

  complex voltage;

  voltage = inputVoltage;

  if (root_P == (struct node *)NULL)
    return;
  if (root_P->lumped) {
    root_P->inCurrent = Trc_LumpedLineCur(root_P, inputCurrent);

  } else {
    root_P->inCurrent =
        Trc_LineCur(root_P, inputCurrent, root_P->zo, root_P->gamma,
                    root_P->zLoad, root_P->length, root_P->infinite);
  }
  if (root_P->infinite)
    root_P->voltage = Trc_LineVolt(root_P, inputVoltage);
  else {
    root_P->voltage = Cx_Mult(root_P->zLoad, root_P->inCurrent);
#if 000
    sprintf(cs_consoleBuffer,
            "zLoad.re=%f zLoad.im=%f inCurr.re=%f inCurr.im=%f volt.re=%f "
            "volt.im=%f \n",
            root_P->zLoad.re, root_P->zLoad.im, root_P->inCurrent.re,
            root_P->inCurrent.im, root_P->voltage.re, root_P->voltage.im);
    PrintConsole();
#endif

    voltage = root_P->voltage;
  }
  if (root_P->left_P != (struct node *)NULL) {
    if (root_P->type == NODE_TYPE_BINARY)
      root_P->leftCurrent =
          Trc_LeftCur(root_P->leftImp, root_P->rightImp, root_P->inCurrent);
    else
      root_P->leftCurrent = root_P->inCurrent;

    Trc_CalcCurrent(root_P->left_P, root_P->leftCurrent, voltage, root_P->zo,
                    root_P->gamma);
  }
  if (root_P->right_P != (struct node *)NULL) {
    root_P->rightCurrent =
        Trc_RightCur(root_P->leftImp, root_P->rightImp, root_P->inCurrent);
    Trc_CalcCurrent(root_P->right_P, root_P->rightCurrent, voltage, root_P->zo,
                    root_P->gamma);
  }
}
